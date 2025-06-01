#ifndef __LIGHTING_IF_H__
#define __LIGHTING_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param mcu object
*/
#define LIGHTING_mcu_stm32f411ce      1
#define LIGHTING_mcu_stm32f103c8      0
#if LIGHTING_mcu_stm32f411ce
    #include "tim.h"
    #include "gpio.h"
    /**
    * @param tim_number object
    */
    #define LIGHTING_tim_number_tim1      1
    #define LIGHTING_tim_number_tim2      0
    #define LIGHTING_tim_number_tim3      0
    #define LIGHTING_tim_number_tim4      0
    #define LIGHTING_tim_number_tim5      0
    #define LIGHTING_tim_number_tim9      0
    #define LIGHTING_tim_number_tim10      0
    #define LIGHTING_tim_number_tim11      0
    #if LIGHTING_tim_number_tim1
        #define LIGHTING_tim_handle &htim1
        #define LIGHTING_MX_TIM_Init MX_TIM1_Init

            /**
            * @param channel object
            */
            #define LIGHTING_channel_ch1      0
            #define LIGHTING_channel_ch2      0
            #define LIGHTING_channel_ch3      0
            #define LIGHTING_channel_ch4      0
            #define LIGHTING_channel_ch1n      1
            #define LIGHTING_channel_ch2n      0
            #define LIGHTING_channel_ch3n      0
            #if LIGHTING_channel_ch1
            #define LIGHTING_tim_channel TIM_CHANNEL_1
            #define LIGHTING_on_complementary 0
            #endif /* LIGHTING_channel_ch1 */
            #if LIGHTING_channel_ch2
            #define LIGHTING_tim_channel TIM_CHANNEL_2
            #define LIGHTING_on_complementary 0
            #endif /* LIGHTING_channel_ch2 */
            #if LIGHTING_channel_ch3
            #define LIGHTING_tim_channel TIM_CHANNEL_3
            #define LIGHTING_on_complementary 0
            #endif /* LIGHTING_channel_ch3 */
            #if LIGHTING_channel_ch4
            #define LIGHTING_tim_channel TIM_CHANNEL_4
            #define LIGHTING_on_complementary 0
            #endif /* LIGHTING_channel_ch4 */
            #if LIGHTING_channel_ch1n
            #define LIGHTING_tim_channel TIM_CHANNEL_1
            #define LIGHTING_on_complementary 1
            #endif /* LIGHTING_channel_ch1n */
            #if LIGHTING_channel_ch2n
            #define LIGHTING_tim_channel TIM_CHANNEL_2
            #define LIGHTING_on_complementary 1
            #endif /* LIGHTING_channel_ch2n */
            #if LIGHTING_channel_ch3n
            #define LIGHTING_tim_channel TIM_CHANNEL_3
            #define LIGHTING_on_complementary 1
            #endif /* LIGHTING_channel_ch3n */
    #endif /* LIGHTING_tim_number_tim1 */
    #if LIGHTING_tim_number_tim2
        #define LIGHTING_tim_handle &htim2
        #define LIGHTING_MX_TIM_Init MX_TIM2_Init


            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           #if LIGHTING_channel_ch2
           #define LIGHTING_tim_channel TIM_CHANNEL_2
           #endif /* LIGHTING_channel_ch2 */
           #if LIGHTING_channel_ch3
           #define LIGHTING_tim_channel TIM_CHANNEL_3
           #endif /* LIGHTING_channel_ch3 */
           #if LIGHTING_channel_ch4
           #define LIGHTING_tim_channel TIM_CHANNEL_4
           #endif /* LIGHTING_channel_ch4 */

           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim2 */
    #if LIGHTING_tim_number_tim3
        #define LIGHTING_tim_handle &htim3
        #define LIGHTING_MX_TIM_Init MX_TIM3_Init



            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           #if LIGHTING_channel_ch2
           #define LIGHTING_tim_channel TIM_CHANNEL_2
           #endif /* LIGHTING_channel_ch2 */
           #if LIGHTING_channel_ch3
           #define LIGHTING_tim_channel TIM_CHANNEL_3
           #endif /* LIGHTING_channel_ch3 */
           #if LIGHTING_channel_ch4
           #define LIGHTING_tim_channel TIM_CHANNEL_4
           #endif /* LIGHTING_channel_ch4 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim3 */
    #if LIGHTING_tim_number_tim4
        #define LIGHTING_tim_handle &htim4
        #define LIGHTING_MX_TIM_Init MX_TIM4_Init

        

            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           #if LIGHTING_channel_ch2
           #define LIGHTING_tim_channel TIM_CHANNEL_2
           #endif /* LIGHTING_channel_ch2 */
           #if LIGHTING_channel_ch3
           #define LIGHTING_tim_channel TIM_CHANNEL_3
           #endif /* LIGHTING_channel_ch3 */
           #if LIGHTING_channel_ch4
           #define LIGHTING_tim_channel TIM_CHANNEL_4
           #endif /* LIGHTING_channel_ch4 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim4 */
    #if LIGHTING_tim_number_tim5
        #define LIGHTING_tim_handle &htim5
        #define LIGHTING_MX_TIM_Init MX_TIM5_Init

        

            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           #if LIGHTING_channel_ch2
           #define LIGHTING_tim_channel TIM_CHANNEL_2
           #endif /* LIGHTING_channel_ch2 */
           #if LIGHTING_channel_ch3
           #define LIGHTING_tim_channel TIM_CHANNEL_3
           #endif /* LIGHTING_channel_ch3 */
           #if LIGHTING_channel_ch4
           #define LIGHTING_tim_channel TIM_CHANNEL_4
           #endif /* LIGHTING_channel_ch4 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim5 */
    #if LIGHTING_tim_number_tim9
        #define LIGHTING_tim_handle &htim9
        #define LIGHTING_MX_TIM_Init MX_TIM9_Init

        

            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           #if LIGHTING_channel_ch2
           #define LIGHTING_tim_channel TIM_CHANNEL_2
           #endif /* LIGHTING_channel_ch2 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim9 */
    #if LIGHTING_tim_number_tim10
        #define LIGHTING_tim_handle &htim10
        #define LIGHTING_MX_TIM_Init MX_TIM10_Init
        

            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim10 */
    #if LIGHTING_tim_number_tim11
        #define LIGHTING_tim_handle &htim11
        #define LIGHTING_MX_TIM_Init MX_TIM11_Init
        

            /**
            * @param channel object
            */
           #define LIGHTING_channel_ch1      0
           #define LIGHTING_channel_ch2      0
           #define LIGHTING_channel_ch3      0
           #define LIGHTING_channel_ch4      0
           #define LIGHTING_channel_ch1n      1
           #define LIGHTING_channel_ch2n      0
           #define LIGHTING_channel_ch3n      0
           #if LIGHTING_channel_ch1
           #define LIGHTING_tim_channel TIM_CHANNEL_1
           #endif /* LIGHTING_channel_ch1 */
           
           #define LIGHTING_on_complementary 0
    #endif /* LIGHTING_tim_number_tim11 */

    /**
    * @param GPIO_Port string
    */
    #define LIGHTING_GPIO_Port      GPIOB
    /**
     * @param Pin string
     */
    #define LIGHTING_Pin      GPIO_PIN_13

    /**
    * @param Polarity object
    */
    #define LIGHTING_Polarity_High      1
    #define LIGHTING_Polarity_Low      0
    #if LIGHTING_Polarity_High
        // ...
    #endif /* LIGHTING_Polarity_High */
    #if LIGHTING_Polarity_Low
        // ...
    #endif /* LIGHTING_Polarity_Low */


#endif /* LIGHTING_mcu_stm32f411ce */
#if LIGHTING_mcu_stm32f103c8
    // ...
#endif /* LIGHTING_mcu_stm32f103c8 */

int lighting_if_init(void);
void lighting_if_Deinit(void);
void lighting_if_set_lightness(uint32_t level);
uint32_t lighting_if_get_lightness(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIGHTING_IF_H__ */
