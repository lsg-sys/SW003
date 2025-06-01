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
 * @file Keyboard.h
 */

#include "SmartWatches.h"
#include "Keyboard.h"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "driver_lib/print/print.h"
#include "driver_lib/button/button.h"
/* USER_CODE_END */

/* USER_CODE_BEGIN CUSTOM_CODE_1 */
extern osMessageQueueId_t keyboardEventQueueHandle;
extern osMessageQueueId_t swEventQueueHandle;


extern "C"  void on_keybard_IRQ(uint16_t GPIO_Pin) {

    uint16_t cur_pin = GPIO_Pin;
    button_external_trigger_input_event(&cur_pin);
}

// Function to process valid key press
void processValidKeyPress(uint16_t pin) {
    SmartWatches::Msg swMsg = {SmartWatchesEvents::KeyboardInput, pin};
    osMessageQueuePut(swEventQueueHandle, &swMsg, 0, 0);
}

/* USER_CODE_END */

void Keyboard::Initialize_TriggerKeyboardInitialization_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_TriggerKeyboardInitialization_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::KeyboardInitializationIsComplete, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void Keyboard::Running_TriggerPause_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_TriggerPause_callback */

    /* USER_CODE_END */
}

void Keyboard::Pause_TriggerResume_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_TriggerResume_callback */

    /* USER_CODE_END */
}

void Keyboard::Pause_Sync_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_Sync_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::Sync, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_2 */
extern osSemaphoreId_t Sem_button_scanHandle;
static int scan_is_enabled;

static const struct ButtonEventCallbacks button_event_callbacks[BUTTON_button_number] = {
    {
        .long_press = [](){
            print("button 1 long press\n");
        },
        .single_click = [](){
            // print("button 1 single click\n");
            processValidKeyPress(BUTTON_KEY1_Pin);
        },
        .double_click = [](){
            print("button 1 double click\n");
        }
    },
    {
        .long_press = [](){
            print("button 2 long press\n");
        },
        .single_click = [](){
            // print("button 2 single click\n");
            processValidKeyPress(BUTTON_KEY2_Pin);
        },
        .double_click = [](){
            print("button 2 double click\n");
        }
    },
    {
        .long_press = [](){
            print("button 3 long press\n");
        },
        .single_click = [](){
            // print("button 3 single click\n");
            processValidKeyPress(BUTTON_KEY3_Pin);
        },
        .double_click = [](){
            
        }
    }
};

static const ButtonScanTimerControler scan_timer_controler = {
    .init = [](){
        scan_is_enabled = 0;
        return 0;
    }, 
    .open = [](){

        uint32_t mask = portSET_INTERRUPT_MASK_FROM_ISR();// critical_enter();
        if (0 == scan_is_enabled) {
            scan_is_enabled = 1;
            portCLEAR_INTERRUPT_MASK_FROM_ISR(mask); // critical_leave();
            osSemaphoreRelease(Sem_button_scanHandle);
        } else {
            portCLEAR_INTERRUPT_MASK_FROM_ISR(mask); // critical_leave();
        }

        return 0;
    }, 
    .close = [](){ // scan timer close, on button_PeriodElapsedCallback
        
        // critical_enter();
        scan_is_enabled = 0;
        // critical_leave();
        print("close scan timer\n");
    }
};
/* USER_CODE_END */

void Keyboard::en_Running()
{
    /* USER_CODE_BEGIN en_Running */
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    /* USER_CODE_END */
}

void Keyboard::du_Running()
{
    /* USER_CODE_BEGIN du_Running */
    portENTER_CRITICAL(); // critical_enter();
    if (scan_is_enabled) {
        button_PeriodElapsedCallback();
        portEXIT_CRITICAL(); // critical_leave();
        osSemaphoreAcquire(Sem_button_scanHandle, 0);
    } else {
        portEXIT_CRITICAL(); // critical_leave();
        osSemaphoreAcquire(Sem_button_scanHandle, 60'000);
    }
    /* USER_CODE_END */
}

void Keyboard::ex_Running()
{
    /* USER_CODE_BEGIN ex_Running */
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    /* USER_CODE_END */
}

void Keyboard::en_Pause()
{
    /* USER_CODE_BEGIN en_Pause */

    /* USER_CODE_END */
}

void Keyboard::du_Pause()
{
    /* USER_CODE_BEGIN du_Pause */

    /* USER_CODE_END */
}

void Keyboard::ex_Pause()
{
    /* USER_CODE_BEGIN ex_Pause */

    /* USER_CODE_END */
}

void Keyboard::en_Initialize()
{
    /* USER_CODE_BEGIN en_Initialize */

    /* USER_CODE_END */
}

void Keyboard::du_Initialize()
{
    /* USER_CODE_BEGIN du_Initialize */

    /* USER_CODE_END */
}

void Keyboard::ex_Initialize()
{
    /* USER_CODE_BEGIN ex_Initialize */
    button_init(&scan_timer_controler);

    for (int i = 0; i < BUTTON_button_number; i++) {
        button_register_event_callbacks(i, &button_event_callbacks[i]);
    }
    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_3 */

/* USER_CODE_END */

int Keyboard::recvMessage(Msg &msg) const
{
    int ret = -1;
    /* USER_CODE_BEGIN recvMessage */
    osStatus_t stat = osMessageQueueGet(keyboardEventQueueHandle, &msg, nullptr, pdMS_TO_TICKS(BUTTON_scan_timer_period));
    if (stat == osOK)
        ret = 0;
    /* USER_CODE_END */
    return ret;
}

void Keyboard::logOutput(std::string &output) const
{
    /* USER_CODE_BEGIN logOutput */
    print("[ Keyboard ] \t%s", output.c_str());
    /* USER_CODE_END */
}

