#include "clock_style_page.h"

#include "settings_page.h"
#include "ui/ui_framework.h"

#include "eez/ui/screens.h"

lv_group_t *group_dd_clock_style;

lv_obj_t * clock_style_create(void *args) {

    create_screen_clock_style_page();

    group_dd_clock_style = lv_group_create();
    lv_group_add_obj(group_dd_clock_style, objects.cs_dropdown);

    return objects.clock_style_page;
}

void clock_style_page_created_callback(void *args) {
    lv_group_focus_obj(objects.cs_dropdown);
}

void clock_style_destroy(void *args) {
    lv_obj_delete(objects.cs_dropdown);

    lv_group_remove_all_objs(group_dd_clock_style);
    lv_group_delete(group_dd_clock_style);
}

void clock_style_key_callback(lv_event_t * e) {

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP: {
        uint32_t dd_sel = lv_dropdown_get_selected(objects.cs_dropdown);
        if ((0 < dd_sel) &&
            lv_dropdown_is_open(objects.cs_dropdown)) {
            lv_dropdown_set_selected(objects.cs_dropdown, dd_sel - 1);
        }
    } break;
    case LV_KEY_DOWN: {
        uint32_t dd_sel = lv_dropdown_get_selected(objects.cs_dropdown);
        if ((dd_sel < lv_dropdown_get_option_count(objects.cs_dropdown) - 1) &&
            lv_dropdown_is_open(objects.cs_dropdown)) {
            lv_dropdown_set_selected(objects.cs_dropdown, dd_sel + 1);
        }
    } break;
    case LV_KEY_ENTER: {
        if (lv_dropdown_is_open(objects.cs_dropdown)) {
            lv_dropdown_close(objects.cs_dropdown);
        } else {
            lv_dropdown_open(objects.cs_dropdown);
        }
    } break;
    case LV_KEY_ESC: {
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        config.clock_style = (unsigned char)lv_dropdown_get_selected(objects.cs_dropdown);
        hardware_api_handler->set_config(&config);
        // hardware_api_handler->setSleepThreshold(config.sleep_index);
        UIFW_async_switch_to_page(&settings_page);
    } break;
    default:
        break;
    }
}

struct PageControler clock_style_page = {
    .create_page           = clock_style_create,
    .delete_page           = clock_style_destroy,
    .key_event_callback    = clock_style_key_callback,
    .page_created_callback = clock_style_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_NONE};
