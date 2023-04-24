/*-----------------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【dev.env.】IAR8.30.1及以上版本
--------------------------------------------------------------------*/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "LQ_LED.h"
#include "LQ_KEY.h"
#include "LQ_GPIO.h"
/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


#if  1   //使用fsl库 操作GPIO

/**
  * @brief    初始化龙邱母板上的按键
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO3_IO04 ------ 管脚P2   ----- >  母板按键K2
  * @note     GPIO2_IO27 ------ 管脚C13  ----- >  母板按键K0
  * @note     GPIO2_IO30 ------ 管脚C14  ----- >  母板按键k1
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void KEY_Init(void)
{  
    CLOCK_EnableClock(kCLOCK_Iomuxc);                    // IO口时钟使能
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);//管脚P2
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,0U);   //管脚C13
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,0U);   //管脚C14
    
    /*
    字段解释： 
    2.0、SRE 数据位：0 
    SRE(Slew Rate Field)：转换速度字段 这是一个可以调整高低电平切换上升和下降时间速度的设置项，
    在一般的应用中使用芯片默认的设置就可以了。在需要微调SI或EMI性能时可以尝试修改配置 
    可选的值： 
    0 SRE_0_Slow_Slew_Rate — Slow Slew Rate：转换速度慢 
    1 SRE_1_Fast_Slew_Rate — Fast Slew Rate： 转换速度快
    
    2.1、DSE 数据位5-3 
    DSE(Drive Strength Field)：驱动能力字段，作为输出时有效 
    可选的值： 
    000 DSE_0_output_driver_disabled_ — output driver disabled; 禁止输出 
    001 DSE_1_R0_260_Ohm___3_3V__150_Ohm_1_8V__240_Ohm_for_DDR_ — R0(260 Ohm 
    @ 3.3V, 150 Ohm@1.8V, 240 Ohm for DDR) -R0：260欧姆@3.3v；150欧姆@1.8v 
    010 DSE_2_R0_2 — R0/2 -R0的二分之一，一下类同 
    011 DSE_3_R0_3 — R0/3 
    100 DSE_4_R0_4 — R0/4 
    101 DSE_5_R0_5 — R0/5 
    110 DSE_6_R0_6 — R0/6 
    111 DSE_7_R0_7 — R0/7
    
    2.2、SPEED 数据位7-6 
    Speed Field：速度字段 
    可选的值 
    00 SPEED_0_low_50MHz_ — low(50MHz) 最低50MHz 
    01 SPEED_1_medium_100MHz_ — medium(100MHz)中间值100MHz 
    10 SPEED_2_medium_100MHz_ — medium(100MHz)中间值100MHz 
    11 SPEED_3_max_200MHz_ — max(200MHz) 最大200MHz
    
    2.3、ODE 数据位11 
    ODE(Open Drain Enable Field)：漏极开路？？？作为输出时有效，依靠外部电压，提高输出能力 
    可选的值： 
    0 ODE_0_Open_Drain_Disabled — Open Drain Disabled ：禁止漏极开路 
    1 ODE_1_Open_Drain_Enabled — Open Drain Enabled： 使能漏极开路
    
    2.4、PKE 数据位12 
    PKE(Pull / Keep Enable Field)：拉/保持使能字段 
    可选的值： 
    0 PKE_0_Pull_Keeper_Disabled — Pull/Keeper Disabled： 禁止拉/保持 
    1 PKE_1_Pull_Keeper_Enabled — Pull/Keeper Enabled： 使能拉/保持
    
    2.5、PUE 数据位13 
    PUE(Pull / Keep Select Field)：拉/保持选择字段？？？两者不能同时设置， 
    保持：使能输入状态保存器可以在IO供电NVCC_xxx关掉之后，使输入缓冲器的输出自动维持在关电之前的逻辑状态。其价值可以在低功耗的应用中得到体现。 
    使能输出状态保存器可以在内核供电关掉之后，使IO的输出自动维持在关电之前的逻辑状态(需要注意的是输出状态保存器不能与上下拉同时工作)。其价值同样可以体现在低功耗的应用中 
    可选的值： 
    0 PUE_0_Keeper — Keeper ：保持 
    1 PUE_1_Pull — Pull ：拉
    
    2.6、PUS 数据位15-14 
    PUS(Pull Up / Down Config. Field)：上拉/下拉配置？？？主要作用是提高输出信号的驱动能力、确定输入信号的电平（防止干扰） 
    可选的值： 
    00 PUS_0_100K_Ohm_Pull_Down — 100K Ohm Pull Down ：100K欧姆的拉低 
    01 PUS_1_47K_Ohm_Pull_Up — 47K Ohm Pull Up ：47K欧姆的拉高 
    10 PUS_2_100K_Ohm_Pull_Up — 100K Ohm Pull Up ：100K欧姆的拉高 
    11 PUS_3_22K_Ohm_Pull_Up — 22K Ohm Pull Up ：22K欧姆的拉高
    
    2.77、HYS 数据位16 
    HYS(Hyst. Enable Field)：磁滞使能字段？？？作为输入时有效 
    0 HYS_0_Hysteresis_Disabled — Hysteresis Disabled：禁止磁滞 
    1 HYS_1_Hysteresis_Enabled — Hysteresis Enabled： 使能磁滞*/
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0xF080); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0xF080);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0xF080);
    
    //一般输入口管脚定义
    gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                                            1,                    //高电平
                                            kGPIO_NoIntmode,      //不触发中断
                                            };
    
    GPIO_PinInit(GPIO2, 27, &GPIO_Input_Config);        // GPIO输入口，非中断
    GPIO_PinInit(GPIO2, 30, &GPIO_Input_Config);        // GPIO输入口，非中断
    GPIO_PinInit(GPIO3,  4, &GPIO_Input_Config);        // GPIO输入口，非中断
    
}


/**
  * @brief    读取按键状态
  *
  * @param    mode： 0：不支持连续按;   1：支持连续按;
  *
  * @return   0：无按键按下  1：KEY0按下  2:KEY1按下  3:KEY2按下
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
        key_up=1;      //支持连按
    }
    if(key_up && (GPIO_PinRead(GPIO2, 27)==0 || GPIO_PinRead(GPIO2, 30)==0 || GPIO_PinRead(GPIO3, 4)==0))
    {
        delayms(10);   //消抖
        key_up=0;
        if(GPIO_PinRead(GPIO2, 27)==0)      
        {
            return 1;
        }
        
        else if(GPIO_PinRead(GPIO2, 30)==0) 
        {
            return 2;    
        }
        
        else if(GPIO_PinRead(GPIO3,  4)==0) 
        {
            return 3;    
        }
        
    }
    if(GPIO_PinRead(GPIO2, 27)==1 && GPIO_PinRead(GPIO2, 30)==1 && GPIO_PinRead(GPIO3,  4)==1) 
    {
        key_up=1;   
    }
    return 0;   //无按键按下
    
}



/*!
  * @brief    测试GPIIO外部中断
  *
  * @param    
  *
  * @return   
  *
  * @note     按下母板上不同的按键 不同颜色的灯闪烁
  *
  * @example  
  *
  * @date     2019/8/19 星期一
  */
void Test_ExtiKEY(void)
{
    LED_Init();
    
    /*GPIO 外部中断配置开始*/
    CLOCK_EnableClock(kCLOCK_Iomuxc);                         // IO口时钟使能
    gpio_pin_config_t GPIO_Input_Config =                     // GPIO初始化结构体
    {
        .direction = kGPIO_DigitalInput,                      // 输入
        .outputLogic = 0U,                                  
        .interruptMode = kGPIO_IntFallingEdge                 // 下降沿中断
    };
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,0U);        // 设置管脚复用功能                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0x90B0U);// 配置管脚 上拉 
    GPIO_PinInit(GPIO2, 27U, &GPIO_Input_Config);             // GPIO输入口，上拉 下降沿触发中断
    GPIO_PortEnableInterrupts(GPIO2,1<<27);			          // C13 GPIO2_27中断使能
    
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,0U);        // 设置管脚复用功能                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0x90B0U);// 配置管脚 上拉
    GPIO_PinInit(GPIO2, 30U, &GPIO_Input_Config);             // GPIO输入口，上拉 下降沿触发中断   
    GPIO_PortEnableInterrupts(GPIO2,1<<30);			          // C14 GPIO2_30中断使能
 
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);        // 设置管脚复用功能                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0x90B0U);// 配置管脚 上拉
    GPIO_PinInit(GPIO3, 4U, &GPIO_Input_Config);                 // GPIO输入口，上拉 下降沿触发中断   
    GPIO_PortEnableInterrupts(GPIO3,1<<4);			             // P2 GPIO3_4中断使能
    
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31IO的中断  
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(GPIO3_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO3_Combined_0_15_IRQn);			          //使能GPIO3_0~15IO的中断 
    /*GPIO 外部中断配置结束*/  
    
    /* 在irq.c 中 处理中断 */
    while(1);
}


#else //使用封装过的fsl库操作GPIO

/**
  * @brief    初始化龙邱母板上的按键
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO3_IO04 ------ 管脚P2   ----- >  母板按键K2
  * @note     GPIO2_IO27 ------ 管脚C13  ----- >  母板按键K0
  * @note     GPIO2_IO30 ------ 管脚C14  ----- >  母板按键k1
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void KEY_Init(void)
{  
    /* 设置上拉输入 */
    LQ_PinInit(P2,  PIN_MODE_INPUT_PULLUP, 1);
    LQ_PinInit(C13, PIN_MODE_INPUT_PULLUP, 1);
    LQ_PinInit(C14, PIN_MODE_INPUT_PULLUP, 1);  
}


/**
  * @brief    读取按键状态
  *
  * @param    mode： 0：不支持连续按;   1：支持连续按;
  *
  * @return   0：无按键按下  1：KEY0按下  2:KEY1按下  3:KEY2按下
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
        key_up=1;      //支持连按
    }
    if(key_up && (LQ_PinRead(C13)==0 || LQ_PinRead(C14)==0 || LQ_PinRead(P2)==0))
    {
        delayms(10);   //消抖
        key_up=0;
        if(LQ_PinRead(C13)==0)      
        {
            return 1;
        }
        
        else if(LQ_PinRead(C14)==0) 
        {
            return 2;    
        }
        
        else if(LQ_PinRead(P2)==0) 
        {
            return 3;    
        }
        
    }
    if(LQ_PinRead(C13)==1 && LQ_PinRead(C14)==1 && LQ_PinRead(P2)==1) 
    {
        key_up=1;   
    }
    return 0;   //无按键按下
    
}



/*!
  * @brief    测试GPIIO外部中断
  *
  * @param    
  *
  * @return   
  *
  * @note     按下母板上不同的按键 不同颜色的灯闪烁
  *
  * @example  
  *
  * @date     2019/8/19 星期一
  */
void Test_ExtiKEY(void)
{
    LED_Init();
    
    /* 配置下降沿触发中断 */
    LQ_PinExti(C13, PIN_IRQ_MODE_FALLING);
    LQ_PinExti(C14, PIN_IRQ_MODE_FALLING);
    LQ_PinExti(P2,  PIN_IRQ_MODE_FALLING);
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31IO的中断  
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(GPIO3_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO3_Combined_0_15_IRQn);			          //使能GPIO3_0~15IO的中断 
    /*GPIO 外部中断配置结束*/  
    
    /* 在irq.c 中 处理中断 */
    while(1);



}
#endif

/**
  * @brief    测试按键
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void Test_KEY(void)
{ 
    LED_Init();
    KEY_Init(); 
    while (1)
    {  
        switch(KEY_Read(1))  
        {
            case 1:
                LED_Color(red);     
                break;           
            case 2:      
                LED_Color(green);   
                break;
            case 3:      
                LED_Color(blue);    
                break;
            default:
                LED_Color(white);   
                break;
        }

        delayms(50);
    }
}


