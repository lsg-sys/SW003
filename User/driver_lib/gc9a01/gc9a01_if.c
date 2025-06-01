#include "gc9a01_if.h"

int gc9a01_if_init(void) {
    /* 跳过GPIO初始化, MX_GPIO_Init()已经在main.c中调用 */

    /* reset */
    HAL_GPIO_WritePin(GC9A01_reset_GPIO_Port, GC9A01_reset_Pin, GPIO_PIN_RESET);
    gc9a01_if_delay(20);
    HAL_GPIO_WritePin(GC9A01_reset_GPIO_Port, GC9A01_reset_Pin, GPIO_PIN_SET);
    gc9a01_if_delay(100);

    /* 通讯接口初始化 */
    GC9A01_MX_SPI_Init();
    
    return 0;
}

void gc9a01_if_deinit(void) {}


/* 
    The DC used to select "Data or Command" in the parallel interface,
    When DCX='1', data is selected.
    When DCX='0', command is selected.
*/

inline void gc9a01_if_select_transmit_command_mode(void) {
    #if GC9A01_mcu_stm32f411ce
        HAL_GPIO_WritePin(GC9A01_data_command_GPIO_Port, GC9A01_data_command_Pin, GPIO_PIN_RESET);
    #endif /* GC9A01_mcu_stm32f411ce */
    #if GC9A01_mcu_esp32s3
        // ...
    #endif /* GC9A01_mcu_esp32s3 */
}
inline void gc9a01_if_select_transmit_data_mode(void) {
    #if GC9A01_mcu_stm32f411ce
        HAL_GPIO_WritePin(GC9A01_data_command_GPIO_Port, GC9A01_data_command_Pin, GPIO_PIN_SET);
    #endif /* GC9A01_mcu_stm32f411ce */
    #if GC9A01_mcu_esp32s3
        // ...
    #endif /* GC9A01_mcu_esp32s3 */
}

void gc9a01_if_transmit(uint8_t const *buf, uint32_t len) {
    #if GC9A01_mcu_stm32f411ce
        #if GC9A01_spi_type_hardwire_hwcs
            if (HAL_SPI_Transmit(GC9A01_SPI_HANDLE, buf, len, 1000) != HAL_OK) {
                // Transmission error in transmission process
                Error_Handler();
            }
        #endif /* GC9A01_spi_type_hardwire_hwcs */
        #if GC9A01_spi_type_hardwire_swcs
            // ...
        #endif /* GC9A01_spi_type_hardwire_swcs */
        #if GC9A01_spi_type_softwire_swcs
            // ...
        #endif /* GC9A01_spi_type_softwire_swcs */
    #endif /* GC9A01_mcu_stm32f411ce */
    #if GC9A01_mcu_esp32s3
        // ...
    #endif /* GC9A01_mcu_esp32s3 */
}

void gc9a01_if_transmit_dma(uint8_t const *buf, uint32_t len) {
    #if GC9A01_mcu_stm32f411ce
        #if GC9A01_spi_type_hardwire_hwcs
            if (HAL_SPI_Transmit_DMA(GC9A01_SPI_HANDLE, buf, len) != HAL_OK) {
                // Transmission error in transmission process
                Error_Handler();
            }
        #endif /* GC9A01_spi_type_hardwire_hwcs */
        #if GC9A01_spi_type_hardwire_swcs
            // ...
        #endif /* GC9A01_spi_type_hardwire_swcs */
        #if GC9A01_spi_type_softwire_swcs
            // ...
        #endif /* GC9A01_spi_type_softwire_swcs */
    #endif /* GC9A01_mcu_stm32f411ce */
    #if GC9A01_mcu_esp32s3
        // ...
    #endif /* GC9A01_mcu_esp32s3 */
}