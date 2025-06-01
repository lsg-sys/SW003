/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
// static uint8_t rxBufForDMA[BufSize_Serial];
// static uint8_t txBufForDMA[BufSize_Serial];
// static uint32_t dmaRemainingBytes = BufSize_Serial;
//static uint32_t dmaReceivedBytes = 0;
// static FlagStatus invalidFrame = RESET;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream5;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
  // __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// void onUartIRQ(void) {
//   uint32_t irqFlag = __HAL_UART_GET_FLAG(handle_serial, UART_FLAG_IDLE);
//   if (irqFlag != RESET) {
//     __HAL_UART_CLEAR_IDLEFLAG(handle_serial);
//     HAL_UART_DMAStop(handle_serial);
//     if (invalidFrame == RESET) {
//       dmaRemainingBytes = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
//       dmaReceivedBytes = BufSize_Serial - dmaRemainingBytes;
//       // HAL_UART_Transmit_DMA(handle_serial, &rxBufForDMA[0], dmaReceivedBytes);
//       extern void onSerialReceive(uint8_t const *data, uint32_t len);
//       onSerialReceive(&rxBufForDMA[0], dmaReceivedBytes);
//     } else {
//       invalidFrame = RESET;
//     }
//     HAL_UART_Receive_DMA(handle_serial, &rxBufForDMA[0], BufSize_Serial);
//   }
// }
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//   if (huart->Instance == USART1) {
//     /* The DMA receive cache is full and therefore marked as an invalid frame */
//     invalidFrame = SET;
//   }
// }
// // void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart){
// //   if (huart->Instance == USART1) {
// //     /* echo */
// //     // dmaRemainingBytes = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
// //     // dmaReceivedBytes = sizeof(rxBufForDMA) - dmaRemainingBytes;
// //     // HAL_UART_Transmit_DMA(huart, &rxBufForDMA[0], dmaReceivedBytes);
// //   }
// // }
// // void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
// //   if (huart->Instance == USART1) {
// //     HAL_UART_DMAStop(huart);
// //     HAL_UART_Receive_DMA(huart, rxBufForDMA, sizeof(rxBufForDMA));
// //   }
// // }

// #include <string.h>
// #include <stdarg.h>

// /**
//  * Formats a string and stores it in a buffer using a variable argument list.
//  *
//  * This function is an optimized version of vsnprintf which formats a string
//  * according to the specified format string and stores the result in the provided
//  * buffer. It supports the following format specifiers:
//  * - %d: signed integer
//  * - %x: unsigned integer in hexadecimal
//  * - %c: character
//  * - %s: null-terminated string
//  *
//  * The function ensures that the result is null-terminated as long as the buffer size
//  * is greater than zero. If the formatted string is longer than the buffer size, it
//  * truncates the output to fit.
//  *
//  * @param buffer  The destination buffer where the formatted string will be stored.
//  * @param size    The size of the buffer.
//  * @param format  The format string containing the text to be written, interspersed with
//  *                format specifiers.
//  * @param args    A va_list representing the variable arguments to format.
//  * @return        The total number of characters written, excluding the null terminator.
//  *                If the output is truncated, the return value indicates the total length
//  *                of the formatted string that would have been written.
//  */
// int optimized_vsnprintf(char *buffer, size_t size, const char *format, va_list args)
// {
//     char *p = buffer;
//     const char *fmt = format;
//     size_t remaining = size;
//     int total = 0;

//     while (*fmt && remaining > 0) {
//         if (*fmt != '%') {
//             *p++ = *fmt++;
//             remaining--;
//             total++;
//             continue;
//         }

//         fmt++;  // 跳过'%'
//         if (!*fmt) break;

//         switch (*fmt) {
//             case 'd': {
//                 int num = va_arg(args, int);
//                 unsigned long long abs_num;
//                 int neg = 0;
//                 char num_buf[16];
//                 int digits = 0;

//                 if (num < 0) {
//                     neg = 1;
//                     abs_num = (unsigned long long)(-(long long)num);
//                 } else {
//                     abs_num = (unsigned long long)num;
//                 }

//                 // 处理0的特殊情况
//                 if (abs_num == 0) {
//                     num_buf[digits++] = '0';
//                 } else {
//                     while (abs_num > 0) {
//                         num_buf[digits++] = (abs_num % 10) + '0';
//                         abs_num /= 10;
//                     }
//                 }

//                 // 计算需要的空间
//                 int needed = digits + neg;
//                 total += needed;

//                 // 写入符号
//                 if (neg && remaining > 0) {
//                     *p++ = '-';
//                     remaining--;
//                 }

//                 // 逆序写入数字
//                 for (int i = digits-1; i >= 0 && remaining > 0; i--) {
//                     *p++ = num_buf[i];
//                     remaining--;
//                 }
//                 break;
//             }

//             case 'u': {
//                 unsigned int num = va_arg(args, unsigned int);
//                 char num_buf[16];
//                 int digits = 0;

//                 if (num == 0) {
//                     num_buf[digits++] = '0';
//                 } else {
//                     while (num > 0) {
//                         num_buf[digits++] = (num % 10) + '0';
//                         num /= 10;
//                     }
//                 }

//                 total += digits;
//                 for (int i = digits-1; i >= 0 && remaining > 0; i--) {
//                     *p++ = num_buf[i];
//                     remaining--;
//                 }
//                 break;
//             }

//             case 'x': {
//                 unsigned int num = va_arg(args, unsigned int);
//                 char hex_buf[8];
//                 int digits = 0;
//                 const char *hex_digits = "0123456789abcdef";

//                 if (num == 0) {
//                     hex_buf[digits++] = '0';
//                 } else {
//                     while (num > 0) {
//                         hex_buf[digits++] = hex_digits[num % 16];
//                         num /= 16;
//                     }
//                 }

//                 total += digits;
//                 for (int i = digits-1; i >= 0 && remaining > 0; i--) {
//                     *p++ = hex_buf[i];
//                     remaining--;
//                 }
//                 break;
//             }

//             case 'c': {
//                 char c = (char)va_arg(args, int);
//                 total++;
//                 if (remaining > 0) {
//                     *p++ = c;
//                     remaining--;
//                 }
//                 break;
//             }

//             case 's': {
//                 const char *str = va_arg(args, const char *);
//                 while (*str && remaining > 0) {
//                     *p++ = *str++;
//                     remaining--;
//                     total++;
//                 }
//                 break;
//             }

//             default:  // 处理无效格式说明符
//                 if (remaining > 0) {
//                     *p++ = '%';
//                     remaining--;
//                     total++;
//                 }
//                 if (remaining > 0) {
//                     *p++ = *fmt;
//                     remaining--;
//                     total++;
//                 }
//                 break;
//         }
//         fmt++;
//     }

//     // 保证null终止
//     if (size > 0) {
//         *p = '\0';
//         if (p - buffer >= size) {
//             buffer[size-1] = '\0';
//         }
//     }

//     return total;
// }

// #include <stdio.h>
// void print(const char * format, ...)
// {
//   va_list args;
//   va_start(args, format);
//   // size_t len = optimized_vsnprintf((char *)txBufForDMA, sizeof(txBufForDMA), format, args);
//   size_t len = vsnprintf((char *)txBufForDMA, sizeof(txBufForDMA), format, args);
//   va_end(args);
//   serial_output(txBufForDMA, len);
// }

// void test_print(void) {
//   print("hello world\n");

//   print("char: %c\n", 'A');
//   print("int positive: %d\n", 1234567);
//   print("int negative: %d\n", -1234567);
//   print("unsigned int: %u\n", 7654321);
//   print("string: %s\n", "hello");
//   print("hex: %x\n", 0x12345678);
// }
/* USER CODE END 1 */
