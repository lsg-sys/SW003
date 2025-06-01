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
 * @file Cmd.h
 */

#include "Cmd.h"
#include "SmartWatches.h"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include <cstring>

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "driver_lib/print/print.h"
/* USER_CODE_END */

/* USER_CODE_BEGIN CUSTOM_CODE_1 */
extern osMessageQueueId_t swEventQueueHandle;
extern osMessageQueueId_t cmdEventQueueHandle;

struct s_CmdEntry {
    char const* str;
    void (*handler)(void);
};
using CmdEntry = struct s_CmdEntry;

// Function to send command input message
static void sendCommandInputToSmartWatches(InputKeys key) {
    SmartWatches::Msg swMsg = {SmartWatchesEvents::CmdInput, static_cast<uint32_t>(key)};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
}

void CmdHandle_PanelSleep() {
    SmartWatches::Msg swMsg = {SmartWatchesEvents::TriggerSleep, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
}

void CmdHandle_PanelWakeup() {
    /* 输入任意按键即可唤醒屏幕 */
    sendCommandInputToSmartWatches(InputKeys::ESCAPE);
}

void CmdHandle_PanelSimulationInputUp() {
    sendCommandInputToSmartWatches(InputKeys::UP);
}

void CmdHandle_PanelSimulationInputDown() {
    sendCommandInputToSmartWatches(InputKeys::DOWN);
}

void CmdHandle_PanelSimulationInputLeft() {
    sendCommandInputToSmartWatches(InputKeys::LEFT);
}

void CmdHandle_PanelSimulationInputRight() {
    sendCommandInputToSmartWatches(InputKeys::RIGHT);
}

void CmdHandle_PanelSimulationInputEnter() {
    sendCommandInputToSmartWatches(InputKeys::ENTER);
}

void CmdHandle_PanelSimulationInputEscape() {
    sendCommandInputToSmartWatches(InputKeys::ESCAPE);
}

const CmdEntry cmdList[] = {
    {"Panel sleep", CmdHandle_PanelSleep},
    {"Panel wakeup", CmdHandle_PanelWakeup},
    {"Panel simulation input up", CmdHandle_PanelSimulationInputUp},
    {"Panel simulation input down", CmdHandle_PanelSimulationInputDown},
    {"Panel simulation input left", CmdHandle_PanelSimulationInputLeft},
    {"Panel simulation input right", CmdHandle_PanelSimulationInputRight},
    {"Panel simulation input enter", CmdHandle_PanelSimulationInputEnter},
    {"Panel simulation input escape", CmdHandle_PanelSimulationInputEscape},
};

std::string extractContent(const std::string& input) {
    size_t start = input.find('>');
    if (start == std::string::npos) {
        return ""; // 没有找到 '>' 字符
    }
    start++; // 跳过 '>' 字符

    size_t end = input.find('<', start);
    if (end == std::string::npos) {
        return ""; // 没有找到 '<' 字符
    }

    return input.substr(start, end - start);
}

void cmdProcess(std::string& cmd) {

    std::string result = extractContent(cmd);

    for (int i = 0; i < (int)(sizeof(cmdList)/sizeof(cmdList[0])); i++) {
        if (result == cmdList[i].str) {
            cmdList[i].handler();
            break;
        }
    }
}

uint8_t rxBuf[256];
typedef struct {
    uint8_t len;
    uint8_t data[];
} SerialReceivePaket;

extern "C" void onSerialReceive(uint8_t const *data, uint32_t len) {

    // configASSERT(len < sizeof(rxBuf)-1);
    uint32_t max_receive_len = sizeof(rxBuf)-1;
    if (len > max_receive_len) {
        len = max_receive_len;
    }
    
    SerialReceivePaket *paket = (SerialReceivePaket*)rxBuf;

    paket->len = (uint8_t)len;
    memcpy(paket->data, data, len);
 
    Cmd::Msg cmdMsg;
    cmdMsg.event = CmdEvents::IRQInput;
    cmdMsg.arg = (uint32_t)paket;
    osStatus_t stat = osMessageQueuePut(cmdEventQueueHandle, &cmdMsg, 0, 0);
    configASSERT(stat == osOK);
}
/* USER_CODE_END */

void Cmd::Initialize_TriggerCmdInitialization_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_TriggerCmdInitialization_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::CmdInitializationIsComplete, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void Cmd::Running_IRQInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_IRQInput_callback */
    SerialReceivePaket *paket = (SerialReceivePaket *)msg.arg;
    
    // echo
    print("%s\n", (char *)(paket->data));

    std::string cmdString((char *)(paket->data));
    cmdProcess(cmdString);
    /* USER_CODE_END */
}

void Cmd::Running_TriggerPause_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_TriggerPause_callback */

    /* USER_CODE_END */
}

void Cmd::Pause_TriggerResume_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_TriggerResume_callback */

    /* USER_CODE_END */
}

void Cmd::Pause_Sync_callback(Msg msg)
{
    /* USER_CODE_BEGIN Pause_Sync_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::Sync, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_2 */

/* USER_CODE_END */

void Cmd::en_Running()
{
    /* USER_CODE_BEGIN en_Running */

    /* USER_CODE_END */
}

void Cmd::du_Running()
{
    /* USER_CODE_BEGIN du_Running */
    char buf[256];
    int received_len = scan(buf, sizeof(buf), PRINT_SCAN_TIMEOUT_NEVER);
    if (0 < received_len) {
        onSerialReceive((uint8_t *)buf, received_len);
    }
    /* USER_CODE_END */
}

void Cmd::ex_Running()
{
    /* USER_CODE_BEGIN ex_Running */

    /* USER_CODE_END */
}

void Cmd::en_Pause()
{
    /* USER_CODE_BEGIN en_Pause */

    /* USER_CODE_END */
}

void Cmd::du_Pause()
{
    /* USER_CODE_BEGIN du_Pause */

    /* USER_CODE_END */
}

void Cmd::ex_Pause()
{
    /* USER_CODE_BEGIN ex_Pause */

    /* USER_CODE_END */
}

void Cmd::en_Initialize()
{
    /* USER_CODE_BEGIN en_Initialize */

    /* USER_CODE_END */
}

void Cmd::du_Initialize()
{
    /* USER_CODE_BEGIN du_Initialize */

    /* USER_CODE_END */
}

void Cmd::ex_Initialize()
{
    /* USER_CODE_BEGIN ex_Initialize */

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_3 */

/* USER_CODE_END */

int Cmd::recvMessage(Msg &msg) const
{
    int ret = -1;
    /* USER_CODE_BEGIN recvMessage */
    osStatus_t stat = osMessageQueueGet(cmdEventQueueHandle, (void *)(&msg), nullptr, 50 / portTICK_PERIOD_MS);
    if (stat == osOK)
        ret = 0;
    /* USER_CODE_END */
    return ret;
}

void Cmd::logOutput(std::string &output) const
{
    /* USER_CODE_BEGIN logOutput */
    print("[ Cmd ] \t%s", output.c_str());
    /* USER_CODE_END */
}

