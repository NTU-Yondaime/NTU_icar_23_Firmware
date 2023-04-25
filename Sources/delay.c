#include "delay.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpt��ʼ��
//  @param      void
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void system_delay_init(void)
{
    gpt_config_t gptConfig;

    system_init_flag = 1;
    GPT_GetDefaultConfig(&gptConfig); // ��ȡĬ������
    gptConfig.clockSource = kGPT_ClockSource_Osc;
    gptConfig.divider = 1;
    GPT_Init(SYSTEM_DELAY_GPT, &gptConfig); // GPT��ʼ�� ���ڴ�ʱ��
    GPT_Deinit(SYSTEM_DELAY_GPT);           // GPT����ʼ��
    GPT_Init(SYSTEM_DELAY_GPT, &gptConfig); // GPT��ʼ��
}

void system_delay(uint32 time, uint32 num)
{
    if (0 == system_init_flag)
    {
        system_delay_init();
    }

    while (num--)
    {
        GPT_SetOutputCompareValue(SYSTEM_DELAY_GPT, kGPT_OutputCompare_Channel1, time); // ���ö�ʱʱ��
        GPT_StartTimer(SYSTEM_DELAY_GPT);                                               // ������ʱ��
        while (!GPT_GetStatusFlags(SYSTEM_DELAY_GPT, kGPT_OutputCompare1Flag))
            ;
        GPT_ClearStatusFlags(SYSTEM_DELAY_GPT, kGPT_OutputCompare1Flag);
        GPT_StopTimer(SYSTEM_DELAY_GPT);
    }
}