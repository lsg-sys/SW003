#ifndef __W25Q128_H__
#define __W25Q128_H__

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define W25Q128_FLASH_SIZE                      0x1000000 /* 128 MBits => 16MBytes */
#define W25Q128_TOTAL_SECTORS                   0x1000 /* total of 4096 sectors */
#define W25Q128_SECTOR_SIZE                     0x1000 /* unit: bytes */
#define W25Q128_PAGE_SIZE                       0x100 /* unit: bytes */
#define W25Q128_DUMMY_CYCLES_READ               4
#define W25Q128_DUMMY_CYCLES_READ_QUAD          10
#define W25Q128_BULK_ERASE_MAX_TIME             250000
#define W25Q128_SECTOR_ERASE_MAX_TIME           3000
#define W25Q128_SUBSECTOR_ERASE_MAX_TIME        800
#define W25Q128_TIMEOUT_VALUE                   3000
/* Reset Operations */
#define W25Q128_CMD_RESET_ENABLE                     0x66
#define W25Q128_CMD_RESET_MEMORY                     0x99
#define W25Q128_CMD_ENTER_QPI_MODE                   0x38
#define W25Q128_CMD_EXIT_QPI_MODE                    0xFF
/* Identification Operations */
#define W25Q128_CMD_READ_ID                          0x90
#define W25Q128_CMD_DUAL_READ_ID                     0x92
#define W25Q128_CMD_QUAD_READ_ID                     0x94
#define W25Q128_CMD_READ_JEDEC_ID                    0x9F
/* Read Operations */
#define W25Q128_CMD_READ                             0x03
#define W25Q128_CMD_FAST_READ                        0x0B
#define W25Q128_CMD_DUAL_OUT_FAST_READ               0x3B
#define W25Q128_CMD_DUAL_INOUT_FAST_READ             0xBB
#define W25Q128_CMD_QUAD_OUT_FAST_READ               0x6B
#define W25Q128_CMD_QUAD_INOUT_FAST_READ             0xEB
/* Write Operations */
#define W25Q128_CMD_WRITE_ENABLE                     0x06
#define W25Q128_CMD_WRITE_DISABLE                    0x04
/* Register Operations */
#define W25Q128_CMD_READ_STATUS_REG1                  0x05
#define W25Q128_CMD_READ_STATUS_REG2                  0x35
#define W25Q128_CMD_READ_STATUS_REG3                  0x15
#define W25Q128_CMD_WRITE_STATUS_REG1                 0x01
#define W25Q128_CMD_WRITE_STATUS_REG2                 0x31
#define W25Q128_CMD_WRITE_STATUS_REG3                 0x11
/* Program Operations */
#define W25Q128_CMD_PAGE_PROG                        0x02
#define W25Q128_CMD_QUAD_INPUT_PAGE_PROG             0x32
/* Erase Operations */
#define W25Q128_CMD_SECTOR_ERASE                     0x20
#define W25Q128_CMD_CHIP_ERASE                       0xC7
#define W25Q128_CMD_PROG_ERASE_RESUME                0x7A
#define W25Q128_CMD_PROG_ERASE_SUSPEND               0x75
/* Flag Status Register */
#define W25Q128_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q128_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q128_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */

#define W25Q128_CMD_GLOBAL_BLOCK_LOCK               0x7E
#define W25Q128_CMD_GLOBAL_BLOCK_UNLOCK             0x98
#define W25Q128_CMD_READ_BLOCK_LOCK                 0x3d
#define W25Q128_CMD_INDIVIDUAL_BLOCK_LOCK           0x36
#define W25Q128_CMD_INDIVIDUAL_BLOCK_UNLOCK         0x39

#define W25Q128_Instruction_PowerDown               0xB9
#define W25Q128_Instruction_ReleasePowerDown        0xAB

#define W25Q128_DUMMY                     ((uint8_t)0x00)


#define W25Q128_OK            ((uint8_t)0x00)
#define W25Q128_ERROR         ((uint8_t)0x01)
#define W25Q128_BUSY          ((uint8_t)0x02)
#define W25Q128_TIMEOUT		((uint8_t)0x03)


uint8_t w25q128_Init(void);
uint8_t w25q128_WriteEnable(void);
void w25q128_Read_Manufacturer_and_ID(uint8_t *manufacturer, uint8_t *ID);
uint8_t w25q128_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t w25q128_Write(uint8_t const* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t w25q128_sector_erase_4KB(uint32_t Address);
uint8_t w25q128_Erase_Chip(void);
uint8_t w25q128_GetStatus(void);

void w25q128_ReleaseDependency(void);
void w25q128_AddDependency(void);

#ifdef __cplusplus
}
#endif
#endif 
