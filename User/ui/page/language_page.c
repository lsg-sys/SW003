#include "language_page.h"

#include "settings_page.h"
#include "ui/ui_framework.h"

#include "eez/ui/screens.h"

lv_font_t  *lg_page_sourceHanSans_14 = NULL;
lv_group_t *group_dd_language;

lv_obj_t * language_create(void *args) {

    create_screen_language_page();

    lg_page_sourceHanSans_14 = lv_binfont_create("0:SourceHanSans_14.bin");
    lv_obj_set_style_text_font(objects.cn_dropdown, lg_page_sourceHanSans_14, LV_PART_MAIN);
    lv_obj_set_style_text_font(objects.cn_dropdown, LV_FONT_DEFAULT, LV_PART_INDICATOR);
    lv_obj_t *dropdown_list = lv_dropdown_get_list(objects.cn_dropdown);
    if (dropdown_list) {
        lv_obj_set_style_text_font(dropdown_list, lg_page_sourceHanSans_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    group_dd_language = lv_group_create();
    lv_group_add_obj(group_dd_language, objects.cn_dropdown);
    

    return objects.language_page;
}

void language_page_created_callback(void *args) {
    lv_group_focus_obj(objects.cn_dropdown);
}

void language_destroy(void *args) {
    lv_obj_delete(objects.cn_dropdown);
    lv_binfont_destroy(lg_page_sourceHanSans_14);

    lv_group_remove_all_objs(group_dd_language);
    lv_group_delete(group_dd_language);
}

void language_key_callback(lv_event_t * e) {

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP: {
        uint32_t dd_sel = lv_dropdown_get_selected(objects.cn_dropdown);
        if ((0 < dd_sel) &&
            lv_dropdown_is_open(objects.cn_dropdown)) {
            lv_dropdown_set_selected(objects.cn_dropdown, dd_sel - 1);
        }
    } break;
    case LV_KEY_DOWN: {
        uint32_t dd_sel = lv_dropdown_get_selected(objects.cn_dropdown);
        if ((dd_sel < lv_dropdown_get_option_count(objects.cn_dropdown) - 1) &&
            lv_dropdown_is_open(objects.cn_dropdown)) {
            lv_dropdown_set_selected(objects.cn_dropdown, dd_sel + 1);
        }
    } break;
    case LV_KEY_ENTER: {
        if (lv_dropdown_is_open(objects.cn_dropdown)) {
            lv_dropdown_close(objects.cn_dropdown);
        } else {
            lv_dropdown_open(objects.cn_dropdown);
        }
    } break;
    case LV_KEY_ESC: {
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        config.language = lv_dropdown_get_selected(objects.cn_dropdown);
        hardware_api_handler->set_config(&config);
        // hardware_api_handler->setSleepThreshold(config.sleep_index);
        UIFW_async_switch_to_page(&settings_page);
    } break;
    default:
        break;
    }
}

struct PageControler language_page = {
    .create_page           = language_create,
    .delete_page           = language_destroy,
    .key_event_callback    = language_key_callback,
    .page_created_callback = language_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_NONE};
