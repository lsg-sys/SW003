#include "threshold_counter_filter.h"


// 初始化滤波器
void init_threshold_counter_filter(ThresholdFilter *cf) {
    cf->counter = 0;
    cf->current_state = 0;       // 初始状态为低电平
}


// 滤波函数：根据输入信号更新计数器
int filter(ThresholdFilter *cf, int input) {

    
    if (input) {                      // 输入为高电平
        cf->counter += THRESHOLD_COUNTER_FILTER_increment_step;
        cf->counter = (cf->counter > THRESHOLD_COUNTER_FILTER_threshold_high) ? 
        THRESHOLD_COUNTER_FILTER_threshold_high : 
        cf->counter;     // 防止溢出
    } else {                          // 输入为低电平
        cf->counter -= THRESHOLD_COUNTER_FILTER_decrement_step;
        #if THRESHOLD_COUNTER_FILTER_Mode_biased
        // 强制归零：如果输入为低电平且计数器未达到滞回阈值，则归零
        if (cf->counter < THRESHOLD_COUNTER_FILTER_threshold_low) {
            cf->counter = 0;
        }
        #endif /* THRESHOLD_COUNTER_FILTER_Mode_biased */
        #if THRESHOLD_COUNTER_FILTER_Mode_unbiased
        cf->counter = (cf->counter < 0) ? 
                      0 : 
                      cf->counter;     // 防止负数
        #endif /* THRESHOLD_COUNTER_FILTER_Mode_unbiased */
    }

    // 滞回阈值判断
    if (cf->counter >= THRESHOLD_COUNTER_FILTER_threshold_high) {
        cf->current_state = 1;
    } else if (cf->counter <= THRESHOLD_COUNTER_FILTER_threshold_low) {
        cf->current_state = 0;
    }
    // 否则保持当前状态不变

    return cf->current_state;
}

// 获取当前滤波后的状态
int get_state(ThresholdFilter *cf) {
    return cf->current_state;
}

#include <stdio.h>
void test_threshold_counter_filter(void) {
    
    int testArray[] = {
        0, 0, 0, 1, 0, 1, 1, 0, 1, 0, // 模拟按键初始抖动
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 模拟按键稳定按下
        1, 1, 0, 1, 0, 1, 0, 0, 1, 0, // 模拟按键释放时抖动
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 模拟按键稳定释放
        0, 0, 1, 0, 1, 0, 1, 0, 0, 0, // 模拟再次抖动
        0, 0, 0, 1, 0, 1, 1, 0, 1, 0, // 模拟按键初始抖动
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 模拟按键稳定按下
        1, 1, 0, 1, 0, 1, 0, 0, 1, 0, // 模拟按键释放时抖动
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 模拟按键稳定释放
        0, 0, 1, 0, 1, 0, 1, 0, 0, 0  // 模拟再次抖动
    };

    // 打印原始波形
    for (int i = 0; i < sizeof(testArray)/sizeof(testArray[0]); i++) {
        char c = testArray[i] ? '#' : '_';
        printf("%c", c);
    }
    printf("\n");
    
    // 打印滤波后的波形
    ThresholdFilter cf;
    init_threshold_counter_filter(&cf);
    for (int i = 0; i < sizeof(testArray)/sizeof(testArray[0]); i++) {
        char c = filter(&cf, testArray[i]) ? '#' : '_';
        printf("%c", c);
    }
    printf("\n");

    // 模拟传感器高电平持续10次
    for (int i = 0; i < 10; i++) {
        filter(&cf, 1);
        printf("Counter: %u, State: %d\n", cf.counter, get_state(&cf));
    }
    // 模拟传感器低电平持续10次
    for (int i = 0; i < 10; i++) {
        filter(&cf, 0);
        printf("Counter: %u, State: %d\n", cf.counter, get_state(&cf));
    }
}