#ifndef __PRINT_IF_H__
#define __PRINT_IF_H__

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @param OUTPUT_BUFFER_SIZE string
*/
#define PRINT_OUTPUT_BUFFER_SIZE      256

extern char print_output_buffer[];

/**
* @param vsnprintf object
*/
#define PRINT_vsnprintf_std      1
#define PRINT_vsnprintf_optimized      0
#if PRINT_vsnprintf_std
    #include <stdio.h>
    #define PRINT_vsnprintf vsnprintf
#endif /* PRINT_vsnprintf_std */
#if PRINT_vsnprintf_optimized
    int optimized_vsnprintf(char *buffer, size_t size, const char *format, va_list args);
    #define PRINT_vsnprintf optimized_vsnprintf
#endif /* PRINT_vsnprintf_optimized */

/**
* @param mcu object
*/
#define PRINT_mcu_stm32f411ce      1
#define PRINT_mcu_stm32f103c8      0
#if PRINT_mcu_stm32f411ce
    #include "usart.h"
    /**
    * @param uart_handle string
    */
    #define PRINT_uart_handle      &huart1


    /**
    * @param scan object
    */
    #define PRINT_scan_enable      1
    #define PRINT_scan_disable      0
    #if PRINT_scan_enable
        int serial_input(char *buffer, int size, uint32_t timeout_ms);

        #define PRINT_SCAN_TIMEOUT_NEVER      ((uint32_t)0xffffffff)
        #define PRINT_scan(buffer, size, timeout_ms) serial_input(buffer, size, timeout_ms)
        
        /**
        * @param scanning_interval string
        */
        #define PRINT_scanning_interval      50

        /**
        * @param OS object
        */
        #define PRINT_OS_None      0
        #define PRINT_OS_cmsis2      1
        #if PRINT_OS_None
            #define PRINT_OS_DelayMs(ms) HAL_Delay(ms)
        #endif /* PRINT_OS_None */
        #if PRINT_OS_cmsis2
            #include "cmsis_os2.h"
            #define PRINT_OS_DelayMs(ms) osDelay(ms)
        #endif /* PRINT_OS_cmsis2 */
    #endif /* PRINT_scan_enable */
    #if PRINT_scan_disable
        // ...
    #endif /* PRINT_scan_disable */


#pragma message("请实现：\
void USART1_IRQHandler(void)\
{\
}\
/**\
 * @brief  Tx Transfer completed callback\
 * @param  huart: UART handle.\
 * @note   This example shows a simple way to report end of DMA Tx transfer, and\
 *         you can add your own implementation.\
 * @retval None\
 */\
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)\
{\
}\
\
/**\
 * @brief  Rx Transfer completed callback\
 * @param  huart: UART handle\
 * @note   This example shows a simple way to report end of DMA Rx transfer, and\
 *         you can add your own implementation.\
 * @retval None\
 */\
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)\
{\
}\
\
/**\
 * @brief  UART error callbacks\
 * @param  huart: UART handle\
 * @note   This example shows a simple way to report transfer error, and you can\
 *         add your own implementation.\
 * @retval None\
 */\
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)\
{\
}\
")
#endif /* PRINT_mcu_stm32f411ce */
#if PRINT_mcu_stm32f103c8

    /**
    * @param scan object
    */
   #define PRINT_scan_enable      1
   #define PRINT_scan_disable      0
   #if PRINT_scan_enable
       #define PRINT_scan(buffer, size) (int)-1;
   #endif /* PRINT_scan_enable */
   #if PRINT_scan_disable
       // ...
   #endif /* PRINT_scan_disable */
#endif /* PRINT_mcu_stm32f103c8 */

void serial_output(char *buf, int len);



#ifdef __cplusplus
}
#endif

#endif /* __PRINT_IF_H__ */
