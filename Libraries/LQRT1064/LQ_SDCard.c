#include <stdio.h>
#include <string.h>
#include "fsl_sd.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "LQ_SDCard.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"




/* ȫ�ֱ��� �ļ�ϵͳ */
static FATFS g_fileSystem; /* File system object */


/**
  * @brief    sdcard �ܽų�ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 ������
  */
void SDCARD_PinInit(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);        
                                                                  
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_GPIO2_IO28,0U);                                                                     
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3,0U);                                   
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_GPIO2_IO28,0x017089u);                                                                                                                                            
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,0x017089u);                                                                     
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,0x014089u);                                                                     
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0,0x017089u);                                                                       
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0x017089u);                                                                       
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0x017089u);                                                                      
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0x017089u);                            
    
}



/**
  * @brief    ��⿨�Ƿ����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 ������
  */
static status_t sdcardWaitCardInsert(void)
{
    const sdmmchost_detect_card_t s_sdCardDetect = {
        .cdType = kSDMMCHOST_DetectCardByGpioCD,
        .cdTimeOut_ms = (~0U),
    };
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD������ʼ��ʧ��\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\n��⵽SD������.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nδ��⵽SD������.\r\n");
        return kStatus_Fail;
    }
    
    return kStatus_Success;
}

/**
  * @brief    SDCard ��ʼ�� 
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 ������
  */
int SDCARD_Init(void)
{
    FRESULT error;
    
    /* �ܽų�ʼ�� */
    SDCARD_PinInit();
    
    /* ʱ�ӳ�ʼ�� */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0U);
    
    PRINTF("�����SD��...\r\n");
    
    /* �ȴ������� */
    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return -1;
    }

    char ch;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    /* ����Ŀ��� */
    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("���ؾ�ʧ����\r\n");
        PRINTF("\r\n���� q ��ʽ��SD�������¹��� \n������������������.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
        if(ch == 'q')
        {
            BYTE work[FF_MAX_SS];
            PRINTF("\r\n�����ļ�ϵͳ����ʼ��SD��......\n���sd���Ƚϴ���Ҫ�ܳ�ʱ��\r\n");
            if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
            {
                PRINTF("�����ļ�ϵͳʧ��\r\n");
                return -1;
            }
        }
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("�ı�����ʧ��\r\n");
        return -1;
    }
#endif

    return 0;
}




/* ���ڶ�д���ԵĻ�������С */
#define BUFFER_SIZE (100U)

/**
  * @brief    ����SDCard 
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 ������
  */
int Test_SDCard(void)
{
    
    /* @brief decription about the read/write buffer
    * The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
    * block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
    * can define the block length by yourself if the card supports partial access.
    * The address of the read/write buffer should align to the specific DMA data buffer address align value if
    * DMA transfer is used, otherwise the buffer address is not important.
    * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
    */
    SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
              MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
    SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
              MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
    
    FRESULT error;
    DIR directory; /* Directory object */
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    static FIL g_fileObject;   
    volatile bool failedFlag = false;
    char ch = '0';

/* ����ļ��д������ɹ� ���Թص�Cache ���� */
//    SCB_DisableDCache();
//    SCB_DisableICache();
    
    PRINTF("\r\nFATFSʾ����ʾ��ν�FATFS��SD��һ��ʹ�� card\r\n");

    PRINTF("\r\n�����SD��\r\n");

    SDCARD_Init();

    PRINTF("\r\n�½��ļ��� dir_1......\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("�ļ��д����ɹ�\r\n");
        }
        else
        {
            PRINTF("�ļ��д���ʧ��\r\n");
            return -1;
        }
    }

    PRINTF("\r\n���ļ���dir_1�´���һ��txt�ļ�����......\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("txt�ļ��������򿪳ɹ�\r\n");
        }
        else
        {
            PRINTF("���ļ�ʧ��\r\n");
            return -1;
        }
    }

    PRINTF("\r\n���ļ���dir_1�� �������ļ���dir_2......\r\n");
    error = f_mkdir(_T("/dir_1/dir_2"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("���ļ��д����ɹ�\r\n");
        }
        else
        {
            PRINTF("���ļ��д���ʧ��\r\n");
            return -1;
        }
    }

    PRINTF("\r\n���ļ���dir_1......\r\n");
    if (f_opendir(&directory, "/dir_1"))
    {
        PRINTF("��ʧ��\r\n");
        return -1;
    }
    PRINTF("\r\n�ļ���dir_1�µ��ļ���......\r\n");
    for (;;)
    {
        error = f_readdir(&directory, &fileInformation);

        /* To the end. */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
        {
            break;
        }
        if (fileInformation.fname[0] == '.')
        {
            continue;
        }
        if (fileInformation.fattrib & AM_DIR)
        {
            PRINTF("�ļ��� : %s\r\n", fileInformation.fname);
        }
        else
        {
            PRINTF("�ļ� : %s\r\n", fileInformation.fname);
        }
    }

    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';

    
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        PRINTF("\r\n��f_1.txtд������ 98���ַ�a\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("д��ʧ��. \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("\r\n���ļ�ָ��ָ��f_1.txt �ĵ�0���ֽ�......\r\n");
        /* Move the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("���ļ�ָ��ָ��f_1.txt �ĵ�0���ֽ� ʧ��. \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("��ȡ�ļ�.\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("��ȡʧ�� \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("�Ƚ϶�/д�����Ƿ�һ��......\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("��д��һ��.\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("��дһ�� ���Գɹ�.\r\n");

        PRINTF("\r\n���� q �˳����� \n�����������ٴν��ж�д����.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\n���Խ���.\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\n���һ��Ҫ�ǵùر��ļ����������ݿ��ܶ�ʧ\r\n");
        return -1;
    }

    while (true)
    {
    }

	
}


