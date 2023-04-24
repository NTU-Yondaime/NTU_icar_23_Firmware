/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】测试线性CCD
【注意事项】

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
【函    数】Test_HCSR04
【功    能】测试超声波模块
【参    数】无
【返 回 值】无
【实    例】Test_HCSR04();
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR04(void)
{
	/* 初始化超声波的IO */
	CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0u);

	//输出口管脚配置结构体
    gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                            0,                   //低电平
                                            kGPIO_NoIntmode      //非中断模式
                                            };  
    
	//一般输入口管脚定义
    gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                                            0,                    //低电平
                                            kGPIO_NoIntmode,      //不触发中断
                                            };
	
    GPIO_PinInit(GPIO1, 4, &GPIO_Output_Config);     //F11
    GPIO_PinInit(GPIO2,24, &GPIO_Input_Config);      //A12  
	

	systime.init();                                  //开启systick定时器
	LED_Init();
	PRINTF("\r\nLQ HC-SR04 Test\n");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
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
			
			/* HC_TRIG 保持8-15us的高电平 */
			GPIO_PinWrite(GPIO1, 4, 1);
			systime.delay_us(12);
			GPIO_PinWrite(GPIO1, 4, 0);
			
			/* 超声波模块开始发送超声波 */
			while(!GPIO_PinRead(GPIO2, 24));
			
			/* 记录高电平时长 */
			hc_time = systime.get_time_us();
			
			/* 超声波模块收到超声波反射 */
			while(GPIO_PinRead(GPIO2, 24));
		
			/* 记录高电平时长 */
			hc_time = systime.get_time_us() - hc_time;
			
			/* 计算距离 hc_time/2 * 340m/s */
			dis = (float)(hc_time * 0.017);          //计算距离dis
			
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
//hc_starttime 记录超声波测距ECHO 高电平开始时间  hc_time 记录ECHO高电平持续时间
//--------------------------------------------------------------------------- 
volatile uint64_t hc_starttime; 
volatile uint16_t hc_time;

/*------------------------------------------------------------------------------------------------------
【函    数】Test_HCSR
【功    能】测试超声波模块   使用中断
【参    数】无
【返 回 值】无
【实    例】Test_HCSR();
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR(void)
{
    systime.init();                      //开启systick定时器
	LED_Init();
	PRINTF("\r\nLQ HC-SR04 Test \n");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	float dis = 0;
    
    
	/* 初始化超声波的IO */
	CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,0x10B0u);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0u);

	//输出口管脚配置结构体
    gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                            0,                   //低电平
                                            kGPIO_NoIntmode      //非中断模式
                                            };  
    // GPIO初始化结构体
	gpio_pin_config_t GPIO_Input_Config =  {
											kGPIO_DigitalInput,  // GPIO为输入方向
											0,                   // 低电平
											kGPIO_IntRisingOrFallingEdge,// 跳变沿触发中断
											};
	
    GPIO_PinInit(GPIO1, 4, &GPIO_Output_Config);     //F11
    GPIO_PinInit(GPIO2,24, &GPIO_Input_Config);      //A12  
	GPIO_PortEnableInterrupts(GPIO2,1<<24);			 // GPIO2_24中断使能
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			 //使能GPIO2_16~31IO的中断  
    /*GPIO 外部中断配置结束*/  
	
	
	while(1)
	{       
		if(systime.get_time_us() - now_time >= 50000)
		{
			now_time = systime.get_time_us();
			
			/* HC_TRIG 保持8-15us的高电平 */
			GPIO_PinWrite(GPIO1, 4, 1);
			systime.delay_us(12);
			GPIO_PinWrite(GPIO1, 4, 0);
			
		
			/* 计算距离 hc_time/2 * 340m/s */
			dis = (float)(hc_time * 0.017);          //计算距离dis
			
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

