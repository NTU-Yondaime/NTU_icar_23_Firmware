/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPT_H_
#define _LQ_GPT_H_

#include "fsl_gpt.h"
/**
  * @brief    GPT ͨ�ö�ʱ����ʼ��Ϊ�����ʱ��
  *
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     ʹ��GPT�����ʱ��ǰ���ȵ��øú������г�ʼ��
  *
  * @example  GPT_TimeInitConfig(GPT1);  //��ʼ��GPT1Ϊ�����ʱ��
  *
  * @date     2019/6/10 ����һ
  */
void GPT_TimeInitConfig(GPT_Type *base);


/**
  * @brief    gpt ��ʱ����ʼ��ʱ
  *
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 1 000000 = 4290s 
  *
  * @example  GPT_TimeStart(GPT1);  //��ʼ����
  *
  * @date     2019/6/10 ����һ
  */
void GPT_TimeStart(GPT_Type *base);


/**
  * @brief    gpt ��ʱ����ʼ��ʱ
  *
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 1 000000 = 4290s 
  *
  * @example  GPT_TimeGetUs(GPT1); //��ȡ��GPT_TimeStart(GPT1)��ʼ�����ڵ�us��
  *
  * @date     2019/6/10 ����һ
  */
uint32_t GPT_TimeGetUs(GPT_Type *base);


/**
  * @brief    GPT ͨ�ö�ʱ����ʼ��Ϊ��ʱ��ʱ��
  *
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     ʹ��GPT��ʱǰ���ȵ��øú������г�ʼ��
  *
  * @example  GPT_DelayInitConfig(GPT1);  //��ʼ��
  *
  * @date     2019/6/10 ����һ
  */
void GPT_DelayInitConfig(GPT_Type *base);


/**
  * @brief    gpt ��ʱ����ȷ��ʱ
  *
  * @param    ms �� ��ʱʱ�� ms
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 5 000000 = 868s
  *
  * @example  GPT_DelayMs(GPT1, 1000); //��ʱ1s
  *
  * @date     2019/6/10 ����һ
  */
void GPT_DelayMs(GPT_Type *base, uint32_t ms);


/**
  * @brief    gpt ��ʱ����ȷ��ʱ
  *
  * @param    us �� ��ʱʱ�� us
  * @param    base�� GPT1��GPT2
  *
  * @return   ��
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 5 000000 = 868s
  *
  * @example  GPT_DelayUs(GPT1, 1000); //��ʱ1ms
  *
  * @date     2019/6/10 ����һ
  */
void GPT_DelayUs(GPT_Type *base, uint32_t us);






#endif  
