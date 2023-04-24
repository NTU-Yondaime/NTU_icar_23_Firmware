/*---------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
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
  * @brief    flash �ܽų�ʼ�� 
  *
  * @param    
  *
  * @return   
  *
  * @note     �ڲ����� 
  *
  * @example  
  *
  * @date     2019/6/19 ������
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
  * @brief    ��������
  *
  * @param    sector_num �� �������  0 - 4095��16MB FLASH  һ������4K һ��4096��������
  *
  * @return   0:�����ɹ�    ����ʧ��
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
int FLASH_EraseSector(uint16_t sector_num)
{
    return flexspi_nor_flash_erase_sector(FLEXSPI, sector_num * SECTOR_SIZE);
}


/**
  * @brief    ��������flash
  *
  * @param    
  *
  * @return   
  *
  * @note     ʱ��ܳ�
  *
  * @example  
  *
  * @date     2019/6/20 ������
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
  * @brief    ��ȡ����
  *
  * @param    sector_num �� �������      0 - 4095��16MB FLASH  һ������4K һ��4096�������� 
  * @param    page       �� ������ҳ���  0 - 15  ��һ������4K  һҳ256�ֽ� һ��16ҳ�� 
  * @param    offset     �� ҳ�еڼ����ֽ�  0 - 255��1һҳ256���ֽڣ� 
  * @param    buff       �� �������ݴ�ŵ�ַ 
  * @param    len        �� ��������
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
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
  * @brief    д����
  *
  * @param    sector_num �� �������      0 - 4095��16MB FLASH  һ������4K һ��4096�������� 
  * @param    page       �� ������ҳ���  0 - 15  ��һ������4K  һҳ256�ֽ� һ��16ҳ�� 
  * @param    offset     �� ҳ�еڼ����ֽ�  0 - 255��1һҳ256���ֽڣ� 
  * @param    buff       �� д�����ݴ�ŵ�ַ 
  * @param    len        �� д�볤��
  *
  * @return   
  *
  * @note     д֮ǰҪ����  
  *           flash Ϊʲôд֮ǰҪ��������Ϊflashֻ��д0, ������ʱ��flash���0xff �Ϳ���д0�ı�������
  * @example  
  *
  * @date     2019/6/20 ������
  */
int FLASH_WriteBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len)
{
    int temp = 1;
    uint32_t len_temp;
    uint8_t *head;
    /* �����ҳд���� */
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
    /* �������AHB������ */
    FLEXSPI_SoftwareReset(FLEXSPI);
    
    return status;
}





/**
  * @brief    16M  �ⲿFLASH��ʼ��
  *
  * @param    
  *
  * @return   0����ʼ��OK   ��������
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/19 ������
  */
int LQ_FLASH_Init(void)
{
    status_t status;
    FLASH_PinInit();
    
    /* ����ʱ�� */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x2); /* Choose PLL2 PFD2 clock as flexspi source clock. 396M */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 133M. */
    
    flexspi_config_t config;
    

    FLEXSPI_GetDefaultConfig(&config);

    /*����ͨ��AHB���߶�ȡ���ݵ�AHB��������С. */
    config.ahbConfig.enableAHBPrefetch = true;
    config.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(FLEXSPI, &config);

    /* ����FLASH. */
    FLEXSPI_SetFlashConfig(FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* ���� LUT ��. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* ��λSPI. */
    FLEXSPI_SoftwareReset(FLEXSPI);
    
    uint8_t vendorID = 0;
    
    /* ��ȡID. */
    status = flexspi_nor_get_vendor_id(FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    if(vendorID != 0x17)
    {   
        PRINTF("\r\nû�ж���FLASH ID \r\n");
        PRINTF("Vendor ID: 0x%x\r\n", vendorID);
        return 1;
    }
    /* �������ģʽ. */
    status = flexspi_nor_enable_quad_mode(FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    
    
    return 0;
}

/* �����õ�����*/
#define EXAMPLE_SECTOR 4

/**
  * @brief    flash ��д����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
int Test_Flash(void)
{
    uint8_t s_nor_program_buffer[4096];
    uint8_t s_nor_read_buffer[4096];
    
    uint32_t i = 0;
    status_t status;

    PRINTF("\r\n�ⲿflash ��д���� !\r\n");
    
    /*ʹ���ⲿflashʱ����ð�D-Cache ���ж� �ص�*/
    DisableInterrupts;
    SCB_DisableDCache();

    /* ��ʼ�� */
    LQ_FLASH_Init();
    /* �������� */
    PRINTF("����һ������...\r\n");
    status = FLASH_EraseSector(EXAMPLE_SECTOR);
    if (status != kStatus_Success)
    {
        PRINTF("����ʧ�� !\r\n");
        return -1;
    }

    /* ����Ƿ������� */
    /* ��s_nor_program_buffer ��Ϊ 0xFF�� �����������Ӧ�ö��� 0xff */
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    
    /* �������� */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("����ʧ�� �����ݲ���0xff !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("�����ɹ�. \r\n");
    }

    
    /* д�����ݳ�ʼ�� */
    for (i = 0; i < sizeof(s_nor_program_buffer); i++)
    {
        s_nor_program_buffer[i] = i;
    }

    PRINTF("д��flash... !\r\n");
    /* д������ */
    status =
        FLASH_WriteBuff(EXAMPLE_SECTOR, 0, 0, s_nor_program_buffer, sizeof(s_nor_program_buffer));
    if (status != kStatus_Success)
    {
        PRINTF("д��flash ʧ�� !\r\n");
        return -1;
    }
    PRINTF("д��flash�ɹ� !\r\n");
    PRINTF("��ȡflash����... !\r\n");
    /* �������� */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("flash ����������д�Ĳ�һ�� !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("flash ��д���Գɹ�. \r\n");
    }

	/* ��д���������� */
    PRINTF("flash ��д�����������. \r\n");
    float f_data_rade[100], f_data_write[100];
    
    /* �������� */
    PRINTF("����һ������...\r\n");
    status = FLASH_EraseSector(EXAMPLE_SECTOR);
    if (status != kStatus_Success)
    {
        PRINTF("����ʧ�� !\r\n");
        return -1;
    }

    /* ����Ƿ������� */
    /* ��s_nor_program_buffer ��Ϊ 0xFF�� �����������Ӧ�ö��� 0xff */
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    
    /* �������� */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, s_nor_read_buffer, sizeof(s_nor_program_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("����ʧ�� �����ݲ���0xff !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("�����ɹ�. \r\n");
    }

    
    /* д�����ݳ�ʼ�� */
    for (i = 0; i < sizeof(f_data_write); i++)
    {
        f_data_write[i] = i * 1.25;
    }

    PRINTF("д��flash... !\r\n");
    /* д������ */
    status =
        FLASH_WriteBuff(EXAMPLE_SECTOR, 0, 0, (uint8_t *)f_data_write, sizeof(f_data_write));
    if (status != kStatus_Success)
    {
        PRINTF("д��flash ʧ�� !\r\n");
        return -1;
    }
    PRINTF("д��flash�ɹ� !\r\n");
    PRINTF("��ȡflash����... !\r\n");
    /* �������� */
    FLASH_ReadBuff(EXAMPLE_SECTOR, 0, 0, (uint8_t *)f_data_rade, sizeof(f_data_write));

    if (memcmp(f_data_rade, f_data_write, sizeof(f_data_write)) != 0)
    {
        PRINTF("flash ����������д�Ĳ�һ�� !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("flash ��������д���Գɹ�. \r\n");
    }
	
    while (1)
    {
    }
}
