#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include <stdbool.h>

#include "button_if.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    KEY_IDLE,        // 空闲状态
    KEY_PRESSED,     // 按下状态
    KEY_RELEASED,    // 释放状态
    KEY_LONG_PRESSED,// 长按状态
    KEY_DOUBLE_CLICK,// 双击状态
} KeyState_t;


typedef struct {

    uint32_t prev_state;         // 上一次按键状态
#if (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click)
    uint32_t press_time;         // 第一次按下时间,用于长按判断
#endif /* (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click) */
#if BUTTON_feature_single_double_long_click
    uint32_t release_time;       // 释放时间,用于双击判断
#endif /* BUTTON_feature_single_double_long_click */
#if BUTTON_filter_threshold_counter
    ThresholdFilter cf;          // 滤波器
#endif /* BUTTON_filter_threshold_counter */
#if BUTTON_filter_moving_average
    #error "undef BUTTON_filter_moving_average"
#endif /* BUTTON_filter_moving_average */
#if BUTTON_lowpower_on
    uint32_t last_active_time;
#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
#endif /* BUTTON_lowpower_off */
#if BUTTON_app_handle_mode_polling
    uint8_t is_long_press;       // 是否触发长按
    uint8_t is_single_click;     // 是否触发单击
    uint8_t is_double_click;     // 是否触发双击
#endif /* BUTTON_app_handle_mode_polling */
#if BUTTON_app_handle_mode_callback
    void (*handle_long_press)(void);
    void (*handle_single_click)(void);
    void (*handle_double_click)(void);
#endif /* BUTTON_app_handle_mode_callback */

} ButtonConfig_t;



#if BUTTON_app_handle_mode_polling
static inline void button_polling_handler(ButtonConfig_t *instance,
    void (*long_press_fn)(void),
    void (*single_click_fn)(void),
    void (*double_click_fn)(void)) {

    if (instance->is_long_press) {
        long_press_fn();
        instance->is_long_press = 0;
    }
    if (instance->is_single_click) {
        single_click_fn();
        instance->is_single_click = 0;
    }
    if (instance->is_double_click) {
        double_click_fn();
        instance->is_double_click = 0;
    }
}
#endif /* BUTTON_app_handle_mode_polling */
#if BUTTON_app_handle_mode_callback
// static inline void register_long_press_callback(ButtonConfig_t *instance, void (*cb)(void)) {instance->handle_long_press = cb;}
// static inline void register_single_click_callback(ButtonConfig_t *instance, void (*cb)(void)) {instance->handle_single_click = cb;}
// static inline void register_double_click_callback(ButtonConfig_t *instance, void (*cb)(void)) {instance->handle_double_click = cb;}

struct ButtonEventCallbacks {
    void (*long_press)(void);
    void (*single_click)(void);
    void (*double_click)(void);
};

void button_register_event_callbacks(uint32_t                           key_index,
                                     struct ButtonEventCallbacks const *callbacks);
#endif /* BUTTON_app_handle_mode_callback */



// void button_scan(ButtonConfig_t *instance, bool current_is_active);
int button_min_preferred_response_time(void);
void button_PeriodElapsedCallback(void);

#if BUTTON_lowpower_on
    void button_external_trigger_input_event(void *arg);
    int button_init(struct ButtonScanTimerControler const *scan_timer_controler);
#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
    // ...
    int button_init(void);
#endif /* BUTTON_lowpower_off */


#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H__ */
