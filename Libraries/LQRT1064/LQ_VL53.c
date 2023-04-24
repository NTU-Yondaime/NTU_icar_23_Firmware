/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.3
【功    能】
【注意事项】   
-------------------------------------------------------------*/

#include "include.h"
#include "stdio.h"
#include "fsl_debug_console.h"
#include "LQ_VL53.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
#include "LQ_LED.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
/* 根据使用的接线方式，选择不同头文件 */
#if (VL53_USE_SELECT == USE_HAL_IIC)
#include "LQ_IIC.h"

#elif (VL53_USE_SELECT == USE_SOFT_IIC) 
#include "LQ_SOFTIIC.h"

#endif

/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53(void)
{
    LED_Init();
	PRINTF("\r\nLQ VL53 Test");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ VL53 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	
#if (VL53_USE_SELECT == USE_HAL_IIC)
    LQ_IIC_Init(LPI2C1, 400000);
#elif (VL53_USE_SELECT == USE_SOFT_IIC) 
    IIC_Init(); 
#endif       

    uint8_t VL53_STAR = 0x01;    //0x02 连续测量模式    0x01 单次测量模式
    uint8_t dis_buff[2];
    uint16_t dis;
    while(1)
    {
        /* 获取测量数据 */
		VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
        
        /* 开始一次测距 */
        VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);
            
        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        if(dis > 8000)
        {
            dis = 0;
        }
		sprintf(txt, "DIS %5d mm",dis);
#ifdef OLED
		LCD_P8x16Str(0,5,txt);
#else
		TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
		delayms(100);
		LED_ColorReverse(blue);
    
    }
    
}







/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     测试一路IIC接两个VL53  VL53模块的IIC地址可以用程序修改，但是上电的时候
  *           默认IIC地址都是0x29    所以修改地址的时候先失能一个模块（XS拉低），修改使能模块的
  *           IIC地址，修改完后，再将模块使能，两个模块的地址不同了，就可以按不同地址操作了。
  *
  * @example  
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53_TWO(void)
{

    /* 初始化 A12作为 一个VL53 的XS使能管脚 */
    CLOCK_EnableClock(kCLOCK_Iomuxc);           

    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0u);
    
    //输出口管脚配置结构体
    gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                            0,                   //低电平
                                            kGPIO_NoIntmode      //非中断模式
                                            };  
    
    GPIO_PinInit(GPIO2,24, &GPIO_Output_Config);                 //A12    
    
    LED_Init();
	PRINTF("\r\nLQ VL53 Test");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ VL53 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	
#if (VL53_USE_SELECT == USE_HAL_IIC)
    LQ_IIC_Init(LPI2C1, 400000);
#elif (VL53_USE_SELECT == USE_SOFT_IIC) 
    IIC_Init(); 
#endif    
    
    GPIO_PinWrite(GPIO2,24, 0U);   //失能A12连接的VL53 
    delayms(100);
    /* 修改IIC地址后，将使能的VL53地址改为 0x30 */
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, VL53NEWADDR);
    
    GPIO_PinWrite(GPIO2,24, 1U);   //使能A12连接的VL53 
    delayms(100);
    
    uint8_t VL53_STAR = 0x01;
    uint8_t dis_buff[2];
    uint16_t dis1, dis2;
    while(1)
    {
        
        
        /* 获取测量数据 */
		VL53_Read_nByte(VL53NEWADDR, VL53_REG_DIS, 2, dis_buff);
        
        /* 开始一次测距 */
        VL53_Write_Byte(VL53NEWADDR, VL53L0X_REG_SYSRANGE_START,  VL53_STAR);
        
        /* 转换数据 */
        dis1 = (dis_buff[0]<<8) | (dis_buff[1]);
        if(dis1 > 8000)
        {
            dis1 = 0;
        }
        
        /* 获取测量数据 */
		VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
        
        /* 开始一次测距 */
        VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);
        
        /* 转换数据 */
        dis2 = (dis_buff[0]<<8) | (dis_buff[1]);
        if(dis1 > 8000)
        {
            dis2 = 0;
        }
        
		sprintf(txt, "DIS %5d mm",dis1);
#ifdef OLED
		LCD_P8x16Str(0,3,txt);
#else
		TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
        sprintf(txt, "DIS %5d mm",dis2);
#ifdef OLED
		LCD_P8x16Str(0,5,txt);
#else
		TFTSPI_P8X16Str(2,7,txt,u16RED,u16BLUE);
#endif
        
		delayms(100);
		LED_ColorReverse(blue);
    
    }
    
}


#if (VL53_USE_SELECT == USE_HAL_IIC)

/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data)
{
    IIC_WriteReg(LPI2C1, dev, reg, data);    
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_ReadReg(LPI2C1, dev, reg, data, length);   
}


#elif (VL53_USE_SELECT == USE_SOFT_IIC) 

/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data)
{
    IIC_WriteByteToSlave(dev<<1, reg, data);  
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_ReadMultByteFromSlave(dev<<1, reg, length, data);    
}

#endif 
