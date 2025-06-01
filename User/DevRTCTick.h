#ifndef __DEV_RTC_TICK_H__
#define __DEV_RTC_TICK_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t RTCTick_get_tick_ms(void);
uint32_t RTCTick_get_elapsedTime_ms(uint32_t start_time, uint32_t end_time);
void set_wakeup_time_interrupt_ms(uint32_t expected_time_ms);
void clear_wakeup_time_interrupt(void);

#ifdef __cplusplus  
}
#endif



#endif /* __DEV_RTC_TICK_H__ */