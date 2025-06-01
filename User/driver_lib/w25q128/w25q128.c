#include "w25q128.h"
#include "w25q128_if.h"

static ResourceHandle ResourceManager_w25q128_handle = NULL;
static uint8_t        w25q128_inited                 = 0;

uint8_t w25q128_GetStatus(void) {
    uint8_t header[1] = {W25Q128_CMD_READ_STATUS_REG1};
    uint8_t status[1];
    w25q128_if_Receive_Data(header, sizeof(header), status, sizeof(status));

    /* Check the value of the register */
    if ((status[0] & W25Q128_FSR_BUSY) != 0) {
        return W25Q128_BUSY;
    } else {
        return W25Q128_OK;
    }
}

/* 任何写入和擦除都需要等待空闲才能够继续 */
static int WaitForBusy(uint32_t timeout) {
    uint32_t tickstart = w25q128_if_get_system_time_in_ms();
    while (w25q128_GetStatus() == W25Q128_BUSY) {
        if ((w25q128_if_get_system_time_in_ms() - tickstart) > timeout) {
            return (-1);
        }
    }
    return (0);
}

static void w25q128_Enter_PowerDownMode(void) {
    uint8_t header[] = {W25Q128_Instruction_PowerDown};
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);
    // tDP > 3us
    w25q128_if_delay_us(3);
}

static void w25q128_Exit_PowerDownMode(void) {
    uint8_t header[] = {W25Q128_Instruction_ReleasePowerDown};
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);
    // tRES1 > 3us
    w25q128_if_delay_us(3);
}

static void w25q128_Reset(void)
{
    /* “Enable Reset (66h)” and “Reset (99h)” instructions can be issued in SPI. To avoid accidental reset, both
    instructions must be issued in sequence. Any other commands other than “Reset (99h)” after the “Enable
    Reset (66h)” command will disable the “Reset Enable” state. A new sequence of “Enable Reset (66h)” and
    “Reset (99h)” is needed to reset the device. Once the Reset command is accepted by the device, the
    device will take approximately tRST=30us to reset. During this period, no command will be accepted.
    Data corruption may happen if there is an on-going or suspended internal Erase or Program operation
    when Reset command sequence is accepted by the device. It is recommended to check the BUSY bit and
    the SUS bit in Status Register before issuing the Reset command sequence. */

    uint8_t header[1];

    header[0] = W25Q128_CMD_RESET_ENABLE;
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);
    
    w25q128_if_delay_us(60);
    
    header[0] = W25Q128_CMD_RESET_MEMORY;
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);
    
    w25q128_if_delay_us(5000);

    /* Global Block Unlock */
    header[0] = W25Q128_CMD_GLOBAL_BLOCK_UNLOCK;
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);
}

uint8_t w25q128_Init(void)
{
    if (w25q128_inited == 0) {
        /* 
        While in the power-down state only the Release Power-down / 
        Device ID (ABh) instruction, which restores the device to normal operation, will be recognized. All other 
        instructions are ignored.
        */
        w25q128_Exit_PowerDownMode();

        w25q128_Reset();
        if (WaitForBusy(W25Q128_TIMEOUT_VALUE) < 0) {
            return W25Q128_TIMEOUT;
        }

        w25q128_Enter_PowerDownMode();

        if (0 != ResourceManager_Init()) {
            return W25Q128_ERROR;
        }

        ResourceManager_w25q128_handle =  ResourceManager_RegisterResource(
            w25q128_Exit_PowerDownMode,
            w25q128_Enter_PowerDownMode,
            3000
        );
        if (NULL == ResourceManager_w25q128_handle) {
            return -1;
        }

        w25q128_inited = 1;
    }

    return W25Q128_OK;
}

void w25q128_AddDependency(void) {
    if (ResourceManager_w25q128_handle)
        ResourceManager_AddDependency(ResourceManager_w25q128_handle);
    else
        while (1);
}

void w25q128_ReleaseDependency(void) {
    if (ResourceManager_w25q128_handle)
        ResourceManager_ReleaseDependency(ResourceManager_w25q128_handle);
    else
        while (1);
}

uint32_t w25q128_is_free(void) {
    if (ResourceManager_w25q128_handle)
        return (ResourceManager_is_free(ResourceManager_w25q128_handle));
    else
        while (1);
}

/* 任何写入和擦除前都需要打开写使能 */
uint8_t w25q128_WriteEnable(void)
{
    // uint8_t txData[] = {W25Q128_CMD_WRITE_ENABLE};
    // uint8_t rxData[sizeof(txData)];

    // /*Select the FLASH: Chip Select low */
    // CS_Enable();
    // /* Send the read ID command */
    // HAL_SPI_TransmitReceive_DMA(SPI_HANDLE, txData, rxData, sizeof(txData));
    // wait_data_transfer_to_finish();
    // /*Deselect the FLASH: Chip Select high */
    // CS_Disable();
    

    /* Wait the end of Flash writing */
    // uint32_t tickstart = w25q128_if_get_system_time_in_ms();
    // while (w25q128_GetStatus() == W25Q128_BUSY) {
    //     /* Check for the Timeout */
    //     if ((w25q128_if_get_system_time_in_ms() - tickstart) > W25Q128_TIMEOUT_VALUE) {
    //         return W25Q128_TIMEOUT;
    //     }
    // }
    // return W25Q128_OK;

    uint8_t header[1] = {W25Q128_CMD_WRITE_ENABLE};
    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);

    if (WaitForBusy(W25Q128_TIMEOUT_VALUE) < 0) {
        return W25Q128_TIMEOUT;
    } else {
        return W25Q128_OK;
    }
}

/* 通常不需要实现，因为写完后硬件会自动关闭 */
uint8_t w25q128_WriteDisable(void)
{
	/*
    uint8_t txData[] = {W25Q128_CMD_WRITE_DISABLE};
    uint8_t rxData[sizeof(txData)];

    CS_Enable();
    HAL_SPI_TransmitReceive_DMA(SPI_HANDLE, txData, rxData, sizeof(txData));
    wait_data_transfer_to_finish();
    CS_Disable();

    uint32_t tickstart = w25q128_if_get_system_time_in_ms();
    while (w25q128_GetStatus() == W25Q128_BUSY) {
        if ((w25q128_if_get_system_time_in_ms() - tickstart) > W25Q128_TIMEOUT_VALUE) {
            return W25Q128_TIMEOUT;
        }
    }
    */
    return W25Q128_OK;
}


void w25q128_Read_Manufacturer_and_ID(uint8_t *manufacturer, uint8_t *ID)
{
    /* | 90h | Dummy | Dummy | 00h | (MF7-MF0) | (ID7-ID0)  */
    uint8_t header[] = {W25Q128_CMD_READ_ID, W25Q128_DUMMY, W25Q128_DUMMY, 0x00};
    uint8_t rxData[2];

    w25q128_if_Receive_Data(header, sizeof(header), rxData, sizeof(rxData));

    *manufacturer = rxData[0];
    *ID = rxData[1];
}

uint8_t w25q128_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    if (Size == 0 || pData == NULL) {
        return W25Q128_ERROR;
    }

    /* 能够一次性读取整个FLASH */

    uint8_t header[] = {
        W25Q128_CMD_FAST_READ,
        (uint8_t)(ReadAddr >> 16),
        (uint8_t)(ReadAddr >> 8),
        (uint8_t)(ReadAddr),
        W25Q128_DUMMY
    };

    w25q128_if_Receive_Data(header, sizeof(header), pData, Size);

    return W25Q128_OK;
}

/* 写入前需要先按sector擦除 */
uint8_t w25q128_Write(uint8_t const *pData, uint32_t WriteAddr, uint32_t Size)
{

    if (Size == 0 || pData == NULL || WriteAddr % W25Q128_PAGE_SIZE != 0) {
        return W25Q128_ERROR;
    }

    /* 只能按page写入 */
    /* Write the data by page */
    uint8_t header[] = {W25Q128_CMD_PAGE_PROG, W25Q128_DUMMY, W25Q128_DUMMY, W25Q128_DUMMY};
    while (Size > 0) {

        /* 每次写完都会自动关闭，所以每次都要重新开启 */
        w25q128_WriteEnable();

        /* 02h | A23-A16 | A15-A8 | A7-A0 | D7-D0 | ...  */
        /* At least one byte of data input is required for Page Program, Quad Page Program and Program Security
        Registers, up to 256 bytes of data input. If more than 256 bytes of data are sent to the device, the
        addressing will wrap to the beginning of the page and overwrite previously sent data. */

        header[1] = (uint8_t)(WriteAddr >> 16);
        header[2] = (uint8_t)(WriteAddr >> 8);
        header[3] = (uint8_t)(WriteAddr);

        w25q128_if_Transmit_Data(header, sizeof(header), pData, W25Q128_PAGE_SIZE);

        if (WaitForBusy(W25Q128_TIMEOUT_VALUE) < 0) {
            w25q128_WriteDisable();
            return W25Q128_TIMEOUT;
        }

        pData += W25Q128_PAGE_SIZE;
        WriteAddr += W25Q128_PAGE_SIZE;
        Size -= W25Q128_PAGE_SIZE;
    }
    
    w25q128_WriteDisable();
    return W25Q128_OK;
}

uint8_t w25q128_sector_erase_4KB(uint32_t Address)
{
    // print("Sector Erase Operation: %d\n", Address);

    w25q128_WriteEnable();

    uint8_t header[] = {
        W25Q128_CMD_SECTOR_ERASE,
        (uint8_t)(Address >> 16),
        (uint8_t)(Address >> 8),
        (uint8_t)(Address)
    };

    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);

    if (WaitForBusy(W25Q128_TIMEOUT_VALUE) < 0) {
        w25q128_WriteDisable();
        return W25Q128_TIMEOUT;
    }

    w25q128_WriteDisable();
    return W25Q128_OK;
}

/**********************************************************************************
 * 函数功能: 芯片擦除
 */
uint8_t w25q128_Erase_Chip(void)
{
    uint8_t header[] = {W25Q128_CMD_CHIP_ERASE};

    w25q128_WriteEnable();

    w25q128_if_Transmit_Data(header, sizeof(header), NULL, 0);

    if (WaitForBusy(W25Q128_BULK_ERASE_MAX_TIME) < 0) {
        w25q128_WriteDisable();
        return W25Q128_TIMEOUT;
    }

    w25q128_WriteDisable();
    return W25Q128_OK;
}

#include "FreeRTOS.h"
int test_w25q128(void)
{
    int ret = -1;

    w25q128_Init();

    uint8_t manufacturer = 0, device_id = 0;
    
    w25q128_AddDependency();
    
    w25q128_Read_Manufacturer_and_ID(&manufacturer, &device_id);
    print("Manufacturer: 0x%x, Device ID: 0x%x\n", manufacturer, device_id);

    // static uint8_t writeData_buffer[W25Q128_SECTOR_SIZE];
    // static uint8_t readData_buffer[W25Q128_SECTOR_SIZE];
    uint8_t* writeData_buffer = (uint8_t*)pvPortMalloc(W25Q128_SECTOR_SIZE);
    uint8_t* readData_buffer = (uint8_t*)pvPortMalloc(W25Q128_SECTOR_SIZE);
    if (writeData_buffer == NULL || readData_buffer == NULL) {
        print("Allocate Memory Failed!\n");
        goto test_end;
    }

    /* 填充数据 */
    for (uint32_t i = 0; i < W25Q128_SECTOR_SIZE; i++) {
        writeData_buffer[i] = i % 256;
    }

    /* 读写整个Flash */
    uint32_t addr = 0;
    uint32_t len = W25Q128_SECTOR_SIZE;
    for (uint32_t i = 0; i < W25Q128_TOTAL_SECTORS; i++) {
        addr = i*W25Q128_SECTOR_SIZE;
        print("Erase/Write/Read Operation: %d/%d\n", 
            i+1, W25Q128_TOTAL_SECTORS);
        if (W25Q128_OK != w25q128_sector_erase_4KB(addr)) {
            print("Erase Operation Failed! (1)\n");
            goto test_end;
        }
        if (W25Q128_OK != w25q128_Read(readData_buffer, addr, len)) {
            print("Read Operation Failed! (1)\n");
            goto test_end;
        }
        for (uint32_t j = 0; j < len; j++) {
            if (readData_buffer[j] != 0xFF) {
                print("Erase Operation Failed! (2)\n");
                goto test_end;
            }
        }
        if (W25Q128_OK != w25q128_Write(writeData_buffer, addr, len)) {
            print("Write Operation Failed!\n");
            goto test_end;
        }
        if (W25Q128_OK != w25q128_Read(readData_buffer, addr, len)) {
            print("Read Operation Failed!\n");
            goto test_end;
        }
        if (memcmp(writeData_buffer, readData_buffer, len) != 0) {
            print("Write/Read Operation Failed!\n");
            goto test_end;
        }
    }
    
    print("Self Test Passed!\n");
    ret = 0;

test_end:
    vPortFree(writeData_buffer);
    vPortFree(readData_buffer);
    w25q128_ReleaseDependency();

    return (ret);
}
