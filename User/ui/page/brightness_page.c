#include "brightness_page.h"

#include "ui/ui_framework.h"
#include "settings_page.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BRIGHTNESS_MAX 100
#define BRIGHTNESS_MIN 1

lv_obj_t * brightness_arc;
static lv_style_t style;
static lv_obj_t * label;
lv_style_t style_label;
lv_timer_t * page_timer = NULL;

void brightness_value_change(lv_obj_t *arc, lv_obj_t *label) {
    
    extern float get_dev_lsm6dso_radian(void);
    int rad_10000 = get_dev_lsm6dso_radian() * 10000;
    int rad_100 = (rad_10000 / (314 * 2)) % 101;

    /* limit the radian */
    if (rad_100 < 12) {
        rad_100 = 12;
    }
    if (rad_100 > 88) {
        rad_100 = 88;
    }

    /* map the radian to 1 to 100 */
    rad_100 = ((float)(rad_100 - 11) * (float)(100.0f / 77.0f));

    lv_arc_set_value(brightness_arc, rad_100);
    hardware_api_handler->set_lightness(rad_100);

    // lv_label_set_text_fmt(label, "%d %%", val);
    lv_label_set_text_fmt(label, "%d %%", rad_100);
}

void page_timer_callback(lv_timer_t * timer) {

    brightness_value_change(brightness_arc, label);
}

void brightness_create(lv_obj_t *screen) {

    lv_style_init(&style_label);
    lv_style_set_text_font(&style_label, &lv_font_montserrat_24);
    lv_style_set_text_color(&style_label, lv_color_hex(0x000000));
    label = lv_label_create(screen);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(label, &style_label, 0);    

    /*Initialize the style for the arc*/
    lv_style_init(&style);
    lv_style_set_arc_color(&style, lv_color_hex(0xb3b9b7));
    lv_style_set_arc_width(&style, 4);
    /*Create an arc for brightness control*/
    brightness_arc = lv_arc_create(screen);
    lv_arc_set_range(brightness_arc, BRIGHTNESS_MIN, BRIGHTNESS_MAX); // Brightness range from 0 to 100
    lv_obj_set_size(brightness_arc, 200, 200);
    lv_obj_add_style(brightness_arc, &style, 0);

    lv_obj_align(brightness_arc, LV_ALIGN_CENTER, 0, 0);

    page_timer = lv_timer_create(page_timer_callback, 300, NULL);
}

void brightness_destroy(lv_obj_t *screen) {

    lv_timer_delete(page_timer);
}

void brightness_key_callback(lv_event_t *e) {
    uint32_t key = lv_event_get_key(e);

    switch (key) {
    case LV_KEY_ESC:
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        config.brightness = hardware_api_handler->get_lightness();
        hardware_api_handler->set_config(&config);
        UIFW_async_switch_to_page(&settings_page);
        break;
    default:
        break;
    }
}

app_page_controler_t const brightness_page_ctrl = {
    .create_page_cb = brightness_create,
    .delete_page_cb = brightness_destroy,
    .key_event_cb   = brightness_key_callback
};


lv_obj_t* brightness_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    brightness_create(new_screen);
    return new_screen;
}

void brightness_page_created_callback(void *args) {}

void brightness_destroy_new(void *args) {
    brightness_destroy(NULL);
}

struct PageControler brightness_page = {
    .create_page             = brightness_create_new,
    .delete_page             = brightness_destroy_new,
    .key_event_callback = brightness_key_callback,
    .page_created_callback   = brightness_page_created_callback,
    .anim               = LV_SCR_LOAD_ANIM_FADE_IN};