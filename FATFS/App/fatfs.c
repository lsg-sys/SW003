/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#include "driver_lib/w25q128/w25q128.h"
#include "driver_lib/print/print.h"

#include <string.h>
// #include "usart.h"
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return (0);
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

#define mini_fsWorkBufferSize 4096
BYTE fsWorkBuffer[mini_fsWorkBufferSize];

void unmount_fatfs(void) {
    f_mount(NULL, USERPath, 0);
}

int mount_fatfs(void) {

    FRESULT result;

    result = f_mount(&USERFatFS, USERPath, 1);
    if (result == FR_NO_FILESYSTEM) {
            print("No filesystem found, starting formatting\n");

            // Format the filesystem
            result = f_mkfs(USERPath, FM_FAT, 0, fsWorkBuffer, sizeof(fsWorkBuffer));
            if (result != FR_OK) {
                print("Formatting failed with error code %d\n", result);
                return (-1);
            }

            print("Filesystem formatted successfully\n");

            print("Re-mounting filesystem\n");
            // Re-mount after formatting if needed
            result = f_mount(&USERFatFS, USERPath, 1);
            if (result != FR_OK) {
                print("Failed to re-mount filesystem with error code %d\n", result);
                return (-1);
            }
            print("Filesystem remounted successfully\n");
    } else if (result != FR_OK) {
        print("Mounting failed with error code %d\n", result);
        return (-1);
    } else {
        print("Filesystem mounted successfully\n");
    }

    return (0);
} 

int fatfs_init(void) {
    return (mount_fatfs());
}

int test_fatfs(void) {

    FRESULT result;
    UINT fnum;
    uint8_t WriteBuffer[] = "Hello World!";
    size_t test_len = sizeof(WriteBuffer);

    print("Waiting for chip erase...\n");
    if (W25Q128_OK != w25q128_Init()) {
        return (-1);        
    }
    w25q128_AddDependency();
    if (W25Q128_OK != w25q128_Erase_Chip()) {
      print("Chip erase failed!\n");
      w25q128_ReleaseDependency();
      return (-1);
    }
    w25q128_ReleaseDependency();

    if (mount_fatfs() < 0) {
        return (-1);
    }

    print("\n****** File write test coming up... ******\n");
    /* Open the file, if the file does not exist, create it */
    result = f_open(&USERFile, "0:FsWR.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (result != FR_OK) {
        /* 注：若打不开并且未知报错，则可能是硬件驱动在其他地方调用导致的，需要重新格式化硬盘并重新格式化硬盘 */
        print("Failed to open/create a file!! (%d)\n", result);
        return (-1);
    } else {
        print("Opened/created file successfully, data will be written to the file.\n");
        /* 将指定存储区内容写入到文件内 */
        memcpy(fsWorkBuffer, WriteBuffer, test_len);
        result = f_write(&USERFile, fsWorkBuffer, test_len, &fnum);
        if (result != FR_OK) {
            print("File write failed!! (%d)\n", result);
            f_close(&USERFile);
            return (-1);
        } else {
            print("The file is written successfully, and byte data is written:%d\n", fnum);
            print("The data written to the file is:\n%s\n", fsWorkBuffer);
            /* 不再读写，关闭文件 */
            f_close(&USERFile);
        }
    }

    print("****** File reading test coming up... ******\n");
    result = f_open(&USERFile, "0:FsWR.txt", FA_OPEN_EXISTING | FA_READ);
    if (result != FR_OK) {
        print("Failed to open the file.\n");
        return (-1);
    } else {
        print("Successfully opened the file.\n");
        result = f_read(&USERFile, fsWorkBuffer, test_len, &fnum);
        if (result != FR_OK) {
            print("File read failure: (%d)\n", result);
            f_close(&USERFile);
            return (-1);
        } else {
            print("File read successfully, read byte data:%d\n", fnum);
            print("Read the obtained file data as follows:\n%s \n", fsWorkBuffer);
            f_close(&USERFile);
        }
    }

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,USERPath,1);

    return (0);
}
/* USER CODE END Application */
