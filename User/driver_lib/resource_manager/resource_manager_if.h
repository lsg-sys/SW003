#ifndef __RESOURCE_MANAGER_IF_H__
#define __RESOURCE_MANAGER_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param MAX_RESOURCES string
*/
#define RESOURCE_MANAGER_MAX_RESOURCES      10

/**
* @param mcu object
*/
#define RESOURCE_MANAGER_mcu_stm32f411ce      1
#define RESOURCE_MANAGER_mcu_esp32s3      0
#define RESOURCE_MANAGER_mcu_linuxDebug      0
#if RESOURCE_MANAGER_mcu_stm32f411ce

    #include "cmsis_os2.h"
    #include "cmsis_compiler.h"
    #include "FreeRTOS.h"
    #include "timers.h"

    #define RESOURCE_MANAGER_get_time_stamp_ms() xTaskGetTickCount()

    /**
     * 关闭全局中断的宏（需根据具体平台调整）
     */
    static inline uint32_t DISABLE_INTERRUPTS(void) { 
        if (__get_IPSR() == 0U) { // not in ISR
            portENTER_CRITICAL();
            return 0;
        } else {
            return portSET_INTERRUPT_MASK_FROM_ISR();
        }
    }
    static inline void ENABLE_INTERRUPTS(uint32_t x) {
        if (__get_IPSR() == 0U) {
            portEXIT_CRITICAL();
        } else {
            portCLEAR_INTERRUPT_MASK_FROM_ISR(x);
        }
    }

    /**
     * @param serial_fun object
     */
    #define RESOURCE_MANAGER_serial_fun_off      1
    #define RESOURCE_MANAGER_serial_fun_stdio      0
    #define RESOURCE_MANAGER_serial_fun_print      0
    #define RESOURCE_MANAGER_serial_fun_custom      0
    #if RESOURCE_MANAGER_serial_fun_off
        #define resource_manager_if_printf(...) do { } while (0)
    #endif /* RESOURCE_MANAGER_serial_fun_off */
    #if RESOURCE_MANAGER_serial_fun_stdio
        #include <stdio.h>
        #define resource_manager_if_printf(...) printf(__VA_ARGS__)
    #endif /* RESOURCE_MANAGER_serial_fun_stdio */
    #if RESOURCE_MANAGER_serial_fun_print
        /**
        * @param print reference
        */
        #include "../print/print.h"
        #define resource_manager_if_printf(...) print(__VA_ARGS__)
    #endif /* RESOURCE_MANAGER_serial_fun_print */
    #if RESOURCE_MANAGER_serial_fun_custom
        #define resource_manager_if_printf(...) do { } while (0)
    #endif /* RESOURCE_MANAGER_serial_fun_custom */

    
    #define resource_manager_if_os_sleep(s) osDelay(pdMS_TO_TICKS(s*1000))


#endif /* RESOURCE_MANAGER_mcu_stm32f411ce */
#if RESOURCE_MANAGER_mcu_esp32s3

    // #define DISABLE_INTERRUPTS() __disable_irq()
    // #define ENABLE_INTERRUPTS() __enable_irq()

    
    // #define resource_manager_if_os_sleep(s) HAL_Delay(s*1000)

#endif /* RESOURCE_MANAGER_mcu_esp32s3 */
#if RESOURCE_MANAGER_mcu_linuxDebug
    #include <unistd.h>

    int resource_manager_simulate_global_interrupts_init(void);
    void resource_manager_simulate_global_interrupts_deinit(void);
    void resource_manager_simulate_closing_global_interrupts(void);
    void resource_manager_simulate_opening_global_interrupts(void);
    
    static inline uint32_t DISABLE_INTERRUPTS(void) { 
        resource_manager_simulate_closing_global_interrupts();
        return (0);
    }
    static inline void ENABLE_INTERRUPTS(uint32_t x) {
        (void)x;
        resource_manager_simulate_opening_global_interrupts();
    }

    /**
     * @param serial_fun object
     */
    #define RESOURCE_MANAGER_serial_fun_off      1
    #define RESOURCE_MANAGER_serial_fun_stdio      0
    #if RESOURCE_MANAGER_serial_fun_off
        #define resource_manager_if_printf(...) do { } while (0)
    #endif /* RESOURCE_MANAGER_serial_fun_off */
    #if RESOURCE_MANAGER_serial_fun_stdio
        #include <stdio.h>
        #define resource_manager_if_printf(...) printf(__VA_ARGS__)
    #endif /* RESOURCE_MANAGER_serial_fun_stdio */

    #define resource_manager_if_os_sleep(s) sleep(s)
#endif /* RESOURCE_MANAGER_mcu_linuxDebug */

/**
* @param timer_period_ms string
*/
#define RESOURCE_MANAGER_timer_period_ms      100

/**
* @param timer_mode object
*/
#define RESOURCE_MANAGER_timer_mode_always_on      0
#define RESOURCE_MANAGER_timer_mode_resource_manager      1
#if RESOURCE_MANAGER_timer_mode_always_on
    // ...
#endif /* RESOURCE_MANAGER_timer_mode_always_on */
#if RESOURCE_MANAGER_timer_mode_resource_manager
    // ...
#endif /* RESOURCE_MANAGER_timer_mode_resource_manager */



#ifdef __cplusplus
}
#endif

#endif /* __RESOURCE_MANAGER_IF_H__ */
