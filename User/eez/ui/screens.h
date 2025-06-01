#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <driver_lib/lvgl_v9_3_0/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *clock_style_page;
    lv_obj_t *language_page;
    lv_obj_t *main_page;
    lv_obj_t *setting_page;
    lv_obj_t *themes_page;
    lv_obj_t *clock_2_page;
    lv_obj_t *obj0;
    lv_obj_t *cs_dropdown;
    lv_obj_t *cn_dropdown;
    lv_obj_t *main_img_pxx;
    lv_obj_t *main_arc_bat;
    lv_obj_t *main_arc_tmprt;
    lv_obj_t *main_arc_wifi;
    lv_obj_t *main_lab_hour_min;
    lv_obj_t *main_lab_sec;
    lv_obj_t *main_lab_bat;
    lv_obj_t *main_lab_tmprt;
    lv_obj_t *settings_flex_container;
    lv_obj_t *bat0;
    lv_obj_t *bat1;
    lv_obj_t *bat2;
    lv_obj_t *bat3;
    lv_obj_t *theme_slider_red;
    lv_obj_t *theme_slider_green;
    lv_obj_t *theme_slider_blue;
    lv_obj_t *theme_switch_dark;
    lv_obj_t *theme_arc_cur_color;
    lv_obj_t *theme_lab_dark;
    lv_obj_t *theme_lab_accent_color;
    lv_obj_t *theme_lab_cur_color;
    lv_obj_t *c2_img;
    lv_obj_t *obj1;
    lv_obj_t *c2_hour;
    lv_obj_t *c2_min;
    lv_obj_t *c2_sec_w;
    lv_obj_t *c2_sec_r;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_CLOCK_STYLE_PAGE = 1,
    SCREEN_ID_LANGUAGE_PAGE = 2,
    SCREEN_ID_MAIN_PAGE = 3,
    SCREEN_ID_SETTING_PAGE = 4,
    SCREEN_ID_THEMES_PAGE = 5,
    SCREEN_ID_CLOCK_2_PAGE = 6,
};

void create_screen_clock_style_page();
void tick_screen_clock_style_page();

void create_screen_language_page();
void tick_screen_language_page();

void create_screen_main_page();
void tick_screen_main_page();

void create_screen_setting_page();
void tick_screen_setting_page();

void create_screen_themes_page();
void tick_screen_themes_page();

void create_screen_clock_2_page();
void tick_screen_clock_2_page();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/