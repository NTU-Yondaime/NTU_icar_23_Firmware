/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2018��10��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
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
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//IAR����Ҫ��options -> C/C++compiler -> Preprocessor ��Ӻ궨�� _DLIB_FILE_DESCRIPTOR
#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
    /* ���� printf ʹ�� LPUART1 ���������޸� */
	UART_PutChar(LPUART1, (uint8_t)ch);	
	return ch;
}
#endif 



/**
  * @brief    ����һ���ַ�
  *
  * @param    base  �� LPUART1 - LPUART8
  * @param    data  �� ���͵��ַ�
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutChar(LPUART1, char);  //����1  ����char
  *
  * @date     2019/6/6 ������
  */
inline void UART_PutChar(LPUART_Type *base, uint8_t data)
{
    /* �ȴ�base->STATΪ�� */
    while (!(base->STAT & LPUART_STAT_TDRE_MASK));
    
    base->DATA = data;
}


/**
  * @brief    ����һ������
  *
  * @param    base  �� LPUART1 - LPUART8
  * @param    data  �� ���͵��ַ�
  * @param    length�� ���͵��ַ�����
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutBuff(LPUART1, buff, 10);//����1������һ������Ϊ10��ַΪbuff������
  *
  * @date     2019/6/6 ������
  */
void UART_PutBuff(LPUART_Type *base, uint8_t * buff, uint32_t length)
{
    while (length--)
    {
        /* �ȴ�base->STATΪ�� */
        while (!(base->STAT & LPUART_STAT_TDRE_MASK));
        
        base->DATA = *(buff++);
    }
}



/**
  * @brief    ����һ���ַ���
  *
  * @param    base  �� LPUART1 - LPUART8
  * @param    data  �� ���͵��ַ�
  *
  * @return   
  *
  * @note     
  *
  * @example  UART_PutBuff(LPUART1, buff, 10);//����1������һ������Ϊ10��ַΪbuff������
  *
  * @date     2019/6/6 ������
  */
void UART_PutStr(LPUART_Type *base, uint8_t * str)
{
    while (*str)
    {
        UART_PutChar(base, *(str++));
    }
}



/**
  * @brief    ���ڳ�ʼ��
  *
  * @param    base  �� LPUART1 - LPUART8
  * @param    bound �� ���ڲ�����
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void LQ_UART_Init(LPUART_Type *base, uint32_t bound)
{
    /* ��ʼ�����ڶ�Ӧ�Ĺܽ� ��Ӧ�ܽ���LQ_UART.h ��ͨ���궨��ȷ�� */
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
    
    /* ���ò����� */
    config.baudRate_Bps = bound;
    
    /* ������żУ�� */
    config.parityMode = kLPUART_ParityDisabled;
    
    /* ����ֹͣλ */
    config.stopBitCount = kLPUART_OneStopBit;
    
    /* ��������λ�� */
    config.dataBitsCount = kLPUART_EightDataBits;
    
    /* ʹ���շ� */
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(base, &config, BOARD_DebugConsoleSrcFreq());
    
    /* ʹ�ܽ����ж� */
    LPUART_EnableInterrupts(base, kLPUART_RxDataRegFullInterruptEnable);
}






/**
  * @brief    ���ڲ��Ժ���
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void Test_UART(void)
{
    LED_Init();   
    
    LQ_UART_Init(LPUART1, 115200);   
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //ʹ��LPUART1�ж�

	LQ_UART_Init(LPUART8, 115200); 
	
    delayms(100);
    printf("\r\n���񴮿ڲ�������\r\n");
    printf("����ģ�鲨����:\n");
    printf("LPUART1 init: 115200!\n");

	uint8_t a = 'a';
	uint8_t buff[10] = {'a','b','c','d','E','F','G'};
    while(1)
    {
        
        UART_PutChar(LPUART1, a);           //��ӡ�ַ�a
        delayms(100);
        
        UART_PutBuff(LPUART1, buff, 10);    //��ӡ �ַ���
		UART_PutBuff(LPUART8, buff, 10);    //��ӡ �ַ���
        delayms(100);
        
        printf("���񴮿ڴ�ӡ����\n");
        
        UART_PutStr(LPUART1,"UART_PutStr��ӡ����\n");
        UART_PutStr(LPUART8,"UART_PutStr��ӡ����\n");
        delayms(10000);
        LED_ColorReverse(blue);
    }  
}

/**
  * @brief    �������Ÿ��ó�ʼ��
  *
  * @param    base�� LPUART1 - LPUART8
  *
  * @return   
  *
  * @note     �ڲ����õģ����������ô��ڹܽŸ��õ�
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void UART_PinInit(LPUART_Type *base)
{	
	/* ��ʱ�� */
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
            PRINTF("LP_UART1_RX ����ѡ������.\r\n");
        }
#endif
        if(LP_UART1_TX == K14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,0U);
        }
#ifdef DEBUG
        else
        {
            PRINTF("LP_UART1_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART2_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART6_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART3_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART3_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART4_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART4_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART5_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART6_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART6_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART6_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART7_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART7_TX ����ѡ������.\r\n");
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
            PRINTF("LP_UART8_RX ����ѡ������.\r\n");
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
            PRINTF("LP_UART8_TX ����ѡ������.\r\n");
        }
#endif 
    }
}

