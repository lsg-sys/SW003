#include "lighting_if.h"

static uint32_t pwm_hw_resolution;

/**
 * @brief 初始化照明灯，默认初始化完成后不点亮照明，需要调用 lighting_if_set_lightness 设置亮度
 */
int lighting_if_init(void) {
    #if LIGHTING_mcu_stm32f411ce
        /* 其他初始化在main中的MX_xxx_Init中已经完成 (MX_TIM_Init) */
        LIGHTING_MX_TIM_Init();

        TIM_HandleTypeDef *htim = LIGHTING_tim_handle;
        pwm_hw_resolution = htim->Init.Period + 1;

        __HAL_TIM_SET_COMPARE(LIGHTING_tim_handle, LIGHTING_tim_channel, 0); // 设置PWM比较值为0
        #if LIGHTING_on_complementary
            HAL_TIMEx_PWMN_Start(LIGHTING_tim_handle, LIGHTING_tim_channel); // 反通道PWM
        #else
            HAL_TIM_PWM_Start(LIGHTING_tim_handle, LIGHTING_tim_channel); // 正通道PWM 
        #endif /* LIGHTING_on_complementary */
    #endif /* LIGHTING_mcu_stm32f411ce */
    #if LIGHTING_mcu_stm32f103c8
        // ...
    #endif /* LIGHTING_mcu_stm32f103c8 */

    return 0;
}

void lighting_if_Deinit(void) {
    #if LIGHTING_mcu_stm32f411ce
        __HAL_TIM_SET_COMPARE(LIGHTING_tim_handle, LIGHTING_tim_channel, 0); // 设置PWM比较值为0
        #if LIGHTING_on_complementary
            HAL_TIMEx_PWMN_Stop(LIGHTING_tim_handle, LIGHTING_tim_channel); // 反通道PWM
        #else
            HAL_TIM_PWM_Stop(LIGHTING_tim_handle, LIGHTING_tim_channel); // 正通道PWM 
        #endif /* LIGHTING_on_complementary */
        HAL_TIM_PWM_DeInit(LIGHTING_tim_handle);

        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = LIGHTING_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(LIGHTING_GPIO_Port, &GPIO_InitStruct);
        #if LIGHTING_Polarity_High
        HAL_GPIO_WritePin(LIGHTING_GPIO_Port, LIGHTING_Pin, GPIO_PIN_RESET);
        #endif /* LIGHTING_Polarity_High */
        #if LIGHTING_Polarity_Low
        HAL_GPIO_WritePin(LIGHTING_GPIO_Port, LIGHTING_Pin, GPIO_PIN_SET);
        #endif /* LIGHTING_Polarity_Low */

    #endif /* LIGHTING_mcu_stm32f411ce */
    #if LIGHTING_mcu_stm32f103c8
        // ...
    #endif /* LIGHTING_mcu_stm32f103c8 */
}

/**
 * @brief 设置亮度
 * @param level 亮度百分比，0~100；0为完全关闭，非0为打开
 */
void lighting_if_set_lightness(uint32_t level) {
	if (level > 100) {
		level = 100;
	}
    
    uint32_t hw_compare_value = level * pwm_hw_resolution / 100;
    
    #if LIGHTING_mcu_stm32f411ce
    __HAL_TIM_SET_COMPARE(LIGHTING_tim_handle, LIGHTING_tim_channel, hw_compare_value);
    #endif /* LIGHTING_mcu_stm32f411ce */
    #if LIGHTING_mcu_stm32f103c8
        // ...
    #endif /* LIGHTING_mcu_stm32f103c8 */
}

/**
 * @brief 获取实时亮度
 */
uint32_t lighting_if_get_lightness(void) {
    #if LIGHTING_mcu_stm32f411ce
    uint32_t hw_compare_value = __HAL_TIM_GET_COMPARE(LIGHTING_tim_handle, LIGHTING_tim_channel);
    return hw_compare_value * 100 / pwm_hw_resolution;
    #endif /* LIGHTING_mcu_stm32f411ce */
    #if LIGHTING_mcu_stm32f103c8
        // ...
    #endif /* LIGHTING_mcu_stm32f103c8 */
}