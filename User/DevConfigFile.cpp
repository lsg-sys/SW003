#include "DevConfigFile.h"

#include "utility/utility.h"

#include "task.h"
#include "fatfs.h"

#include "driver_lib/w25q128/w25q128.h"
#include "driver_lib/print/print.h"

char const path[] = "0:SwCfg.txt";

int DevConfigFile::init() {

    /* Mount the FAT file system */
    print("Mounting FAT file system\n");
    if (fatfs_init() < 0) {
        extern void debug_hardware_output_error(int code);
        debug_hardware_output_error(HW_ERROR_CODE_FLASH);
        configASSERT(false);
    }

    /* Make sure the config file exists */
    FRESULT res;
    res = f_open(&USERFile, path, FA_READ);
    if (res == FR_NO_FILE) {
        print("Config file does not exist\n");
        /* File does not exist, create it */
        UINT fnum;
        f_open(&USERFile, path, FA_CREATE_ALWAYS | FA_WRITE);
        f_lseek(&USERFile, 0);
        f_write(&USERFile, &config, sizeof(config), &fnum);
    } else {
        print("Config file exists\n");
    }
    f_close(&USERFile);

    // res = f_unlink("0:ota_data_info.txt");
    // print("delet file : ota_data_info.txt  %u", (uint32_t)res);
    // res = f_unlink("0:backup.bin");
    // print("delet file : backup.bin  %u", (uint32_t)res);

    return 0;
}

int DevConfigFile::SaveConfig() {

    taskENTER_CRITICAL();

    FRESULT res;
    res = f_open(&USERFile, path, FA_WRITE);
    if (res != FR_OK) {
        taskEXIT_CRITICAL();
        print("Failed to save config %d\n", res);
        return -1;
    }

    UINT fnum;
    f_lseek(&USERFile, 0);
    f_write(&USERFile, &config, sizeof(config), &fnum);
    f_close(&USERFile);

    taskEXIT_CRITICAL();

    print("Saving config\n");
    return 0;
}

int DevConfigFile::LoadConfig() {

    taskENTER_CRITICAL();

    if (f_open(&USERFile, path, FA_READ) != FR_OK) {
        taskEXIT_CRITICAL();
        print("Failed to load config\n");
        return -1;
    }

    UINT fnum;
    f_lseek(&USERFile, 0);
    f_read(&USERFile, &config, sizeof(config), &fnum);
    f_close(&USERFile);

    taskEXIT_CRITICAL();

    print("Loading config\n");
    return 0;
}

int DevConfigFile::GetConfig(SmartWatchesConfig_t *config) {
    *config = this->config;
    return 0;
}

int DevConfigFile::SetConfig(SmartWatchesConfig_t *config) {
    taskENTER_CRITICAL();
    this->config = *config;
    taskEXIT_CRITICAL();
    print("Setting config\n");
    return 0;
}
