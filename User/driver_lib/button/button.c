#include "button.h"
#include "button_if.h"


static ButtonConfig_t buttons[BUTTON_button_number];




#if BUTTON_tick_timer_dependent
    static uint32_t local_tick_ms;
    #define incarement_tick_ms() do {local_tick_ms += BUTTON_scan_timer_period;} while(0) 
    #define get_tick_ms() local_tick_ms
#endif /* BUTTON_tick_timer_dependent */
#if BUTTON_tick_timer_independent
    // ...
#endif /* BUTTON_tick_timer_independent */





#include <stdio.h>
// #define debug_log(...) printf(__VA_ARGS__)
#define debug_log(...) do {} while (0)




#if BUTTON_app_handle_mode_polling
    // ...
#endif /* BUTTON_app_handle_mode_polling */
#if BUTTON_app_handle_mode_callback
    // ...

    void button_register_event_callbacks(uint32_t                           key_index,
                                         struct ButtonEventCallbacks const *callbacks) {

        if (key_index < BUTTON_button_number &&
            NULL != callbacks) {
            buttons[key_index].handle_long_press   = callbacks->long_press;
            buttons[key_index].handle_single_click = callbacks->single_click;
            buttons[key_index].handle_double_click = callbacks->double_click;
        }
    }
#endif /* BUTTON_app_handle_mode_callback */


void button_scan(ButtonConfig_t *instance, bool current_is_active) {

    // 状态机逻辑
    switch (instance->prev_state) {
    case KEY_IDLE:
        if (current_is_active) {
            instance->prev_state = KEY_PRESSED;
            debug_log("IDLE --> PRESSED\n");
            #if (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click)
            instance->press_time = get_tick_ms();
            #endif /* (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click) */

            #if BUTTON_lowpower_on
            instance->last_active_time = get_tick_ms();
            #endif /* BUTTON_lowpower_on */
            #if BUTTON_lowpower_off
            #endif /* BUTTON_lowpower_off */
        } else {
        }
        break;

    case KEY_PRESSED:
        if (current_is_active) {
            #if (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click)
                // 检查长按触发
                if ((get_tick_ms() - instance->press_time) >= BUTTON_long_press_time) {
                    instance->prev_state = KEY_LONG_PRESSED;
                    debug_log("PRESSED --> LONG_PRESSED\n");
                    #if BUTTON_app_handle_mode_polling
                        instance->is_long_press = 1;
                    #endif /* BUTTON_app_handle_mode_polling */
                    #if BUTTON_app_handle_mode_callback
                        instance->handle_long_press();
                    #endif /* BUTTON_app_handle_mode_callback */
                }
            #endif /* (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click) */
            #if BUTTON_feature_single_click
                // 什么都不用做
            #endif /* BUTTON_feature_single_click */
        } else {
            instance->prev_state   = KEY_RELEASED;
            debug_log("PRESSED --> RELEASED\n");
        }
        break;

    case KEY_RELEASED:
        #if (BUTTON_feature_single_click | BUTTON_feature_single_long_click)
            instance->prev_state = KEY_IDLE;
            debug_log("RELEASED --> IDLE\n");
            #if BUTTON_app_handle_mode_polling
                instance->is_single_click = 1;
            #endif /* BUTTON_app_handle_mode_polling */
            #if BUTTON_app_handle_mode_callback
                instance->handle_single_click();
            #endif /* BUTTON_app_handle_mode_callback */
        #endif /* (BUTTON_feature_single_click | BUTTON_feature_single_long_click) */
        #if BUTTON_feature_single_double_long_click
            // 检查是否是双击
            debug_log("diff time: %d\n", get_tick_ms() - instance->release_time);
            if ((get_tick_ms() - instance->release_time) <= BUTTON_double_click_window) {
                instance->prev_state = KEY_DOUBLE_CLICK;
                debug_log("RELEASED --> DOUBLE_CLICK\n");
                #if BUTTON_app_handle_mode_polling
                    instance->is_double_click = 1;
                #endif /* BUTTON_app_handle_mode_polling */
                #if BUTTON_app_handle_mode_callback
                    instance->handle_double_click();
                #endif /* BUTTON_app_handle_mode_callback */
            } else {
                instance->prev_state = KEY_IDLE;
                debug_log("RELEASED --> IDLE\n");
                #if BUTTON_app_handle_mode_polling
                    instance->is_single_click = 1;
                #endif /* BUTTON_app_handle_mode_polling */
                #if BUTTON_app_handle_mode_callback
                    instance->handle_single_click();
                #endif /* BUTTON_app_handle_mode_callback */
            }
            instance->release_time = get_tick_ms();
        #endif /* BUTTON_feature_single_double_long_click */
        break;
    
    #if (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click)
    case KEY_LONG_PRESSED:
        if (current_is_active) {
            // 什么都不做，等待释放
        } else {
            instance->prev_state = KEY_IDLE;
            debug_log("LONG_PRESSED --> IDLE\n");
        }
        break;
    #endif /* (BUTTON_feature_single_long_click | BUTTON_feature_single_double_long_click) */
    
    #if BUTTON_feature_single_double_long_click
    case KEY_DOUBLE_CLICK:
        instance->prev_state = KEY_IDLE;
        debug_log("DOUBLE_CLICK --> IDLE\n");
        break;
    #endif /* BUTTON_feature_single_double_long_click */

    default:
        instance->prev_state = KEY_IDLE;
        debug_log("ERROR\n");
        break;
    }
}

#if BUTTON_lowpower_on
int button_init(struct ButtonScanTimerControler const* scan_timer_controler) {
    if (scan_timer_controler == NULL || 
        scan_timer_controler->init == NULL || 
        scan_timer_controler->open == NULL || 
        scan_timer_controler->close == NULL) {
        return (-1);
    }
    button_if_register_scan_timer(scan_timer_controler);

    if (button_if_scan_timer_init() != 0) {
        return (-1);
    }

    for (int i = 0; i < BUTTON_button_number; i++) {
        buttons[i] = (ButtonConfig_t){0};
        buttons[i].prev_state = KEY_IDLE;
        init_threshold_counter_filter(&buttons[i].cf);
    }

    return (0);
}
#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
int button_init(void) {

    for (int i = 0; i < BUTTON_button_number; i++) {
        buttons[i] = (ButtonConfig_t){0};
        buttons[i].prev_state = KEY_IDLE;
        init_threshold_counter_filter(&buttons[i].cf);
    }
}
#endif /* BUTTON_lowpower_off */


int button_min_preferred_response_time(void) {
    #if BUTTON_filter_threshold_counter
        return BUTTON_scan_timer_period * (THRESHOLD_COUNTER_FILTER_threshold_high + 3 /* IDLE + PRESSED + RELEASED */);
    #endif /* BUTTON_filter_threshold_counter */
    #if BUTTON_filter_moving_average
        #error "undef BUTTON_filter_moving_average"
    #endif /* BUTTON_filter_moving_average */
}

void button_PeriodElapsedCallback(void) {

    #if BUTTON_tick_timer_dependent
        incarement_tick_ms();
    #endif /* BUTTON_tick_timer_dependent */

    #if BUTTON_lowpower_on
    {
        bool is_idle = true;
        for (int i = 0; i < BUTTON_button_number; i++) {
            button_scan(&buttons[i], button_read_key(i));
            if (get_tick_ms() -  buttons[i].last_active_time  <=  BUTTON_idle_timeout_ms) {
                is_idle = false;
            }
        }
        if (is_idle) {
            button_if_scan_timer_close();
        }
    }
    #endif /* BUTTON_lowpower_on */
    #if BUTTON_lowpower_off
    {
        for (int i = 0; i < BUTTON_button_number; i++) {
            button_scan(&buttons[i], button_read_key(i));
        }
    }
    #endif /* BUTTON_lowpower_off */
}

#if BUTTON_lowpower_on
    void button_external_trigger_input_event(void *arg) {

        int pass_event = button_if_whether_pass_event(arg);
        if (pass_event) {
        } else {
            for (int i = 0; i < BUTTON_button_number; i++) {
                buttons[i].last_active_time = get_tick_ms();
            }
            
            button_if_scan_timer_open();
        }
    }
#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
    // ...
#endif /* BUTTON_lowpower_off */


/* ------------------ begin of test ------------------ */


#if BUTTON_mcu_linuxDebug

#include <stdio.h>
static void handle_single_click(void) {
    printf("single click\n");
}

static void handle_double_click(void) {
    printf("double click\n");
}

static void handle_long_press(void) {
    printf("long press\n");
}



#define signal_length 500

int noise_10percent[signal_length] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
int noise_20percent[signal_length] = {
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1,
};



static void print_signal(int arry[], int len) {
    for (int i = 0; i < len; i++) {
        // char c = arry[i] ? '#' : '_';
        char c = arry[i] ^ noise_10percent[i] ? '#' : '_';
        printf("%c", c);
    }
    printf("\n");
}

void test_button(void) {
    
    ButtonConfig_t test_buttons[3];

    for (int i = 0; i < sizeof(test_buttons)/sizeof(test_buttons[0]); i++) {
        button_init(&test_buttons[i]);
#if BUTTON_app_handle_mode_callback
        register_long_press_callback(&test_buttons[i], handle_long_press);
        register_double_click_callback(&test_buttons[i], handle_double_click);
        register_single_click_callback(&test_buttons[i], handle_single_click);
#endif /* BUTTON_app_handle_mode_callback */
    }

    int testArray[sizeof(test_buttons)/sizeof(test_buttons[0])][signal_length] = {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        }, // 模拟按键长按
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        }, // 模拟单击
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        }, // 模拟双击
    };

    
    printf("min preferred response time: %d ms\n", button_min_preferred_response_time());

    // 模拟定时扫描，设计应用时放在硬件定时器中断或者软件定时器回调，定时器为固定周期
    printf("long press test: \n");
    print_signal(testArray[0], signal_length);
    for (int i = 0; i < signal_length; i++) {

        #if BUTTON_tick_timer_dependent
        incarement_tick_ms();
        #endif /* BUTTON_tick_timer_dependent */
        button_scan(&test_buttons[0], (bool)filter(&test_buttons[0].cf, testArray[0][i] ^ noise_10percent[i]));
        #if BUTTON_app_handle_mode_polling
            button_polling_handler(&test_buttons[0], handle_long_press, handle_single_click, handle_double_click);
        #endif /* BUTTON_app_handle_mode_polling */
    }
    printf("single click test: \n");
    print_signal(testArray[1], signal_length);
    for (int i = 0; i < signal_length; i++) {

        #if BUTTON_tick_timer_dependent
        incarement_tick_ms();
        #endif /* BUTTON_tick_timer_dependent */
        button_scan(&test_buttons[0], (bool)filter(&test_buttons[1].cf, testArray[1][i] ^ noise_10percent[i]));
        #if BUTTON_app_handle_mode_polling
            button_polling_handler(&test_buttons[0], handle_long_press, handle_single_click, handle_double_click);
        #endif /* BUTTON_app_handle_mode_polling */
    }
    printf("double click test: \n");
    print_signal(testArray[2], signal_length);
    for (int i = 0; i < signal_length; i++) {

        #if BUTTON_tick_timer_dependent
        incarement_tick_ms();
        #endif /* BUTTON_tick_timer_dependent */
        // printf("time: %d\n", get_tick_ms());
        button_scan(&test_buttons[0], (bool)filter(&test_buttons[2].cf, testArray[2][i] ^ noise_10percent[i]));
        #if BUTTON_app_handle_mode_polling
            button_polling_handler(&test_buttons[0], handle_long_press, handle_single_click, handle_double_click);
        #endif /* BUTTON_app_handle_mode_polling */
    }
}

#endif /* BUTTON_mcu_linuxDebug */


/* ------------------ end of test ------------------ */