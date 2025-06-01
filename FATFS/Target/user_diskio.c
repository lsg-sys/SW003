/* USER CODE BEGIN Header */

#include "driver_lib/w25q128/w25q128.h"
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
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

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
uint32_t enter_critical(void);
void leave_critical(uint32_t mask);

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
    Stat = STA_NOINIT;
    
	  if (W25Q128_OK == w25q128_Init())
    {
      Stat &= ~STA_NOINIT;
    }
    return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
  w25q128_AddDependency();

  uint32_t mask = enter_critical();
  {
    Stat = STA_NOINIT;
    if (W25Q128_OK == w25q128_GetStatus()) {
      Stat &= ~STA_NOINIT;
    }
  }
  leave_critical(mask);

  w25q128_ReleaseDependency();

  return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
  w25q128_AddDependency();

  uint32_t mask = enter_critical();
  {
    uint32_t addr = sector * W25Q128_SECTOR_SIZE;
    uint32_t len  = count * W25Q128_SECTOR_SIZE;
    w25q128_Read(buff, addr, len);
  }
  leave_critical(mask);

  w25q128_ReleaseDependency();
  return (RES_OK);
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
  w25q128_AddDependency();

  uint32_t mask = enter_critical();
  {
    uint32_t addr = sector * W25Q128_SECTOR_SIZE;
    uint32_t len = count * W25Q128_SECTOR_SIZE;
    w25q128_sector_erase_4KB(addr);
    w25q128_Write(buff, addr, len);
  }
  leave_critical(mask);
  
  w25q128_ReleaseDependency();
    return (RES_OK);
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
	DRESULT res = RES_OK;
	
	switch(cmd)
	{
		/*以下四个命令都是按照FatFs默认参数配置时必须需要的*/
		//完成挂起的写入过程(在_FS_READONLY == 0时需要)
		case CTRL_SYNC:   
			break;
			
		//获取存储介质容量(在_USE_MKFS == 1时需要)
		case GET_SECTOR_COUNT:  
			//W25Q128总的扇区个数,4096
			// *(DWORD *)buff = FLASH_SECTOR_COUNT;  
			*(DWORD *)buff = 4096;
			break;
			
		//获取扇区大小(_MAX_SS != _MIN_SS时需要)
		case GET_SECTOR_SIZE:  
			//W25Q128每个扇区的大小，4096字节
			// *(DWORD *)buff = FLASH_SECTOR_SIZE;  
			*(DWORD *)buff = 4096;  
			break;
			
		//获取擦除块的大小(_USE_MKFS == 1时需要)
		case GET_BLOCK_SIZE:  
			//W25Q128每个块拥有16个扇区，按块擦除
			*(DWORD *)buff = 16;  
			break;
		
		default:
			res = RES_ERROR;
	}
 
	return (res);
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

