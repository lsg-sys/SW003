#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_clock_style_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.clock_style_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 0, 60);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 1);
            lv_arc_set_bg_end_angle(obj, 0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 120);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 1);
            lv_arc_set_bg_end_angle(obj, 0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff368351), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cs_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.cs_dropdown = obj;
            lv_obj_set_pos(obj, 27, 102);
            lv_obj_set_size(obj, 93, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "A\nB\nC");
            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
            lv_dropdown_set_selected(obj, 0);
        }
    }
    
    tick_screen_clock_style_page();
}

void tick_screen_clock_style_page() {
}

void create_screen_language_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.language_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 89, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 1);
            lv_arc_set_bg_end_angle(obj, 0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cn_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.cn_dropdown = obj;
            lv_obj_set_pos(obj, 27, 102);
            lv_obj_set_size(obj, 93, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "中文\nEnglish");
            lv_dropdown_set_dir(obj, LV_DIR_RIGHT);
            lv_dropdown_set_symbol(obj, LV_SYMBOL_RIGHT);
            lv_dropdown_set_selected(obj, 0);
            // lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_SELECTED | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_language_page();
}

void tick_screen_language_page() {
}

void create_screen_main_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            // main_img_pxx
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.main_img_pxx = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, "0:pxx.bin");
        }
        {
            // main_arc_bat
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.main_arc_bat = obj;
            lv_obj_set_pos(obj, 15, 15);
            lv_obj_set_size(obj, 210, 210);
            lv_arc_set_value(obj, 30);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 270);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff00ffcd), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff00ffcd), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // main_arc_tmprt
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.main_arc_tmprt = obj;
            lv_obj_set_pos(obj, 42, 42);
            lv_obj_set_size(obj, 156, 156);
            lv_arc_set_value(obj, 50);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 270);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xfffff800), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xfffff800), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // main_arc_wifi
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.main_arc_wifi = obj;
            lv_obj_set_pos(obj, 69, 69);
            lv_obj_set_size(obj, 102, 102);
            lv_arc_set_value(obj, 50);
            lv_arc_set_bg_start_angle(obj, 90);
            lv_arc_set_bg_end_angle(obj, 270);
            lv_obj_set_style_arc_rounded(obj, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff3354f9), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff3354f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // main_lab_hour_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_lab_hour_min = obj;
            lv_obj_set_pos(obj, 90, 94);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "88:88");
        }
        {
            // main_lab_sec
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_lab_sec = obj;
            lv_obj_set_pos(obj, 200, 141);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "88");
        }
        {
            // main_lab_bat
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_lab_bat = obj;
            lv_obj_set_pos(obj, 137, 14);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ffcd), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "88");
        }
        {
            // main_lab_tmprt
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_lab_tmprt = obj;
            lv_obj_set_pos(obj, 137, 42);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffff800), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "88");
        }
    }
    
    tick_screen_main_page();
}

void tick_screen_main_page() {
}

void create_screen_setting_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.setting_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            lv_obj_set_pos(obj, 147, 0);
            lv_obj_set_size(obj, 224, 240);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 1);
            lv_arc_set_bg_end_angle(obj, 0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_flex_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_flex_container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bat0
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.bat0 = obj;
                    lv_obj_set_pos(obj, 56, 21);
                    lv_obj_set_size(obj, 115, 35);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "时间同步");
                        }
                    }
                }
                {
                    // bat1
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.bat1 = obj;
                    lv_obj_set_pos(obj, 30, 68);
                    lv_obj_set_size(obj, 100, 38);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "思源黑体");
                        }
                    }
                }
                {
                    // bat2
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.bat2 = obj;
                    lv_obj_set_pos(obj, 30, 129);
                    lv_obj_set_size(obj, 100, 38);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "SourceHanSans");
                        }
                    }
                }
                {
                    // bat3
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.bat3 = obj;
                    lv_obj_set_pos(obj, 71, 180);
                    lv_obj_set_size(obj, 100, 38);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_source_han_sans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "亮度");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_setting_page();
}

void tick_screen_setting_page() {
}

void create_screen_themes_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.themes_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    {
        lv_obj_t *parent_obj = obj;
        {
            // theme_slider_red
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.theme_slider_red = obj;
            lv_obj_set_pos(obj, 43, 56);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 255);
            lv_slider_set_value(obj, 105, LV_ANIM_OFF);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // theme_slider_green
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.theme_slider_green = obj;
            lv_obj_set_pos(obj, 43, 83);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 255);
            lv_slider_set_value(obj, 141, LV_ANIM_OFF);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff00), LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // theme_slider_blue
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.theme_slider_blue = obj;
            lv_obj_set_pos(obj, 43, 110);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 0, 255);
            lv_slider_set_value(obj, 31, LV_ANIM_OFF);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000ff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000ff), LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // theme_switch_dark
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.theme_switch_dark = obj;
            lv_obj_set_pos(obj, 43, 176);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_state(obj, LV_STATE_CHECKED);
        }
        {
            // theme_arc_cur_color
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.theme_arc_cur_color = obj;
            lv_obj_set_pos(obj, 196, 87);
            lv_obj_set_size(obj, 31, 32);
            lv_arc_set_range(obj, 0, 0);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff698d1f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 36, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // theme_lab_dark
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.theme_lab_dark = obj;
            lv_obj_set_pos(obj, 43, 151);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Dark");
        }
        {
            // theme_lab_accent_color
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.theme_lab_accent_color = obj;
            lv_obj_set_pos(obj, 43, 29);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Accent color");
        }
        {
            // theme_lab_cur_color
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.theme_lab_cur_color = obj;
            lv_obj_set_pos(obj, 179, 120);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "#698d1f");
        }
    }
    
    tick_screen_themes_page();
}

void tick_screen_themes_page() {
}

void create_screen_clock_2_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.clock_2_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            // c2_img
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.c2_img = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, "0:c2_bg.bin");
        }
        {
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 111, 111);
            lv_obj_set_size(obj, 18, 18);
            lv_arc_set_range(obj, 0, 0);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff838383), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // c2_hour
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.c2_hour = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_precise_t line_points[] = {
                { 132, 116 },
                { 55, 119 },
                { 132, 123 }
            };
            lv_line_set_points(obj, line_points, 3);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffe4e5e3), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // c2_min
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.c2_min = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_precise_t line_points[] = {
                { 118, 105 },
                { 120, 210 },
                { 119, 105 },
                { 120, 210 },
                { 122, 105 },
                { 120, 210 },
                { 123, 105 },
                { 120, 210 }
            };
            lv_line_set_points(obj, line_points, 8);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffe3e3e3), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // c2_sec_w
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.c2_sec_w = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_precise_t line_points[] = {
                { 102, 102 },
                { 170, 170 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // c2_sec_r
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.c2_sec_r = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 240, 240);
                    static lv_point_precise_t line_points[] = {
                        { 170, 170 },
                        { 193, 193 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffff3c21), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_pos(obj, 116, 116);
                    lv_obj_set_size(obj, 9, 9);
                    lv_arc_set_range(obj, 0, 0);
                    lv_arc_set_value(obj, 0);
                    lv_arc_set_bg_start_angle(obj, 0);
                    lv_arc_set_bg_end_angle(obj, 360);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.obj3 = obj;
                            lv_obj_set_pos(obj, 2, 2);
                            lv_obj_set_size(obj, 5, 5);
                            lv_arc_set_range(obj, 0, 0);
                            lv_arc_set_value(obj, 0);
                            lv_arc_set_bg_start_angle(obj, 0);
                            lv_arc_set_bg_end_angle(obj, 360);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_clock_2_page();
}

void tick_screen_clock_2_page() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_clock_style_page,
    tick_screen_language_page,
    tick_screen_main_page,
    tick_screen_setting_page,
    tick_screen_themes_page,
    tick_screen_clock_2_page,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_clock_style_page();
    create_screen_language_page();
    create_screen_main_page();
    create_screen_setting_page();
    create_screen_themes_page();
    create_screen_clock_2_page();
}
