#include "clock_page.h"

#include "ui/ui_framework.h"

#include "settings_page.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define CLOCK_CENTER_X 120
#define CLOCK_CENTER_Y 120
#define SECOND_HAND_LENGTH 116
#define MINUTE_HAND_LENGTH 85
#define HOUR_HAND_LENGTH 60

typedef struct {
    lv_obj_t * line;
    float length;
    float rad;
    lv_point_precise_t points[2];
} clock_hand_t;

lv_obj_t * date_label;
lv_obj_t * battery_label;

static clock_hand_t hands[] = {
    {NULL, SECOND_HAND_LENGTH, 0, {{CLOCK_CENTER_X, CLOCK_CENTER_Y}, {0,0}}},
    {NULL, MINUTE_HAND_LENGTH, 0, {{CLOCK_CENTER_X, CLOCK_CENTER_Y}, {0,0}}},
    {NULL, HOUR_HAND_LENGTH, 0, {{CLOCK_CENTER_X, CLOCK_CENTER_Y}, {0,0}}}
};

lv_timer_t * main_page_clock_timer = NULL;
extern void rotate_points_inplace(lv_point_precise_t *points,
    uint32_t            point_num,
    float               center_x,
    float               center_y,
    float               angle_degrees);

void updating_the_clock_hands(date_and_time_t * date_and_time) {

    hands[0].points[1].x = CLOCK_CENTER_X;
    hands[0].points[1].y = CLOCK_CENTER_Y - hands[0].length;
    rotate_points_inplace(
        hands[0].points, 2, 
        CLOCK_CENTER_X, CLOCK_CENTER_Y,
        date_and_time->s.sec * 6
    );
    lv_line_set_points(hands[0].line, hands[0].points, 2);

    hands[1].points[1].x = CLOCK_CENTER_X;
    hands[1].points[1].y = CLOCK_CENTER_Y - hands[1].length;
    rotate_points_inplace(
        hands[1].points, 2, 
        CLOCK_CENTER_X, CLOCK_CENTER_Y,
        date_and_time->s.min * 6
    );
    lv_line_set_points(hands[1].line, hands[1].points, 2);

    hands[2].points[1].x = CLOCK_CENTER_X;
    hands[2].points[1].y = CLOCK_CENTER_Y - hands[2].length;
    rotate_points_inplace(
        hands[2].points, 2, 
        CLOCK_CENTER_X, CLOCK_CENTER_Y,
        date_and_time->s.hour * 30
    );
    lv_line_set_points(hands[2].line, hands[2].points, 2);
}

void updating_text_labels(date_and_time_t * date_and_time) {

    lv_label_set_text_fmt(date_label, "%d\n    %d\n        %d", date_and_time->s.year, date_and_time->s.month, date_and_time->s.day);

    uint32_t voltage;
    hardware_api_handler->get_voltage(&voltage);
    uint32_t degreeC;
    hardware_api_handler->get_degreeC(&degreeC);
    
    lv_label_set_text_fmt(battery_label, "%u%%\n    %u°C", (unsigned int)voltage, (unsigned int)degreeC);
}

void clock_page_timer_callback(lv_timer_t * timer) {
    // LV_LOG_USER("1 second has passed");

    date_and_time_t date_and_time;
    hardware_api_handler->get_date_and_time(&date_and_time);

    updating_the_clock_hands(&date_and_time);

    updating_text_labels(&date_and_time);
}

void clock_page_create(lv_obj_t* screen) {
    
    int radius = 119;
    int x = 120;
    int y = 120;

    static const lv_font_t * font = &lv_font_montserrat_24;

    /* Create date labels */
    static lv_style_t style_date_label;
    lv_style_init(&style_date_label);
    lv_style_set_text_color(&style_date_label, lv_theme_get_color_primary(screen));
    // lv_style_set_text_font(&style_date_label, font);
    date_label = lv_label_create(screen);
    lv_obj_add_style(date_label, &style_date_label, 0);
    lv_obj_set_pos(date_label, 120, 70);
    lv_label_set_text_fmt(date_label, "%d\n    %d\n        %d", 8888, 88, 88);

    /* Create battery/temperature labels */
    static lv_style_t style_battery_label;
    lv_style_init(&style_battery_label);
    lv_style_set_text_color(&style_battery_label, lv_theme_get_color_primary(screen));
    // lv_style_set_text_font(&style_battery_label, font);
    battery_label = lv_label_create(screen);
    lv_obj_add_style(battery_label, &style_battery_label, 0);
    lv_obj_set_pos(battery_label, 70, 140);
    lv_label_set_text_fmt(battery_label, "%d%%\n    %d°C", 88, 88);

    /* Create labels */
    static lv_style_t style_label;
    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, lv_color_hex(0xb3b9b7));
    // lv_style_set_text_color(&style_label, lv_color_hex(0xff0000));

    int offset_x = -7;
    int offset_y = -12;
    float factor = 0.76f;

    lv_style_set_text_font(&style_label, font);
    for (int i = 0; i < 12; i++) { 
        int angle = i * 30;
        int x1 = x + factor * radius * cosf((angle - 60) * M_PI / 180) + offset_x;
        int y1 = y + factor * radius * sinf((angle - 60) * M_PI / 180) + offset_y;
        lv_obj_t * label = lv_label_create(screen);
        lv_obj_set_pos(label, x1, y1);
        lv_label_set_text_fmt(label, "%d", (i + 1));
        lv_obj_add_style(label, &style_label, 0);
    }
    
    /* Create tick */
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_theme_get_color_primary(screen));
    // lv_style_set_line_rounded(&style_line, true);

    static lv_point_precise_t points[] = {
        {239, 120},
        {180, 223},
        {60, 223},
        {0, 120},
        {60, 17},
        {180, 17},
        {239, 120}
    };

    static lv_point_precise_t points_rotated[] = {
        {120, 239},
        {17, 180},
        {17, 60},
        {120, 0},
        {223, 60},
        {223, 180},
        {120, 239},
    };

    lv_obj_t * tick = lv_line_create(screen);
    lv_line_set_points(tick, points, sizeof(points)/sizeof(points[0]));
    lv_obj_add_style(tick, &style_line, 0);
    // lv_obj_center(tick);
    // lv_obj_align_to(tick, background, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t* rtick = lv_line_create(screen);
    lv_line_set_points(rtick, points_rotated, sizeof(points_rotated) / sizeof(points_rotated[0]));
    lv_obj_add_style(rtick, &style_line, 0);


    /* Create clock hands */
    hands[0].line = lv_line_create(screen);
    hands[1].line = lv_line_create(screen);
    hands[2].line = lv_line_create(screen);
    
    static lv_style_t style_sec_hand;
    lv_style_init(&style_sec_hand);
    lv_style_set_line_width(&style_sec_hand, 2);
    lv_style_set_line_color(&style_sec_hand, lv_color_hex(0xff0000));
    static lv_style_t style_min_hand;
    lv_style_init(&style_min_hand);
    lv_style_set_line_width(&style_min_hand, 3);
    lv_style_set_line_color(&style_min_hand, lv_theme_get_color_primary(screen));
    static lv_style_t style_hour_hand;
    lv_style_init(&style_hour_hand);
    lv_style_set_line_width(&style_hour_hand, 8);
    lv_style_set_line_color(&style_hour_hand, lv_theme_get_color_primary(screen));

    lv_obj_add_style(hands[0].line, &style_sec_hand, 0);
    lv_obj_add_style(hands[1].line, &style_min_hand, 0);
    lv_obj_add_style(hands[2].line, &style_hour_hand, 0);

    clock_page_timer_callback(NULL); // Initial update
    main_page_clock_timer = lv_timer_create(clock_page_timer_callback, 200, NULL);
}

void clock_page_destroy(lv_obj_t* screen) {
    lv_timer_delete(main_page_clock_timer);
}

lv_obj_t* clock_page_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    clock_page_create(new_screen);
    return new_screen;
}

void clock_page_created_callback(void *args) {}

void clock_page_destroy_new(void *args) {
    clock_page_destroy(NULL);
}

void clock_page_key_callback(lv_event_t *e) {
    // lv_event_code_t code = lv_event_get_code(e);

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP:
        break;
    case LV_KEY_DOWN:
        break;
    case LV_KEY_LEFT:
        break;
    case LV_KEY_RIGHT:
        break;
    case LV_KEY_ENTER:
        UIFW_async_switch_to_page(&settings_page);
        break;
    case LV_KEY_ESC:
        break;
    default:
        break;
    }
}

struct PageControler clock_page = {
    .create_page             = clock_page_create_new,
    .delete_page             = clock_page_destroy_new,
    .key_event_callback = clock_page_key_callback,
    .page_created_callback   = clock_page_created_callback,
    .anim               = LV_SCR_LOAD_ANIM_MOVE_RIGHT};