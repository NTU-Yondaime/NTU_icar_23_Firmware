/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��03��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_debug_console.h"
#include "fsl_tempmon.h"
                      
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);

/**
  * @brief    ���Զ�ȡ�¶�
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/11 ���ڶ�
  */
void Test_Tempmon(void)
{
    float temperature = 0U; 
    tempmon_config_t config;
    

    PRINTF("оƬ�¶Ȳ�������. \r\n");
    

    /* Ĭ�ϵĲ���Ƶ�� 
    config->frequency = 0x03U;
     Ĭ�ϸ߱����¶�
    config->highAlarmTemp = 40U;
     Ĭ�Ͻ��������¶� 
    config->panicAlarmTemp = 90U;
     Ĭ�ϵͱ����¶� 
    config->lowAlarmTemp = 20U;*/
    TEMPMON_GetDefaultConfig(&config);

    config.frequency = 0x03U;
    config.highAlarmTemp = 40U;
    config.lowAlarmTemp = 20;
    config.panicAlarmTemp = 80U;
    
    TEMPMON_Init(TEMPMON, &config);
    TEMPMON_StartMeasure(TEMPMON);

    /* �õ��¶� */
    temperature = TEMPMON_GetCurrentTemperature(TEMPMON);

    PRINTF("оƬ��ǰ�¶ȣ� %.1f ��. \r\n", temperature);

    while (1)
    {
   
        delayms(10000);

        temperature = TEMPMON_GetCurrentTemperature(TEMPMON);
        PRINTF("оƬ��ǰ�¶ȣ� %.1f ��. \r\n", temperature);
    }
    

}




