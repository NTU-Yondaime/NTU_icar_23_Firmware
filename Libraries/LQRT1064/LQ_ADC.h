#ifndef __LQ_ADC_H__
#define __LQ_ADC_H__ 
/* //ADC资源序号与对应的管脚，      管脚名称      BGA管脚编号
ADC1_IN0  Analog channel 1 input0   GPIO_AD_B1_11   J13
ADC1_IN1  Analog channel 1 input1   GPIO_AD_B0_12   K14 
ADC1_IN2  Analog channel 1 input2   GPIO_AD_B0_13   L14
ADC1_IN3  Analog channel 1 input3   GPIO_AD_B0_14   H14
ADC1_IN4  Analog channel 1 input4   GPIO_AD_B0_15   L10
ADC1_IN5  Analog channel 1 input5   GPIO_AD_B1_00   J11
ADC1_IN6  Analog channel 1 input6   GPIO_AD_B1_01   K11
ADC1_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC1_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC1_IN9  Analog channel 1 input9   GPIO_AD_B1_04   L12
ADC1_IN10 Analog channel 1 input10  GPIO_AD_B1_05   K12
ADC1_IN11 Analog channel 1 input11  GPIO_AD_B1_06   J12
ADC1_IN12 Analog channel 1 input12  GPIO_AD_B1_07   K10
ADC1_IN13 Analog channel 1 input13  GPIO_AD_B1_08   H13
ADC1_IN14 Analog channel 1 input14  GPIO_AD_B1_09   M13
ADC1_IN15 Analog channel 1 input15  GPIO_AD_B1_10   L13

ADC2_IN0  Analog channel 2 input0   GPIO_AD_B1_11   J13
ADC2_IN1  Analog channel 2 input1   GPIO_AD_B1_12   H12
ADC2_IN2  Analog channel 2 input2   GPIO_AD_B1_13   H11
ADC2_IN3  Analog channel 2 input3   GPIO_AD_B1_14   G12
ADC2_IN4  Analog channel 2 input4   GPIO_AD_B1_15   J14
ADC2_IN5  Analog channel 1 input5   GPIO_AD_B1_00   J11
ADC2_IN6  Analog channel 1 input6   GPIO_AD_B1_01   K11
ADC2_IN7  Analog channel 1 input7   GPIO_AD_B1_02   L11
ADC2_IN8  Analog channel 1 input8   GPIO_AD_B1_03   M12
ADC2_IN9  Analog channel 1 input9   GPIO_AD_B1_04   L12
ADC2_IN10 Analog channel 1 input10  GPIO_AD_B1_05   K12
ADC2_IN11 Analog channel 1 input11  GPIO_AD_B1_06   J12
ADC2_IN12 Analog channel 1 input12  GPIO_AD_B1_07   K10
ADC2_IN13 Analog channel 1 input13  GPIO_AD_B1_08   H13
ADC2_IN14 Analog channel 1 input14  GPIO_AD_B1_09   M13
ADC2_IN15 Analog channel 1 input15  GPIO_AD_B1_10   L13
*/
typedef enum
{
  //参考手册p484 414.1.5 Modes of Operation  
  ADC1_CH0=0,//
  ADC1_CH1=1,//
  ADC1_CH2=2,//
  ADC1_CH3=3,//
  ADC1_CH4=4,//
  ADC1_CH5=5,//
  ADC1_CH6=6,//
  ADC1_CH7=7,//
  ADC1_CH8=8,//
  ADC1_CH9=9,//
  ADC1_CH10=10,//
  ADC1_CH11=11,//
  ADC1_CH12=12,//  
  ADC1_CH13=13,//
  ADC1_CH14=14,//
  ADC1_CH15=15,//
  
  ADC2_CH0=0,//
  ADC2_CH1=1,//
  ADC2_CH2=2,//
  ADC2_CH3=3,//
  ADC2_CH4=4,//
  ADC2_CH5=5,//
  ADC2_CH6=6,//
  ADC2_CH7=7,//
  ADC2_CH8=8,//
  ADC2_CH9=9,//
  ADC2_CH10=10,//
  ADC2_CH11=11,//
  ADC2_CH12=12,//
  ADC2_CH13=13,//
  ADC2_CH14=14,//
  ADC2_CH15=15,//
  
} ADCn_Ch_e;

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
void LQ_ADC_Init(ADC_Type *base, ADCn_Ch_e CH);


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
uint16_t ADC_Read(ADC_Type *base,ADCn_Ch_e CH);


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
void ADC_PinInit(ADC_Type *base, ADCn_Ch_e CH);



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
void Test_ADC(void);

#endif 
