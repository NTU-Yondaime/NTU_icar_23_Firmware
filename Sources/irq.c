/*-------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【注   意】 中断服务函数 不知道名字可以去startup_MIMXRT1064.s里面找
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
  * @brief    串口中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void uart_rx_interrupt_handler(void);
void LPUART1_IRQHandler(void)
{
	uart_rx_interrupt_handler();
	__DSB();			
  LPUART_ClearStatusFlags(LPUART1, kLPUART_RxOverrunFlag); 	//数据同步屏蔽指令
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