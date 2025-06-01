#ifndef __BUTTON_IF_H__
#define __BUTTON_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
* @param app_handle_mode object
*/
#define BUTTON_app_handle_mode_polling      0
#define BUTTON_app_handle_mode_callback      1
#if BUTTON_app_handle_mode_polling
    // ...
#endif /* BUTTON_app_handle_mode_polling */
#if BUTTON_app_handle_mode_callback
    // ...
#endif /* BUTTON_app_handle_mode_callback */

/**
* @param feature object
*/
#define BUTTON_feature_single_click      0
#define BUTTON_feature_single_long_click      0
#define BUTTON_feature_single_double_long_click      1
#if BUTTON_feature_single_click
    // ...
#endif /* BUTTON_feature_single_click */
#if BUTTON_feature_single_long_click
    // ...
    
    /**
    * @param scan_timer_period string
    * 双击窗口，单位：ms， 默认10ms
    */
    #define BUTTON_scan_timer_period      10

    /**
    * @param long_press_time string
    * 长按阈值，单位：ms， 默认1000ms
    */
    #define BUTTON_long_press_time      1000

#endif /* BUTTON_feature_single_long_click */
#if BUTTON_feature_single_double_long_click
    // ...
    /**
    * @param scan_timer_period string
    * 双击窗口，单位：ms， 默认10ms
    */
    #define BUTTON_scan_timer_period      10

    /**
    * @param double_click_window string
    * 双击窗口，单位：ms， 默认500ms
    */
    #define BUTTON_double_click_window      500

    /**
    * @param long_press_time string
    * 长按阈值，单位：ms， 默认1000ms
    */
    #define BUTTON_long_press_time      1000

#endif /* BUTTON_feature_single_double_long_click */

/**
* @param filter object
*/
#define BUTTON_filter_threshold_counter      1
#define BUTTON_filter_moving_average      0
#if BUTTON_filter_threshold_counter
    /**
    * @param threshold_counter_filter reference
    */
    #include "../threshold_counter_filter/threshold_counter_filter.h"
#endif /* BUTTON_filter_threshold_counter */
#if BUTTON_filter_moving_average
    #include "../moving_average/moving_average.h"
    #error "undef BUTTON_filter_moving_average"
#endif /* BUTTON_filter_moving_average */

/**
* @param mcu object
*/
#define BUTTON_mcu_stm32f411ce      1
#define BUTTON_mcu_esp32s3      0
#define BUTTON_mcu_linuxDebug      0
#if BUTTON_mcu_stm32f411ce
    /**
    * @param button_number object
    */
    #define BUTTON_button_number_1      0
    #define BUTTON_button_number_2      0
    #define BUTTON_button_number_3      1
    #if BUTTON_button_number_1
        #define BUTTON_button_number 1
        /**
         * @param KEY1_GPIO_Port string
         * @param KEY1_Pin string
         */
        #define BUTTON_KEY1_GPIO_Port      GPIOA
        #define BUTTON_KEY1_Pin      GPIO_PIN_0
    #endif /* BUTTON_button_number_1 */
    #if BUTTON_button_number_2
        #define BUTTON_button_number 2
        /**
         * @param KEY1_GPIO_Port string
         * @param KEY1_Pin string
         * @param KEY2_GPIO_Port string
         * @param KEY2_Pin string
         */
        #define BUTTON_KEY1_GPIO_Port      GPIOA
        #define BUTTON_KEY1_Pin      GPIO_PIN_0
        #define BUTTON_KEY2_GPIO_Port      GPIOA
        #define BUTTON_KEY2_Pin      GPIO_PIN_1
    #endif /* BUTTON_button_number_2 */
    #if BUTTON_button_number_3
        #define BUTTON_button_number 3
        /**
         * @param KEY1_GPIO_Port string
         * @param KEY1_Pin string
         * @param KEY2_GPIO_Port string
         * @param KEY2_Pin string
         * @param KEY3_GPIO_Port string
         * @param KEY3_Pin string
         */
        #define BUTTON_KEY1_GPIO_Port      GPIOA
        #define BUTTON_KEY1_Pin      GPIO_PIN_0
        #define BUTTON_KEY2_GPIO_Port      GPIOA
        #define BUTTON_KEY2_Pin      GPIO_PIN_1
        #define BUTTON_KEY3_GPIO_Port      GPIOA
        #define BUTTON_KEY3_Pin      GPIO_PIN_2
    #endif /* BUTTON_button_number_3 */
   
    int button_read_key(int index);
    int button_if_whether_pass_event(void *arg);



#endif /* BUTTON_mcu_stm32f411ce */
#if BUTTON_mcu_esp32s3
    /**
    * @param button_number object
    */
   #define BUTTON_button_number_1      0
   #define BUTTON_button_number_2      0
   #define BUTTON_button_number_3      1
   #if BUTTON_button_number_1
       #define BUTTON_button_number 1
   #endif /* BUTTON_button_number_1 */
   #if BUTTON_button_number_2
       #define BUTTON_button_number 2
   #endif /* BUTTON_button_number_2 */
   #if BUTTON_button_number_3
       #define BUTTON_button_number 3
   #endif /* BUTTON_button_number_3 */
   
   int button_read_key(int index);
   int button_if_whether_pass_event(void *arg);


#endif /* BUTTON_mcu_esp32s3 */
#if BUTTON_mcu_linuxDebug
    // ...
    /**
    * @param button_number object
    */
   #define BUTTON_button_number_1      0
   #define BUTTON_button_number_2      0
   #define BUTTON_button_number_3      1
   #if BUTTON_button_number_1
       #define BUTTON_button_number 1
   #endif /* BUTTON_button_number_1 */
   #if BUTTON_button_number_2
       #define BUTTON_button_number 2
   #endif /* BUTTON_button_number_2 */
   #if BUTTON_button_number_3
       #define BUTTON_button_number 3
   #endif /* BUTTON_button_number_3 */
   
    int button_read_key(int index);
    int button_if_whether_pass_event(void *arg);

#endif /* BUTTON_mcu_linuxDebug */


/**
* @param lowpower object
* 在低功耗模式下，默认扫描定时器是关闭的，按键中断输入，
* 将打开扫描定时器，一段时间没有按键输入，将关闭扫描定时器。
* 在非低功耗模式下，扫描定时器一直打开。
*/
#define BUTTON_lowpower_on      1
#define BUTTON_lowpower_off      0
#if BUTTON_lowpower_on

    struct ButtonScanTimerControler {
        int (*init)(void);
        int (*open)(void);
        void (*close)(void);
    };

    void button_if_register_scan_timer(struct ButtonScanTimerControler const *scan_timer_controler);
    int button_if_scan_timer_init(void);
    int button_if_scan_timer_open(void);
    void button_if_scan_timer_close(void);

    /**
    * @param idle_timeout_ms string
    */
    #define BUTTON_idle_timeout_ms      3000


#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
    // ...
#endif /* BUTTON_lowpower_off */

/**
* @param tick_timer object
*/
#define BUTTON_tick_timer_dependent      1
#define BUTTON_tick_timer_independent      0
#if BUTTON_tick_timer_dependent
    // ...
#endif /* BUTTON_tick_timer_dependent */
#if BUTTON_tick_timer_independent
    #define get_tick_ms() HAL_GetTick()
#endif /* BUTTON_tick_timer_independent */

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_IF_H__ */
