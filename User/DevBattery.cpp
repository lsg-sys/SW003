#include "DevBattery.h"

#include "utility/utility.h"

/* HW driver */
#include "adc.h"
#include "gpio.h"
#include "driver_lib/print/print.h"

#define BAT_ADC_HANDLER (&hadc1)
#define BAT_ADC_CHANNEL ADC_CHANNEL_8
#define EnableBatteryVoltageDivider() HAL_GPIO_WritePin(BAT_ADC_EN_GPIO_Port, BAT_ADC_EN_Pin, GPIO_PIN_SET)
#define DisableBatteryVoltageDivider() HAL_GPIO_WritePin(BAT_ADC_EN_GPIO_Port, BAT_ADC_EN_Pin, GPIO_PIN_RESET)

#define BAT_MAX_VOLTAGE_MV 4100 /* 100% */
#define BAT_MIN_VOLTAGE_MV 3200 /* 0% */

int DevBattery::init() {

    sample();
    current_voltage_percent = convert_voltage_to_percent(current_voltage_mV);
    return 0;
}

uint32_t DevBattery::convert_voltage_to_percent(uint32_t voltage_ms) {

    if (voltage_ms > BAT_MAX_VOLTAGE_MV) {
        voltage_ms = BAT_MAX_VOLTAGE_MV;
    }
    if (voltage_ms < BAT_MIN_VOLTAGE_MV) {
        voltage_ms = BAT_MIN_VOLTAGE_MV;
    }
    return (voltage_ms - BAT_MIN_VOLTAGE_MV) * 100 / (BAT_MAX_VOLTAGE_MV - BAT_MIN_VOLTAGE_MV);
}

void DevBattery::sample() {
    /* 找到内部参考电压数值与寄存器值的对应 */
    /* 
    STM32F411XE_C_Datasheet里找到
     VREFINT , Internal reference voltage, 1.21V

     VREFIN_CAL , Internal reference voltage calibration,
      Raw data acquired at temperature of 30 °C VDDA = 3.3 V,
      address 0x1FFF 7A2A - 0x1FFF 7A2B
     */

    /* 转换的最低时间，确定外设时钟以配置采样周期 */
    /* 
    STM32F411XE_C_Datasheet里找到
    ADC sampling time when reading the internal reference voltage, min 10us
     */
    /* 
    PCLK2 = 100MHz, ADC1CLK = 100MHz / 4 = 25MHz
    ADC1_cycles = 1/25MHz = 0.04us

    10us/0.04us = 250 cycles
     */


    /* 其他通道的电压计算公式 */
    /* 
    uhADCxConvertedData_VrefAnalog_mVolt = __LL_ADC_CALC_VREFANALOG_VOLTAGE(uhADCxConvertedData[1], LL_ADC_RESOLUTION_12B);
    uhADCxConvertedData_VoltageGPIO_mVolt        = __LL_ADC_CALC_DATA_TO_VOLTAGE(uhADCxConvertedData_VrefAnalog_mVolt, uhADCxConvertedData[0], LL_ADC_RESOLUTION_12B);
    uhADCxConvertedData_VrefInt_mVolt            = __LL_ADC_CALC_DATA_TO_VOLTAGE(uhADCxConvertedData_VrefAnalog_mVolt, uhADCxConvertedData[1], LL_ADC_RESOLUTION_12B);
    hADCxConvertedData_Temperature_DegreeCelsius = __LL_ADC_CALC_TEMPERATURE(uhADCxConvertedData_VrefAnalog_mVolt, uhADCxConvertedData[2], LL_ADC_RESOLUTION_12B);
     */

    
    ADC_ChannelConfTypeDef sConfig = {0};
    
    sConfig.Channel      = ADC_CHANNEL_VREFINT;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    HAL_ADC_Start(BAT_ADC_HANDLER);
    HAL_ADC_PollForConversion(BAT_ADC_HANDLER, 50);
    uint32_t refIntVal = HAL_ADC_GetValue(BAT_ADC_HANDLER);
    /* refIntV会根据Vref+引脚的电压的变化而变化，
    根据当前内部参考电压的ADC数值与生产时的内部参考电压的ADC数值和Vref+引脚的电压进行计算，
    得出当前Vref+引脚的电压 */
    uint32_t Vref_mV   = __LL_ADC_CALC_VREFANALOG_VOLTAGE(refIntVal, LL_ADC_RESOLUTION_12B);
    
    EnableBatteryVoltageDivider();
    osDelay(10); /* 等待分压器稳定，数值根据实际情况调整，为提高精度（至少消除5%的误差）不建议删除这一行 */

    // sConfig.Channel      = ADC_CHANNEL_VBAT;
    sConfig.Channel      = BAT_ADC_CHANNEL;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    HAL_ADC_Start(BAT_ADC_HANDLER);
    HAL_ADC_PollForConversion(BAT_ADC_HANDLER, 50);
    // uint32_t batVal  = HAL_ADC_GetValue(BAT_ADC_HANDLER);
    uint32_t batVal  = HAL_ADC_GetValue(BAT_ADC_HANDLER);
    /* 使用当前Vref+引脚的电压和bat引脚的电压计算出bat引脚的电压 */
    uint32_t Vbat_mV = __LL_ADC_CALC_DATA_TO_VOLTAGE(Vref_mV, batVal, LL_ADC_RESOLUTION_12B);
    // Vbat_mV *= 4; // BAT内部分压为四分之一
    // static uint32_t const R1 = 200, R2 = 51;
    // Vbat_mV = (Vbat_mV * (R1 + R2)) / R2; // BAT外部分压为 0.203
    Vbat_mV = Vbat_mV * 4.9215f;
    
    DisableBatteryVoltageDivider();

    sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    HAL_ADC_Start(BAT_ADC_HANDLER);
    HAL_ADC_PollForConversion(BAT_ADC_HANDLER, 50);
    uint32_t tempVal = HAL_ADC_GetValue(BAT_ADC_HANDLER);
    uint32_t temp_DegC = __LL_ADC_CALC_TEMPERATURE(Vref_mV, tempVal, LL_ADC_RESOLUTION_12B);

    print("Vrefint_val/Vref_CAL/Vref_mV: %d/%d/%d\n", refIntVal, (uint32_t)(*VREFINT_CAL_ADDR), Vref_mV);
    print("batVal: %d, Vbat_mV: %d\n",  batVal, Vbat_mV);
    print("tempVal: %d, temp_DegC: %d\n", tempVal, temp_DegC);

    current_voltage_mV = Vbat_mV;
    current_degreeC = temp_DegC;
}
