#ifndef __DEVBATTERY_H
#define __DEVBATTERY_H

#include "utility/format.h"

class DevBattery
{
public:
    static DevBattery& getInstance() {
        static DevBattery instance;
        return instance;
    }

    DevBattery(const DevBattery&) = delete;
    DevBattery& operator=(const DevBattery&) = delete;

    int init();
    void sample();
    uint32_t get_current_voltage_percent() {
        current_voltage_percent = convert_voltage_to_percent(current_voltage_mV);
        return current_voltage_percent;
    }
    uint32_t get_current_degreeC() { return current_degreeC; }

private:
    DevBattery() {
    }

    uint32_t convert_voltage_to_percent(uint32_t voltage);

    uint32_t current_voltage_percent = 0; /* 0 ~ 100 */
    uint32_t current_voltage_mV = 0;
    uint32_t current_degreeC = 0;
};

#endif /* __DEVBATTERY_H */