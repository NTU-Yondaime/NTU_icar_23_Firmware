#ifndef _LQ_QTMR_H_
#define _LQ_QTMR_H_

#include "LQ_GPIO_Cfg.h"
#include "fsl_qtmr.h"


/* ���ռ�ձ� �������޸� */
#define QTMR_DUTY_MAX  10000

//QTMR ���˹̶���ӳ��ܽţ� Ҳ����ͨ��XBARAӳ�䣬������Բο�ENC���ùܽ�
/**********************************  PWM(���Ÿ���) *******************************************************/
//     QTMRͨ��       �˿�        ��ѡ��Χ                   ����
#define QTMR1_CH0     D7          //D7         
#define QTMR1_CH1     E7          //E7
#define QTMR1_CH2     E8          //E8
#define QTMR1_CH3     A12         //A12

#define QTMR2_CH0     D8          //B4  D8                   B4���ⲿSDRAM
#define QTMR2_CH1     C8          //A3  C8                   A3���ⲿSDRAM
#define QTMR2_CH2     B8          //B8  C1                   C1���ⲿSDRAM
#define QTMR2_CH3     A13         //A13 F1                   F1���ⲿSDRAM

#define QTMR3_CH0     A8          //A8  B1  J11              B1���ⲿSDRAM
#define QTMR3_CH1     A9          //A5  A9  K11              A5���ⲿSDRAM
#define QTMR3_CH2     B9          //A4  B9  L11              A4���ⲿSDRAM
#define QTMR3_CH3     B13         //B2  B13 M12              B2���ⲿSDRAM

#define QTMR4_CH0     C9          //C9                       
#define QTMR4_CH1     D9          //D9
#define QTMR4_CH2     A10         //A10
#define QTMR4_CH3     C13         //C13





/**
  * @brief    QTMR PWM�����ʼ��
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    frequency�� PWM  Ƶ��
  * @param    duty     �� ��ʼռ�ձ�*QTMR_DUTY_MAX   ��Χ��1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     QTMR Ĭ��ʹ�� IPGʱ��Դ  150M   PWMƵ�ʲ�ҪС��18Hz
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PWMInit(TMR_Type *base, qtmr_channel_selection_t channel, uint32_t  frequency, uint16_t duty);

      
/**
  * @brief    QTMR ����PWM���ռ�ձ�
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    duty     �� ռ�ձ�*QTMR_DUTY_MAX   ��Χ��1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PWMSetDuty(TMR_Type *base, qtmr_channel_selection_t channel, uint16_t duty);


/**
  * @brief    ����QTMR ���PWM����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void Test_QTMR_PWM(void);



/**
  * @brief    ����QTMR �Ĵ������������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void Test_QTMR_ENC(void);




/**
  * @brief    QTMR ͨ���ܽų�ʼ��
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  *
  * @return   
  *
  * @note     �ڲ�����
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PinInit(TMR_Type *base, qtmr_channel_selection_t channel);




#endif  
