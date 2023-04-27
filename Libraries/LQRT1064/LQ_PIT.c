/*-----------------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��dev.env.��IAR8.30.1�����ϰ汾
--------------------------------------------------------------------*/
#include "fsl_pit.h"
#include "LQ_GPIO_Cfg.h"
#include "LQ_PIT.h"

/**
  * @brief    PIT��ʱ����ʼ��
  *
  * @param    channel�� PITͨ�� kPIT_Chnl_0 - kPIT_Chnl_3
  * @param    count  �� ��ʱ��ʱ�� us
  *
  * @return   
  *
  * @note     PIT��GPTʹ�� ͬһʱ��Դ Ĭ��IPG/2ʱ�� 75M
  *
  * @example  PIT_InitConfig(kPIT_Chnl_0, 5000);  //����ͨ��0��ʱ���жϣ�ÿ5000us�ж�һ��
  *
  * @date     2019/6/4 ���ڶ�
  */
void PIT_InitConfig(pit_chnl_t channel, uint32_t count)
{
    static uint8_t pit_count = 0;
    
    /* ��ֹPIT��γ�ʼ�� */
    if(pit_count == 0)
    {
        pit_count++;
        
        /* PIT��ʼ���ṹ�� */
        pit_config_t pitConfig;                
        
        /*
        * pitConfig.enableRunInDebug = false;  
        */
        PIT_GetDefaultConfig(&pitConfig);   //��ȡĬ�ϲ���
        
        pitConfig.enableRunInDebug = false;
        PIT_Init(PIT, &pitConfig);          //��ʼ��PIT
        
        //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
        NVIC_SetPriority(PIT_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
        
        /* Enable at the NVIC */
        EnableIRQ(PIT_IRQn);                //ʹ���ж�
        
    }
 
    /* ����PITͨ���Ͷ�ʱʱ�� PITĬ��ʹ��IPGʱ�ӵĶ���Ƶ 75Mʱ�� */
    PIT_SetTimerPeriod(PIT, channel, USEC_TO_COUNT(count, CLOCK_GetFreq(kCLOCK_IpgClk)/2)); 
    
    //����жϱ�־λ
    PIT_ClearStatusFlags(PIT, channel, kPIT_TimerFlag); 
    
    //ʹ��ͨ���ж�
    PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);  

    //������ʱ��
    PIT_StartTimer(PIT, channel);   
    
    if(channel == kPIT_Chnl_1)
    {
        PIT_StopTimer(PIT, channel);   
            
        PIT->CHANNEL[1].TCTRL = 0x03;
        
        /* ����PITͨ���Ͷ�ʱʱ�� PITĬ��ʹ��IPGʱ�ӵĶ���Ƶ 75Mʱ�� */
        PIT_SetTimerPeriod(PIT, channel, USEC_TO_COUNT(count, CLOCK_GetFreq(kCLOCK_IpgClk)/2)); 
        
        //����жϱ�־λ
        PIT_ClearStatusFlags(PIT, channel, kPIT_TimerFlag); 
        
        //ʹ��ͨ���ж�
        PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);  
        
        //������ʱ��
        PIT_StartTimer(PIT, channel); 
    }
}

