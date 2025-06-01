#include "eez_page.h"

#include "settings_page.h"
#include "ui/ui_framework.h"

#include "eez/ui/screens.h"

lv_obj_t * eez_create(void *args) {

    create_screen_main_page();
    return objects.main_page;
}

void eez_page_timer_callback(lv_timer_t * timer) {
    uint32_t voltage;
    hardware_api_handler->get_voltage(&voltage);
    lv_arc_set_value(objects.main_arc_bat, (int32_t)voltage);
    lv_label_set_text_fmt(objects.main_lab_bat, "%02d", (int)voltage);

    uint32_t degreeC;
    hardware_api_handler->get_degreeC(&degreeC);
    lv_arc_set_value(objects.main_arc_tmprt, (int32_t)degreeC);
    lv_label_set_text_fmt(objects.main_lab_tmprt, "%02d", (int)degreeC);

    date_and_time_t date_and_time;
    hardware_api_handler->get_date_and_time(&date_and_time);
    lv_label_set_text_fmt(objects.main_lab_hour_min, "%02d:%02d", date_and_time.s.hour, date_and_time.s.min);
    lv_label_set_text_fmt(objects.main_lab_sec, "%02d", date_and_time.s.sec);
}

lv_timer_t * eez_page_timer = NULL;

void eez_page_created_callback(void *args) {
    eez_page_timer = lv_timer_create(eez_page_timer_callback, 300, NULL);
}

void eez_destroy(void *args) {
    lv_timer_delete(eez_page_timer);
}

void eez_key_callback(lv_event_t * e) {

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_ENTER:
        UIFW_async_switch_to_page(&settings_page);
        break;
    default:
        break;
    }
}

struct PageControler eez_page = {
    .create_page           = eez_create,
    .delete_page           = eez_destroy,
    .key_event_callback    = eez_key_callback,
    .page_created_callback = eez_page_created_callback,
    .anim                  = LV_SCR_LOAD_ANIM_NONE};
