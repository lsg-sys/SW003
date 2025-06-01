#include "button_if.h"


#if BUTTON_mcu_stm32f411ce

    #include "gpio.h"

    typedef struct {
        GPIO_TypeDef* port;
        uint16_t pin;
    } stm32KeyInfo_t;

    stm32KeyInfo_t stm32keys[] = {
        #if (BUTTON_button_number_1||BUTTON_button_number_2||BUTTON_button_number_3)
        {BUTTON_KEY1_GPIO_Port, BUTTON_KEY1_Pin},
        #endif /* (BUTTON_button_number_1||BUTTON_button_number_2||BUTTON_button_number_3) */
        #if (BUTTON_button_number_2||BUTTON_button_number_3)
        {BUTTON_KEY2_GPIO_Port, BUTTON_KEY2_Pin},
        #endif /* (BUTTON_button_number_2||BUTTON_button_number_3) */
        #if BUTTON_button_number_3
        {BUTTON_KEY3_GPIO_Port, BUTTON_KEY3_Pin},
        #endif /* BUTTON_button_number_3 */
    };

    int button_read_key(int index) {
        return (int)(HAL_GPIO_ReadPin(stm32keys[index].port, stm32keys[index].pin) == GPIO_PIN_RESET);
    }

    /**
     * @param arg : pointer of GPIO_Pin
     */
    int button_if_whether_pass_event(void *arg) {
        int is_pass = 1;
        uint16_t pin = *(uint16_t *)arg;
        #if BUTTON_button_number_1
        {
            if (BUTTON_KEY1_Pin == pin) {
                is_pass = 0;
            }
        }
        #endif /* BUTTON_button_number_1 */
        #if BUTTON_button_number_2
        {
            if (BUTTON_KEY1_Pin == pin || BUTTON_KEY2_Pin == pin) {
                is_pass = 0;
            }
        }
        #endif /* BUTTON_button_number_2 */
        #if BUTTON_button_number_3
        {
            if (BUTTON_KEY1_Pin == pin || BUTTON_KEY2_Pin == pin || BUTTON_KEY3_Pin == pin) {
                is_pass = 0;
            }
        }
        #endif /* BUTTON_button_number_3 */
        return is_pass;
    }

#endif /* BUTTON_mcu_stm32f411ce */
#if BUTTON_mcu_esp32s3

    int button_read_key(int index) {
        return 0;
    }
    #endif /* BUTTON_mcu_esp32s3 */
    #if BUTTON_mcu_linuxDebug
    int button_read_key(int index) {
        return 0;
    }
    
#endif /* BUTTON_mcu_linuxDebug */

#if BUTTON_lowpower_on
    // ...
    static uint32_t scan_timer_is_running = 0;
    struct ButtonScanTimerControler const *prt_scan_timer_controler = NULL;

    void button_if_register_scan_timer(struct ButtonScanTimerControler const *scan_timer_controler) {
        prt_scan_timer_controler = scan_timer_controler;
    }

    int button_if_scan_timer_init(void) {
        return prt_scan_timer_controler->init();
    }

    int button_if_scan_timer_open(void) {
        int ret = 0;
        if (scan_timer_is_running == 0) {
            scan_timer_is_running = 1;
            ret = prt_scan_timer_controler->open();
        }
        return ret;
    }

    void button_if_scan_timer_close(void) {
        if (scan_timer_is_running == 1) {
            scan_timer_is_running = 0;
            prt_scan_timer_controler->close();
        }        
    }
#endif /* BUTTON_lowpower_on */
#if BUTTON_lowpower_off
    // ...
#endif /* BUTTON_lowpower_off */





