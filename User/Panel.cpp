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
 * @file Panel.h
 */

#include "Panel.h"
#include "SmartWatches.h"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include "driver_lib/lcd/lcd.h"
#include "driver_lib/lvgl_v9_3_0/porting/lv_port_disp.h"
#include "driver_lib/lvgl_v9_3_0/porting/lv_port_indev.h"
#include "driver_lib/lvgl_v9_3_0/lvgl.h"

#include "ui/ui_framework.h"

#include "input_key_def.h"

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "driver_lib/print/print.h"
#include "rtc.h"

#include "DevConfigFile.h"
#include "DevBattery.h"
/* USER_CODE_END */

/* USER_CODE_BEGIN CUSTOM_CODE_1 */
extern osMessageQueueId_t swEventQueueHandle;
extern osMessageQueueId_t panelEventQueueHandle;

int get_date_and_time(date_and_time_t *date_time) {

    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;
    HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
    date_time->s.sec = rtcTime.Seconds % 60;
    date_time->s.min = rtcTime.Minutes % 60;
    date_time->s.hour = rtcTime.Hours  % 24;
    date_time->s.day = rtcDate.Date;
    date_time->s.month = rtcDate.Month;
    date_time->s.year = rtcDate.Year + 2000; /* 硬件上可被4整除就是闰年，在这个应用内定义20=2020 */
    

    // print("Get date and time: %d-%d-%d %d:%d:%d\n", date_time->s.year, date_time->s.month, date_time->s.day, date_time->s.hour, date_time->s.min, date_time->s.sec);

    return (0);
}

int set_date_and_time(date_and_time_t *date_time) {

    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

    rtcTime.Hours = date_time->s.hour;
    rtcTime.Minutes = date_time->s.min;
    rtcTime.Seconds = date_time->s.sec;

    rtcDate.Year = date_time->s.year - 2000;
    rtcDate.Month = date_time->s.month;
    rtcDate.Date = date_time->s.day;

    HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

    return (0);
}

extern void setUnactiveTimeoutThreshold(uint32_t threshold_seconds);

hardware_api_t hw_api = {
    .get_date_and_time = get_date_and_time,

    .set_date_and_time = set_date_and_time,

    .get_lightness = lcd_get_lightness,

    .set_lightness = lcd_set_lightness,

    .get_config = [](SmartWatchesConfig_t *config) { return (DevConfigFile::getInstance().GetConfig(config)); },

    .set_config        = [](SmartWatchesConfig_t *config) {
        DevConfigFile &devCfg = DevConfigFile::getInstance();
        devCfg.SetConfig(config);
        return (devCfg.SaveConfig()); },

    .setSleepThreshold = [](uint32_t sleep_index) { setUnactiveTimeoutThreshold(sleep_index_to_seconds[sleep_index]); },

    .get_voltage = [](uint32_t *voltage) { *voltage = DevBattery::getInstance().get_current_voltage_percent(); },

    .get_degreeC = [](uint32_t *degreeC) { *degreeC = DevBattery::getInstance().get_current_degreeC(); }, 

    .system_reset = [](void) { NVIC_SystemReset(); },
};

static void my_lv_init(void);

static void my_lv_init(void) {

#if LV_USE_LOG
    /* The prototype should be `void my_print(lv_log_level_t level, const char * buf)` */
    lv_log_register_print_cb(my_lv_log);
#endif /* LV_USE_LOG */
    lv_init();
    lv_tick_set_cb(osKernelGetTickCount);
    lv_port_disp_init();
    lv_port_indev_init();
    // lv_user_gui_init();
}
/* USER_CODE_END */

void Panel::Initialize_TriggerPanelInitialization_callback(Msg msg)
{
    /* USER_CODE_BEGIN Initialize_TriggerPanelInitialization_callback */
    /*
    Load and apply the configuration file before building the UI.
    Otherwise, if synchronization is only carried out when using the UI for configuration,
    it may cause a mismatch between the content displayed on the UI and the actual behavior of the device.
    */
    DevConfigFile &devCfg = DevConfigFile::getInstance();
    devCfg.init();
    devCfg.LoadConfig();

    SmartWatchesConfig_t config;
    devCfg.GetConfig(&config);
    set_date_and_time(&config.date_time);
    
    /* set sleep threshold */
    hw_api.setSleepThreshold(config.sleep_index);

    /* init lvgl */
    my_lv_init();

    /* set lightness，要放在LCD_Init之后 */
    hw_api.set_lightness(config.brightness);

    app_ui_entry(indev_keypad, &hw_api);

    SmartWatches::Msg swMsg = {SmartWatchesEvents::PanelInitializationIsComplete, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

void Panel::Running_KeyInput_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_KeyInput_callback */
    std::string output;

    InputKeys key = InputKeys(msg.arg);
    switch (key) {
    case InputKeys::UP:
        print("[panel] PanelInput: UP\n");
        enqueue_key(LV_KEY_UP);
        break;
    case InputKeys::DOWN:
        print("[panel] PanelInput: DOWN\n");
        enqueue_key(LV_KEY_DOWN);
        break;
    case InputKeys::LEFT:
        print("[panel] PanelInput: LEFT\n");
        enqueue_key(LV_KEY_LEFT);
        break;
    case InputKeys::RIGHT:
        print("[panel] PanelInput: RIGHT\n");
        enqueue_key(LV_KEY_RIGHT);
        break;
    case InputKeys::ENTER:
        print("[panel] PanelInput: ENTER\n");
        enqueue_key(LV_KEY_ENTER);
        break;
    case InputKeys::ESCAPE:
        print("[panel] PanelInput: ESCAPE\n");
        enqueue_key(LV_KEY_ESC);
        break;
    default:
        break;
    }
    /* USER_CODE_END */
}

void Panel::Running_TriggerSleep_callback(Msg msg)
{
    /* USER_CODE_BEGIN Running_TriggerSleep_callback */
    lcd_enter_sleep();
    /* USER_CODE_END */
}

void Panel::Sleep_TriggerWakeup_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_TriggerWakeup_callback */
    lcd_exit_sleep();
    /* USER_CODE_END */
}

void Panel::Sleep_Sync_callback(Msg msg)
{
    /* USER_CODE_BEGIN Sleep_Sync_callback */
    SmartWatches::Msg swMsg = {SmartWatchesEvents::Sync, 0};
    osStatus_t stat = osMessageQueuePut(swEventQueueHandle, &swMsg, 0, portMAX_DELAY);
    configASSERT(stat == osOK);
    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_2 */

/* USER_CODE_END */

void Panel::en_Sleep()
{
    /* USER_CODE_BEGIN en_Sleep */

    /* USER_CODE_END */
}

void Panel::du_Sleep()
{
    /* USER_CODE_BEGIN du_Sleep */

    /* USER_CODE_END */
}

void Panel::ex_Sleep()
{
    /* USER_CODE_BEGIN ex_Sleep */

    /* USER_CODE_END */
}

void Panel::en_Running()
{
    /* USER_CODE_BEGIN en_Running */

    /* USER_CODE_END */
}

void Panel::du_Running()
{
    /* USER_CODE_BEGIN du_Running */
    lv_timer_handler();
    UIFW_tick();
    /* USER_CODE_END */
}

void Panel::ex_Running()
{
    /* USER_CODE_BEGIN ex_Running */

    /* USER_CODE_END */
}

void Panel::en_Initialize()
{
    /* USER_CODE_BEGIN en_Initialize */

    /* USER_CODE_END */
}

void Panel::du_Initialize()
{
    /* USER_CODE_BEGIN du_Initialize */

    /* USER_CODE_END */
}

void Panel::ex_Initialize()
{
    /* USER_CODE_BEGIN ex_Initialize */

    /* USER_CODE_END */
}

/* USER_CODE_BEGIN CUSTOM_CODE_3 */

/* USER_CODE_END */

int Panel::recvMessage(Msg &msg) const
{
    int ret = -1;
    /* USER_CODE_BEGIN recvMessage */
    osStatus_t stat = osMessageQueueGet(panelEventQueueHandle, &msg, nullptr, 15 / portTICK_PERIOD_MS);
    if (stat == osOK)
        ret = 0;
    /* USER_CODE_END */
    return ret;
}

void Panel::logOutput(std::string &output) const
{
    /* USER_CODE_BEGIN logOutput */
    print("[ Panel ] \t%s", output.c_str());
    /* USER_CODE_END */
}

