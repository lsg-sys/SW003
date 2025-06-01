#include "DevLsm6dso.h"

#include "driver_lib/print/print.h"

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include <string.h>
#include <main.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define    BOOT_TIME        15 //ms
#define    WAIT_TIME_A     100 //ms
#define    WAIT_TIME_G_01  150 //ms
#define    WAIT_TIME_G_02   50 //ms

/* Self test limits. */
#define    MIN_ST_LIMIT_mg        100
#define    MAX_ST_LIMIT_mg      10000
#define    MIN_ST_LIMIT_mdps   2
#define    MAX_ST_LIMIT_mdps   10000

/* Self test results. */
#define    ST_PASS     1U
#define    ST_FAIL     0U

typedef union {
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

#define MEMSKEY_DEBOUNCE_THRESHOLD 5

typedef struct {
    int16_t min;
    int16_t max;
} axisLimit_t;

typedef enum {
    MEMSKEY_STATE_RELEASED = 0,
    MEMSKEY_STATE_PRESSED,
    MEMSKEY_STATE_LONG_PRESSED
} memskeyState_t;

typedef struct {
    axisLimit_t lim[3];
    char tag;
    uint8_t count;
    uint16_t block_time_ms; /* The key does not trigger for a period of time after being triggered */
    uint16_t repeat_interval_ms;
    memskeyState_t state;
} memskey_t;
#define MEMSKEY_BLOCK_TIME  900 /* ms */
#define MEMSKEY_REPEAT_INTERVAL 500 /* ms */

static axis3bit16_t data_raw_acceleration;
static axis3bit16_t data_raw_angular_rate;

static memskey_t memskeys[] = {
    /* 右手，USB孔在右侧的佩戴方式 */
    {{{-5500, -2400}, {-1600, +1000}, {+6900, +7900}}, 'e', 0, 0, 0, MEMSKEY_STATE_RELEASED},
    {{{+2100, +4000}, {-2300, +1100}, {+6900, +7900}}, 'f', 0, 0, 0, MEMSKEY_STATE_RELEASED},
    {{{-2000, +2400}, {-3600, -1800}, {+6900, +7900}}, 'g', 0, 0, 0, MEMSKEY_STATE_RELEASED},
    {{{-2000, +2400}, {+1950, +3000}, {+6900, +7900}}, 'h', 0, 0, 0, MEMSKEY_STATE_RELEASED}
};

extern "C" float get_dev_lsm6dso_radian(void) {

    vPortEnterCritical();
    axis3bit16_t data = data_raw_acceleration;
    vPortExitCritical();

    float x = - data.i16bit[0];
    float y = data.i16bit[1];

    float radian = atan2(y, x) + M_PI;
    return radian;
}

void scanMemsKeys(axis3bit16_t const*data_raw_acceleration,  int delayVal, void *processValidKeyPressCallback) {

    for (memskey_t& key : memskeys) {

        switch (key.state) {
            case MEMSKEY_STATE_RELEASED:
                {
                    bool is_pressed = 
                        (key.lim[0].min < data_raw_acceleration->i16bit[0] && data_raw_acceleration->i16bit[0] < key.lim[0].max) &&
                        (key.lim[1].min < data_raw_acceleration->i16bit[1] && data_raw_acceleration->i16bit[1] < key.lim[1].max) &&
                        (key.lim[2].min < data_raw_acceleration->i16bit[2] && data_raw_acceleration->i16bit[2] < key.lim[2].max);

                    if (is_pressed) {
                        key.count++;

                        if (MEMSKEY_DEBOUNCE_THRESHOLD <= key.count) {
                            key.count = 0;
                            key.block_time_ms = MEMSKEY_BLOCK_TIME;
                            key.repeat_interval_ms = 0;
                            key.state = MEMSKEY_STATE_PRESSED;

                            // Process valid key press
                            ((void (*)(char))processValidKeyPressCallback)(key.tag);
                        }
                    } else {
                        key.count--;
                    }
                }
                break;

            case MEMSKEY_STATE_PRESSED:
                {
                    // Check if the key is still pressed and needs to be repeated
                    bool is_pressed = 
                        (key.lim[0].min < data_raw_acceleration->i16bit[0] && data_raw_acceleration->i16bit[0] < key.lim[0].max) &&
                        (key.lim[1].min < data_raw_acceleration->i16bit[1] && data_raw_acceleration->i16bit[1] < key.lim[1].max) &&
                        (key.lim[2].min < data_raw_acceleration->i16bit[2] && data_raw_acceleration->i16bit[2] < key.lim[2].max);

                    if (is_pressed) {
                        if (key.block_time_ms) {
                            if (delayVal < key.block_time_ms)
                                key.block_time_ms -= delayVal;
                            else
                                key.block_time_ms = 0;
                        } else {
                            key.state = MEMSKEY_STATE_LONG_PRESSED;
                        }
                    } else {
                        key.state = MEMSKEY_STATE_RELEASED;
                    }
                }
                break;
            case MEMSKEY_STATE_LONG_PRESSED:
                {
                    bool is_pressed = 
                        (key.lim[0].min < data_raw_acceleration->i16bit[0] && data_raw_acceleration->i16bit[0] < key.lim[0].max) &&
                        (key.lim[1].min < data_raw_acceleration->i16bit[1] && data_raw_acceleration->i16bit[1] < key.lim[1].max) &&
                        (key.lim[2].min < data_raw_acceleration->i16bit[2] && data_raw_acceleration->i16bit[2] < key.lim[2].max);

                    if (is_pressed) {
                        key.repeat_interval_ms += delayVal;
                        if (key.repeat_interval_ms >= MEMSKEY_REPEAT_INTERVAL) {
                            key.repeat_interval_ms = 0;
                            ((void (*)(char))processValidKeyPressCallback)(key.tag);
                        }
                    } else {
                        key.repeat_interval_ms = 0;
                        key.state = MEMSKEY_STATE_RELEASED;
                    }
                }
                break;

            default:
                // Handle unexpected states if necessary
                break;
        }
    }
}

void DevLsm6dso::init() {

    uint8_t whoamI;
    lsm6dso_device_id_get(&dev_ctx, &whoamI);
    if (whoamI != LSM6DSO_ID) {
        extern void debug_hardware_output_error(int code);
        debug_hardware_output_error(HW_ERROR_CODE_MEMS);
        configASSERT(false);
    }

    lsm6dso_reset_set(&dev_ctx, PROPERTY_ENABLE);

    uint8_t rst;
    do {
        lsm6dso_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Disable I3C interface */
    lsm6dso_i3c_disable_set(&dev_ctx, LSM6DSO_I3C_DISABLE);
    /* Enable Block Data Update */
    lsm6dso_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set full scale */
    lsm6dso_xl_full_scale_set(&dev_ctx, LSM6DSO_4g);
    lsm6dso_gy_full_scale_set(&dev_ctx, LSM6DSO_250dps);
    /* Set FIFO watermark (number of unread sensor data TAG + 6 bytes
     * stored in FIFO) to 5*2 samples
     */
    lsm6dso_fifo_watermark_set(&dev_ctx, 5*2); /* 1000ms/52Hz * 5 ~= 100ms */
    /* Set FIFO batch XL/Gyro ODR to 52Hz */
    lsm6dso_fifo_xl_batch_set(&dev_ctx, LSM6DSO_XL_BATCHED_AT_52Hz);
    lsm6dso_fifo_gy_batch_set(&dev_ctx, LSM6DSO_GY_BATCHED_AT_52Hz);
    /* Set FIFO mode to Stream mode (aka Continuous Mode) */
    lsm6dso_fifo_mode_set(&dev_ctx, LSM6DSO_STREAM_MODE);
    /* Enable drdy 75 μs pulse: uncomment if interrupt must be pulsed */
    // lsm6dso_data_ready_mode_set(&dev_ctx, LSM6DSO_DRDY_PULSED);
    /* Uncomment if interrupt generation on Free Fall INT1 pin */
    lsm6dso_pin_int1_route_t int1_route;
    lsm6dso_pin_int1_route_get(&dev_ctx, &int1_route);
    // int1_route.reg.int1_ctrl.int1_fifo_th = PROPERTY_ENABLE;
    int1_route.fifo_th = PROPERTY_ENABLE;
    // lsm6dso_pin_int1_route_set(&dev_ctx, &int1_route);
    lsm6dso_pin_int1_route_set(&dev_ctx, int1_route);
    /* Uncomment if interrupt generation on Free Fall INT2 pin */
    // lsm6dso_pin_int2_route_get(&dev_ctx, &int2_route);
    // int2_route.reg.int2_ctrl.int2_fifo_th = PROPERTY_ENABLE;
    // lsm6dso_pin_int2_route_set(&dev_ctx, &int2_route);
    /* Set Output Data Rate */
    lsm6dso_xl_data_rate_set(&dev_ctx, LSM6DSO_XL_ODR_52Hz);
    lsm6dso_gy_data_rate_set(&dev_ctx, LSM6DSO_GY_ODR_52Hz);
}

static bool exist_int1_event = false;
static bool exist_int2_event = false;
extern "C" void on_mems_int1_IRQ(uint16_t GPIO_Pin) {
    if (GPIO_Pin == MEMS_INT1_Pin) {
        exist_int1_event = true;
    }
}
extern "C" void on_mems_int2_IRQ(uint16_t GPIO_Pin) {
    if (GPIO_Pin == MEMS_INT2_Pin) {
        exist_int2_event = true;
    }
}

/* Keep the device stationary during the self-test */
int DevLsm6dso::self_test() {

    stmdev_ctx_t *pDev = &dev_ctx;
    int16_t data_raw[3];
    int32_t val_st_off[3];
    int32_t val_st_on[3];
    int32_t test_val[3];
    uint8_t st_result;
    uint8_t drdy;

    print("lsm6dso test : Check device ID\n");
    uint8_t whoamI;
    lsm6dso_device_id_get(pDev, &whoamI);
    if (whoamI != LSM6DSO_ID)
        return -1;

    /* Restore default configuration */
    uint8_t rst;
    lsm6dso_reset_set(pDev, PROPERTY_ENABLE);
    do {
        lsm6dso_reset_get(pDev, &rst);
    } while (rst);

    /* Enable Block Data Update */
    lsm6dso_block_data_update_set(pDev, PROPERTY_ENABLE);

    /* Set Output Data Rate */
    lsm6dso_xl_data_rate_set(pDev, LSM6DSO_XL_ODR_52Hz);
    /* Set full scale */
    lsm6dso_xl_full_scale_set(pDev, LSM6DSO_4g);
    /* Wait stable output */
    osDelay(WAIT_TIME_A);
   print("lsm6dso test : Gat data from Accelerometer Normal Mode\n");
    /* Check if new value available */
    do {
        lsm6dso_xl_flag_data_ready_get(pDev, &drdy);
    } while (!drdy);
    /* Read dummy data and discard it */
    lsm6dso_acceleration_raw_get(pDev, data_raw);

    memset(val_st_off, 0x00, 3 * sizeof(int32_t));
    /* Read 5 sample and get the average vale for each axis */
    for (int i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
        lsm6dso_xl_flag_data_ready_get(pDev, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dso_acceleration_raw_get(pDev, data_raw);

        for (int j = 0; j < 3; j++) {
            val_st_off[j] += data_raw[j];
        }
    }
    /* Calculate the mg average values */
    for (int i = 0; i < 3; i++) {
        val_st_off[i] /= 5;
    }

    print("lsm6dso test : Gat data from Accelerometer Self-Test Mode (positive)\n");
    /* Enable Self Test positive (or negative) */
    lsm6dso_xl_self_test_set(pDev, LSM6DSO_XL_ST_POSITIVE);
    // lsm6dso_xl_self_test_set(pDev, LSM6DSO_XL_ST_NEGATIVE);
    /* Wait stable output */
    LSM6DSO_platform_delay(WAIT_TIME_A);

    /* Check if new value available */
    do {
        lsm6dso_xl_flag_data_ready_get(pDev, &drdy);
    } while (!drdy);
    /* Read dummy data and discard it */
    lsm6dso_acceleration_raw_get(pDev, data_raw);

    memset(val_st_on, 0x00, 3 * sizeof(int32_t));
    /* Read 5 sample and get the average vale for each axis */
    for (int i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
        lsm6dso_xl_flag_data_ready_get(pDev, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dso_acceleration_raw_get(pDev, data_raw);

        for (int j = 0; j < 3; j++) {
            val_st_on[j] += data_raw[j];
        }
    }
    /* Calculate the mg average values */
    for (int i = 0; i < 3; i++) {
        val_st_on[i] /= 5;
    }


    print("lsm6dso test : Calculate Accelerometer self-test result\n");
    /* Calculate the mg values for self test */
    for (int i = 0; i < 3; i++) {
        test_val[i] = val_st_on[i] - val_st_off[i];
    }
    /* Check self test limit */
    st_result = ST_PASS;
    for (int i = 0; i < 3; i++) {
        if (( MIN_ST_LIMIT_mg > test_val[i] ) || ( test_val[i] > MAX_ST_LIMIT_mg)) {
            st_result = ST_FAIL;
        }
    }
    if (st_result == ST_FAIL) {
        print("lsm6dso test : Accelerometer self-test  FAIL\n");
        return -1;
    } else {
        print("lsm6dso test : Accelerometer self-test  PASS\n");
    }

    /* Disable Self Test */
    lsm6dso_xl_self_test_set(pDev, LSM6DSO_XL_ST_DISABLE);
    /* Disable sensor. */
    lsm6dso_xl_data_rate_set(pDev, LSM6DSO_XL_ODR_OFF);


    /* Set Output Data Rate */
    lsm6dso_gy_data_rate_set(pDev, LSM6DSO_GY_ODR_208Hz);
    /* Set full scale */
    lsm6dso_gy_full_scale_set(pDev, LSM6DSO_2000dps);
    /* Wait stable output */
    LSM6DSO_platform_delay(WAIT_TIME_G_01);

    print("lsm6dso test : Gat data from Gyroscope Normal Mode\n");
    /* Check if new value available */
    do {
        lsm6dso_gy_flag_data_ready_get(pDev, &drdy);
    } while (!drdy);
    /* Read dummy data and discard it */
    lsm6dso_angular_rate_raw_get(pDev, data_raw);

    memset(val_st_off, 0x00, 3 * sizeof(int32_t));
    /* Read 5 sample and get the average vale for each axis */

    for (int i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
        lsm6dso_gy_flag_data_ready_get(pDev, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dso_angular_rate_raw_get(pDev, data_raw);

        for (int j = 0; j < 3; j++) {
            val_st_off[j] += data_raw[j];
        }
    }
    /* Calculate the mg average values */
    for (int i = 0; i < 3; i++) {
        val_st_off[i] /= 5.0f;
    }

    print("lsm6dso test : Gat data from Gyroscope self-test (positive)\n");
    /* Enable Self Test positive (or negative) */
    lsm6dso_gy_self_test_set(pDev, LSM6DSO_GY_ST_POSITIVE);
    //lsm6dso_gy_self_test_set(pDev, LIS2DH12_GY_ST_NEGATIVE);
    /* Wait stable output */
    LSM6DSO_platform_delay(WAIT_TIME_G_02);
    /* Read 5 sample and get the average vale for each axis */
    memset(val_st_on, 0x00, 3 * sizeof(int32_t));
    for (int i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
        lsm6dso_gy_flag_data_ready_get(pDev, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dso_angular_rate_raw_get(pDev, data_raw);

        for (int j = 0; j < 3; j++) {
        val_st_on[j] += data_raw[j];
        }
    }
    /* Calculate the mg average values */
    for (int i = 0; i < 3; i++) {
        val_st_on[i] /= 5.0f;
    }

    print("lsm6dso test : Calculate gyroscope self-test result\n");
    /* Calculate the mg values for self test */
    for (int i = 0; i < 3; i++) {
        test_val[i] = val_st_on[i] - val_st_off[i];
    }
    st_result = ST_PASS;
    /* Check self test limit */
    for (int i = 0; i < 3; i++) {
        if (( MIN_ST_LIMIT_mdps > test_val[i] ) || ( test_val[i] > MAX_ST_LIMIT_mdps)) {
            st_result = ST_FAIL;
        }
    }
    if (st_result == ST_PASS) {
        print("lsm6dso test : Gyroscope self-test  PASS\n");
    } else {
        print("lsm6dso test : Gyroscope self-test  FAIL\n");
        return -1;
    }
    /* Disable Self Test */
    lsm6dso_gy_self_test_set(pDev, LSM6DSO_GY_ST_DISABLE);
    /* Disable sensor. */
    lsm6dso_xl_data_rate_set(pDev, LSM6DSO_XL_ODR_OFF);

    print("lsm6dso test : finished\n");

    LSM6DSO_platform_delay(500);

    return 0;
}

void DevLsm6dso::scanKeys(int delay, void *processValidKeyPressCallback) {
#if 0
    /* Check if new value available */
    uint8_t acc_data_is_ready = PROPERTY_DISABLE;
    uint8_t gyro_data_is_ready = PROPERTY_DISABLE;
    do {
        osDelay(10);
        lsm6dso_xl_flag_data_ready_get(&dev_ctx, &acc_data_is_ready);
        lsm6dso_gy_flag_data_ready_get(&dev_ctx, &gyro_data_is_ready);
    } while (acc_data_is_ready != PROPERTY_ENABLE || gyro_data_is_ready != PROPERTY_ENABLE);

    int16_t acc_data_raw[3];
    lsm6dso_acceleration_raw_get(&dev_ctx, acc_data_raw);

    int16_t gyro_data_raw[3];
    lsm6dso_angular_rate_raw_get(&dev_ctx, gyro_data_raw);

    print("Accelerometer data : %d, %d, %d\n", acc_data_raw[0], acc_data_raw[1], acc_data_raw[2]);
    print("Gyroscope data : %d, %d, %d\n", gyro_data_raw[0], gyro_data_raw[1], gyro_data_raw[2]);
#endif

    uint16_t           num    = 0;
    // uint8_t            wmflag = 0;
    lsm6dso_fifo_tag_t reg_tag;
    axis3bit16_t       dummy;

    // /* Read watermark flag */
    // lsm6dso_fifo_wtm_flag_get(&dev_ctx, &wmflag);

    osDelay(delay);

    // if (wmflag > 0) {
    if (exist_int1_event) {
        exist_int1_event = false;

        /* Read number of samples in FIFO */
        lsm6dso_fifo_data_level_get(&dev_ctx, &num);
        // print("Number of samples in FIFO: %d\n", num);

        while (num--) {
            /* Read FIFO tag */
            lsm6dso_fifo_sensor_tag_get(&dev_ctx, &reg_tag);

            switch (reg_tag) {
            case LSM6DSO_XL_NC_TAG:
                // memset(data_raw_acceleration.u8bit, 0x00, 3 * sizeof(int16_t));
                lsm6dso_fifo_out_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
                // print("Acceleration [raw]: %d %d %d\n",
                //              data_raw_acceleration.i16bit[0], data_raw_acceleration.i16bit[1], data_raw_acceleration.i16bit[2]);
                scanMemsKeys(&data_raw_acceleration, delay, processValidKeyPressCallback);
                break;

            case LSM6DSO_GYRO_NC_TAG:
                memset(data_raw_angular_rate.u8bit, 0x00, 3 * sizeof(int16_t));
                lsm6dso_fifo_out_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
                // print("Gyro [raw]: %d %d %d\n",
                //              data_raw_angular_rate.i16bit[0], data_raw_angular_rate.i16bit[1], data_raw_angular_rate.i16bit[2]);
                break;

            default:
                /* Flush unused samples */
                memset(dummy.u8bit, 0x00, 3 * sizeof(int16_t));
                lsm6dso_fifo_out_raw_get(&dev_ctx, dummy.u8bit);
                break;
            }
        }
    }
}

extern "C" int test_lsm6dso() {
    DevLsm6dso &lsm6dso = DevLsm6dso::getInstance();
    return lsm6dso.self_test();
}
