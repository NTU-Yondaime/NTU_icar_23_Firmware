/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "fsl_adc.h"
#include "fsl_iomuxc.h"
#include "LQ_ADC.h"
#include "LQ_UART.h"
#include "LQ_KEY.h"
#include "LQ_LED.h"
#include "LQ_SGP18T.h"
#include "LQ_12864.h"
#include "stdio.h"

/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


/**
  * @brief    ADC模块初始化
  *
  * @param    base ： ADC1、ADC2
  * @param    CH   ： ADC通道号
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void LQ_ADC_Init(ADC_Type *base, ADCn_Ch_e CH)
{    
    ADC_PinInit(base, CH);
    
    adc_config_t adcConfigStrcut;   
    
    /*
    *  config->enableAsynchronousClockOutput = true;
    *  config->enableOverWrite =                 false;
    *  config->enableContinuousConversion =    false;
    *  config->enableHighSpeed =               false;
    *  config->enableLowPower =                false;
    *  config->enableLongSample =              false;
    *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
    *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
    *  config->clockSource =                   kADC_ClockSourceAD;
    *  config->clockDriver =                   kADC_ClockDriver1;
    *  config->resolution =                    kADC_Resolution12Bit;
    */
    ADC_GetDefaultConfig(&adcConfigStrcut);
    
    /* ADC分辨率 12位 */
    adcConfigStrcut.resolution=kADC_Resolution12Bit;  
    
    ADC_Init(base, &adcConfigStrcut);                              
    
    /* 硬件平均 */
    ADC_SetHardwareAverageConfig(base, kADC_HardwareAverageCount32);
    
    /* 硬件校准 */
    ADC_DoAutoCalibration(base);                                    
    
}



/**
  * @brief    ADC读取通道数值
  *
  * @param    base ： ADC1、ADC2
  * @param    CH   ： ADC通道号
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
uint16_t ADC_Read(ADC_Type *base,ADCn_Ch_e CH)
{     
    adc_channel_config_t adcChannelConfigStruct;                           
    
    /* 传输通道 */
    adcChannelConfigStruct.channelNumber = CH;
    
    /* 关转换完成中断 */
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;  
    
    /* 开始传输 */
    ADC_SetChannelConfig(base, 0, &adcChannelConfigStruct);               
    
    /* 等待转换完成 */
    while (0U == ADC_GetChannelStatusFlags(base,0)){;}                    
    
    /* 返回转换结果 */
    return (ADC_GetChannelConversionValue(base,0));                     
}


/**
  * @brief    显示ADC电压
  *
  * @param    
  *
  * @return   
  *
  * @note     L14 和 K14 这里用作ADC功能了，不要初始化为串口
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
#ifdef OLED
void Test_ADC(void)
{
    char txt[16];   
    LCD_Init();                  //LCD初始化
    LCD_CLS();                   //LCD清屏
    LED_Init();
    KEY_Init();               
    LQ_ADC_Init(ADC1, ADC1_CH1);   
    LQ_ADC_Init(ADC1, ADC1_CH2); 
    LQ_ADC_Init(ADC1, ADC1_CH3); 
    LQ_ADC_Init(ADC1, ADC1_CH4); 
    LQ_ADC_Init(ADC1, ADC1_CH7); 
    LQ_ADC_Init(ADC1, ADC1_CH8); 
    LQ_ADC_Init(ADC1, ADC1_CH10); 
    
    LQ_UART_Init(LPUART8, 115200); 
    
    LCD_P8x16Str(4,0,"LQ ADC Test"); 
    UART_PutStr(LPUART8, (uint8_t *)"\r\nLQ ADC Test");
    
    uint16_t batv0=(uint16_t)(ADC_Read(ADC1,ADC1_CH10)*4.6);   //获取ADC值 K12 电池电压 * 100
    uint16_t batv1=(uint16_t)(ADC_Read(ADC1,ADC1_CH4)*0.806);  //获取ADC值 L10
    uint16_t batv2=(uint16_t)(ADC_Read(ADC1,ADC1_CH7)*0.806);  //获取ADC值 L11
    uint16_t batv3=(uint16_t)(ADC_Read(ADC1,ADC1_CH8)*0.806);  //获取ADC值 M12
    uint16_t batv4=(uint16_t)(ADC_Read(ADC1,ADC1_CH3)*0.806);  //获取ADC值 H14
    uint16_t batv5=(uint16_t)(ADC_Read(ADC1,ADC1_CH1)*0.806);  //获取ADC值 K14
    uint16_t batv6=(uint16_t)(ADC_Read(ADC1,ADC1_CH2)*0.806);  //获取ADC值 L14
    uint8_t key = 0;
    while (1)
    {           
        //测试ADC转换   ADC的基准电压是3.3V的，RT1052母板上，K12通过1K和4.7K的分压电阻测量电池电压 电池电压 =  （ADC_Read / 4095） * 3.3 * （1 + 4.7）
        batv0 = (uint16_t)(batv0*0.7 + 0.3*(uint16_t)(ADC_Read(ADC1,ADC1_CH10)*4.6));//电压采集 
        
        batv1 = (uint16_t)(batv1*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH4)*0.806);             //获取ADC值 L10
        batv2 = (uint16_t)(batv2*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH7)*0.806);             //获取ADC值 L11
        batv3 = (uint16_t)(batv3*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH8)*0.806);             //获取ADC值 M12
        batv4 = (uint16_t)(batv4*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH3)*0.806);             //获取ADC值 H14
        batv5 = (uint16_t)(batv5*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH1)*0.806);             //获取ADC值 K14
        batv6 = (uint16_t)(batv6*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH2)*0.806);             //获取ADC值 L14
        
        switch(KEY_Read(1))     //按键按下翻页
        {
            case 1:
                LCD_CLS();          //LCD清屏
                key = 0;            //K0 按下 第一页
                break;           
            case 2: 
                LCD_CLS();          //LCD清屏
                key = 1;            //K1 按下 第二页
                break;
            default:
                
                break;
        }
        UART_PutStr(LPUART8,(uint8_t *)"\r\n-----------------------------\n");
        
        if(0 == key)
        {
            LCD_P8x16Str(4,0,"LQ ADC Test Bat"); 
            
            sprintf(txt,"BAT:%3.2fV ",batv0/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,2,txt);
            
            sprintf(txt,"L10:%3.2fV ",batv1/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,4,txt);
            
            sprintf(txt,"L11:%3.2fV ",batv2/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,6,txt);
            
        }
        if(1 == key)
        {
            sprintf(txt,"M12:%3.2fV ",batv3/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,0,txt);
            
            sprintf(txt,"H14:%3.2fV ",batv4/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,2,txt);
            
            sprintf(txt,"K14:%3.2fV ",batv5/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,4,txt);
            
            sprintf(txt,"L14:%3.2fV ",batv6/1000.0f);
            UART_PutStr(LPUART8, (uint8_t *)txt);
            LCD_P8x16Str(20,6,txt);    
        }
   
        LED_ColorReverse(blue);    //蓝灯   
        delayms(100);
    }
}


#else



void Test_ADC(void)
{
    char txt[16];
      
    TFTSPI_Init(1);                 //LCD初始化  0:横屏  1：竖屏
    TFTSPI_CLS(u16BLUE);            //清屏
    LED_Init();
    KEY_Init();          
    
    LQ_ADC_Init(ADC1, ADC1_CH1);   
    LQ_ADC_Init(ADC1, ADC1_CH2); 
    LQ_ADC_Init(ADC1, ADC1_CH3); 
    LQ_ADC_Init(ADC1, ADC1_CH4); 
    LQ_ADC_Init(ADC1, ADC1_CH7); 
    LQ_ADC_Init(ADC1, ADC1_CH8); 
    LQ_ADC_Init(ADC1, ADC1_CH10); 
    
    LQ_UART_Init(LPUART8, 115200);
    TFTSPI_P8X16Str(2,0,"LQ ADC Test",u16RED,u16BLUE);
    UART_PutStr(LPUART8,(uint8_t *)"\r\nLQ ADC Test");

    uint16_t batv0=(uint16_t)(ADC_Read(ADC1,ADC1_CH10)*4.6);   //获取ADC值 K12 电池电压 * 1000
    uint16_t batv1=(uint16_t)(ADC_Read(ADC1,ADC1_CH4)*0.806);  //获取ADC值 L10
    uint16_t batv2=(uint16_t)(ADC_Read(ADC1,ADC1_CH7)*0.806);  //获取ADC值 L11
    uint16_t batv3=(uint16_t)(ADC_Read(ADC1,ADC1_CH8)*0.806);  //获取ADC值 M12
    uint16_t batv4=(uint16_t)(ADC_Read(ADC1,ADC1_CH3)*0.806);  //获取ADC值 H14
    uint16_t batv5=(uint16_t)(ADC_Read(ADC1,ADC1_CH1)*0.806);  //获取ADC值 K14
    uint16_t batv6=(uint16_t)(ADC_Read(ADC1,ADC1_CH2)*0.806);  //获取ADC值 L14
    while (1)
    {           
        //测试ADC转换   ADC的基准电压是3.3V的，RT1052母板上，K12通过1K和4.7K的分压电阻测量电池电压 电池电压 =  （ADC_Read / 4095） * 3.3 * （1 + 4.7）
        batv0 = (uint16_t)(batv0*0.7 + 0.3*(uint16_t)(ADC_Read(ADC1,ADC1_CH10)*4.6));  //电压采集  mv
        
        batv1 = (uint16_t)(batv1*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH4)*0.806);             //获取ADC值 L10
        batv2 = (uint16_t)(batv2*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH7)*0.806);             //获取ADC值 L11
        batv3 = (uint16_t)(batv3*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH8)*0.806);             //获取ADC值 M12
        batv4 = (uint16_t)(batv4*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH3)*0.806);             //获取ADC值 H14
        batv5 = (uint16_t)(batv5*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH1)*0.806);             //获取ADC值 K14
        batv6 = (uint16_t)(batv6*0.7 + 0.3*ADC_Read(ADC1,ADC1_CH2)*0.806);             //获取ADC值 L14
        
        UART_PutStr(LPUART8,(uint8_t *)"\r\n-----------------------------\n");
        sprintf(txt,"BAT:%3.2fV ",batv0/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,1,txt,u16RED,u16BLUE);
                                  
        sprintf(txt,"L10:%3.2fV ",batv1/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,2,txt,u16RED,u16BLUE);
        
        sprintf(txt,"L11:%3.2fV ",batv2/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,3,txt,u16RED,u16BLUE);
        
        sprintf(txt,"M12:%3.2fV ",batv3/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,4,txt,u16RED,u16BLUE);
        
        sprintf(txt,"H14:%3.2fV ",batv4/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,5,txt,u16RED,u16BLUE);
        
        sprintf(txt,"K14:%3.2fV ",batv5/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,6,txt,u16RED,u16BLUE);
        
        sprintf(txt,"L14:%3.2fV ",batv6/1000.0f);
        UART_PutStr(LPUART8, (uint8_t *)txt);
        TFTSPI_P8X16Str(3,7,txt,u16RED,u16BLUE);
        
        LED_ColorReverse(blue);    //蓝灯   
        delayms(100);
    }
}
#endif



/**
  * @brief    ADC 管脚初始化
  *
  * @param    base ： ADC1、ADC2
  * @param    CH   ： ADC通道号  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/7/22 星期一
  */
void ADC_PinInit(ADC_Type *base, ADCn_Ch_e CH)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
    
    if(base == ADC1)
    {
        switch(CH)
        {
          case 0:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27,0U);
            break;
          case 1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_GPIO1_IO12,0U);
            break;
          case 2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13,0U);
            break;
          case 3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14,0U);
            break;
          case 4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15,0U);
            break;
          case 5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0U);
            break;
          case 6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0U);
            break;
          case 7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18,0U);
            break;
          case 8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_GPIO1_IO19,0U);
            break;
          case 9:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20,0U);
            break;
          case 10:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21,0U);
            break;
          case 11:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0U);
            break;
          case 12:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0U);
            break;
          case 13:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,0U);
            break;
          case 14:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0U);
            break;
          case 15:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0U);
            break;
        }
    }
    else
    {
        switch(CH)
        {
          case 0:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27,0U);
            break;
          case 1:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28,0U);
            break;
          case 2:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29,0U);
            break;
          case 3:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,0U);
            break;
          case 4:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31,0U);
            break;
          case 5:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0U);
            break;
          case 6:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0U);
            break;
          case 7:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18,0U);
            break;
          case 8:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_GPIO1_IO19,0U);
            break;
          case 9:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20,0U);
            break;
          case 10:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21,0U);
            break;
          case 11:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0U);
            break;
          case 12:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0U);
            break;
          case 13:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,0U);
            break;
          case 14:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0U);
            break;
          case 15:
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0U);
            break;
        }
    }

}


