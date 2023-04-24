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




/* 全局变量 文件系统 */
static FATFS g_fileSystem; /* File system object */


/**
  * @brief    sdcard 管脚初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 星期五
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
  * @brief    检测卡是否插入
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 星期五
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
        PRINTF("\r\nSD主机初始化失败\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\n检测到SD卡插入.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\n未检测到SD卡插入.\r\n");
        return kStatus_Fail;
    }
    
    return kStatus_Success;
}

/**
  * @brief    SDCard 初始化 
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 星期五
  */
int SDCARD_Init(void)
{
    FRESULT error;
    
    /* 管脚初始化 */
    SDCARD_PinInit();
    
    /* 时钟初始化 */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0U);
    
    PRINTF("请插入SD卡...\r\n");
    
    /* 等待卡插入 */
    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return -1;
    }

    char ch;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    /* 挂载目标卷 */
    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("挂载卷失败了\r\n");
        PRINTF("\r\n输入 q 格式化SD卡并重新挂载 \n输入其他，结束测试.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
        if(ch == 'q')
        {
            BYTE work[FF_MAX_SS];
            PRINTF("\r\n创建文件系统并初始化SD卡......\n如果sd卡比较大，需要很长时间\r\n");
            if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
            {
                PRINTF("创建文件系统失败\r\n");
                return -1;
            }
        }
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("改变驱动失败\r\n");
        return -1;
    }
#endif

    return 0;
}




/* 用于读写测试的缓冲区大小 */
#define BUFFER_SIZE (100U)

/**
  * @brief    测试SDCard 
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/21 星期五
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

/* 如果文件夹创建不成功 可以关掉Cache 尝试 */
//    SCB_DisableDCache();
//    SCB_DisableICache();
    
    PRINTF("\r\nFATFS示例演示如何将FATFS与SD卡一起使用 card\r\n");

    PRINTF("\r\n请插入SD卡\r\n");

    SDCARD_Init();

    PRINTF("\r\n新建文件夹 dir_1......\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("文件夹创建成功\r\n");
        }
        else
        {
            PRINTF("文件夹创建失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n在文件夹dir_1下创建一个txt文件并打开......\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("txt文件创建并打开成功\r\n");
        }
        else
        {
            PRINTF("打开文件失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n在文件夹dir_1下 创建子文件夹dir_2......\r\n");
    error = f_mkdir(_T("/dir_1/dir_2"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("子文件夹创建成功\r\n");
        }
        else
        {
            PRINTF("子文件夹创建失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n打开文件夹dir_1......\r\n");
    if (f_opendir(&directory, "/dir_1"))
    {
        PRINTF("打开失败\r\n");
        return -1;
    }
    PRINTF("\r\n文件夹dir_1下的文件有......\r\n");
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
            PRINTF("文件夹 : %s\r\n", fileInformation.fname);
        }
        else
        {
            PRINTF("文件 : %s\r\n", fileInformation.fname);
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

        PRINTF("\r\n向f_1.txt写入数据 98个字符a\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("写入失败. \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("\r\n将文件指针指向f_1.txt 的第0个字节......\r\n");
        /* Move the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("将文件指针指向f_1.txt 的第0个字节 失败. \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("读取文件.\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("读取失败 \r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("比较读/写内容是否一致......\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("读写不一致.\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("读写一致 测试成功.\r\n");

        PRINTF("\r\n输入 q 退出测试 \n输入其他，再次进行读写测试.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\n测试结束.\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\n最后一定要记得关闭文件，否则数据可能丢失\r\n");
        return -1;
    }

    while (true)
    {
    }

	
}


