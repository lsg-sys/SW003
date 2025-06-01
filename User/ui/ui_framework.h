#ifndef __UI_FRAMEWORK_H
#define __UI_FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui/page/page_def.h"

#include "utility/format.h"

typedef struct {
    int (*get_date_and_time)(date_and_time_t *date_time);
    int (*set_date_and_time)(date_and_time_t *date_time);
    uint32_t (*get_lightness)(void);
    void (*set_lightness)(uint32_t lightness);
    int (*get_config)(SmartWatchesConfig_t *config);
    int (*set_config)(SmartWatchesConfig_t *config);
    void (*setSleepThreshold)(uint32_t sleepThreshold);
    void (*get_voltage)(uint32_t *voltage);
    void (*get_degreeC)(uint32_t *degreeC);
    void (*system_reset)(void);
} hardware_api_t;

extern struct PageControler clock_page;
extern struct PageControler settings_page;
extern struct PageControler sleep_page;
extern struct PageControler wearable_mode_page;
extern struct PageControler sync_time_page;
extern struct PageControler brightness_page;
extern struct PageControler eez_page;
extern struct PageControler language_page;
extern struct PageControler themes_page;
extern struct PageControler clock_style_page;
extern struct PageControler clock_2_page;

void app_ui_entry(void* indev_keypad, hardware_api_t * p_hw_api);
void UIFW_async_switch_to_page(struct PageControler const * page_ctrl);
void UIFW_tick(void);

/* ------------------------------------------------------------------ */

extern hardware_api_t *hardware_api_handler;

void switch_to_page(app_page_controler_t const * new_page_ctrl);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __UI_FRAMEWORK_H */