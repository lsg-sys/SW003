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
 * @file Mems.h
 */

#include "Mems.h"
#include "SmartWatches.h"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include <cstring>

#include "cmsis_os2.h"
#include "FreeRTOS.h"

#include "driver_lib/print/print.h"

#include "DevLsm6dso.h"
#include "DevBattery.h"

#include "spi.h"
#include "adc.h"

/* USER_CODE_END */

/* USER_CODE_BEGIN CUSTOM_CODE_1 */
extern osMessageQueueId_t memsEventQueueHandle;
extern osMessageQueueId_t swEventQueueHandle;

static std::string outStr;

void processValidMemsKeyPress(char tag) {

    print("Input mems key: %c\n", tag);

    SmartWatches::Msg swMsg = {SmartWatchesEvents::MemsInput, (uint32_t)tag};
    osMessageQueuePut(swEventQueueHandle, &swMsg, 0, 0);
}
/* USER_CODE_END */

void Mems::Initialize_TriggerMemsInitialization_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_TriggerMemsInitialization_callback */
    DevLsm6dso &dev_lsm6dso = DevLsm6dso::getInstance();
    dev_lsm6dso.init();

    DevBattery &dev_battery = DevBattery::getInstance();
    dev_battery.init();

    SmartWatches::Msg swMsg = {SmartWatchesEvents::MemsInitializationIsComplete, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void Mems::Running_TriggerPause_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_TriggerPause_callback */

    /* USER_CODE_END */
}

void Mems::Pause_TriggerResume_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_TriggerResume_callback */

    /* USER_CODE_END */
}

void Mems::Pause_Sync_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_Sync_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::Sync, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_2 */

/* USER_CODE_END */

void Mems::en_Running()
{
    /* USER_CODE_BEGIN en_Running */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    /* USER_CODE_END */
}

void Mems::du_Running()
{
    /* USER_CODE_BEGIN du_Running */
    DevLsm6dso::getInstance().scanKeys(10, (void *)processValidMemsKeyPress);

    static int count_for_battery_sample = 0;
    count_for_battery_sample += 10;
    if (count_for_battery_sample == 30*1000) { // 30s
        count_for_battery_sample = 0;
        DevBattery::getInstance().sample();
    }
    /* USER_CODE_END */
}

void Mems::ex_Running()
{
    /* USER_CODE_BEGIN ex_Running */
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    /* USER_CODE_END */
}

void Mems::en_Pause()
{
    /* USER_CODE_BEGIN en_Pause */
    
    HAL_ADC_DeInit(&hadc1);
    HAL_SPI_DeInit(&hspi2);

    /* USER_CODE_END */
}

void Mems::du_Pause()
{
    /* USER_CODE_BEGIN du_Pause */

    /* USER_CODE_END */
}

void Mems::ex_Pause()
{
    /* USER_CODE_BEGIN ex_Pause */

    MX_SPI2_Init();
    MX_ADC1_Init();

    /* USER_CODE_END */
}

void Mems::en_Initialize()
{
    /* USER_CODE_BEGIN en_Initialize */

    /* USER_CODE_END */
}

void Mems::du_Initialize()
{
    /* USER_CODE_BEGIN du_Initialize */

    /* USER_CODE_END */
}

void Mems::ex_Initialize()
{
    /* USER_CODE_BEGIN ex_Initialize */

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_3 */

/* USER_CODE_END */

int Mems::recvMessage(Msg &msg) const
{
    int ret = -1;
    /* USER_CODE_BEGIN recvMessage */
    osStatus_t stat = osMessageQueueGet(memsEventQueueHandle, (void *)(&msg), nullptr, 50 / portTICK_PERIOD_MS);
    if (stat == osOK)
        ret = 0;
    /* USER_CODE_END */
    return ret;
}

void Mems::logOutput(std::string &output) const
{
    /* USER_CODE_BEGIN logOutput */
    print("[ Mems ] \t%s", output.c_str());
    /* USER_CODE_END */
}

