#include "sleep_page.h"

#include "ui/ui_framework.h"

#include "settings_page.h"

lv_obj_t * dd_sleep;
lv_group_t * group_dd_sleep;

void sleep_create(lv_obj_t *screen) {

    /*Create a normal drop down list*/
    dd_sleep = lv_dropdown_create(screen);
    lv_dropdown_set_options(dd_sleep, 
                            "5 seconds\n"
                            "10 seconds\n"
                            "30 seconds\n"
                            "1 minute\n"
                            "2 minutes\n"
                            "5 minutes\n"
                            "10 minutes\n"
                            "Never");

    lv_obj_align(dd_sleep, LV_ALIGN_TOP_MID, 0, 40);

    group_dd_sleep = lv_group_create();
    lv_group_add_obj(group_dd_sleep, dd_sleep);
}

void sleep_destroy(lv_obj_t *screen) {

    lv_group_remove_all_objs(group_dd_sleep);
    lv_group_delete(group_dd_sleep);
}

void sleep_key_callback(lv_event_t *e) {
    
    // lv_event_code_t code = lv_event_get_code(e);
    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP: {
        uint32_t dd_sel = lv_dropdown_get_selected(dd_sleep);
        if ((0 < dd_sel) && lv_dropdown_is_open(dd_sleep)) {
            lv_dropdown_set_selected(dd_sleep, dd_sel - 1);
        }
    } break;
    case LV_KEY_DOWN: {
        uint32_t dd_sel = lv_dropdown_get_selected(dd_sleep);
        if ((dd_sel < lv_dropdown_get_option_count(dd_sleep) - 1) && lv_dropdown_is_open(dd_sleep)) {
            lv_dropdown_set_selected(dd_sleep, dd_sel + 1);
        }
    } break;
    case LV_KEY_ENTER: {
        if (lv_dropdown_is_open(dd_sleep)) {
            lv_dropdown_close(dd_sleep);
        } else {
            lv_dropdown_open(dd_sleep);
        }
    } break;
    case LV_KEY_ESC: {
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        config.sleep_index = lv_dropdown_get_selected(dd_sleep);
        hardware_api_handler->set_config(&config);
        hardware_api_handler->setSleepThreshold(config.sleep_index);
        UIFW_async_switch_to_page(&settings_page);
    } break;
    default:
        break;
    }
}

void sleep_page_created_callback(void *args) {
    lv_group_focus_obj(dd_sleep);
}

lv_obj_t* sleep_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    sleep_create(new_screen);
    return new_screen;
}

void sleep_destroy_new(void *args) {
    sleep_destroy(NULL);
}

struct PageControler sleep_page = {
    .create_page           = sleep_create_new,
    .delete_page           = sleep_destroy_new,
    .key_event_callback    = sleep_key_callback,
    .page_created_callback = sleep_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_FADE_IN};