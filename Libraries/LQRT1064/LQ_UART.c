/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年10月18日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
================================================
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "stdio.h"	
#include "LQ_UART.h"
#include "LQ_LED.h"
/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//IAR，需要在options -> C/C++compiler -> Preprocessor 添加宏定义 _DLIB_FILE_DESCRIPTOR
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
    /* 这里 printf 使用 LPUART1 可以自行修改 */
	UART_PutChar(LPUART1, (uint8_t)ch);	
	return ch;
}
#endif 



/**
  * @brief    发送一个字符
  *
  * @param    base  ： LPUART1 - LPUART8
  * @param    data  ： 发送的字符
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutChar(LPUART1, char);  //串口1  发送char
  *
  * @date     2019/6/6 星期四
  */
inline void UART_PutChar(LPUART_Type *base, uint8_t data)
{
    /* 等待base->STAT为空 */
    while (!(base->STAT & LPUART_STAT_TDRE_MASK));
    
    base->DATA = data;
}


/**
  * @brief    发送一个数组
  *
  * @param    base  ： LPUART1 - LPUART8
  * @param    data  ： 发送的字符
  * @param    length： 发送的字符长度
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutBuff(LPUART1, buff, 10);//串口1，发送一个长度为10地址为buff的数组
  *
  * @date     2019/6/6 星期四
  */
void UART_PutBuff(LPUART_Type *base, uint8_t * buff, uint32_t length)
{
    while (length--)
    {
        /* 等待base->STAT为空 */
        while (!(base->STAT & LPUART_STAT_TDRE_MASK));
        
        base->DATA = *(buff++);
    }
}



/**
  * @brief    发送一个字符串
  *
  * @param    base  ： LPUART1 - LPUART8
  * @param    data  ： 发送的字符
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutBuff(LPUART1, buff, 10);//串口1，发送一个长度为10地址为buff的数组
  *
  * @date     2019/6/6 星期四
  */
void UART_PutStr(LPUART_Type *base, uint8_t * str)
{
    while (*str)
    {
        UART_PutChar(base, *(str++));
    }
}



/**
  * @brief    串口初始化
  *
  * @param    base  ： LPUART1 - LPUART8
  * @param    bound ： 串口波特率
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void LQ_UART_Init(LPUART_Type *base, uint32_t bound)
{
    /* 初始化串口对应的管脚 对应管脚在LQ_UART.h 中通过宏定义确定 */
    UART_PinInit(base);
    
    lpuart_config_t config;
    
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    
    /* 设置波特率 */
    config.baudRate_Bps = bound;
    
    /* 设置奇偶校验 */
    config.parityMode = kLPUART_ParityDisabled;
    
    /* 设置停止位 */
    config.stopBitCount = kLPUART_OneStopBit;
    
    /* 设置数据位宽 */
    config.dataBitsCount = kLPUART_EightDataBits;
    
    /* 使能收发 */
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(base, &config, BOARD_DebugConsoleSrcFreq());
    
    /* 使能接收中断 */
    LPUART_EnableInterrupts(base, kLPUART_RxDataRegFullInterruptEnable);
}






/**
  * @brief    串口测试函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void Test_UART(void)
{
    LED_Init();   
    
    LQ_UART_Init(LPUART1, 115200);   
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //使能LPUART1中断

	LQ_UART_Init(LPUART8, 115200); 
	
    delayms(100);
    printf("\r\n龙邱串口测试例程\r\n");
    printf("串口模块波特率:\n");
    printf("LPUART1 init: 115200!\n");

	uint8_t a = 'a';
	uint8_t buff[10] = {'a','b','c','d','E','F','G'};
    while(1)
    {
        
        UART_PutChar(LPUART1, a);           //打印字符a
        delayms(100);
        
        UART_PutBuff(LPUART1, buff, 10);    //打印 字符串
		UART_PutBuff(LPUART8, buff, 10);    //打印 字符串
        delayms(100);
        
        printf("龙邱串口打印测试\n");
        
        UART_PutStr(LPUART1,"UART_PutStr打印测试\n");
        UART_PutStr(LPUART8,"UART_PutStr打印测试\n");
        delayms(10000);
        LED_ColorReverse(blue);
    }  
}

/**
  * @brief    串口引脚复用初始化
  *
  * @param    base： LPUART1 - LPUART8
  *
  * @return   
  *
  * @note     内部调用的，仅用来设置串口管脚复用的
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void UART_PinInit(LPUART_Type *base)
{	
	/* 开时钟 */
	CLOCK_EnableClock(kCLOCK_Iomuxc);
	
    if(base == LPUART1)
    {
        if(LP_UART1_RX == L14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,0U);   
        }
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART1_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART1_TX == K14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,0U);
        }
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART1_TX 引脚选择有误.\r\n");
        }
#endif    
    }
    else if(base == LPUART2)
    {
		if(LP_UART2_RX == M12)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_LPUART2_RX,0U);   
        }
		else if(LP_UART2_RX == P4)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_LPUART2_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART2_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART2_TX == L11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_LPUART2_TX,0U);
        }
		else if(LP_UART2_TX == P5)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_LPUART2_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART6_TX 引脚选择有误.\r\n");
        }
#endif    
         
    }
    else if(base == LPUART3)
    {
        if(LP_UART3_RX == B6)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_14_LPUART3_RX,0U);   
        }
		else if(LP_UART3_RX == C9)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LPUART3_RX,0U); 
		}
		else if(LP_UART3_RX == K10)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPUART3_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART3_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART3_TX == A6)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_13_LPUART3_TX,0U);
        }
		else if(LP_UART3_TX == B9)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LPUART3_TX,0U);
		}
		else if(LP_UART3_TX == C12)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPUART3_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART3_TX 引脚选择有误.\r\n");
        }
#endif   
    }
    else if(base == LPUART4)
    {
        if(LP_UART4_RX == A3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_LPUART4_RX,0U);   
        }
		else if(LP_UART4_RX == B11)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LPUART4_RX,0U); 
		}
		else if(LP_UART4_RX == M5)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_LPUART4_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART4_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART4_TX == A11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LPUART4_TX,0U);
        }
		else if(LP_UART4_TX == B4)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_LPUART4_TX,0U);
		}
		else if(LP_UART4_TX == L5)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_LPUART4_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART4_TX 引脚选择有误.\r\n");
        }
#endif 
    }
    else if(base == LPUART5)
    {
        if(LP_UART5_RX == D3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_24_LPUART5_RX,0U);   
        }
		else if(LP_UART5_RX == D14)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART5_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART5_TX == D13)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TX,0U);
        }
		else if(LP_UART5_TX == G2)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_23_LPUART5_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART6_TX 引脚选择有误.\r\n");
        }
#endif    
    }
    else if(base == LPUART6)
    {
        if(LP_UART6_RX == B3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_26_LPUART6_RX,0U);   
        }
		else if(LP_UART6_RX == G11)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_LPUART6_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART6_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART6_TX == D2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_25_LPUART6_TX,0U);
        }
		else if(LP_UART6_TX == M11)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_LPUART6_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART6_TX 引脚选择有误.\r\n");
        }
#endif     
    }
    else if(base == LPUART7)
    {
        if(LP_UART7_RX == D5)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_LPUART7_RX,0U);   
        }
		else if(LP_UART7_RX == N4)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_LPUART7_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART7_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART7_TX == C5)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_LPUART7_TX,0U);
        }
		else if(LP_UART7_TX == P3)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_LPUART7_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART7_TX 引脚选择有误.\r\n");
        }
#endif  
    }
    else if(base == LPUART8)
    {
        if(LP_UART8_RX == B7)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_39_LPUART8_RX,0U);   
        }
		else if(LP_UART8_RX == J2)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_LPUART8_RX,0U); 
		}
		else if(LP_UART8_RX == J13)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_LPUART8_RX,0U); 
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART8_RX 引脚选择有误.\r\n");
        }
#endif
        if(LP_UART8_TX == D6)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_38_LPUART8_TX,0U);
        }
		else if(LP_UART8_TX == H2)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_LPUART8_TX,0U);
		}
		else if(LP_UART8_TX == L13)
		{
			IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_LPUART8_TX,0U);
		}
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART8_TX 引脚选择有误.\r\n");
        }
#endif 
    }
}

