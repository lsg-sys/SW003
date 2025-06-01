#include "clock_2_page.h"

#include "settings_page.h"
#include "ui/ui_framework.h"

#include "eez/ui/screens.h"

#include <string.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926f
#endif

#ifndef arry_len
    #define arry_len(arry) (sizeof(arry)/sizeof(arry[0]))
#endif

/**
 * @brief Rotate a set of points around a center point, updating the same array.
 *
 * @param points          Array of points to rotate and update in-place
 * @param point_num       Number of points
 * @param center_x        Rotation center (cx, cy)
 * @param center_y
 * @param angle_degrees   Rotation angle in degrees (counter-clockwise)
 */
void rotate_points_inplace(lv_point_precise_t *points,
                           uint32_t            point_num,
                           float               center_x,
                           float               center_y,
                           float               angle_degrees) {

    float angle_rad = angle_degrees * M_PI / 180.0f;
    float cos_theta = cosf(angle_rad);
    float sin_theta = sinf(angle_rad);

    for (uint32_t i = 0; i < point_num; i++) {
        float x = (float)points[i].x;
        float y = (float)points[i].y;

        // Translate point to origin
        float x_trans = x - center_x;
        float y_trans = y - center_y;

        // Apply rotation (counter-clockwise)
        float x_rot = x_trans * cos_theta - y_trans * sin_theta;
        float y_rot = x_trans * sin_theta + y_trans * cos_theta;

        // Translate back and convert to int32_t with rounding, then update the original array
        points[i].x = (int32_t)(x_rot + center_x + 0.5f); // 四舍五入
        points[i].y = (int32_t)(y_rot + center_y + 0.5f); // 四舍五入
    }
}

void clock_2_page_timer_callback(lv_timer_t * timer) {

    date_and_time_t date_and_time;
    hardware_api_handler->get_date_and_time(&date_and_time);

    static const lv_point_precise_t points_hour_init[] = {
        {132, 116},
        {55, 119},
        {132, 123}}; // 初始位置是指向9点
    static lv_point_precise_t points_hour[arry_len(points_hour_init)];
    memcpy(points_hour, points_hour_init, sizeof(points_hour_init));
    rotate_points_inplace(
        points_hour, arry_len(points_hour_init), 
        119.0f, 119.0f, 
        date_and_time.s.hour * 30 + 90 // 旋转角度 = 小时 * 30度 + 90度的初始偏移量
    );
    lv_line_set_points(objects.c2_hour, points_hour, arry_len(points_hour_init));

    static const lv_point_precise_t points_min_init[] = {
        { 118, 105 },
        { 120, 210 },
        { 119, 105 },
        { 120, 210 },
        { 122, 105 },
        { 120, 210 },
        { 123, 105 },
        { 120, 210 }}; // 初始位置是指向6点
    static lv_point_precise_t points_min[arry_len(points_min_init)];
    memcpy(points_min, points_min_init, sizeof(points_min_init));
    rotate_points_inplace(
        points_min, arry_len(points_min_init), 
        119.0f, 119.0f, 
        date_and_time.s.min * 6 + 180 // 旋转角度 = 分钟 * 6度 + 180度的初始偏移量
    );
    lv_line_set_points(objects.c2_min, points_min, arry_len(points_min_init));

    /* 秒针由一段白线和一段红线组成 */
    static lv_point_precise_t points_sec_w_init[] = {
        { 102, 102 },
        { 170, 170 }
    };
    static lv_point_precise_t points_sec_w[arry_len(points_sec_w_init)];
    memcpy(points_sec_w, points_sec_w_init, sizeof(points_sec_w_init));
    rotate_points_inplace(
        points_sec_w, arry_len(points_sec_w_init), 
        119.0f, 119.0f, 
        date_and_time.s.sec * 6 + 225 // 旋转角度 = 秒 * 6度 + 225度的初始偏移量
    );
    lv_line_set_points(objects.c2_sec_w, points_sec_w, arry_len(points_sec_w_init));
    
    static lv_point_precise_t points_sec_r_init[] = {
        { 170, 170 },
        { 193, 193 }
    };
    static lv_point_precise_t points_sec_r[arry_len(points_sec_r_init)];
    memcpy(points_sec_r, points_sec_r_init, sizeof(points_sec_r_init));
    rotate_points_inplace(
        points_sec_r, arry_len(points_sec_r_init), 
        119.0f, 119.0f, 
        date_and_time.s.sec * 6 + 225 // 旋转角度 = 秒 * 6度 + 225度的初始偏移量
    );
    lv_line_set_points(objects.c2_sec_r, points_sec_r, arry_len(points_sec_r_init));
}

lv_timer_t * clock_2_page_timer = NULL;

lv_obj_t * clock_2_create(void *args) {
    create_screen_clock_2_page();
    return objects.clock_2_page;
}

void clock_2_page_created_callback(void *args) {
    clock_2_page_timer = lv_timer_create(clock_2_page_timer_callback, 300, NULL);
}

void clock_2_destroy(void *args) {
    lv_timer_delete(clock_2_page_timer);
}

void clock_2_key_callback(lv_event_t * e) {

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_ENTER:
        UIFW_async_switch_to_page(&settings_page);
        break;
    default:
        break;
    }
}

struct PageControler clock_2_page = {
    .create_page           = clock_2_create,
    .delete_page           = clock_2_destroy,
    .key_event_callback    = clock_2_key_callback,
    .page_created_callback = clock_2_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_NONE};
