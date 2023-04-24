#ifndef _LQ_QTMR_H_
#define _LQ_QTMR_H_

#include "LQ_GPIO_Cfg.h"
#include "fsl_qtmr.h"


/* 最大占空比 可自行修改 */
#define QTMR_DUTY_MAX  10000

//QTMR 除了固定的映射管脚， 也可以通过XBARA映射，具体可以参考ENC配置管脚
/**********************************  PWM(引脚复用) *******************************************************/
//     QTMR通道       端口        可选范围                   建议
#define QTMR1_CH0     D7          //D7         
#define QTMR1_CH1     E7          //E7
#define QTMR1_CH2     E8          //E8
#define QTMR1_CH3     A12         //A12

#define QTMR2_CH0     D8          //B4  D8                   B4接外部SDRAM
#define QTMR2_CH1     C8          //A3  C8                   A3接外部SDRAM
#define QTMR2_CH2     B8          //B8  C1                   C1接外部SDRAM
#define QTMR2_CH3     A13         //A13 F1                   F1接外部SDRAM

#define QTMR3_CH0     A8          //A8  B1  J11              B1接外部SDRAM
#define QTMR3_CH1     A9          //A5  A9  K11              A5接外部SDRAM
#define QTMR3_CH2     B9          //A4  B9  L11              A4接外部SDRAM
#define QTMR3_CH3     B13         //B2  B13 M12              B2接外部SDRAM

#define QTMR4_CH0     C9          //C9                       
#define QTMR4_CH1     D9          //D9
#define QTMR4_CH2     A10         //A10
#define QTMR4_CH3     C13         //C13





/**
  * @brief    QTMR PWM输出初始化
  *
  * @param    base     ： TMR1  -  TMR4
  * @param    channel  ： kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    frequency： PWM  频率
  * @param    duty     ： 初始占空比*QTMR_DUTY_MAX   范围：1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     QTMR 默认使用 IPG时钟源  150M   PWM频率不要小于18Hz
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void QTMR_PWMInit(TMR_Type *base, qtmr_channel_selection_t channel, uint32_t  frequency, uint16_t duty);

      
/**
  * @brief    QTMR 设置PWM输出占空比
  *
  * @param    base     ： TMR1  -  TMR4
  * @param    channel  ： kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    duty     ： 占空比*QTMR_DUTY_MAX   范围：1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void QTMR_PWMSetDuty(TMR_Type *base, qtmr_channel_selection_t channel, uint16_t duty);


/**
  * @brief    测试QTMR 输出PWM功能
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void Test_QTMR_PWM(void);



/**
  * @brief    测试QTMR 的带方向计数功能
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void Test_QTMR_ENC(void);




/**
  * @brief    QTMR 通道管脚初始化
  *
  * @param    base     ： TMR1  -  TMR4
  * @param    channel  ： kQTMR_Channel_0  -  kQTMR_Channel_3
  *
  * @return   
  *
  * @note     内部调用
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void QTMR_PinInit(TMR_Type *base, qtmr_channel_selection_t channel);




#endif  
