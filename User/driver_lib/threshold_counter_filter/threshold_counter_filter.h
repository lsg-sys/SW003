#ifndef __THRESHOLD_COUNTER_FILTER_H__
#define __THRESHOLD_COUNTER_FILTER_H__

// 特性
/* 
    滞回阈值：
    高阈值（THRESHOLD_HIGH）：计数器超过该值时触发高电平状态。
    低阈值（THRESHOLD_LOW）：计数器低于该值时触发低电平状态。
    中间区域（LOW < counter < HIGH）：保持当前状态，避免频繁切换。
    动态阈值：根据噪声特性调整阈值。（待实现）
*/

// 示例用法
/* 

int main() {

    ThresholdFilter cf;
    init_threshold_filter(&cf);
    
    // 模拟传感器高电平持续10次
    for (int i = 0; i < 15; i++) {
        filter(&cf, 1);
        printf("Counter: %u, State: %d\n", cf.counter, get_state(&cf));
    }

    return 0;
}

*/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param threshold_high string
*/
#define THRESHOLD_COUNTER_FILTER_threshold_high      2

/**
* @param threshold_low string
*/
#define THRESHOLD_COUNTER_FILTER_threshold_low      2

/**
* @param increment_step string
*/
#define THRESHOLD_COUNTER_FILTER_increment_step      1

/**
* @param decrement_step string
*/
#define THRESHOLD_COUNTER_FILTER_decrement_step      1


/**
* @param Mode object
*/
#define THRESHOLD_COUNTER_FILTER_Mode_biased      1
#define THRESHOLD_COUNTER_FILTER_Mode_unbiased      0
#if THRESHOLD_COUNTER_FILTER_Mode_biased
    /* 
        强制归零：
        当输入为低电平且计数器低于低阈值时，直接归零，加速状态切换到低电平。
    */

#endif /* THRESHOLD_COUNTER_FILTER_Mode_biased */
#if THRESHOLD_COUNTER_FILTER_Mode_unbiased
    // ...
#endif /* THRESHOLD_COUNTER_FILTER_Mode_unbiased */


typedef struct {
    int counter;           // 计数器值
    int current_state;           // 当前滤波后的状态（0或1）
} ThresholdFilter;


void init_threshold_counter_filter(ThresholdFilter *cf);
int filter(ThresholdFilter *cf, int input);
int get_state(ThresholdFilter *cf);

#ifdef __cplusplus
}
#endif

#endif /* __THRESHOLD_COUNTER_FILTER_H__ */
