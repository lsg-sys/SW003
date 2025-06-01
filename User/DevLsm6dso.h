#ifndef __DEV_LSM6DSO_H__
#define __DEV_LSM6DSO_H__

#include "driver_lib/lsm6dso/driver/lsm6dso_reg.h"
#include "driver_lib/lsm6dso/lsm6dso_if.h"
#include "driver_lib/lsm6dso/driver/lsm6dso.h"

class DevLsm6dso
{
public:
    static DevLsm6dso& getInstance() {
        static DevLsm6dso instance;
        return instance;
    }

    DevLsm6dso(const DevLsm6dso&) = delete;
    DevLsm6dso& operator=(const DevLsm6dso&) = delete;

    int self_test();
    void scanKeys(int delay, void *processValidKeyPressCallback);
    void init();

private:
    DevLsm6dso() {

        /* Initialize mems driver interface */
        dev_ctx.write_reg = LSM6DSO_platform_write;
        dev_ctx.read_reg = LSM6DSO_platform_read;
        dev_ctx.mdelay = LSM6DSO_platform_delay;
        // dev_ctx.handle = &SENSOR_BUS;
        LSM6DSO_get_sensor_bus_handle(&dev_ctx.handle);
        /* Init test platform */
        LSM6DSO_platform_init();
    }

    stmdev_ctx_t dev_ctx;
};


#endif //__DEV_LSM6DSO_H__
