/**
 * 
 *      ___                                  
 *     /   ""=-,_                   ,  _.    
 *     |         .E=-,_  __          \\/     
 *      \        \  ``, '7 "-,     //' \\    
 *       \        \    ``+ 1  "=,//'         
 *        \        \\     ",.||/ \           
 *          \        ",     ,^:'"; \\        
 *           \\        "-//    ', "-_l       
 *             "1      //"\_     '"_-.       
 *               ""_//`     "+,    '='\      
 *                //\:         `";_  `=|     
 *              //'   `\_          `""`=,    
 *           //'         "=_            ^\   
 *         //'              "=_          '\  
 *       //'                   ""=_      '|  
 *                                 '""'==*'  
 * 
 * @author lsg
 * @file SmartWatches.h
 */

#include "Panel.h"
#include "Cmd.h"
#include "Mems.h"
#include "SmartWatches.h"
#include "Keyboard.h"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"

#include "driver_lib/print/print.h"
#include "iwdg.h"
#include "usb_device.h"

#include "test.h"

#include "usart.h"

/* USER_CODE_END */

/* USER_CODE_BEGIN CUSTOM_CODE_1 */

#include "usart.h"
extern void SystemClock_set_HCLK_Freq(uint32_t freq);
extern "C" void vPortSetupTimerInterrupt( void );
extern "C" uint32_t w25q128_is_free(void);

extern osMessageQueueId_t swEventQueueHandle;
extern osMessageQueueId_t panelEventQueueHandle;
extern osMessageQueueId_t memsEventQueueHandle;
extern osMessageQueueId_t cmdEventQueueHandle;
extern osMessageQueueId_t keyboardEventQueueHandle;
extern osTimerId_t        TimeoutTimerHandle;

extern osSemaphoreId_t Sem_button_scanHandle;

#define TIMEOUT_BASE_SECONDS 5
uint32_t timeout_threshold = TIMEOUT_BASE_SECONDS;
uint32_t timeout_count_seconds;
void startUnactiveTimer() {
    /* 断言，定时器已经初始化了并且是osTimerPeriodic */
    osTimerStart(TimeoutTimerHandle, pdMS_TO_TICKS(1000));
}

void stopUnactiveTimer() {
    osTimerStop(TimeoutTimerHandle);
}

void resetUnactiveTimeoutSeconds() {
    /* 直接清零计数，最大误差≤1秒 */
    timeout_count_seconds = 0;
}
void setUnactiveTimeoutThreshold(uint32_t threshold_seconds) {
    resetUnactiveTimeoutSeconds();
    timeout_threshold = threshold_seconds;
    print("setUnactiveTimeoutThreshold %u seconds\n", threshold_seconds);
}

void incrementUnactiveTimeoutSeconds() {

    if (timeout_threshold <= ++timeout_count_seconds) {
        timeout_count_seconds = 0;

        stopUnactiveTimer();
        // clearUnactiveTimeout();

        SmartWatches::Msg msg = {SmartWatchesEvents::Timeout, 0};
        osMessageQueuePut(swEventQueueHandle, &msg, 0, 0);
    }
}

static void deivce_pause_input() {
    
    osStatus_t stat;

    Keyboard::Msg keyboardMsg = {KeyboardEvents::TriggerPause, 0};
    stat = osMessageQueuePut(keyboardEventQueueHandle, &keyboardMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    Cmd::Msg cmdMsg = {CmdEvents::TriggerPause, 0};
    stat = osMessageQueuePut(cmdEventQueueHandle, &cmdMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    Mems::Msg memsMsg = {MemsEvents::TriggerPause, 0};
    stat = osMessageQueuePut(memsEventQueueHandle, &memsMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    HAL_UART_Abort(&huart1);
}

static void deivce_sycn_input() {
    
    SmartWatches::Msg msg;
    
    Keyboard::Msg keyboardMsg = {KeyboardEvents::Sync, 0};
    osMessageQueuePut(keyboardEventQueueHandle, &keyboardMsg, 0, portMAX_DELAY);
    osSemaphoreRelease(Sem_button_scanHandle); // 如果 keyboard 在 running, 则 keyboard 会长时间阻塞，需要唤醒，才会及时响应，否则可能要等很久
    msg.event = SmartWatchesEvents::None;
    while (SmartWatchesEvents::Sync != msg.event) {
        osMessageQueueGet(swEventQueueHandle, &msg, nullptr, portMAX_DELAY);
    }

    Cmd::Msg cmdMsg = {CmdEvents::Sync, 0};
    osMessageQueuePut(cmdEventQueueHandle, &cmdMsg, 0, portMAX_DELAY);
    msg.event = SmartWatchesEvents::None;
    while (SmartWatchesEvents::Sync != msg.event) {
        osMessageQueueGet(swEventQueueHandle, &msg, nullptr, portMAX_DELAY);
    }

    Mems::Msg memsMsg = {MemsEvents::Sync, 0};
    osMessageQueuePut(memsEventQueueHandle, &memsMsg, 0, portMAX_DELAY);
    msg.event = SmartWatchesEvents::None;
    while (SmartWatchesEvents::Sync != msg.event) {
        osMessageQueueGet(swEventQueueHandle, &msg, nullptr, portMAX_DELAY);
    }
}

static void deivce_resume_input() {
    
    osStatus_t stat;

    Keyboard::Msg keyboardMsg = {KeyboardEvents::TriggerResume, 0};
    stat = osMessageQueuePut(keyboardEventQueueHandle, &keyboardMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    Cmd::Msg cmdMsg = {CmdEvents::TriggerResume, 0};
    stat = osMessageQueuePut(cmdEventQueueHandle, &cmdMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    Mems::Msg memsMsg = {MemsEvents::TriggerResume, 0};
    stat = osMessageQueuePut(memsEventQueueHandle, &memsMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
}

/* 调用此函数前，必须保证panel已经进入sleep */
static void deivce_set_system_freq(uint32_t freq) {
    deivce_pause_input();
    deivce_sycn_input();

    /* 关闭全局中断 */
    __disable_irq();
    
    /* USB未连接时可随意变频 或 USB连接时可升频率 */
    if (w25q128_is_free() ||
        HAL_RCC_GetHCLKFreq() < freq ) {

        HAL_UART_DeInit(&huart1);
    
        /* 配置时钟 */
        SystemClock_set_HCLK_Freq(freq);
        vPortSetupTimerInterrupt();
    
        MX_USART1_UART_Init();
    }

    /* 恢复全局中断 */
    __enable_irq();

    deivce_resume_input();
}

static void deivce_enter_sleep() {
    
    osStatus_t stat;

    Panel::Msg panelMsg = {PanelEvents::TriggerSleep, 0};
    stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
}

static void deivce_exit_sleep() {
    Panel::Msg panelMsg = {PanelEvents::TriggerWakeup, 0};
    osStatus_t stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    startUnactiveTimer();
}

/* USER_CODE_END */

void SmartWatches::Initialize_PowerOn_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_PowerOn_callback */
    test();

    Cmd::Msg cmdMsg = {CmdEvents::TriggerCmdInitialization, 0};
    osStatus_t stat = osMessageQueuePut(cmdEventQueueHandle, &cmdMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void SmartWatches::Initialize_CmdInitializationIsComplete_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_CmdInitializationIsComplete_callback */
    Panel::Msg panelMsg = {PanelEvents::TriggerPanelInitialization, 0};
    osStatus_t stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void SmartWatches::Initialize_PanelInitializationIsComplete_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_PanelInitializationIsComplete_callback */
    Mems::Msg memsMsg = {MemsEvents::TriggerMemsInitialization, 0};
    osStatus_t stat = osMessageQueuePut(memsEventQueueHandle, &memsMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void SmartWatches::Initialize_MemsInitializationIsComplete_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_MemsInitializationIsComplete_callback */
    Keyboard::Msg keyboardMsg = {KeyboardEvents::TriggerKeyboardInitialization, 0};
    osStatus_t stat = osMessageQueuePut(keyboardEventQueueHandle, &keyboardMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void SmartWatches::Initialize_KeyboardInitializationIsComplete_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_KeyboardInitializationIsComplete_callback */
    MX_USB_DEVICE_Init();
    
    /*
    Start the timeout timer after the Panel module has been initialized.
    This is to avoid triggering the timeout during the Panel module's initialization.
    */
    startUnactiveTimer();
    /* USER_CODE_END */
}

void SmartWatches::Running_KeyboardInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_KeyboardInput_callback */
    InputKeys key;
    if (msg.arg == KEY_B_Pin) {
        key = InputKeys::ENTER;
    } else if (msg.arg == KEY_D_Pin) {
        key = InputKeys::ESCAPE;
    } else {
        /* undefined */
        return;
    }

    Panel::Msg panelMsg = {PanelEvents::KeyInput, static_cast<uint32_t>(key)};
    osStatus_t stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    resetUnactiveTimeoutSeconds();
    /* USER_CODE_END */
}

void SmartWatches::Running_MemsInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_MemsInput_callback */
    char tag = (char)msg.arg;

    InputKeys key;
    switch (tag) {
        case 'e':
            key = InputKeys::UP;
            break;
        case 'f':
            key = InputKeys::DOWN;
            break;
        case 'g':
            key = InputKeys::LEFT;
            break;
        case 'h':
            key = InputKeys::RIGHT;
            break;
        default:
            // break;
            return;
    }
    Panel::Msg panelMsg = {PanelEvents::KeyInput, static_cast<uint32_t>(key)};
    osStatus_t stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    resetUnactiveTimeoutSeconds();
    /* USER_CODE_END */
}

void SmartWatches::Running_CmdInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_CmdInput_callback */
    Panel::Msg panelMsg = {PanelEvents::KeyInput, msg.arg};
    osStatus_t stat = osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    resetUnactiveTimeoutSeconds();
    /* USER_CODE_END */
}

void SmartWatches::Running_TriggerSleep_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_TriggerSleep_callback */

    /* USER_CODE_END */
}

void SmartWatches::Running_Timeout_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_Timeout_callback */

    /* USER_CODE_END */
}

void SmartWatches::Sleep_KeyboardInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_KeyboardInput_callback */

    /* USER_CODE_END */
}

void SmartWatches::Sleep_MemsInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_MemsInput_callback */

    /* USER_CODE_END */
}

void SmartWatches::Sleep_CmdInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_CmdInput_callback */

    /* USER_CODE_END */
}

void SmartWatches::Sleep_Sync_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_Sync_callback */

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_2 */

/* USER_CODE_END */

void SmartWatches::en_Sleep()
{
    /* USER_CODE_BEGIN en_Sleep */
    deivce_enter_sleep();
    /* 
        检查条件 disable async：
        + adc1 同步 mems
        + spi2 同步 mems
        + spi1 同步 panle
        + tim1  同步 panle
        usart1 同步cmd 其他线程可能在使用，但可以强行关闭
        + spi3 同步 中断和panle
        
        tim11 可以直接关
        systick 可以直接关
    */
    Msg msg;

    Panel::Msg panelMsg = {PanelEvents::Sync, 0};
    osMessageQueuePut(panelEventQueueHandle, &panelMsg, 0, portMAX_DELAY);
    msg.event = SmartWatchesEvents::None;
    osMessageQueueGet(swEventQueueHandle, &msg, nullptr, portMAX_DELAY);
    configASSERT(SmartWatchesEvents::Sync == msg.event);

    deivce_set_system_freq(24'000'000u);

    /* USER_CODE_END */
}

void SmartWatches::du_Sleep()
{
    /* USER_CODE_BEGIN du_Sleep */
    static int temp_count = 0;
    temp_count+=20;
    if (10000 < temp_count) {
        temp_count = 0;
        print("Sleep 10s\n");
        print("current HCLK : %u\n", HAL_RCC_GetHCLKFreq());
        print("w25q is free : %d\n", w25q128_is_free());

        /* 每10秒检查是否为低频模式，并尝试进入低频模式 */
        if (HAL_RCC_GetHCLKFreq() != 24'000'000u) {
            deivce_set_system_freq(24'000'000u);
        }
    }
    /* USER_CODE_END */
}

void SmartWatches::ex_Sleep()
{
    /* USER_CODE_BEGIN ex_Sleep */

    deivce_set_system_freq(96'000'000u);
    
    deivce_exit_sleep();

    /* USER_CODE_END */
}

void SmartWatches::en_Running()
{
    /* USER_CODE_BEGIN en_Running */

    /* USER_CODE_END */
}

void SmartWatches::du_Running()
{
    /* USER_CODE_BEGIN du_Running */
    static int temp_count = 0;
    if (600 < temp_count) {
        temp_count = 0;
        // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        uint8_t CPU_RunInfo[400];
        static uint32_t countForCPU = 0;
        if (countForCPU < 6000) {
            countForCPU+=600;
        } else {
            countForCPU = 0;
            memset(CPU_RunInfo,0,400); 
            print("TaskName\tState\tPrio\tStkRmn\tCrtOdr\n");
            vTaskList((char *)&CPU_RunInfo);
            print("%s", CPU_RunInfo);
            memset(CPU_RunInfo,0,400);
            print("TaskName\tCount\tpercent\n");
            vTaskGetRunTimeStats((char *)&CPU_RunInfo);
            print("%s", CPU_RunInfo);
        }
    }
    temp_count+=20;
    /* USER_CODE_END */
}

void SmartWatches::ex_Running()
{
    /* USER_CODE_BEGIN ex_Running */

    /* USER_CODE_END */
}

void SmartWatches::en_Initialize()
{
    /* USER_CODE_BEGIN en_Initialize */

    /* USER_CODE_END */
}

void SmartWatches::du_Initialize()
{
    /* USER_CODE_BEGIN du_Initialize */

    /* USER_CODE_END */
}

void SmartWatches::ex_Initialize()
{
    /* USER_CODE_BEGIN ex_Initialize */

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_3 */

/* USER_CODE_END */

int SmartWatches::recvMessage(Msg &msg) const
{
    int ret = -1;
    /* USER_CODE_BEGIN recvMessage */
    HAL_IWDG_Refresh(&hiwdg);

    osStatus_t stat = osMessageQueueGet(swEventQueueHandle, &msg, nullptr, pdMS_TO_TICKS(20));
    if (stat == osOK)
        ret = 0;
    /* USER_CODE_END */
    return ret;
}

void SmartWatches::logOutput(std::string &output) const
{
    /* USER_CODE_BEGIN logOutput */
    print("[ sw ] \t%s", output.c_str());
    /* USER_CODE_END */
}

