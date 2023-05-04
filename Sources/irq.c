#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_lpuart.h"
#include "stdio.h"

#define pit_flag_get(pit_chn) PIT_GetStatusFlags(PIT, (pit_chnl_t)pit_chn)
#define pit_flag_clear(pit_chn) PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_chn, kPIT_TimerFlag)

void uart_rx_interrupt_handler(void);
void LPUART1_IRQHandler(void)
{
    uart_rx_interrupt_handler();
    __DSB();
    LPUART_ClearStatusFlags(LPUART1, kLPUART_RxOverrunFlag); // 数据同步屏蔽指令
}

extern void pit_handler(void);
void PIT_IRQHandler(void)
{
    if (pit_flag_get(kPIT_Chnl_0))
    {
        pit_flag_clear(kPIT_Chnl_0);
    }

    if (pit_flag_get(kPIT_Chnl_1))
    {
        pit_flag_clear(kPIT_Chnl_1);
        pit_handler();
    }

    if (pit_flag_get(kPIT_Chnl_2))
    {
        pit_flag_clear(kPIT_Chnl_2);
    }

    if (pit_flag_get(kPIT_Chnl_3))
    {
        pit_flag_clear(kPIT_Chnl_3);
    }

    __DSB();
}