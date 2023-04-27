/*-------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��ע   �⡿ �жϷ����� ��֪�����ֿ���ȥstartup_MIMXRT1064.s������
---------------------------------------------------------*/
#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_lpuart.h"
#include "stdio.h"





#define pit_flag_get(pit_chn)       PIT_GetStatusFlags(PIT, (pit_chnl_t)pit_chn)
#define pit_flag_clear(pit_chn)     PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_chn, kPIT_TimerFlag)

/**
  * @brief    �����жϷ�����
  *
  * @param    
  *
  * @return   
  *
  * @note     ���Դ����ж�����ʹ�øú���
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void uart_rx_interrupt_handler(void);
void LPUART1_IRQHandler(void)
{
	uart_rx_interrupt_handler();
	__DSB();			
  LPUART_ClearStatusFlags(LPUART1, kLPUART_RxOverrunFlag); 	//����ͬ������ָ��
}

extern void pit_handler (void);
void PIT_IRQHandler(void)
{
    if(pit_flag_get(kPIT_Chnl_0))
    {
        pit_flag_clear(kPIT_Chnl_0);
        
    }
    
    if(pit_flag_get(kPIT_Chnl_1))
    {
        pit_flag_clear(kPIT_Chnl_1);
        pit_handler();
    }
    
    if(pit_flag_get(kPIT_Chnl_2))
    {
        pit_flag_clear(kPIT_Chnl_2);
    }
    
    if(pit_flag_get(kPIT_Chnl_3))
    {
        pit_flag_clear(kPIT_Chnl_3);
    }

    __DSB();
}