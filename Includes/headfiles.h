#include "algorithms.h"
#include "delay.h"
#include "fifo.h"
#include "coeffs.h"

#include "LQ_UART.h"
#include "LQ_PIT.h"
#include "LQ_UART.h"
#include "LQ_PWM.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_Cfg.h"
#include "LQ_Encoder.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "stdio.h"

/* �ж����ȼ��� */
#define NVIC_Group0 0x07
#define NVIC_Group1 0x06
#define NVIC_Group2 0x05
#define NVIC_Group3 0x04
#define NVIC_Group4 0x03
void BOARD_INIT(void)
{
    BOARD_ConfigMPU();        /* ��ʼ���ڴ汣����Ԫ */
    BOARD_BootClockRUN();     /* ��ʼ��������ʱ��   */
    BOARD_InitPins();         /* ���ڹܽų�ʼ��     */
    BOARD_InitDebugConsole(); /* ��ʼ������         */
    /*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ�
     1: 2����ռ���ȼ� 8�������ȼ�
     2: 4����ռ���ȼ� 4�������ȼ�
     3: 8����ռ���ȼ� 2�������ȼ�
     4: 16����ռ���ȼ� 0�������ȼ�
     */
    /* �������ȼ��� 2: 4����ռ���ȼ� 4�������ȼ� */
    NVIC_SetPriorityGrouping(NVIC_Group2);
    LQ_PinInit(H10, PIN_MODE_OUTPUT, 1);
    system_delay_ms(5);
}