/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月13日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_Cfg.h"

#define __RT1052_PIN(PORT, PIN) {PORT, PIN}
#define __RT1052_PIN_DEFAULT {0, 0}

struct rt1052_pin
{
    GPIO_Type    *gpio;
    uint32_t      pin;
};

/* 映射关系数组 */
static struct rt1052_pin rt_pin_map[] =
{
    __RT1052_PIN_DEFAULT,       /* A1 */
	__RT1052_PIN(GPIO4, 27),    /* A2 GPIO_EMC_27 */
	__RT1052_PIN(GPIO4, 20),    /* A3 GPIO_EMC_20 */
	__RT1052_PIN(GPIO4, 17),    /* A4 GPIO_EMC_17 */
	__RT1052_PIN(GPIO4, 16),    /* A5 GPIO_EMC_16 */
	__RT1052_PIN(GPIO4, 13),    /* A6 GPIO_EMC_13 */
	__RT1052_PIN(GPIO3, 26),    /* A7 GPIO_EMC_40 */
	__RT1052_PIN(GPIO2,  6),    /* A8 GPIO_B0_06 */
	__RT1052_PIN(GPIO2,  7),    /* A9 GPIO_B0_07 */
	__RT1052_PIN(GPIO2, 11),    /* A10 GPIO_B0_11 */
	__RT1052_PIN(GPIO2, 16),    /* A11 GPIO_B1_00 */
	__RT1052_PIN(GPIO2, 24),    /* A12 GPIO_B1_08 */
	__RT1052_PIN(GPIO2, 25),    /* A13 GPIO_B1_09 */
	__RT1052_PIN_DEFAULT,       /* A14 */
	

	__RT1052_PIN(GPIO4, 15),    /* B1 GPIO_EMC_15 */
	__RT1052_PIN(GPIO4, 18),    /* B2 GPIO_EMC_18 */
	__RT1052_PIN(GPIO4, 26),    /* B3 GPIO_EMC_26 */
	__RT1052_PIN(GPIO4, 19),    /* B4 GPIO_EMC_19 */
	__RT1052_PIN_DEFAULT,       /* B5  */
	__RT1052_PIN(GPIO4, 14),    /* B6 GPIO_EMC_14 */
	__RT1052_PIN(GPIO3, 25),    /* B7 GPIO_EMC_39 */
	__RT1052_PIN(GPIO2,  5),    /* B8 GPIO_B0_05 */
	__RT1052_PIN(GPIO2,  8),    /* B9 GPIO_B0_08 */
	__RT1052_PIN_DEFAULT,       /* B10  */
	__RT1052_PIN(GPIO2, 17),    /* B11 GPIO_B1_01 */
	__RT1052_PIN(GPIO2, 23),    /* B12 GPIO_B1_07 */
	__RT1052_PIN(GPIO2, 26),    /* B13 GPIO_B1_10 */
	__RT1052_PIN(GPIO2, 31),    /* B14 GPIO_B1_15 */
	

	__RT1052_PIN(GPIO4, 21),    /* C1 GPIO_EMC_21 */
	__RT1052_PIN(GPIO4,  9),    /* C2 GPIO_EMC_09 */
	__RT1052_PIN(GPIO3, 22),    /* C3 GPIO_EMC_36 */
	__RT1052_PIN(GPIO3, 19),    /* C4 GPIO_EMC_33 */
	__RT1052_PIN(GPIO4, 31),    /* C5 GPIO_EMC_31 */
	__RT1052_PIN(GPIO4, 30),    /* C6 GPIO_EMC_30 */
	__RT1052_PIN(GPIO3, 27),    /* C7 GPIO_EMC_41 */
	__RT1052_PIN(GPIO2,  4),    /* C8 GPIO_B0_04 */
	__RT1052_PIN(GPIO2,  9),    /* C9 GPIO_B0_09 */
	__RT1052_PIN(GPIO2, 12),    /* C10 GPIO_B0_12 */
	__RT1052_PIN(GPIO2, 18),    /* C11 GPIO_B1_02 */
	__RT1052_PIN(GPIO2, 22),    /* C12 GPIO_B1_06 */
	__RT1052_PIN(GPIO2, 27),    /* C13 GPIO_B1_11 */
	__RT1052_PIN(GPIO2, 30),    /* C14 GPIO_B1_14 */
	

	__RT1052_PIN(GPIO4, 28),    /* D1 GPIO_EMC_28 */
	__RT1052_PIN(GPIO4, 25),    /* D2 GPIO_EMC_25 */
	__RT1052_PIN(GPIO4, 24),    /* D3 GPIO_EMC_24 */
	__RT1052_PIN(GPIO3, 20),    /* D4 GPIO_EMC_34 */
	__RT1052_PIN(GPIO3, 18),    /* D5 GPIO_EMC_32 */
	__RT1052_PIN(GPIO3, 24),    /* D6 GPIO_EMC_38 */
	__RT1052_PIN(GPIO2,  0),    /* D7 GPIO_B0_00 */
	__RT1052_PIN(GPIO2,  3),    /* D8 GPIO_B0_03 */
	__RT1052_PIN(GPIO2, 10),    /* D9 GPIO_B0_10 */
	__RT1052_PIN(GPIO2, 13),    /* D10 GPIO_B0_13 */
	__RT1052_PIN(GPIO2, 19),    /* D11 GPIO_B1_03 */
	__RT1052_PIN(GPIO2, 21),    /* D12 GPIO_B1_05 */
	__RT1052_PIN(GPIO2, 28),    /* D13 GPIO_B1_12 */
	__RT1052_PIN(GPIO2, 29),    /* D14 GPIO_B1_13 */
	

	__RT1052_PIN(GPIO4, 29),    /* E1 GPIO_EMC_29 */
	__RT1052_PIN_DEFAULT,       /* E2  */
	__RT1052_PIN(GPIO4,  0),    /* E3 GPIO_EMC_00 */
	__RT1052_PIN(GPIO3, 23),    /* E4 GPIO_EMC_37 */
	__RT1052_PIN(GPIO3, 21),    /* E5 GPIO_EMC_35 */
	__RT1052_PIN_DEFAULT,       /* E6  */
	__RT1052_PIN(GPIO2,  1),    /* E7 GPIO_B0_01 */
	__RT1052_PIN(GPIO2,  2),    /* E8 GPIO_B0_02 */
	__RT1052_PIN_DEFAULT,       /* E9  */
	__RT1052_PIN(GPIO2, 14),    /* E10 GPIO_B0_14 */
	__RT1052_PIN(GPIO2, 15),    /* E11 GPIO_B0_15 */
	__RT1052_PIN(GPIO2, 20),    /* E12 GPIO_B1_04 */
	__RT1052_PIN_DEFAULT,       /* E13  */
	__RT1052_PIN(GPIO1,  6),    /* E14 GPIO_AD_B0_06 */
	

	__RT1052_PIN(GPIO4, 22),    /* F1 GPIO_EMC_22 */
	__RT1052_PIN(GPIO4,  4),    /* F2 GPIO_EMC_04 */
	__RT1052_PIN(GPIO4,  1),    /* F3 GPIO_EMC_01 */
	__RT1052_PIN(GPIO4,  2),    /* F4 GPIO_EMC_02 */
	__RT1052_PIN_DEFAULT,       /* F5  */
	__RT1052_PIN_DEFAULT,       /* F6  */
	__RT1052_PIN_DEFAULT,       /* F7  */
	__RT1052_PIN_DEFAULT,       /* F8  */
	__RT1052_PIN_DEFAULT,       /* F9  */
	__RT1052_PIN_DEFAULT,       /* F10  */
	__RT1052_PIN(GPIO1,  4),    /* F11 GPIO_AD_B0_04 */
	__RT1052_PIN(GPIO1,  7),    /* F12 GPIO_AD_B0_07 */
	__RT1052_PIN(GPIO1,  8),    /* F13 GPIO_AD_B0_08 */
	__RT1052_PIN(GPIO1,  9),    /* F14 GPIO_AD_B0_09 */
	

	__RT1052_PIN(GPIO4, 10),    /* G1 GPIO_EMC_10 */
	__RT1052_PIN(GPIO4, 23),    /* G2 GPIO_EMC_23 */
	__RT1052_PIN(GPIO4, 11),    /* G3 GPIO_EMC_11 */
	__RT1052_PIN(GPIO4,  3),    /* G4 GPIO_EMC_03 */
	__RT1052_PIN(GPIO4,  5),    /* G5 GPIO_EMC_05 */
	__RT1052_PIN_DEFAULT,       /* G6  */
	__RT1052_PIN_DEFAULT,       /* G7  */
	__RT1052_PIN_DEFAULT,       /* G8  */
	__RT1052_PIN_DEFAULT,       /* G9  */
	__RT1052_PIN(GPIO1, 11),    /* G10 GPIO_AD_B0_11 */
	__RT1052_PIN(GPIO1,  3),    /* G11 GPIO_AD_B0_03 */
	__RT1052_PIN(GPIO1, 30),    /* G12 GPIO_AD_B1_14 */
	__RT1052_PIN(GPIO1, 10),    /* G13 GPIO_AD_B0_10 */
	__RT1052_PIN(GPIO1,  5),    /* G14 GPIO_AD_B0_05 */
	

	__RT1052_PIN(GPIO4, 12),    /* H1 GPIO_EMC_12 */
	__RT1052_PIN(GPIO3, 16),    /* H2 GPIO_SD_B0_04 */
	__RT1052_PIN(GPIO4,  8),    /* H3 GPIO_EMC_08 */
	__RT1052_PIN(GPIO4,  7),    /* H4 GPIO_EMC_07 */
	__RT1052_PIN(GPIO4,  6),    /* H5 GPIO_EMC_06 */
	__RT1052_PIN_DEFAULT,       /* H6  */
	__RT1052_PIN_DEFAULT,       /* H7  */
	__RT1052_PIN_DEFAULT,       /* H8  */
	__RT1052_PIN_DEFAULT,       /* H9  */
	__RT1052_PIN(GPIO1,  1),    /* H10 GPIO_AD_B0_01 */
	__RT1052_PIN(GPIO1, 29),    /* H11 GPIO_AD_B1_13 */
	__RT1052_PIN(GPIO1, 28),    /* H12 GPIO_AD_B1_12 */
	__RT1052_PIN(GPIO1, 24),    /* H13 GPIO_AD_B1_08 */
	__RT1052_PIN(GPIO1, 14),    /* H14 GPIO_AD_B0_14 */
	

	__RT1052_PIN(GPIO3, 14),    /* J1 GPIO_SD_B0_02 */
	__RT1052_PIN(GPIO3, 17),    /* J2 GPIO_SD_B0_05 */
	__RT1052_PIN(GPIO3, 13),    /* J3 GPIO_SD_B0_01 */
	__RT1052_PIN(GPIO3, 12),    /* J4 GPIO_SD_B0_00 */
	__RT1052_PIN_DEFAULT,       /* J5  */
	__RT1052_PIN_DEFAULT,       /* J6  */
	__RT1052_PIN_DEFAULT,       /* J7  */
	__RT1052_PIN_DEFAULT,       /* J8  */
	__RT1052_PIN_DEFAULT,       /* J9  */
	__RT1052_PIN_DEFAULT,       /* J10  */
	__RT1052_PIN(GPIO1, 16),    /* J11 GPIO_AD_B1_00 */
	__RT1052_PIN(GPIO1, 22),    /* J12 GPIO_AD_B1_06 */
	__RT1052_PIN(GPIO1, 27),    /* J13 GPIO_AD_B1_11 */
	__RT1052_PIN(GPIO1, 31),    /* J14 GPIO_AD_B1_15 */
	

	__RT1052_PIN(GPIO3, 15),    /* K1 GPIO_SD_B0_03 */
	__RT1052_PIN_DEFAULT,       /* K2  */
	__RT1052_PIN_DEFAULT,       /* K3  */
	__RT1052_PIN_DEFAULT,       /* K4  */
	__RT1052_PIN_DEFAULT,       /* K5  */
	__RT1052_PIN_DEFAULT,       /* K6  */
	__RT1052_PIN(GPIO5,  1),    /* K7 PMIC_ON_REQ */
	__RT1052_PIN_DEFAULT,       /* K8  */
	__RT1052_PIN_DEFAULT,       /* K9  */
	__RT1052_PIN(GPIO1, 23),    /* K10 GPIO_AD_B1_07 */
	__RT1052_PIN(GPIO1, 17),    /* K11 GPIO_AD_B1_01 */
	__RT1052_PIN(GPIO1, 21),    /* K12 GPIO_AD_B1_05 */
	__RT1052_PIN_DEFAULT,       /* K13  */
	__RT1052_PIN(GPIO1, 12),    /* K14 GPIO_AD_B0_12 */
	

	__RT1052_PIN_DEFAULT,       /* L1  */
	__RT1052_PIN_DEFAULT,       /* L2  */
	__RT1052_PIN(GPIO3,  6),    /* L3 GPIO_SD_B1_06 */
	__RT1052_PIN(GPIO3,  7),    /* L4 GPIO_SD_B1_07 */
	__RT1052_PIN(GPIO3,  0),    /* L5 GPIO_SD_B1_00 */
	__RT1052_PIN(GPIO5,  0),    /* L6 WAKEUP */
	__RT1052_PIN(GPIO5,  2),    /* L7 PMIC_STBY_REQ */
	__RT1052_PIN_DEFAULT,       /* L8 */
	__RT1052_PIN_DEFAULT,       /* L9  */
	__RT1052_PIN(GPIO1, 15),    /* L10 GPIO_AD_B0_15 */
	__RT1052_PIN(GPIO1, 18),    /* L11 GPIO_AD_B1_02 */
	__RT1052_PIN(GPIO1, 20),    /* L12 GPIO_AD_B1_04 */
	__RT1052_PIN(GPIO1, 26),    /* L13 GPIO_AD_B1_10 */
	__RT1052_PIN(GPIO1, 13),    /* L14 GPIO_AD_B0_13 */
	

	__RT1052_PIN_DEFAULT,       /* M1  */
	__RT1052_PIN_DEFAULT,       /* M2  */
	__RT1052_PIN(GPIO3,  2),    /* M3 GPIO_SD_B1_02 */
	__RT1052_PIN(GPIO3,  3),    /* M4 GPIO_SD_B1_03 */
	__RT1052_PIN(GPIO3,  1),    /* M5 GPIO_SD_B1_01 */
	__RT1052_PIN_DEFAULT,       /* M6  */
	__RT1052_PIN_DEFAULT,       /* M7  */
	__RT1052_PIN_DEFAULT,       /* M8  */
	__RT1052_PIN_DEFAULT,       /* M9  */
	__RT1052_PIN_DEFAULT,       /* M10  */
	__RT1052_PIN(GPIO1,  2),    /* M11 GPIO_AD_B0_02 */
	__RT1052_PIN(GPIO1, 19),    /* M12 GPIO_AD_B1_03 */
	__RT1052_PIN(GPIO1, 25),    /* M13 GPIO_AD_B1_09 */
	__RT1052_PIN(GPIO1,  0),    /* M14 GPIO_AD_B0_00 */

    
    __RT1052_PIN_DEFAULT,       /* N1  */
	__RT1052_PIN_DEFAULT,       /* N2  */
	__RT1052_PIN(GPIO3,  5),    /* N3 GPIO_SD_B1_05 */
	__RT1052_PIN(GPIO3,  9),    /* N4 GPIO_SD_B1_09 */
	__RT1052_PIN_DEFAULT,       /* N5  */
	__RT1052_PIN_DEFAULT,       /* N6  */
	__RT1052_PIN_DEFAULT,       /* N7  */
	__RT1052_PIN_DEFAULT,       /* N8  */
	__RT1052_PIN_DEFAULT,       /* N9  */
	__RT1052_PIN_DEFAULT,       /* N10  */
	__RT1052_PIN_DEFAULT,       /* N11  */
	__RT1052_PIN_DEFAULT,       /* N12 */
	__RT1052_PIN_DEFAULT,       /* N13  */
	__RT1052_PIN_DEFAULT,       /* N14  */


    __RT1052_PIN_DEFAULT,       /* P1  */
	__RT1052_PIN(GPIO3,  4),    /* P2 GPIO_SD_B1_04 */
	__RT1052_PIN(GPIO3,  8),    /* P3 GPIO_SD_B1_08 */
	__RT1052_PIN(GPIO3, 10),    /* P4 GPIO_SD_B1_10 */
    __RT1052_PIN(GPIO3, 11),    /* P5 GPIO_SD_B1_11 */
	__RT1052_PIN_DEFAULT,       /* P6  */
	__RT1052_PIN_DEFAULT,       /* P7  */
	__RT1052_PIN_DEFAULT,       /* P8  */
	__RT1052_PIN_DEFAULT,       /* P9  */
	__RT1052_PIN_DEFAULT,       /* P10  */
	__RT1052_PIN_DEFAULT,       /* P11  */
	__RT1052_PIN_DEFAULT,       /* P12 */
	__RT1052_PIN_DEFAULT,       /* P13  */
	__RT1052_PIN_DEFAULT,       /* P14  */
      
};



/*!
  * @brief    GPIO初始化
  *
  * @param    pin      ： 管教标号
  * @param    mode     ： GPIO 模式   LQ_GPIO.h中的宏定义中选择
  * @param    output   ： 输出模式时，输出电平 1：高电平  0：低电平
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinInit(A7, PIN_MODE_OUTPUT, 1);//设置管脚A7为输出模式 高电平
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinInit(GPIO_Name_t pin, uint8_t mode, uint8_t output)
{
	gpio_pin_config_t gpio;
    uint32_t config_value = 0;
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + pin * 4, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_IomuxcSnvs);
        IOMUXC_SetPinMux(0x400A8000U + (pin - 125) * 4, 0x5U, 0, 0, 0, 1);
    }
    
    gpio.outputLogic = output;
    gpio.interruptMode = kGPIO_NoIntmode;
    
    switch (mode)
    {
      case PIN_MODE_OUTPUT:
        {
            config_value = 0x0030U;/* Drive Strength R0/6 */
            gpio.direction = kGPIO_DigitalOutput;
        }
        break;
        
      case PIN_MODE_INPUT:
        {
            config_value = 0x0830U;/* Open Drain Enable */
            gpio.direction = kGPIO_DigitalInput;
        }
        break;
        
      case PIN_MODE_INPUT_PULLDOWN:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.direction = kGPIO_DigitalInput;
        }
        break;
        
      case PIN_MODE_INPUT_PULLUP:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.direction = kGPIO_DigitalInput;
        }
        break;
        
      case PIN_MODE_OUTPUT_OD:
        {
            config_value = 0x0830U;/* Open Drain Enable */
            gpio.direction = kGPIO_DigitalOutput;
        }
        break;
    }
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + pin * 4, config_value);
    }
    else
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x400A8018U + (pin - 125) * 4, config_value);
    }
    
    GPIO_PinInit(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, &gpio);
}


/*!
  * @brief    读取GPIO状态
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO要初始化为输入模式
  *
  * @example  LQ_PinRead(A7); //获取A7状态
  *
  * @date     2019/8/19 星期一
  */
uint8_t LQ_PinRead(GPIO_Name_t pin)
{
	return GPIO_PinRead(rt_pin_map[pin].gpio, rt_pin_map[pin].pin);
}


/*!
  * @brief    设置GPIO输出状态
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinWrite(A7, 1);//A7置为高电平
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinWrite(GPIO_Name_t pin, uint8_t output)
{
	GPIO_PinWrite(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, output);
}


/*!
  * @brief    GPIO外部中断初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinExti(C13, PIN_IRQ_MODE_FALLING);//配置C13 下降沿触发中断
  * @example  优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
  * @example  NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31IO的中断  
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinExti(GPIO_Name_t pin, uint8_t mode)
{
	gpio_pin_config_t gpio;
    uint32_t config_value = 0; 
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc);
        IOMUXC_SetPinMux(0x401F8010U + pin * 4, 0x5U, 0, 0, 0, 1);
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_IomuxcSnvs);
        IOMUXC_SetPinMux(0x400A8000U + (pin - 125) * 4, 0x5U, 0, 0, 0, 1);
    }
    
    gpio.direction     = kGPIO_DigitalInput;
    gpio.outputLogic   = 0;
    
    switch (mode)
    {
      case PIN_IRQ_MODE_RISING:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntRisingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_FALLING:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_RISING_FALLING:
        {
            config_value = 0x0830U;/* Open Drain Enable */
            gpio.interruptMode = kGPIO_IntRisingOrFallingEdge;
        }
        break;
        
      case PIN_IRQ_MODE_HIGH_LEVEL:
        {
            config_value = 0x3030U;/* 100K Ohm Pull Down */
            gpio.interruptMode = kGPIO_IntHighLevel;
        }
        break;
        
      case PIN_IRQ_MODE_LOW_LEVEL:
        {
            config_value = 0xB030U;/* 100K Ohm Pull Up */
            gpio.interruptMode = kGPIO_IntLowLevel;
        }
        break;
    }
    
    if (rt_pin_map[pin].gpio != GPIO5)
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x401F8200U + pin * 4, config_value);
    }
    else
    {
        IOMUXC_SetPinConfig(0, 0, 0, 0, 0x400A8018U + (pin - 125) * 4, config_value);
    }

    GPIO_PinInit(rt_pin_map[pin].gpio, rt_pin_map[pin].pin, &gpio);
    GPIO_PortEnableInterrupts(rt_pin_map[pin].gpio, 1U << rt_pin_map[pin].pin);
    
}




