#ifndef __W25Q128_IF_H__
#define __W25Q128_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param mcu object
 */
#define W25Q128_mcu_stm32f411ce      1
#define W25Q128_mcu_esp32s3      0
#if W25Q128_mcu_stm32f411ce
    /**
     * @param serial_fun object
     */
    #define W25Q128_serial_fun_off      0
    #define W25Q128_serial_fun_stdio      0
    #define W25Q128_serial_fun_print      1
    #define W25Q128_serial_fun_custom      0
    #if W25Q128_serial_fun_off
        #define w25q128_if_printf(...) do { } while (0)
    #endif /* W25Q128_serial_fun_off */
    #if W25Q128_serial_fun_stdio
        #include <stdio.h>
        #define w25q128_if_printf(...) printf(__VA_ARGS__)
    #endif /* W25Q128_serial_fun_stdio */
    #if W25Q128_serial_fun_print
        /**
        * @param print reference
        */
        #include "../print/print.h"
        #define w25q128_if_printf(...) print(__VA_ARGS__)
    #endif /* W25Q128_serial_fun_print */
    #if W25Q128_serial_fun_custom
        #define w25q128_if_printf(...) do { } while (0)
    #endif /* W25Q128_serial_fun_custom */

    /**
     * @param SWCS_GPIO_Port string
     * @param SWCS_Pin string
     */
    #define W25Q128_SWCS_GPIO_Port      GPIOB
    #define W25Q128_SWCS_Pin      GPIO_PIN_8

    /**
     * @param SPI_TYPE object
     */
    #define W25Q128_SPI_TYPE_hardwire      1
    #define W25Q128_SPI_TYPE_softwire      0
    #if W25Q128_SPI_TYPE_hardwire
        /**
         * @param SPI_HANDLE string
         */
        #define W25Q128_SPI_HANDLE      &hspi3
    #endif /* W25Q128_SPI_TYPE_hardwire */
    #if W25Q128_SPI_TYPE_softwire
        // ...
    #endif /* W25Q128_SPI_TYPE_softwire */
#endif /* W25Q128_mcu_stm32f411ce */
#if W25Q128_mcu_esp32s3
    // ...
#endif /* W25Q128_mcu_esp32s3 */

/**
* @param resource_manager reference
*/
#include "../resource_manager/resource_manager.h"

void     w25q128_if_delay_us(uint32_t us);
uint32_t w25q128_if_get_system_time_in_ms(void);
void     w25q128_if_Transmit_Data(uint8_t const *pHeader, uint32_t headerSize, uint8_t const *pData, uint32_t Size);
void     w25q128_if_Receive_Data(uint8_t const *pHeader, uint32_t headerSize, uint8_t *pData, uint32_t Size);



#ifdef __cplusplus
}
#endif

#endif /* __W25Q128_IF_H__ */