/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��13��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "fsl_gpt.h"
#include "LQ_GPT.h"

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
void GPT_TimeInitConfig(GPT_Type *base)
{
    gpt_config_t gptConfig;
    
    GPT_GetDefaultConfig(&gptConfig);
    
    GPT_Init(base, &gptConfig);          //��ʼ��GPT
    
    /* GPT �� PIT ʹ��ͬһʱ��ԴIGP��2��Ƶ Ĭ��75M */
    GPT_SetClockDivider(base, 75);       // GPT ʱ�� 75M / 75 = 1M
    
    GPT_SetOutputCompareValue(base, kGPT_OutputCompare_Channel1, 0XFFFFFFFF);   //GPT ��32Ϊ������ ����������ֵ 0xFFFF FFFF 
}



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
void GPT_TimeStart(GPT_Type *base)
{
    GPT_StopTimer(base);
    GPT_StartTimer(base); 
}



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
uint32_t GPT_TimeGetUs(GPT_Type *base)
{
    if(GPT_GetStatusFlags(base, kGPT_RollOverFlag))   //���
    {
#ifdef DEBUG
        PRINTF("\n GPT ��ʱ�������\n");
#endif
        return 0;
    }
        
    else
        return GPT_GetCurrentTimerCount(base) - 1;
}






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
void GPT_DelayInitConfig(GPT_Type *base)
{
    gpt_config_t gptConfig;
    
    GPT_GetDefaultConfig(&gptConfig);
    
    GPT_Init(base, &gptConfig);          //��ʼ��GPT
    
    /* GPT �� PIT ʹ��ͬһʱ��ԴIGP��2��Ƶ Ĭ��75M */
    GPT_SetClockDivider(base, 15);       // GPT ʱ�� 75M /  5 = 5M
    
    GPT_SetOutputCompareValue(base, kGPT_OutputCompare_Channel1, 0XFFFFFFFF);   //GPT ��32Ϊ������ ����������ֵ 0xFFFF FFFF 

}


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
void GPT_DelayMs(GPT_Type *base, uint32_t ms)
{
    GPT_DelayUs(base, ms * 1000);
}



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
void GPT_DelayUs(GPT_Type *base, uint32_t us)
{
    /* Start Timer */
    GPT_StartTimer(base);
    uint32_t time = us * 5 - 4;
    while(GPT_GetCurrentTimerCount(base) < time);
    
    GPT_StopTimer(base);
}

