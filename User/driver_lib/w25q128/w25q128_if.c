#include "w25q128_if.h"

#if W25Q128_mcu_stm32f411ce
    
    #include "stm32f4xx_hal.h"

    static uint32_t get_current_clock_in_Hz(void) {
        return HAL_RCC_GetHCLKFreq();
    }

    void w25q128_if_delay_us(uint32_t us) {
        uint32_t cur_clock_in_MHz = get_current_clock_in_Hz() / 1000000;
        for (uint32_t i = 0; i < us; i++) {
            for (uint32_t j = 0; j < cur_clock_in_MHz; j++) {
                __NOP();
            }
        }
    }

    uint32_t w25q128_if_get_system_time_in_ms(void) {
        return HAL_GetTick();
    }

    /* ---------------------------------------------- */
    
    #define CS_Enable()             HAL_GPIO_WritePin(W25Q128_SWCS_GPIO_Port, W25Q128_SWCS_Pin, GPIO_PIN_RESET)
    #define CS_Disable()            HAL_GPIO_WritePin(W25Q128_SWCS_GPIO_Port, W25Q128_SWCS_Pin, GPIO_PIN_SET)
    
    /* ---------------------------------------------- */

    #if W25Q128_SPI_TYPE_hardwire
        #include "spi.h"

        static void wait_data_transfer_to_finish(void) {
            while (HAL_SPI_GetState(W25Q128_SPI_HANDLE) != HAL_SPI_STATE_READY);
        }
        
        void w25q128_if_Transmit_Data(uint8_t const*pHeader, uint32_t headerSize, uint8_t const*pData, uint32_t Size) {
            CS_Enable();
            HAL_SPI_Transmit_DMA(W25Q128_SPI_HANDLE, pHeader, headerSize);
            wait_data_transfer_to_finish();
            if (Size > 0) {
                HAL_SPI_Transmit_DMA(W25Q128_SPI_HANDLE, pData, Size);
                wait_data_transfer_to_finish();
            }
            CS_Disable();
        }
        
        void w25q128_if_Receive_Data(uint8_t const*pHeader, uint32_t headerSize, uint8_t *pData, uint32_t Size) {
            /* 手动严格控制CS，中间不论首发多少次数据，W25QX都会判定为同一条指令 */
            CS_Enable();
            HAL_SPI_Transmit_DMA(W25Q128_SPI_HANDLE, pHeader, headerSize);
            wait_data_transfer_to_finish();
            if (Size > 0) {
                HAL_SPI_Receive_DMA(W25Q128_SPI_HANDLE, pData, Size);
                wait_data_transfer_to_finish();
            }
            CS_Disable();
        }
    
    #endif /* W25Q128_SPI_TYPE_hardwire */
    #if W25Q128_SPI_TYPE_softwire
        // ...
    #endif /* W25Q128_SPI_TYPE_softwire */
#endif /* W25Q128_mcu_stm32f411ce */
#if W25Q128_mcu_esp32s3
    // ...
#endif /* W25Q128_mcu_esp32s3 */
