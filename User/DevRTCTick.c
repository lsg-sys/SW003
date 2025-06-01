#include "DevRTCTick.h"


#include "rtc.h"
#include "cmsis_os2.h"

// #define RTC_CLOCK_SOURCE_Hz         (32768)
#define RTC_CLOCK_SOURCE_Hz         (1000000)

uint32_t RTC_TimeToMilliseconds(RTC_TimeTypeDef *sTime) {
    float second_ratio = (float)(sTime->SecondFraction - sTime->SubSeconds) / (float)(sTime->SecondFraction + 1);
    uint32_t ms = (uint32_t)(second_ratio * 1000.0f);
    uint32_t sec = sTime->Hours * 3600 + sTime->Minutes * 60 + sTime->Seconds;

    return (sec * 1000 + ms);
}

/**
 * @brief Get current time in ms, max  (86,400,000-1) ms, 1day = 86,400,000 ms
 */
uint32_t RTCTick_get_tick_ms(void) {
    
    extern RTC_HandleTypeDef hrtc;

    RTC_TimeTypeDef sTime = {0};
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    return (RTC_TimeToMilliseconds(&sTime));
}

uint32_t RTCTick_get_elapsedTime_ms(uint32_t start_time, uint32_t end_time) {

    if (end_time < start_time) {
        end_time += 86400000u;
    }

    return (end_time - start_time);
}


// 辅助函数：将总毫秒数转换为 RTC 时间
void RTC_MillisecondsToTime(uint32_t total_ms, RTC_TimeTypeDef *time, uint32_t second_fraction) {
    time->Hours = total_ms / 3600000;
    total_ms %= 3600000;
    time->Minutes = total_ms / 60000;
    total_ms %= 60000;
    time->Seconds = total_ms / 1000;
    total_ms %= 1000;

    // 子秒转换
    time->SubSeconds = (total_ms * (second_fraction + 1)) / 1000;
}

/**
 * @brief Set wakeup time interrupt
 * @param expected_time_ms 小于 16s=16000ms
 */
void set_wakeup_time_interrupt_ms(uint32_t expected_time_ms) {

    float count = (float)expected_time_ms * ((float)(RTC_CLOCK_SOURCE_Hz/16) / 1000.0f);
    uint32_t WakeUpCounter = (uint32_t)count;
    if (WakeUpCounter > 0xffff) {
        WakeUpCounter = 0xffff;
    }
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WakeUpCounter, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK) {
        Error_Handler();
    }
}

void clear_wakeup_time_interrupt(void) {
    
    if (HAL_RTCEx_DeactivateWakeUpTimer(&hrtc) != HAL_OK) {
        Error_Handler();
    }
}

int test_RTCTick(void) {

    uint32_t start_time = get_FreeRunningTime_ms();
    osDelay(10);
    uint32_t end_time = get_FreeRunningTime_ms();
    // print("elapsed time %u ms %u\n", get_elapsedTime_ms(start_time, end_time), get_elapsedTime_ms(86'400'000u-10, 10));
    if (10-2 <= get_elapsedTime_ms(start_time, end_time) &&
        get_elapsedTime_ms(start_time, end_time) <= 10+2) {
        return 0;
    } else {
        return -1;
    }
}