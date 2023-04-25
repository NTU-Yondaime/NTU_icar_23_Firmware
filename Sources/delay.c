#include "delay.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpt初始化
//  @param      void
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void system_delay_init(void)
{
    gpt_config_t gptConfig;

    system_init_flag = 1;
    GPT_GetDefaultConfig(&gptConfig); // 获取默认配置
    gptConfig.clockSource = kGPT_ClockSource_Osc;
    gptConfig.divider = 1;
    GPT_Init(SYSTEM_DELAY_GPT, &gptConfig); // GPT初始化 便于打开时钟
    GPT_Deinit(SYSTEM_DELAY_GPT);           // GPT反初始化
    GPT_Init(SYSTEM_DELAY_GPT, &gptConfig); // GPT初始化
}

void system_delay(uint32 time, uint32 num)
{
    if (0 == system_init_flag)
    {
        system_delay_init();
    }

    while (num--)
    {
        GPT_SetOutputCompareValue(SYSTEM_DELAY_GPT, kGPT_OutputCompare_Channel1, time); // 设置定时时间
        GPT_StartTimer(SYSTEM_DELAY_GPT);                                               // 启动定时器
        while (!GPT_GetStatusFlags(SYSTEM_DELAY_GPT, kGPT_OutputCompare1Flag))
            ;
        GPT_ClearStatusFlags(SYSTEM_DELAY_GPT, kGPT_OutputCompare1Flag);
        GPT_StopTimer(SYSTEM_DELAY_GPT);
    }
}