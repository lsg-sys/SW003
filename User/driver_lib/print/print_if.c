#include "print_if.h"
#include <string.h>
#include <stdio.h>


char print_output_buffer[PRINT_OUTPUT_BUFFER_SIZE];

#if PRINT_mcu_stm32f411ce
    void serial_output(char *buf, int len) {
        if (0 < len && len < 0xffff) {
            HAL_UART_Transmit_DMA(PRINT_uart_handle, (uint8_t *)buf, len);
            HAL_UART_StateTypeDef state;
            do {
                state = HAL_UART_GetState(PRINT_uart_handle);
            } while (state == HAL_UART_STATE_BUSY_TX || state == HAL_UART_STATE_BUSY_TX_RX);
        }
    }

    static uint16_t rx_len;
    /**
     * @brief  阻塞接收数据，有截断功能，不保证最后一个数据是'\0'
     * @param  buffer: 接收数据缓存
     * @param  size: 接收数据缓存大小
     * @param  timeout_ms: 超时时间，步长为 scanning_interval，0xffffffff为无限
     * @return 接收数据长度，-1表示失败
     */
    int serial_input(char *buffer, int size, uint32_t timeout_ms) {
        rx_len = 0;
        HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(PRINT_uart_handle,
                                                                (uint8_t *)buffer, size);
        if (status == HAL_OK) {
            uint32_t timeout_count = 0;
            while (HAL_UART_GetState(PRINT_uart_handle) != HAL_UART_STATE_READY) {
                PRINT_OS_DelayMs(PRINT_scanning_interval);
                timeout_count += PRINT_scanning_interval;
                if (timeout_ms != 0xffffffff && timeout_count >= timeout_ms) {
                    HAL_UART_DMAStop((PRINT_uart_handle));
                    break;
                }
            }
            rx_len = (PRINT_uart_handle)->RxXferSize - __HAL_DMA_GET_COUNTER((PRINT_uart_handle)->hdmarx);
            // if (rx_len == 0) {
            //     HAL_DMA_Abort((PRINT_uart_handle)->hdmarx);
            // }
            return ((int)rx_len);
        } else {
            return (-1);
        }
    }

    // /**
    //  * @brief  Rx Transfer completed callback
    //  * @param  huart: UART handle
    //  * @note   This example shows a simple way to report end of DMA Rx transfer, and
    //  *         you can add your own implementation.
    //  * @retval None
    //  */
    // void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    // {
    //     if (huart->Instance == PRINT_uart_handle) {
    //         rx_len = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    //     }
    // }

#endif /* PRINT_mcu_stm32f411ce */
#if PRINT_mcu_stm32f103c8
    void serial_output(char *buf, int len) {
        // TODO
    }

    int serial_input(buffer, size) {
        // TODO

        return -1;
    }
#endif /* PRINT_mcu_stm32f103c8 */



/**
 * Formats a string and stores it in a buffer using a variable argument list.
 *
 * This function is an optimized version of vsnprintf which formats a string
 * according to the specified format string and stores the result in the provided
 * buffer. It supports the following format specifiers:
 * - %d: signed integer
 * - %x: unsigned integer in hexadecimal
 * - %c: character
 * - %s: null-terminated string
 *
 * The function ensures that the result is null-terminated as long as the buffer size
 * is greater than zero. If the formatted string is longer than the buffer size, it
 * truncates the output to fit.
 *
 * @param buffer  The destination buffer where the formatted string will be stored.
 * @param size    The size of the buffer.
 * @param format  The format string containing the text to be written, interspersed with
 *                format specifiers.
 * @param args    A va_list representing the variable arguments to format.
 * @return        The total number of characters written, excluding the null terminator.
 *                If the output is truncated, the return value indicates the total length
 *                of the formatted string that would have been written.
 */
int optimized_vsnprintf(char *buffer, size_t size, const char *format, va_list args) {
    char       *p         = buffer;
    const char *fmt       = format;
    size_t      remaining = size;
    int         total     = 0;

    while (*fmt && remaining > 0) {
        if (*fmt != '%') {
            *p++ = *fmt++;
            remaining--;
            total++;
            continue;
        }

        fmt++; // 跳过'%'
        if (!*fmt)
            break;

        switch (*fmt) {
        case 'd': {
            int                num = va_arg(args, int);
            unsigned long long abs_num;
            int                neg = 0;
            char               num_buf[16];
            int                digits = 0;

            if (num < 0) {
                neg     = 1;
                abs_num = (unsigned long long)(-(long long)num);
            } else {
                abs_num = (unsigned long long)num;
            }

            // 处理0的特殊情况
            if (abs_num == 0) {
                num_buf[digits++] = '0';
            } else {
                while (abs_num > 0) {
                    num_buf[digits++] = (abs_num % 10) + '0';
                    abs_num /= 10;
                }
            }

            // 计算需要的空间
            int needed = digits + neg;
            total += needed;

            // 写入符号
            if (neg && remaining > 0) {
                *p++ = '-';
                remaining--;
            }

            // 逆序写入数字
            for (int i = digits - 1; i >= 0 && remaining > 0; i--) {
                *p++ = num_buf[i];
                remaining--;
            }
            break;
        }

        case 'u': {
            unsigned int num = va_arg(args, unsigned int);
            char         num_buf[16];
            int          digits = 0;

            if (num == 0) {
                num_buf[digits++] = '0';
            } else {
                while (num > 0) {
                    num_buf[digits++] = (num % 10) + '0';
                    num /= 10;
                }
            }

            total += digits;
            for (int i = digits - 1; i >= 0 && remaining > 0; i--) {
                *p++ = num_buf[i];
                remaining--;
            }
            break;
        }

        case 'x': {
            unsigned int num = va_arg(args, unsigned int);
            char         hex_buf[8];
            int          digits     = 0;
            const char  *hex_digits = "0123456789abcdef";

            if (num == 0) {
                hex_buf[digits++] = '0';
            } else {
                while (num > 0) {
                    hex_buf[digits++] = hex_digits[num % 16];
                    num /= 16;
                }
            }

            total += digits;
            for (int i = digits - 1; i >= 0 && remaining > 0; i--) {
                *p++ = hex_buf[i];
                remaining--;
            }
            break;
        }

        case 'c': {
            char c = (char)va_arg(args, int);
            total++;
            if (remaining > 0) {
                *p++ = c;
                remaining--;
            }
            break;
        }

        case 's': {
            const char *str = va_arg(args, const char *);
            while (*str && remaining > 0) {
                *p++ = *str++;
                remaining--;
                total++;
            }
            break;
        }

        default: // 处理无效格式说明符
            if (remaining > 0) {
                *p++ = '%';
                remaining--;
                total++;
            }
            if (remaining > 0) {
                *p++ = *fmt;
                remaining--;
                total++;
            }
            break;
        }
        fmt++;
    }

    // 保证null终止
    if (size > 0) {
        *p = '\0';
        if (p - buffer >= size) {
            buffer[size - 1] = '\0';
        }
    }

    return (total);
}
