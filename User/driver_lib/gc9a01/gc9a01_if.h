#ifndef __GC9A01_IF_H__
#define __GC9A01_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param mcu object
*/
#define GC9A01_mcu_stm32f411ce      1
#define GC9A01_mcu_esp32s3      0
#if GC9A01_mcu_stm32f411ce
    /**
     * @param DELAY_MS_IF object
     */
    #define GC9A01_DELAY_MS_IF_cmsis_os2      1
    #define GC9A01_DELAY_MS_IF_HAL      0
    #define GC9A01_DELAY_MS_IF_CPU_nop      0
    #if GC9A01_DELAY_MS_IF_cmsis_os2
        #include "cmsis_os2.h"
        #include "FreeRTOS.h"
        static inline void gc9a01_if_delay(uint32_t ms) {
            osDelay(pdMS_TO_TICKS(ms));
        }
    #endif /* GC9A01_DELAY_MS_IF_cmsis_os2 */
    #if GC9A01_DELAY_MS_IF_HAL
        static inline void gc9a01_if_delay(uint32_t ms) {
            HAL_delay(ms);
        }
    #endif /* GC9A01_DELAY_MS_IF_HAL */
    #if GC9A01_DELAY_MS_IF_CPU_nop
        static inline void gc9a01_if_delay(uint32_t ms) {
            while (1);
        }
    #endif /* GC9A01_DELAY_MS_IF_CPU_nop */

    /**
    * @param data_command_GPIO_Port string
    */
    #define GC9A01_data_command_GPIO_Port      GPIOA
    /**
    * @param data_command_Pin string
    */
    #define GC9A01_data_command_Pin      GPIO_PIN_6
    /**
    * @param reset_GPIO_Port string
    */
    #define GC9A01_reset_GPIO_Port      GPIOA
    /**
    * @param reset_Pin string
    */
    #define GC9A01_reset_Pin      GPIO_PIN_3

    /**
    * @param spi_type object
    */
    #define GC9A01_spi_type_hardwire_hwcs      1
    #define GC9A01_spi_type_hardwire_swcs      0
    #define GC9A01_spi_type_softwire_swcs      0
    #if GC9A01_spi_type_hardwire_hwcs
        #include "spi.h"
        /**
        * @param spi_number object
        */
        #define GC9A01_spi_number_SPI1      1
        #define GC9A01_spi_number_SPI2      0
        #if GC9A01_spi_number_SPI1
            #define GC9A01_MX_SPI_Init MX_SPI1_Init
            #define GC9A01_SPI_HANDLE &hspi1
            #define GC9A01_SPI_DeInit() HAL_SPI_DeInit(GC9A01_SPI_HANDLE)
        #endif /* GC9A01_spi_number_SPI1 */
        #if GC9A01_spi_number_SPI2
            #define GC9A01_MX_SPI_Init MX_SPI2_Init
            #define GC9A01_SPI_HANDLE &hspi2
            #define GC9A01_SPI_DeInit() HAL_SPI_DeInit(GC9A01_SPI_HANDLE)
        #endif /* GC9A01_spi_number_SPI2 */
    #endif /* GC9A01_spi_type_hardwire_hwcs */
    #if GC9A01_spi_type_hardwire_swcs
        #include "spi.h"
        /**
        * @param SPI_HANDLE string
        */
        #define GC9A01_SPI_HANDLE &hspi1

        /**
        * @param SWCS_GPIO_Port string
        */
        #define GC9A01_SWCS_GPIO_Port GPIOA
        /**
        * @param SWCS_Pin string
        */
        #define GC9A01_SWCS_Pin GPIO_PIN_4
    #endif /* GC9A01_spi_type_hardwire_swcs */
    #if GC9A01_spi_type_softwire_swcs
        // ...
    #endif /* GC9A01_spi_type_softwire_swcs */
#endif /* GC9A01_mcu_stm32f411ce */
#if GC9A01_mcu_esp32s3
    // ...
#endif /* GC9A01_mcu_esp32s3 */

int gc9a01_if_init(void);
void gc9a01_if_deinit(void);

void gc9a01_if_select_transmit_command_mode(void);
void gc9a01_if_select_transmit_data_mode(void);
void gc9a01_if_transmit(uint8_t const *buf, uint32_t len);
void gc9a01_if_transmit_dma(uint8_t const *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __GC9A01_IF_H__ */