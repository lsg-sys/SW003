#include "periodic_timer.h"


static uint32_t timer_is_open;

#if PERIODIC_TIMER_mcu_stm32f411ce
    #include <assert.h>

    
    #if PERIODIC_TIMER_os_None
        static struct PeriodicTimer *pSwTimer;

        // ...
        /* manual register to `void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)` */
        void periodic_timer_irq_callback(TIM_HandleTypeDef *htim) {
            if ((uint32_t)(&(htim->Instance)) == (uint32_t)PERIODIC_TIMER_tim_handle) {
                pSwTimer->time_stamp_ms += pSwTimer->period_ms;
                if (NULL != pSwTimer->on_timer_callback) {
                    pSwTimer->on_timer_callback(pSwTimer->arg);
                }
            }
        }
    #endif /* PERIODIC_TIMER_os_None */
    #if PERIODIC_TIMER_os_FreeRTOS
        // ...
        osTimerId_t _periodicTimerHandle;
        const osTimerAttr_t _periodicTimer_attributes = {
            .name = "_periodicTimer"
        };

        void _periodicTimerCallbackCallback(void *argument) {
            struct PeriodicTimer *ptimer = (struct PeriodicTimer *)argument;
            ptimer->time_stamp_ms += ptimer->period_ms;
            if (ptimer->on_timer_callback) {
                ptimer->on_timer_callback(ptimer->arg);
            }
        }
    #endif /* PERIODIC_TIMER_os_FreeRTOS */


#endif /* PERIODIC_TIMER_mcu_stm32f411ce */
#if PERIODIC_TIMER_mcu_esp32s3
    // ...
#endif /* PERIODIC_TIMER_mcu_esp32s3 */
#if PERIODIC_TIMER_mcu_linuxDebug
    // ...

    #include <assert.h>

    void* periodic_timer_thread_func(void *arg) {
        struct PeriodicTimer *ptimer = (struct PeriodicTimer *)arg;
        uint32_t period_us = ptimer->period_ms * 1000;
        while (1) {
            usleep(period_us);
            ptimer->time_stamp_ms += ptimer->period_ms;
            if (NULL != ptimer->on_timer_callback) {
                ptimer->on_timer_callback();
            }

            // if (time_stamp_ms % 500 == 0) {
            //     periodic_timer_if_printf("time_stamp_ms = %d\n", time_stamp_ms);
            // }

            sem_wait(&(ptimer->sem));
            if (ptimer->exit_timer_thread_flag) {
                sem_post(&(ptimer->sem));
                break;
            }
            sem_post(&(ptimer->sem));
        }
        pthread_exit(NULL);
    }

#endif /* PERIODIC_TIMER_mcu_linuxDebug */




int periodic_timer_init(void) {
    periodic_timer_printf("init\n");

    timer_is_open = 0;
    
    #if PERIODIC_TIMER_mcu_stm32f411ce
        // ...
        #if PERIODIC_TIMER_os_FreeRTOS
            // ...
        #endif /* PERIODIC_TIMER_os_FreeRTOS */
        #if PERIODIC_TIMER_os_None
            // ...
            pSwTimer = NULL;
        #endif /* PERIODIC_TIMER_os_None */

    #endif /* PERIODIC_TIMER_mcu_stm32f411ce */
    #if PERIODIC_TIMER_mcu_esp32s3
        // ...
    #endif /* PERIODIC_TIMER_mcu_esp32s3 */
    #if PERIODIC_TIMER_mcu_linuxDebug
        // ...
    #endif /* PERIODIC_TIMER_mcu_linuxDebug */

    return (0);
}

void periodic_timer_deinit(void) {
    periodic_timer_printf("deinit\n");

    #if PERIODIC_TIMER_mcu_stm32f411ce
        // ...
    #endif /* PERIODIC_TIMER_mcu_stm32f411ce */
    #if PERIODIC_TIMER_mcu_esp32s3
        // ...
    #endif /* PERIODIC_TIMER_mcu_esp32s3 */
    #if PERIODIC_TIMER_mcu_linuxDebug
        // ...
    #endif /* PERIODIC_TIMER_mcu_linuxDebug */
}

int periodic_timer_open(struct PeriodicTimer *ptimer, 
    uint32_t period_ms, void (*callback)(void *arg)) {

    periodic_timer_printf("open\n");

    if (timer_is_open) {
        periodic_timer_printf("🟨warning: timer is opened\n");
    } else {

        #if PERIODIC_TIMER_mcu_stm32f411ce
            // ...
            #if PERIODIC_TIMER_os_None
            {
                PERIODIC_TIMER_MX_TIM_Init();
                if (period_ms != 0 && period_ms <= 0xffff) {
                    __HAL_TIM_SET_AUTORELOAD(PERIODIC_TIMER_tim_handle, period_ms - 1);
                } else {
                    return (-1);
                }

                __HAL_TIM_CLEAR_IT(PERIODIC_TIMER_tim_handle, TIM_IT_UPDATE);
                __HAL_TIM_SET_COUNTER(PERIODIC_TIMER_tim_handle, 0);
                HAL_TIM_Base_Start_IT(PERIODIC_TIMER_tim_handle);

                pSwTimer = ptimer;
            }
            #endif /* PERIODIC_TIMER_os_None */
            #if PERIODIC_TIMER_os_FreeRTOS
            {
                if (0 == period_ms) {
                    return (-1);
                } else {
                    _periodicTimerHandle = osTimerNew(
                        _periodicTimerCallbackCallback,
                        osTimerPeriodic,
                        ptimer,
                        &_periodicTimer_attributes);
                    if (NULL == _periodicTimerHandle ||
                        osOK != osTimerStart(_periodicTimerHandle, pdMS_TO_TICKS(period_ms))) {
                        configASSERT(0);
                        return (-1);
                    }
                }
            }
            #endif /* PERIODIC_TIMER_os_FreeRTOS */

        #endif /* PERIODIC_TIMER_mcu_stm32f411ce */
        #if PERIODIC_TIMER_mcu_esp32s3
            // ...
        #endif /* PERIODIC_TIMER_mcu_esp32s3 */
        #if PERIODIC_TIMER_mcu_linuxDebug
            // ...
            int ret;
            ret = sem_init(&(ptimer->sem), 0, 1);
            if (ret != 0) {
                periodic_timer_printf("sem_init error\n");
                return (-1);
            }
            ret = pthread_create(&(ptimer->thread), NULL, periodic_timer_thread_func, ptimer);
            if (ret != 0) {
                periodic_timer_printf("pthread_create error\n");
                sem_destroy(&(ptimer->sem));
                return (-1);
            }
            ptimer->exit_timer_thread_flag = 0;
        #endif /* PERIODIC_TIMER_mcu_linuxDebug */

        ptimer->period_ms         = period_ms;
        ptimer->on_timer_callback = callback;
        ptimer->time_stamp_ms     = 0;

        timer_is_open = 1;
    }


    return (0);
}

void periodic_timer_close(struct PeriodicTimer *ptimer) {

    periodic_timer_printf(" closed\n");

    if (timer_is_open) {

        #if PERIODIC_TIMER_mcu_stm32f411ce
            // ...
            #if PERIODIC_TIMER_os_FreeRTOS
                // ...
                osTimerStop(_periodicTimerHandle);
                osTimerDelete(_periodicTimerHandle);
                #endif /* PERIODIC_TIMER_os_FreeRTOS */
            #if PERIODIC_TIMER_os_None
                // ...
                pSwTimer = NULL;
                HAL_TIM_Base_Stop_IT(PERIODIC_TIMER_tim_handle);
                HAL_TIM_Base_DeInit(PERIODIC_TIMER_tim_handle);
            #endif /* PERIODIC_TIMER_os_None */

        #endif /* PERIODIC_TIMER_mcu_stm32f411ce */
        #if PERIODIC_TIMER_mcu_esp32s3
            // ...
        #endif /* PERIODIC_TIMER_mcu_esp32s3 */
        #if PERIODIC_TIMER_mcu_linuxDebug
            // ...
            sem_wait(&(ptimer->sem));
            ptimer->exit_timer_thread_flag = 1;
            sem_post(&(ptimer->sem));
            pthread_join(ptimer->thread, NULL);
            sem_destroy(&(ptimer->sem));
        #endif /* PERIODIC_TIMER_mcu_linuxDebug */

        timer_is_open = 0;
    } else {
        periodic_timer_printf("🟨warning: timer is closed\n");
    }
}

uint32_t periodic_timer_get_time_stamp_ms(struct PeriodicTimer *ptimer) {
    #if PERIODIC_TIMER_mcu_stm32f411ce
        // ...
        #if PERIODIC_TIMER_os_FreeRTOS
            // ...
            return (ptimer->time_stamp_ms);
        #endif /* PERIODIC_TIMER_os_FreeRTOS */
        #if PERIODIC_TIMER_os_None
            // ...
            return (pSwTimer->time_stamp_ms);
        #endif /* PERIODIC_TIMER_os_None */

    #endif /* PERIODIC_TIMER_mcu_stm32f411ce */
    #if PERIODIC_TIMER_mcu_esp32s3
        // ...
    #endif /* PERIODIC_TIMER_mcu_esp32s3 */
    #if PERIODIC_TIMER_mcu_linuxDebug
        // ...
        sem_wait(&(ptimer->sem));
        uint32_t time_stamp_ms = ptimer->time_stamp_ms;
        sem_post(&(ptimer->sem));
        return time_stamp_ms;
    #endif /* PERIODIC_TIMER_mcu_linuxDebug */
}


/* ---------- begin for test ---------- */


static int callback_count = 0;

void temp_callback(void *arg) {
    callback_count++;
    periodic_timer_if_printf("500ms timer callback, count = %d\n", callback_count);
}

int test_periodic_timer(void) {
    periodic_timer_if_printf("测试定时器功能：\n");
    periodic_timer_if_printf("定时器开启后，每500ms执行一次回调函数；定时器关闭后，不再执行回调函数\n");
    periodic_timer_if_printf("从开启定时器到第一次定时器回调的时间间隔由硬件系统决定，通常等于设定的时间间隔\n");
    periodic_timer_init();
    struct PeriodicTimer ptimer;
    uint32_t time_stamp_ms[2] = {0};
    periodic_timer_open(&ptimer, 500, temp_callback);
    time_stamp_ms[0] = periodic_timer_get_time_stamp_ms(&ptimer);
    periodic_timer_if_printf("main sleep 3s，开启了定时器，等待定时器回调\n");

    // 模拟在忙碌状态
    periodic_timer_if_os_sleep(3000);

    time_stamp_ms[1] = periodic_timer_get_time_stamp_ms(&ptimer);
    assert(2500 <= (time_stamp_ms[1] - time_stamp_ms[0]) && "定时器间戳错误");
    assert(callback_count >= 5 && "定时器回调次数错误");
    periodic_timer_if_printf("通过测试：回调功能和时间戳功能\n");

    periodic_timer_close(&ptimer);
    int count[2] = {0};
    periodic_timer_if_printf("main sleep 2s，关闭了定时器，等待定时器回调\n");
    count[0] = callback_count;
    periodic_timer_if_os_sleep(2000);
    count[1] = callback_count;
    assert((count[0] == count[1]) && "定时器关闭后，回调函数不应该执行");
    periodic_timer_if_printf("通过测试：定时器关闭后，回调函数不执行\n");
    
    periodic_timer_if_printf("再次开启定时器，设定为不同的时间间隔，等待定时器回调。\n");
    periodic_timer_open(&ptimer, 100, temp_callback);
    periodic_timer_if_os_sleep(2000);
    periodic_timer_close(&ptimer);
    count[0] = callback_count;
    assert((count[0] - count[1]) >= 19 && "定时器回调次数错误");
    periodic_timer_if_printf("通过测试：再次开启定时器，设定为不同的时间间隔\n");
    

    periodic_timer_if_printf("全部测试通过！\n");

    return (0);
}

    /* ---------- end for test ---------- */
