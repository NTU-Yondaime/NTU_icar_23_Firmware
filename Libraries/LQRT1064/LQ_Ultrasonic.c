/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ���������CCD
��ע�����

-------------------------------------------------------------------------------------------------------*/
#include "stdio.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "include.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
#include "LQ_LED.h"
#include "LQ_SYSTICK.h"
#include "LQ_LED.h"


/*------------------------------------------------------------------------------------------------------
����    ����Test_HCSR04
����    �ܡ����Գ�����ģ��
����    ������
���� �� ֵ����
��ʵ    ����Test_HCSR04();
��ע�����
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR04(void)
{
	/* ��ʼ����������IO */
	CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO��ʱ��ʹ��

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0u);

	//����ڹܽ����ýṹ��
    gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIOΪ�������
                                            0,                   //�͵�ƽ
                                            kGPIO_NoIntmode      //���ж�ģʽ
                                            };  
    
	//һ������ڹܽŶ���
    gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIOΪ���뷽��
                                            0,                    //�͵�ƽ
                                            kGPIO_NoIntmode,      //�������ж�
                                            };
	
    GPIO_PinInit(GPIO1, 4, &GPIO_Output_Config);     //F11
    GPIO_PinInit(GPIO2,24, &GPIO_Input_Config);      //A12  
	

	systime.init();                                  //����systick��ʱ��
	LED_Init();
	PRINTF("\r\nLQ HC-SR04 Test\n");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8��ʼ��  0��������ʾ  1��������ʾ  
    TFTSPI_CLS(u16BLUE);           //����
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	uint64_t hc_time;
	float dis = 0;
	while(1)
	{
		if(systime.get_time_us() - now_time >= 50000)
		{
			now_time = systime.get_time_us();
			
			/* HC_TRIG ����8-15us�ĸߵ�ƽ */
			GPIO_PinWrite(GPIO1, 4, 1);
			systime.delay_us(12);
			GPIO_PinWrite(GPIO1, 4, 0);
			
			/* ������ģ�鿪ʼ���ͳ����� */
			while(!GPIO_PinRead(GPIO2, 24));
			
			/* ��¼�ߵ�ƽʱ�� */
			hc_time = systime.get_time_us();
			
			/* ������ģ���յ����������� */
			while(GPIO_PinRead(GPIO2, 24));
		
			/* ��¼�ߵ�ƽʱ�� */
			hc_time = systime.get_time_us() - hc_time;
			
			/* ������� hc_time/2 * 340m/s */
			dis = (float)(hc_time * 0.017);          //�������dis
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm ", dis);
				PRINTF(txt);
                PRINTF("\n");
				LED_ColorReverse(green);
#ifdef OLED
				LCD_P8x16Str(0,3,txt);
#else
				TFTSPI_P8X16Str(2,3,txt,u16RED,u16BLUE);
#endif
			}
		}
	}
}

//---------------------------------------------------------------------------
//hc_starttime ��¼���������ECHO �ߵ�ƽ��ʼʱ��  hc_time ��¼ECHO�ߵ�ƽ����ʱ��
//--------------------------------------------------------------------------- 
volatile uint64_t hc_starttime; 
volatile uint16_t hc_time;

/*------------------------------------------------------------------------------------------------------
����    ����Test_HCSR
����    �ܡ����Գ�����ģ��   ʹ���ж�
����    ������
���� �� ֵ����
��ʵ    ����Test_HCSR();
��ע�����
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR(void)
{
    systime.init();                      //����systick��ʱ��
	LED_Init();
	PRINTF("\r\nLQ HC-SR04 Test \n");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8��ʼ��  0��������ʾ  1��������ʾ  
    TFTSPI_CLS(u16BLUE);           //����
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	float dis = 0;
    
    
	/* ��ʼ����������IO */
	CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO��ʱ��ʹ��

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0u);

	//����ڹܽ����ýṹ��
    gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIOΪ�������
                                            0,                   //�͵�ƽ
                                            kGPIO_NoIntmode      //���ж�ģʽ
                                            };  
    // GPIO��ʼ���ṹ��
	gpio_pin_config_t GPIO_Input_Config =  {
											kGPIO_DigitalInput,  // GPIOΪ���뷽��
											0,                   // �͵�ƽ
											kGPIO_IntRisingOrFallingEdge,// �����ش����ж�
											};
	
    GPIO_PinInit(GPIO1, 4, &GPIO_Output_Config);     //F11
    GPIO_PinInit(GPIO2,24, &GPIO_Input_Config);      //A12  
	GPIO_PortEnableInterrupts(GPIO2,1<<24);			 // GPIO2_24�ж�ʹ��
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			 //ʹ��GPIO2_16~31IO���ж�  
    /*GPIO �ⲿ�ж����ý���*/  
	
	
	while(1)
	{       
		if(systime.get_time_us() - now_time >= 50000)
		{
			now_time = systime.get_time_us();
			
			/* HC_TRIG ����8-15us�ĸߵ�ƽ */
			GPIO_PinWrite(GPIO1, 4, 1);
			systime.delay_us(12);
			GPIO_PinWrite(GPIO1, 4, 0);
			
		
			/* ������� hc_time/2 * 340m/s */
			dis = (float)(hc_time * 0.017);          //�������dis
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm ", dis);
				PRINTF(txt);
                PRINTF("\n");
				LED_ColorReverse(green);
#ifdef OLED
				LCD_P8x16Str(0,3,txt);
#else
				TFTSPI_P8X16Str(2,3,txt,u16RED,u16BLUE);
#endif
			}
		}
	}
}

