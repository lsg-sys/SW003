

#include <string.h>
#include "lsm6dso_if.h"

/**
 * @param mcu object
 */
#define LSM6DSO_mcu_stm32f411ce      1
#define LSM6DSO_mcu_esp32s3      0
#if LSM6DSO_mcu_stm32f411ce
    /**
     * @param DELAY_MS_IF object
     */
    #define LSM6DSO_DELAY_MS_IF_FreeRTOS      1
    #define LSM6DSO_DELAY_MS_IF_custom      0
    #if LSM6DSO_DELAY_MS_IF_FreeRTOS
    #include "FreeRTOS.h"
    #include "task.h"
        void LSM6DSO_platform_delay(uint32_t ms) {
            vTaskDelay(ms / portTICK_PERIOD_MS);
        }
    #endif /* LSM6DSO_DELAY_MS_IF_FreeRTOS */
    #if LSM6DSO_DELAY_MS_IF_custom
        // ...
    #endif /* LSM6DSO_DELAY_MS_IF_custom */

    #include "gpio.h"
    /**
     * @param SWCS_GPIO_Port string
     * @param SWCS_Pin string
     */
    #define LSM6DSO_SWCS_GPIO_Port      GPIOA
    #define LSM6DSO_SWCS_Pin      GPIO_PIN_8

    #define CS_Enable()      HAL_GPIO_WritePin(LSM6DSO_SWCS_GPIO_Port, LSM6DSO_SWCS_Pin, GPIO_PIN_RESET)
    #define CS_Disable()     HAL_GPIO_WritePin(LSM6DSO_SWCS_GPIO_Port, LSM6DSO_SWCS_Pin, GPIO_PIN_SET)
    
    /**
     * @param SPI_TYPE object
     */
    #define LSM6DSO_SPI_TYPE_hardwire      1
    #define LSM6DSO_SPI_TYPE_softwire      0
    #if LSM6DSO_SPI_TYPE_hardwire
        #include "spi.h"
        /**
         * @param SPI_HANDLE string
         */
        #define LSM6DSO_SPI_HANDLE      &hspi2
    
        #define SPI_DMA_TX_BUFFER_SIZE 64
        #define SPI_DMA_RX_BUFFER_SIZE 64
        uint8_t spi_tx_buf[SPI_DMA_TX_BUFFER_SIZE];
        uint8_t spi_rx_buf[SPI_DMA_RX_BUFFER_SIZE];

        void LSM6DSO_get_sensor_bus_handle(void **handle)
        {
            *handle = LSM6DSO_SPI_HANDLE;
        }

        /*
        * @brief  Write generic device register (platform dependent)
        *
        * @param  handle    customizable argument. In this examples is used in
        *                   order to select the correct sensor bus handler.
        * @param  reg       register to write
        * @param  bufp      pointer to data to write in register reg
        * @param  len       number of consecutive register to write
        *
        */
        int32_t LSM6DSO_platform_write(void *handle, uint8_t reg, uint8_t *bufp,
                               uint16_t len)
        {
            
            if (SPI_DMA_TX_BUFFER_SIZE  < len+1)
            {
                return -1;
            }
            
            spi_tx_buf[0] = reg;
            memcpy(&spi_tx_buf[1], bufp, len);
            
            CS_Enable();

            // configASSERT(HAL_OK == HAL_SPI_Transmit_DMA((SPI_HandleTypeDef *)handle, spi_tx_buf, len + 1));
            HAL_SPI_Transmit_DMA((SPI_HandleTypeDef *)handle, spi_tx_buf, len + 1);
            while (HAL_SPI_GetState((SPI_HandleTypeDef *)handle) != HAL_SPI_STATE_READY);

            // configASSERT(HAL_OK == HAL_SPI_Transmit_DMA((SPI_HandleTypeDef*)handle, &reg, 1));
            // while (HAL_SPI_GetState((SPI_HandleTypeDef*)handle) == HAL_SPI_STATE_BUSY_TX);

            // configASSERT(HAL_OK == HAL_SPI_Transmit_DMA((SPI_HandleTypeDef*)handle, (uint8_t*) bufp, len));
            // while (HAL_SPI_GetState((SPI_HandleTypeDef*)handle) == HAL_SPI_STATE_BUSY_TX);

            CS_Disable();

            return 0;
        }

        /*
        * @brief  Read generic device register (platform dependent)
        *
        * @param  handle    customizable argument. In this examples is used in
        *                   order to select the correct sensor bus handler.
        * @param  reg       register to read
        * @param  bufp      pointer to buffer that store the data read
        * @param  len       number of consecutive register to read
        *
        */
        int32_t LSM6DSO_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                              uint16_t len)
        {
            if (SPI_DMA_RX_BUFFER_SIZE < len+1) {
                return -1;
            }

            reg |= 0x80;
  
            spi_tx_buf[0] = reg;
            // memcpy(&spi_tx_buf[1], bufp, len);
            
            CS_Enable();
            // configASSERT(HAL_OK == HAL_SPI_TransmitReceive_DMA((SPI_HandleTypeDef *)handle, spi_tx_buf, spi_rx_buf, len + 1));
            HAL_SPI_TransmitReceive_DMA((SPI_HandleTypeDef *)handle, spi_tx_buf, spi_rx_buf, len + 1);
            while (HAL_SPI_GetState((SPI_HandleTypeDef *)handle) != HAL_SPI_STATE_READY)
                ;

            memcpy(bufp, &spi_rx_buf[1], len);

            CS_Disable();

            return 0;
        }

    #endif /* LSM6DSO_SPI_TYPE_hardwire */
    #if LSM6DSO_SPI_TYPE_softwire
        // ...
    #endif /* LSM6DSO_SPI_TYPE_softwire */

#endif /* LSM6DSO_mcu_stm32f411ce */
#if LSM6DSO_mcu_esp32s3
// ...
#endif /* LSM6DSO_mcu_esp32s3 */

/*
    * @brief  platform specific initialization (platform dependent)
    */
void LSM6DSO_platform_init(void)
{
}

// #include "usart.h"

// /*
// * @brief  Write generic device register (platform dependent)
// *
// * @param  tx_buffer     buffer to transmit
// * @param  len           number of byte to send
// *
// */
// void LSM6DSO_tx_com(uint8_t *tx_buffer, uint16_t len)
// {
//     // HAL_UART_Transmit(&huart2, tx_buffer, len, 1000);
//     // HAL_UART_Transmit_DMA(handle_serial, tx_buffer, len);
//     serial_output(tx_buffer, len);
// }