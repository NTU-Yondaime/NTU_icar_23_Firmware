/*---------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
---------------------------------------------------------*/
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "LQ_W25Q128.h"
#include "LQ_FLASH.h"
#include "include.h"


/**
  * @brief    flash 管脚初始化 
  *
  * @param    
  *
  * @return   
  *
  * @note     内部调用 
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
void  FLASH_PinInit(void)
{
	CLOCK_EnableClock(kCLOCK_Iomuxc); 
	
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS, 1U);   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,1U);                                 
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,1U);                                  
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,1U);                                
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,1U);                                
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,1U);                                
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,1U);                                
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,0x10F1u); 
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,0x10F1u);                               
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,0x10F1u);                               
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,0x10F1u);                               
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,0x10F1u);                               
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,0x10F1u);                               
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,0x10F1u);                               
    
    
}

/**
  * @brief    擦出扇区
  *
  * @param    sector_num ： 扇区编号  0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区）
  *
  * @return   0:擦出成功    其他失败
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_EraseSector(uint16_t sector_num)
{
    return flexspi_nor_flash_erase_sector(FLEXSPI, sector_num * SECTOR_SIZE);
}


/**
  * @brief    擦出整块flash
  *
  * @param    
  *
  * @return   
  *
  * @note     时间很长
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_EraseAll(void)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(FLEXSPI, 0);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASECHIP;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(FLEXSPI);

    return status;

}



/**
  * @brief    读取数据
  *
  * @param    sector_num ： 扇区编号      0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区） 
  * @param    page       ： 扇区内页编号  0 - 15  （一个扇区4K  一页256字节 一共16页） 
  * @param    offset     ： 页中第几个字节  0 - 255（1一页256个字节） 
  * @param    buff       ： 读出数据存放地址 
  * @param    len        ： 读出长度
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
void FLASH_ReadBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len)
{
    uint8_t *addr = (uint8_t *)((0x60000000U)+ sector_num * SECTOR_SIZE + page * FLASH_PAGE_SIZE + offset);
    for(int i = 0; i < len; i++)
    {
        *(buff++) = *(addr++);
    
    }
}


/**
  * @brief    写数据
  *
  * @param    sector_num ： 扇区编号      0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区） 
  * @param    page       ： 扇区内页编号  0 - 15  （一个扇区4K  一页256字节 一共16页） 
  * @param    offset     ： 页中第几个字节  0 - 255（1一页256个字节） 
  * @param    buff       ： 写入数据存放地址 
  * @param    len        ： 写入长度
  *
  * @return   
  *
  * @note     写之前要擦除  
  *           flash 为什么写之前要擦除，因为flash只能写0, 擦除的时候flash变成0xff 就可以写0改变数据了
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_WriteBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len)
{
    int temp = 1;
    uint32_t len_temp;
    uint8_t *head;
    /* 如果本页写不完 */
    if(offset + len > 255)
    {
        temp = (offset + len) / 256 + 1;
        len_temp = 256 - offset;
    }
    else
    {
        temp = 1;
        len_temp = len;
    }
        
    uint32_t dstAddr = (sector_num * SECTOR_SIZE + page * FLASH_PAGE_SIZE + offset);
    status_t status;
    flexspi_transfer_t flashXfer;

    do{
        /* Write neable */
        status = flexspi_nor_write_enable(FLEXSPI, dstAddr);
        
        if (status != kStatus_Success)
        {
            return status;
        }
        
        /* Prepare page program command */
        flashXfer.deviceAddress = dstAddr;
        flashXfer.port = kFLEXSPI_PortA1;
        flashXfer.cmdType = kFLEXSPI_Write;
        flashXfer.SeqNumber = 1;
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
        flashXfer.data = (uint32_t *)buff;
        flashXfer.dataSize = len_temp;
        status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);
        
        if (status != kStatus_Success)
        {
            return status;
        }
        
        status = flexspi_nor_wait_bus_busy(FLEXSPI);
        dstAddr += len_temp;
        head = (uint8_t *)buff;
        head += len_temp;
        buff = head;
        len = len - len_temp;
        if(len > 255)
        {
            len_temp = 256;
        }
        else
        {
            len_temp = len;
        }
        
    }while(--temp);
    /* 软件重置AHB缓冲区 */
    FLEXSPI_SoftwareReset(FLEXSPI);
    
    return status;
}





/**
  * @brief    16M  外部FLASH初始化
  *
  * @param    
  *
  * @return   0：初始化OK   其他错误
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
int LQ_FLASH_Init(void)
{
    status_t status;
    FLASH_PinInit();
    
    /* 配置时钟 */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x2); /* Choose PLL2 PFD2 clock as flexspi source clock. 396M */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 133M. */
    
    flexspi_config_t config;
    

    FLEXSPI_GetDefaultConfig(&config);

    /*设置通过AHB总线读取数据的AHB缓冲区大小. */
    config.ahbConfig.enableAHBPrefetch = true;
    config.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(FLEXSPI, &config);

    /* 配置FLASH. */
    FLEXSPI_SetFlashConfig(FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* 更新 LUT 表. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* 复位SPI. */
    FLEXSPI_SoftwareReset(FLEXSPI);
    
    uint8_t vendorID = 0;
    
    /* 获取ID. */
    status = flexspi_nor_get_vendor_id(FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    if(vendorID != 0x17)
    {   
        PRINTF("\r\n没有读到FLASH ID \r\n");
        PRINTF("Vendor ID: 0x%x\r\n", vendorID);
        return 1;
    }
    /* 进入快速模式. */
    status = flexspi_nor_enable_quad_mode(FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    
    
    return 0;
}

/* 例子用的扇区*/
#define EXAMPLE_SECTOR 4

/**
  * @brief    flash 读写测试
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int Test_Flash(void)
{
    uint8_t s_nor_program_buffer[4096];
    uint8_t s_nor_read_buffer[4096];
    
    uint32_t i = 0;
    status_t status;

    PRINTF("\r\n外部flash 读写测试 !\r\n");
    
    /*使用外部flash时，最好把D-Cache 和中断 关掉*/
    DisableInterrupts;
    SCB_DisableDCache();

    /* 初始化 */
    LQ_FLASH_Init();
    /* 擦出扇区 */
    PRINTF("擦除一个扇区...\r\n");
    status = FLASH_EraseSector(EXAMPLE_SECTOR);
    if (status != kStatus_Success)
    {
        PRINTF("擦除失败 !\r\n");
        return -1;
    }

    /* 检查是否擦除完成 */
    /* 将s_nor_program_buffer 设为 0xFF， 擦除后的扇区应该都是 0xff */
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    
    /* 读出数据 */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("擦除失败 有数据不是0xff !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("擦除成功. \r\n");
    }

    
    /* 写入数据初始化 */
    for (i = 0; i < sizeof(s_nor_program_buffer); i++)
    {
        s_nor_program_buffer[i] = i;
    }

    PRINTF("写入flash... !\r\n");
    /* 写入数据 */
    status =
        FLASH_WriteBuff(EXAMPLE_SECTOR, 0, 0, s_nor_program_buffer, sizeof(s_nor_program_buffer));
    if (status != kStatus_Success)
    {
        PRINTF("写入flash 失败 !\r\n");
        return -1;
    }
    PRINTF("写入flash成功 !\r\n");
    PRINTF("读取flash数据... !\r\n");
    /* 读出数据 */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("flash 读出数据与写的不一致 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("flash 读写测试成功. \r\n");
    }

	/* 读写浮点数测试 */
    PRINTF("flash 读写浮点数组测试. \r\n");
    float f_data_rade[100], f_data_write[100];
    
    /* 擦出扇区 */
    PRINTF("擦除一个扇区...\r\n");
    status = FLASH_EraseSector(EXAMPLE_SECTOR);
    if (status != kStatus_Success)
    {
        PRINTF("擦除失败 !\r\n");
        return -1;
    }

    /* 检查是否擦除完成 */
    /* 将s_nor_program_buffer 设为 0xFF， 擦除后的扇区应该都是 0xff */
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    
    /* 读出数据 */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("擦除失败 有数据不是0xff !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("擦除成功. \r\n");
    }

    
    /* 写入数据初始化 */
    for (i = 0; i < sizeof(f_data_write); i++)
    {
        f_data_write[i] = i * 1.25;
    }

    PRINTF("写入flash... !\r\n");
    /* 写入数据 */
    status =
        FLASH_WriteBuff(EXAMPLE_SECTOR, 0, 0, (uint8_t *)f_data_write, sizeof(f_data_write));
    if (status != kStatus_Success)
    {
        PRINTF("写入flash 失败 !\r\n");
        return -1;
    }
    PRINTF("写入flash成功 !\r\n");
    PRINTF("读取flash数据... !\r\n");
    /* 读出数据 */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, (uint8_t *)f_data_rade, sizeof(f_data_write));

    if (memcmp(f_data_rade, f_data_write, sizeof(f_data_write)) != 0)
    {
        PRINTF("flash 读出数据与写的不一致 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("flash 浮点数读写测试成功. \r\n");
    }
	
    while (1)
    {
    }
}
