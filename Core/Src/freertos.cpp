/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "gpio.h"
#include "dma.h"
#include "usart.h"
#include "tim.h"

#include "SmartWatches.h"
#include "Panel.h"
#include "Mems.h"
#include "Cmd.h"
#include "Keyboard.h"

#include "DevRTCTick.h"
#include "driver_lib/print/print.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for cmdTask */
osThreadId_t cmdTaskHandle;
const osThreadAttr_t cmdTask_attributes = {
  .name = "cmdTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PanelTask */
osThreadId_t PanelTaskHandle;
const osThreadAttr_t PanelTask_attributes = {
  .name = "PanelTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for MEMSTask */
osThreadId_t MEMSTaskHandle;
const osThreadAttr_t MEMSTask_attributes = {
  .name = "MEMSTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for keyboardTask */
osThreadId_t keyboardTaskHandle;
const osThreadAttr_t keyboardTask_attributes = {
  .name = "keyboardTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for swEventQueue */
osMessageQueueId_t swEventQueueHandle;
const osMessageQueueAttr_t swEventQueue_attributes = {
  .name = "swEventQueue"
};
/* Definitions for memsEventQueue */
osMessageQueueId_t memsEventQueueHandle;
const osMessageQueueAttr_t memsEventQueue_attributes = {
  .name = "memsEventQueue"
};
/* Definitions for panelEventQueue */
osMessageQueueId_t panelEventQueueHandle;
const osMessageQueueAttr_t panelEventQueue_attributes = {
  .name = "panelEventQueue"
};
/* Definitions for cmdEventQueue */
osMessageQueueId_t cmdEventQueueHandle;
const osMessageQueueAttr_t cmdEventQueue_attributes = {
  .name = "cmdEventQueue"
};
/* Definitions for keyboardEventQueue */
osMessageQueueId_t keyboardEventQueueHandle;
const osMessageQueueAttr_t keyboardEventQueue_attributes = {
  .name = "keyboardEventQueue"
};
/* Definitions for TimeoutTimer */
osTimerId_t TimeoutTimerHandle;
const osTimerAttr_t TimeoutTimer_attributes = {
  .name = "TimeoutTimer"
};
/* Definitions for Sem_button_scan */
osSemaphoreId_t Sem_button_scanHandle;
const osSemaphoreAttr_t Sem_button_scan_attributes = {
  .name = "Sem_button_scan"
};
/* Definitions for pclk_Sem */
osSemaphoreId_t pclk_SemHandle;
const osSemaphoreAttr_t pclk_Sem_attributes = {
  .name = "pclk_Sem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartCmdTask(void *argument);
void StartPanelTask(void *argument);
void StartMEMSTask(void *argument);
void StartKeyboardTask(void *argument);
void TimeoutTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
  // 使用TIM2，1ms增加1计数值（1kHz），最大计数0xFFFF'FFFF
  // HAL_TIM_Base_Start(&htim2);
}

__weak unsigned long getRunTimeCounterValue(void)
{
  // return __HAL_TIM_GET_COUNTER(&htim2);

  if (__get_IPSR() == 0U) { // not in ISR
      return ((unsigned long)xTaskGetTickCount());
  } else {
      return ((unsigned long)xTaskGetTickCountFromISR());
  }
}
/* USER CODE END 1 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */

  extern void lv_tick_inc(uint32_t tick_period);
  const uint32_t tick_inc_1ms = 1;
  lv_tick_inc(tick_inc_1ms);
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
   (void)xTask;
   (void)pcTaskName;
   configASSERT(0);
}

void debug_hardware_output_error(int code) {

  switch (code) {

  case HW_ERROR_CODE_FLASH:
      for (size_t i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        osDelay(1000);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        osDelay(1000);
      }
      break;
      
      case HW_ERROR_CODE_MEMS:
      for (size_t i = 0; i < 6; i++) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        osDelay(300);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        osDelay(300);
      }
      break;

  default:
      break;
  }
}
/* USER CODE END 4 */

// /* USER CODE BEGIN PREPOSTSLEEP */
// __weak void PreSleepProcessing(uint32_t ulExpectedIdleTime)
// {
// /* place for user code */
//   HAL_SuspendTick();
//   // HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
//   HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
// }

// __weak void PostSleepProcessing(uint32_t ulExpectedIdleTime)
// {
// /* place for user code */
//   // SystemCoreClockUpdate();
//   HAL_ResumeTick();
// }
// /* USER CODE END PREPOSTSLEEP */

/* USER CODE BEGIN VPORT_SUPPORT_TICKS_AND_SLEEP */
__weak void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
  // Generated when configUSE_TICKLESS_IDLE == 2.
  // Function called in tasks.c (in portTASK_FUNCTION).
  // TO BE COMPLETED or TO BE REPLACED by a user one, overriding that weak one.

  unsigned long    ulLowPowerTimeBeforeSleep, ulLowPowerTimeAfterSleep;
  eSleepModeStatus eSleepStatus;

  /* Read the current time from a time source that will remain operational
     while the microcontroller is in a low power state. */
  // ulLowPowerTimeBeforeSleep = ulGetExternalTime();
  ulLowPowerTimeBeforeSleep = RTCTick_get_tick_ms();

  /* Stop the timer that is generating the tick interrupt. */
  // prvStopTickInterruptTimer();
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

  /* Enter a critical section that will not effect interrupts bringing the MCU
     out of sleep mode. */
  // disable_interrupts();
  __asm volatile( "cpsid i" ::: "memory" );
  __asm volatile( "dsb" );
  __asm volatile( "isb" );

  /* Ensure it is still ok to enter the sleep mode. */
  eSleepStatus = eTaskConfirmSleepModeStatus();
  if (eSleepStatus == eAbortSleep) {
      /* A task has been moved out of the Blocked state since this macro was
         executed, or a context siwth is being held pending. Do not enter a
         sleep state. Restart the tick and exit the critical section. */
      // prvStartTickInterruptTimer();
      SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
      // enable_interrupts();
      __asm volatile( "cpsie i" ::: "memory" );

  } else {

      /* Configure an interrupt to bring the microcontroller out of its low
          power state at the time the kernel next needs to execute. The
          interrupt must be generated from a source that remains operational
          when the microcontroller is in a low power state. */
      // vSetWakeTimeInterrupt(xExpectedIdleTime);
      set_wakeup_time_interrupt_ms(xExpectedIdleTime);

      /* Enter the low power state. */
      // prvSleep();
      HAL_SuspendTick();
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      HAL_ResumeTick();
      clear_wakeup_time_interrupt();

      /* Determine how long the microcontroller was actually in a low power
          state for, which will be less than xExpectedIdleTime if the
          microcontroller was brought out of low power mode by an interrupt
          other than that configured by the vSetWakeTimeInterrupt() call.
          Note that the scheduler is suspended before
          portSUPPRESS_TICKS_AND_SLEEP() is called, and resumed when
          portSUPPRESS_TICKS_AND_SLEEP() returns. Therefore no other tasks will
          execute until this function completes. */
      // ulLowPowerTimeAfterSleep = ulGetExternalTime();
      ulLowPowerTimeAfterSleep = RTCTick_get_tick_ms();

      /* Correct the kernels tick count to account for the time the
          microcontroller spent in its low power state. */
      // vTaskStepTick(ulLowPowerTimeAfterSleep - ulLowPowerTimeBeforeSleep);
      unsigned long delta_ticks = RTCTick_get_elapsedTime_ms(ulLowPowerTimeBeforeSleep, ulLowPowerTimeAfterSleep);
      // unsigned long temp = 0;
      if (xExpectedIdleTime < delta_ticks) {
        // temp = delta_ticks - xExpectedIdleTime;
        delta_ticks = xExpectedIdleTime;
      }
      vTaskStepTick(delta_ticks);
      
      /* Exit the critical section - it might be possible to do this immediately
      after the prvSleep() calls. */
      // enable_interrupts();
      __asm volatile( "cpsie i" ::: "memory" );
      
      // if (temp) {
      //   print("额外sleep: %d | ", temp);
      // }
      // print("delta ticks: %d\n", delta_ticks);

      /* Restart the timer that is generating the tick interrupt. */
      // prvStartTickInterruptTimer();
      SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  }
}
/* USER CODE END VPORT_SUPPORT_TICKS_AND_SLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Sem_button_scan */
  Sem_button_scanHandle = osSemaphoreNew(2, 0, &Sem_button_scan_attributes);

  /* creation of pclk_Sem */
  pclk_SemHandle = osSemaphoreNew(6, 6, &pclk_Sem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of TimeoutTimer */
  TimeoutTimerHandle = osTimerNew(TimeoutTimerCallback, osTimerPeriodic, NULL, &TimeoutTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of swEventQueue */
  swEventQueueHandle = osMessageQueueNew (5, sizeof(int64_t), &swEventQueue_attributes);

  /* creation of memsEventQueue */
  memsEventQueueHandle = osMessageQueueNew (5, sizeof(int64_t), &memsEventQueue_attributes);

  /* creation of panelEventQueue */
  panelEventQueueHandle = osMessageQueueNew (5, sizeof(int64_t), &panelEventQueue_attributes);

  /* creation of cmdEventQueue */
  cmdEventQueueHandle = osMessageQueueNew (5, sizeof(int64_t), &cmdEventQueue_attributes);

  /* creation of keyboardEventQueue */
  keyboardEventQueueHandle = osMessageQueueNew (5, sizeof(int64_t), &keyboardEventQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of cmdTask */
  cmdTaskHandle = osThreadNew(StartCmdTask, NULL, &cmdTask_attributes);

  /* creation of PanelTask */
  PanelTaskHandle = osThreadNew(StartPanelTask, NULL, &PanelTask_attributes);

  /* creation of MEMSTask */
  MEMSTaskHandle = osThreadNew(StartMEMSTask, NULL, &MEMSTask_attributes);

  /* creation of keyboardTask */
  keyboardTaskHandle = osThreadNew(StartKeyboardTask, NULL, &keyboardTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  auto &fsm = SmartWatches::getInstance();
  fsm.run();
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartCmdTask */
/**
* @brief Function implementing the cmdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCmdTask */
void StartCmdTask(void *argument)
{
  /* USER CODE BEGIN StartCmdTask */
  auto &fsm = Cmd::getInstance();
  fsm.run();
  /* USER CODE END StartCmdTask */
}

/* USER CODE BEGIN Header_StartPanelTask */
/**
* @brief Function implementing the PanelTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPanelTask */
void StartPanelTask(void *argument)
{
  /* USER CODE BEGIN StartPanelTask */
  auto &fsm = Panel::getInstance();
  fsm.run();
  /* USER CODE END StartPanelTask */
}

/* USER CODE BEGIN Header_StartMEMSTask */
/**
* @brief Function implementing the MEMSTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMEMSTask */
void StartMEMSTask(void *argument)
{
  /* USER CODE BEGIN StartMEMSTask */
  auto &fsm = Mems::getInstance();
  fsm.run();
  /* USER CODE END StartMEMSTask */
}

/* USER CODE BEGIN Header_StartKeyboardTask */
/**
* @brief Function implementing the keyboardTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyboardTask */
void StartKeyboardTask(void *argument)
{
  /* USER CODE BEGIN StartKeyboardTask */
  auto &fsm = Keyboard::getInstance();
  fsm.run();
  /* USER CODE END StartKeyboardTask */
}

/* TimeoutTimerCallback function */
void TimeoutTimerCallback(void *argument)
{
  /* USER CODE BEGIN TimeoutTimerCallback */
  // print("🧶 TimeoutTimerCallback\n");
  extern void incrementUnactiveTimeoutSeconds();
  incrementUnactiveTimeoutSeconds();
  /* USER CODE END TimeoutTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void* operator new(std::size_t size) {
  return pvPortMalloc(size);
}

void operator delete(void* ptr) noexcept {
  vPortFree(ptr);
}

void* operator new[](std::size_t size) {
  return pvPortMalloc(size);
}

void operator delete[](void* ptr) noexcept {
  vPortFree(ptr);
}

extern "C" void pclk_dependent(void) {
  osSemaphoreAcquire(pclk_SemHandle, 0);
}

extern "C" void pclk_release(void) {
  osSemaphoreRelease(pclk_SemHandle);
}

extern "C" uint32_t enter_critical(void) {
  if (__get_IPSR() == 0U) { // not in ISR
    portENTER_CRITICAL();
    return 0;
  } else {
      return portSET_INTERRUPT_MASK_FROM_ISR();
  }
}

extern "C" void leave_critical(uint32_t mask) {
  if (__get_IPSR() == 0U) { // not in ISR
    portEXIT_CRITICAL();
  } else {
    portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
  }
}

/* USER CODE END Application */

