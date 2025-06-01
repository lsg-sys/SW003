#include "themes_page.h"

#include "settings_page.h"
#include "ui/ui_framework.h"

#include "eez/ui/screens.h"

lv_font_t  *th_page_sourceHanSans_14 = NULL;
lv_group_t *group_themes;

uint32_t get_color_from_slider(void) {
    uint8_t r = lv_slider_get_value(objects.theme_slider_red);
    uint8_t g = lv_slider_get_value(objects.theme_slider_green);
    uint8_t b = lv_slider_get_value(objects.theme_slider_blue);
    
    return ((r << 16) + (g << 8) + b);
}

void update_arc_cur_color() {
    uint32_t hex_color = get_color_from_slider();
    lv_obj_set_style_arc_color(objects.theme_arc_cur_color, lv_color_hex(hex_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(objects.theme_lab_cur_color, "#%06x", (unsigned int)hex_color);
}

lv_obj_t * themes_create(void *args) {

    create_screen_themes_page();

    th_page_sourceHanSans_14 = lv_binfont_create("0:SourceHanSans_14.bin");
    lv_obj_set_style_text_font(objects.theme_lab_accent_color, th_page_sourceHanSans_14, LV_PART_MAIN);
    lv_obj_set_style_text_font(objects.theme_lab_dark, th_page_sourceHanSans_14, LV_PART_MAIN);

    group_themes = lv_group_create();
    lv_group_add_obj(group_themes, objects.theme_slider_red);
    lv_group_add_obj(group_themes, objects.theme_slider_green);
    lv_group_add_obj(group_themes, objects.theme_slider_blue);
    lv_group_add_obj(group_themes, objects.theme_switch_dark);

    return objects.themes_page;
}

void themes_page_created_callback(void *args) {

    SmartWatchesConfig_t config;
    hardware_api_handler->get_config(&config);

    bool is_dark_mode = (0 == config.theme_dark) ? false : true;
    lv_obj_remove_state(objects.theme_switch_dark, LV_STATE_CHECKED);
    if (is_dark_mode) {
        lv_obj_add_state(objects.theme_switch_dark, LV_STATE_CHECKED);
    }

    lv_slider_set_value(objects.theme_slider_red, config.theme_color >> 16, LV_ANIM_OFF);
    lv_slider_set_value(objects.theme_slider_green, (config.theme_color >> 8) & 0xFF, LV_ANIM_OFF);
    lv_slider_set_value(objects.theme_slider_blue, config.theme_color & 0xFF, LV_ANIM_OFF);

    update_arc_cur_color();
    lv_group_focus_obj(objects.theme_slider_red);
}

void themes_destroy(void *args) {
    lv_obj_delete(objects.theme_lab_accent_color);
    lv_obj_delete(objects.theme_lab_dark);
    lv_binfont_destroy(th_page_sourceHanSans_14);

    lv_group_remove_all_objs(group_themes);
    lv_group_delete(group_themes);
}

void themes_key_callback(lv_event_t * e) {

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP: {
        lv_group_focus_prev(group_themes);
    } break;
    case LV_KEY_DOWN: {
        lv_group_focus_next(group_themes);
    } break;
    case LV_KEY_LEFT: {
        lv_obj_t * obj = lv_group_get_focused(group_themes);
        if (objects.theme_switch_dark != lv_group_get_focused(group_themes)) {
            int32_t value = lv_slider_get_value(obj);
            value -= 10;
            if (value < 0) {
                value = 0;
            }
            lv_slider_set_value(obj, value, LV_ANIM_ON);
            update_arc_cur_color();
        }
    } break;
    case LV_KEY_RIGHT: {
        lv_obj_t * obj = lv_group_get_focused(group_themes);
        if (objects.theme_switch_dark != lv_group_get_focused(group_themes)) {
            int32_t value = lv_slider_get_value(obj);
            value += 10;
            if (value > 255) {
                value = 255;
            }
            lv_slider_set_value(obj, value, LV_ANIM_ON);
            update_arc_cur_color();
        }
    } break;
    case LV_KEY_ENTER: {
        lv_obj_t * obj = lv_group_get_focused(group_themes);
        if (obj == objects.theme_switch_dark) {
            lv_obj_has_state(obj, LV_STATE_CHECKED) ? lv_obj_remove_state(obj, LV_STATE_CHECKED) : lv_obj_add_state(obj, LV_STATE_CHECKED);
        }
    } break;
    case LV_KEY_ESC: {
        bool is_dark = lv_obj_has_state(objects.theme_switch_dark, LV_STATE_CHECKED);
        uint32_t hex_color = get_color_from_slider();

        lv_disp_t *dispp = lv_disp_get_default();
        lv_theme_t *theme = lv_theme_default_init(dispp, lv_color_hex(hex_color), lv_color_hex(hex_color), is_dark, LV_FONT_DEFAULT);
        lv_disp_set_theme(dispp, theme);

        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        config.theme_color = hex_color;
        config.theme_dark =  is_dark ? 1 : 0;
        hardware_api_handler->set_config(&config);
        
        UIFW_async_switch_to_page(&settings_page);
    } break;
    default:
        break;
    }
}

struct PageControler themes_page = {
    .create_page           = themes_create,
    .delete_page           = themes_destroy,
    .key_event_callback    = themes_key_callback,
    .page_created_callback = themes_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_NONE};
