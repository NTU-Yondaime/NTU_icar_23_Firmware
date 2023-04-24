/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_debug_console.h"
#include "fsl_wdog.h"
#include "LQ_WDog.h"
#include "LQ_LED.h"

/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);


/**
  * @brief    ���Ź���ʼ��
  *
  * @param    base  �� WDOG1��WDOG2
  * @param    time  �� ʵ��ʱ��Ϊ(time+1)*0.5s
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void LQ_WDOG_Init(WDOG_Type *base, uint8_t time)
{
    wdog_config_t config;
    /*
    * wdogConfig->enableWdog = true;            //ʹ�ܿ��Ź�
    * wdogConfig->workMode.enableWait = true;   //�ȴ�ģʽ����Ȼ����
    * wdogConfig->workMode.enableStop = false;  //ֹͣģʽ�¹ر�
    * wdogConfig->workMode.enableDebug = false; //debugģʽ�ر�
    * wdogConfig->enableInterrupt = false;      //��ʹ���ж�
    * wdogConfig->enablePowerdown = false;      
    * wdogConfig->resetExtension = flase;
    * wdogConfig->timeoutValue = 0xFFU;         //����ι��ʱ��
    * wdogConfig->interruptTimeValue = 0x04u;   //��ι��ǰ �೤ʱ������ж� ����ι��
    */
    WDOG_GetDefaultConfig(&config);             //��ȡĬ�����ò���
    config.workMode.enableDebug = true;        //debugģʽ����
    config.timeoutValue = time;                 //���Ź���ʱʱ�䣬ʵ��ʱ��Ϊ(timeoutValue+1)*0.5s
    WDOG_Init(base, &config);
}


/**
  * @brief    ���Կ��Ź�
  *
  * @param    
  *
  * @return   
  *
  * @note     ���Ź��������Ƕ�ʱι������������ܷ�û��ι����ϵͳ��λ
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void Test_WDOG(void)
{
    LED_Init();
    delayms(3000);
    LED_Color(blue);
    LQ_WDOG_Init(WDOG1, 9);
    PRINTF("--- ���Ź����Կ�ʼ ---\r\n");
    
    for (uint32_t i = 0; i < 10; i++)
    {
        WDOG_Refresh(WDOG1);     //ι��
        PRINTF("--- �� %d ��ι�� ---\r\n", i + 1);
        delayms(1000 * i);
    }

    PRINTF("\r\n��ι�� ����λ\r\n");
    while (1)
    {
    }
}


