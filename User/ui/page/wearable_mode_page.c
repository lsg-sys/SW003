#include "wearable_mode_page.h"

#include "ui/ui_framework.h"

#include "settings_page.h"

lv_obj_t * dd_wearable;
lv_group_t * group_dd_wearable;

// 创建页面的回调函数
void wearable_mode_create(lv_obj_t *screen) {
    /*Create a normal drop down list*/
    dd_wearable = lv_dropdown_create(screen);
    lv_dropdown_set_options(dd_wearable, "Left\n"
                            "Right");

    lv_obj_align(dd_wearable, LV_ALIGN_TOP_MID, 0, 40);

    group_dd_wearable = lv_group_create();
    lv_group_add_obj(group_dd_wearable, dd_wearable);
}

// 销毁页面的回调函数
void wearable_mode_destroy(lv_obj_t *screen) {
    lv_group_remove_all_objs(group_dd_wearable);
    lv_group_delete(group_dd_wearable);
}

// 按键事件处理函数
void wearable_mode_key_callback(lv_event_t *e) {
    // lv_event_code_t code = lv_event_get_code(e);
    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP:
        {
        uint32_t dd_sel = lv_dropdown_get_selected(dd_wearable);
        if ((0 < dd_sel) && lv_dropdown_is_open(dd_wearable)) {
            lv_dropdown_set_selected(dd_wearable, dd_sel-1);
        }
        }
        break;
    case LV_KEY_DOWN:
        {
        uint32_t dd_sel = lv_dropdown_get_selected(dd_wearable);
        if ((dd_sel < lv_dropdown_get_option_count(dd_wearable)-1)  && lv_dropdown_is_open(dd_wearable)) {
            lv_dropdown_set_selected(dd_wearable, dd_sel+1);
        }
        }
        break;
    case LV_KEY_ENTER:
        // if (lv_dropdown_is_open(dd_wearable)) {
        //     lv_dropdown_close(dd_wearable);
        // } else {
        //     lv_dropdown_open(dd_wearable);
        // }
        lv_obj_send_event(dd_wearable, LV_EVENT_PRESSED, NULL);
        lv_obj_send_event(dd_wearable, LV_EVENT_RELEASED, NULL);
        break;
    case LV_KEY_ESC:
        UIFW_async_switch_to_page(&settings_page);
        break;
    default:
        break;
    }
}


app_page_controler_t const wearable_mode_page_ctrl = {
    .create_page_cb = wearable_mode_create,
    .delete_page_cb = wearable_mode_destroy,
    .key_event_cb   = wearable_mode_key_callback
};

lv_obj_t* wearable_mode_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    wearable_mode_create(new_screen);
    return new_screen;
}

void wearable_mode_created_callback(void *args) {
    lv_group_focus_obj(dd_wearable);
}

void wearable_mode_destroy_new(void *args) {
    wearable_mode_destroy(NULL);
}

struct PageControler wearable_mode_page = {
    .create_page             = wearable_mode_create_new,
    .delete_page             = wearable_mode_destroy_new,
    .key_event_callback = wearable_mode_key_callback,
    .page_created_callback   = wearable_mode_created_callback,
    .anim               = LV_SCR_LOAD_ANIM_FADE_IN};