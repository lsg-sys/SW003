#ifndef __PERIODIC_TIMER_H__
#define __PERIODIC_TIMER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param mcu object
*/
#define PERIODIC_TIMER_mcu_stm32f411ce      1
#define PERIODIC_TIMER_mcu_esp32s3      0
#define PERIODIC_TIMER_mcu_linuxDebug      0
#if PERIODIC_TIMER_mcu_stm32f411ce

    /**
     * @param serial_fun object
     */
    #define PERIODIC_TIMER_serial_fun_off      1
    #define PERIODIC_TIMER_serial_fun_stdio      0
    #define PERIODIC_TIMER_serial_fun_print      0
    #define PERIODIC_TIMER_serial_fun_custom      0
    #if PERIODIC_TIMER_serial_fun_off
        #define periodic_timer_if_printf(...) do { } while (0)
    #endif /* PERIODIC_TIMER_serial_fun_off */
    #if PERIODIC_TIMER_serial_fun_stdio
        #include <stdio.h>
        #define periodic_timer_if_printf(...) printf(__VA_ARGS__)
    #endif /* PERIODIC_TIMER_serial_fun_stdio */
    #if PERIODIC_TIMER_serial_fun_print
        /**
        * @param print reference
        */
        #include "../print/print.h"
        #define periodic_timer_if_printf(...) print(__VA_ARGS__)
    #endif /* PERIODIC_TIMER_serial_fun_print */
    #if PERIODIC_TIMER_serial_fun_custom
        #define periodic_timer_if_printf(...) do { } while (0)
    #endif /* PERIODIC_TIMER_serial_fun_custom */

    /**
    * @param os object
    */
    #define PERIODIC_TIMER_os_None      0
    #define PERIODIC_TIMER_os_FreeRTOS      1
    #if PERIODIC_TIMER_os_None
        // ...
        #include "tim.h"
        /**
        * @param tim_number object
        */
        #define PERIODIC_TIMER_tim_number_tim1      0
        #define PERIODIC_TIMER_tim_number_tim2      0
        #define PERIODIC_TIMER_tim_number_tim3      0
        #define PERIODIC_TIMER_tim_number_tim4      0
        #define PERIODIC_TIMER_tim_number_tim5      0
        #define PERIODIC_TIMER_tim_number_tim9      1
        #define PERIODIC_TIMER_tim_number_tim10      0
        #define PERIODIC_TIMER_tim_number_tim11      0
        #if PERIODIC_TIMER_tim_number_tim1
            #define PERIODIC_TIMER_tim_handle &htim1
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM1_Init
        #endif /* PERIODIC_TIMER_tim_number_tim1 */
        #if PERIODIC_TIMER_tim_number_tim2
            #define PERIODIC_TIMER_tim_handle &htim2
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM2_Init
        #endif /* PERIODIC_TIMER_tim_number_tim2 */
        #if PERIODIC_TIMER_tim_number_tim3
            #define PERIODIC_TIMER_tim_handle &htim3
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM3_Init
        #endif /* PERIODIC_TIMER_tim_number_tim3 */
        #if PERIODIC_TIMER_tim_number_tim4
            #define PERIODIC_TIMER_tim_handle &htim4
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM4_Init
        #endif /* PERIODIC_TIMER_tim_number_tim4 */
        #if PERIODIC_TIMER_tim_number_tim5
            #define PERIODIC_TIMER_tim_handle &htim5
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM5_Init
        #endif /* PERIODIC_TIMER_tim_number_tim5 */
        #if PERIODIC_TIMER_tim_number_tim9
            #define PERIODIC_TIMER_tim_handle &htim9
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM9_Init
        #endif /* PERIODIC_TIMER_tim_number_tim9 */
        #if PERIODIC_TIMER_tim_number_tim10
            #define PERIODIC_TIMER_tim_handle &htim10
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM10_Init
        #endif /* PERIODIC_TIMER_tim_number_tim10 */
        #if PERIODIC_TIMER_tim_number_tim11
            #define PERIODIC_TIMER_tim_handle &htim11
            #define PERIODIC_TIMER_MX_TIM_Init MX_TIM11_Init
        #endif /* PERIODIC_TIMER_tim_number_tim11 */
    
        #define periodic_timer_if_os_sleep(ms) HAL_Delay(ms*1000)

    #endif /* PERIODIC_TIMER_os_None */
    #if PERIODIC_TIMER_os_FreeRTOS
        // ...
        #include "FreeRTOS.h"
        #include "cmsis_os2.h"

        #define periodic_timer_if_os_sleep(ms) osDelay(pdMS_TO_TICKS(ms))

    #endif /* PERIODIC_TIMER_os_FreeRTOS */


#endif /* PERIODIC_TIMER_mcu_stm32f411ce */
#if PERIODIC_TIMER_mcu_esp32s3
    // ...
    #define periodic_timer_if_printf(...) do { } while (0)

    #define periodic_timer_if_os_sleep(ms) do { } while (0)

#endif /* PERIODIC_TIMER_mcu_esp32s3 */
#if PERIODIC_TIMER_mcu_linuxDebug
    // ...

    #include <pthread.h>
    #include <semaphore.h>
    #include <unistd.h>

    /**
     * @param serial_fun object
     */
    #define PERIODIC_TIMER_serial_fun_off      1
    #define PERIODIC_TIMER_serial_fun_stdio      0
    #if PERIODIC_TIMER_serial_fun_off
        #define periodic_timer_if_printf(...) do { } while (0)
    #endif /* PERIODIC_TIMER_serial_fun_off */
    #if PERIODIC_TIMER_serial_fun_stdio
        #include <stdio.h>
        #define periodic_timer_if_printf(...) printf(__VA_ARGS__)
    #endif /* PERIODIC_TIMER_serial_fun_stdio */

    #define periodic_timer_if_os_sleep(ms) sleep(ms)

#endif /* PERIODIC_TIMER_mcu_linuxDebug */
    
struct PeriodicTimer {
    #if PERIODIC_TIMER_mcu_linuxDebug
        sem_t sem;
        pthread_t thread;
        uint32_t exit_timer_thread_flag; // 其他线程通过open和close控制此flag通知线程退出
    #endif /* PERIODIC_TIMER_mcu_linuxDebug */
    uint32_t time_stamp_ms;
    uint32_t period_ms;
    void (*on_timer_callback)(void *arg);
    void *arg;
};

#define periodic_timer_printf(...) do {\
    periodic_timer_if_printf("⏰ periodic timer: ");\
    periodic_timer_if_printf(__VA_ARGS__);\
} while (0) 


int periodic_timer_init(void);
void periodic_timer_deinit(void);
int periodic_timer_open(struct PeriodicTimer *ptimer, 
    uint32_t period_ms, void (*callback)(void *arg)); 
void periodic_timer_close(struct PeriodicTimer *ptimer);
uint32_t periodic_timer_get_time_stamp_ms(struct PeriodicTimer *ptimer);

#ifdef __cplusplus
}
#endif

#endif /* __PERIODIC_TIMER_H__ */
