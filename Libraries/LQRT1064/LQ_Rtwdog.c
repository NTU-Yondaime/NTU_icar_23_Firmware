/*-----------------------------------------------------------------
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��03��13��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��dev.env.��IAR8.30.1�����ϰ汾
-------------------------------------------------------------------*/
#include "LQ_Rtwdog.h"
#include "fsl_rtwdog.h"
#include "fsl_debug_console.h"
#include "LQ_SYSTICK.h"




/**
  * @brief    ���ڿ��Ź�
  *
  * @param    ofus  : ���Ź����������ʱ�� ms(������ֵ��������λ).��Χ:0~1985 ms
  *           winus : ���Ź�����������ֵ.  ms ��Χ:0~1985 ms,0��ʾ��ʹ�ô���ģʽ.
  * @return   
  *
  * @note     ι��Ҫ�� win - toval ֮��
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void MYRTWDOG_Init(uint16_t ofms,uint16_t winms)
{
    uint16_t overtime = (uint16_t)(ofms * 32.768);
    uint16_t wintime  = (uint16_t)(winms * 32.768);
    rtwdog_config_t rtwdog_config;	
    systime.delay_ms(100);
    RTWDOG_GetDefaultConfig(&rtwdog_config);		//��ȡĬ�ϲ���
    rtwdog_config.testMode=kRTWDOG_UserModeEnabled;	//�û�ģʽ
    rtwdog_config.clockSource=kRTWDOG_ClockSource1; //ʱ��Դѡ��  32.768KHZ
    rtwdog_config.prescaler=kRTWDOG_ClockPrescalerDivide1;				//����Ƶ
    rtwdog_config.timeoutValue=overtime;	        //��ʱֵ
    rtwdog_config.enableWindowMode =true;			//�������ڹ���
	rtwdog_config.enableInterrupt=false;		    //���ж�
    rtwdog_config.enableRtwdog=true;				//ʹ�ܿ��Ź�
    rtwdog_config.windowValue=wintime;              //����ֵ
    RTWDOG_Init(RTWDOG,&rtwdog_config);				//��ʼ�����Ź�
    
	//���ȼ����� ��ռ���ȼ�2  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(RTWDOG_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,2));
    NVIC_EnableIRQ(RTWDOG_IRQn);				    //ʹ��RTWDOG�ж�
}


/**
  * @brief    ���Դ��ڿ��Ź�
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void Test_Rtwdog(void)
{
    systime.init();
    PRINTF("\nRTWdog ���Ź�����\n");
    PRINTF("ι��ʱ��Ҫ��0 ms �� 50ms ֮��\n");
    PRINTF("����50ms ι�������ᵼ�¸�λ\n");
    MYRTWDOG_Init(100, 0);	            //��ʼ��RT���Ź�,100ms���,�Ǵ���ģʽ
    while(1)
    {
        systime.delay_ms(10);
        RTWDOG_Refresh(RTWDOG);         //ι��
        PRINTF("ι���ɹ���ϵͳ��������\n");
        systime.delay_ms(20);
        RTWDOG_Refresh(RTWDOG);         //ι��
        PRINTF("ι���ɹ���ϵͳ��������\n");
        systime.delay_ms(200);
        RTWDOG_Refresh(RTWDOG);         //ι��
        PRINTF("ι���ɹ���ϵͳ��������\n");
        while(1);
    }
    

}


