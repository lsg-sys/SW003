#include "settings_page.h"

#include "ui/ui_framework.h"
#include "brightness_page.h" // Include the brightness page header
#include "clock_page.h"
#include "sync_time_page.h"
#include "language_page.h"
#include "sleep_page.h"

void button_cb_sync_time(lv_event_t * e) {
    UIFW_async_switch_to_page(&sync_time_page);
}

void button_cb_language(lv_event_t * e) {
    UIFW_async_switch_to_page(&language_page);
}

void button_cb_sleep(lv_event_t * e) {
    UIFW_async_switch_to_page(&sleep_page);
}

void button_cb_brightness(lv_event_t * e) {
    UIFW_async_switch_to_page(&brightness_page);
}

void button_cb_restart(lv_event_t * e) {
    hardware_api_handler->system_reset();
}

void button_cb_clock_style(lv_event_t * e) {
    UIFW_async_switch_to_page(&clock_style_page);
}

void button_cb_themes(lv_event_t * e) {
    UIFW_async_switch_to_page(&themes_page);
}

struct settings_page_btn {
    lv_obj_t     *obj;
    lv_obj_t     *label;
    char const   *name_en;
    char const   *name_cn;
    lv_event_cb_t click_event_cb;
} settings_page_btns[] = {
    {NULL, NULL, "clock style", "钟表样式", button_cb_clock_style},
    {NULL, NULL, "brightness", "亮度", button_cb_brightness},
    {NULL, NULL, "sleep", "息屏", button_cb_sleep},
    {NULL, NULL, "synchronize time", "同步时间", button_cb_sync_time},
    {NULL, NULL, "themes", "主题", button_cb_themes},
    {NULL, NULL, "language", "语言", button_cb_language},
    {NULL, NULL, "Restart", "重启", button_cb_restart},
};

/* ==================================================================== */

lv_obj_t   *settings_page_flexContainer;
lv_group_t *group_btns;
lv_font_t  *sourceHanSans_14 = NULL;
lv_style_t  style_label_cn;

void update_button_positions(lv_obj_t * cont) {
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    int32_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    int32_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        int32_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        int32_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        int32_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

void scroll_event_cb(lv_event_t * e) {
    lv_obj_t * cont = (lv_obj_t *)lv_event_get_target(e);
    update_button_positions(cont);
}

void settings_page_create(lv_obj_t * screen) {

    sourceHanSans_14 = lv_binfont_create("0:SourceHanSans_14.bin");
    lv_style_init(&style_label_cn);
    lv_style_set_text_font(&style_label_cn, sourceHanSans_14);

    SmartWatchesConfig_t config;
    hardware_api_handler->get_config(&config);
    unsigned char language = config.language;

    settings_page_flexContainer = lv_obj_create(screen);
    // lv_obj_set_size(settings_page_flexContainer, 240, 240);
    lv_obj_set_size(settings_page_flexContainer, lv_pct(100), lv_pct(100));
    // lv_obj_center(settings_page_flexContainer);
    // lv_obj_align(settings_page_flexContainer, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_flex_flow(settings_page_flexContainer, LV_FLEX_FLOW_COLUMN_WRAP);
    // lv_obj_set_flex_flow(settings_page_flexContainer, LV_FLEX_FLOW_COLUMN_REVERSE);
    lv_obj_set_flex_flow(settings_page_flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(settings_page_flexContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_event_cb(settings_page_flexContainer, scroll_event_cb, LV_EVENT_SCROLL, NULL);
    // lv_obj_set_style_radius(settings_page_flexContainer, LV_RADIUS_CIRCLE, 0);
    // lv_obj_set_style_clip_corner(settings_page_flexContainer, true, 0);
    lv_obj_set_scroll_dir(settings_page_flexContainer, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(settings_page_flexContainer, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(settings_page_flexContainer, LV_SCROLLBAR_MODE_OFF);

    group_btns = lv_group_create();

    
    lv_font_t * lv_binfont_create(const char * path);

    uint32_t i;
    for(i = 0; i < (sizeof(settings_page_btns) / sizeof(settings_page_btns[0])); i++) {
        settings_page_btns[i].obj = lv_button_create(settings_page_flexContainer);
        lv_obj_set_size(settings_page_btns[i].obj, lv_pct(70), lv_pct(16));

        settings_page_btns[i].label = lv_label_create(settings_page_btns[i].obj);
        
        lv_obj_add_style(settings_page_btns[i].label, &style_label_cn, 0);

        if (language == 0) {
            lv_label_set_text(settings_page_btns[i].label, settings_page_btns[i].name_cn);
        } else {
            lv_label_set_text(settings_page_btns[i].label, settings_page_btns[i].name_en);
        }

        lv_group_add_obj(group_btns, settings_page_btns[i].obj);

        if(settings_page_btns[i].click_event_cb != NULL) {
            lv_obj_add_event_cb(settings_page_btns[i].obj, settings_page_btns[i].click_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    /*Update the buttons position manually for first*/
    lv_obj_send_event(settings_page_flexContainer, LV_EVENT_SCROLL, NULL);

    /*Be sure the first button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(settings_page_flexContainer, 0), LV_ANIM_OFF);
}

void settings_page_destroy(lv_obj_t * screen) {
    lv_group_remove_all_objs(group_btns);
    lv_group_delete(group_btns);

    /* 在删除字体前，提前删除使用该字体的对象 */
    for (int i = 0; i < (sizeof(settings_page_btns) / sizeof(settings_page_btns[0])); i++) {
        lv_obj_del(settings_page_btns[i].obj);
    }
    lv_binfont_destroy(sourceHanSans_14);
}

void settings_page_key_callback(lv_event_t *e) {
    // lv_event_code_t code = lv_event_get_code(e);

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP:
        lv_obj_scroll_by(settings_page_flexContainer, 0, 50, LV_ANIM_ON);
        lv_group_focus_prev(group_btns);
        update_button_positions(settings_page_flexContainer);
        break;
    case LV_KEY_DOWN:
        lv_obj_scroll_by(settings_page_flexContainer, 0, -50, LV_ANIM_ON);
        lv_group_focus_next(group_btns);
        update_button_positions(settings_page_flexContainer);
        break;
    case LV_KEY_LEFT:
        break;
    case LV_KEY_RIGHT:
        break;
    case LV_KEY_ENTER: {
        lv_obj_t *focused_obj = lv_group_get_focused(group_btns);
        if (focused_obj) {
            lv_obj_send_event(focused_obj, LV_EVENT_CLICKED, NULL);
        }
    } break;
    case LV_KEY_ESC: {
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        if (0 == config.clock_style) {
            UIFW_async_switch_to_page(&clock_page);
        } else if (1 == config.clock_style) {
            UIFW_async_switch_to_page(&eez_page);
        } else {
            UIFW_async_switch_to_page(&clock_2_page);
        }
    }

    break;
    default:
        break;
    }
}

app_page_controler_t const settings_page_ctrl = {
    .create_page_cb = settings_page_create,
    .delete_page_cb = settings_page_destroy,
    .key_event_cb   = settings_page_key_callback
};


lv_obj_t* settings_page_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    settings_page_create(new_screen);
    return new_screen;
}

void settings_page_destroy_new(void *args) {
    settings_page_destroy(NULL);
}

void settings_page_created_callback(void *args) {
    lv_group_focus_obj(settings_page_btns[0].obj);
}

struct PageControler settings_page = {
    .create_page           = settings_page_create_new,
    .delete_page           = settings_page_destroy_new,
    .key_event_callback    = settings_page_key_callback,
    .page_created_callback = settings_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_MOVE_LEFT};
