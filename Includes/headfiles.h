#include "config.h"

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
#include "LQ_SYSTICK.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "stdio.h"

/* 中断优先级组 */
#define NVIC_Group0 0x07
#define NVIC_Group1 0x06
#define NVIC_Group2 0x05
#define NVIC_Group3 0x04
#define NVIC_Group4 0x03
void BOARD_INIT(void)
{
    BOARD_ConfigMPU();        /* 初始化内存保护单元 */
    BOARD_BootClockRUN();     /* 初始化开发板时钟   */
    BOARD_InitPins();         /* 串口管脚初始化     */
    BOARD_InitDebugConsole(); /* 初始化串口         */
    /*设置中断优先级组  0: 0个抢占优先级16位个子优先级
     1: 2个抢占优先级 8个子优先级
     2: 4个抢占优先级 4个子优先级
     3: 8个抢占优先级 2个子优先级
     4: 16个抢占优先级 0个子优先级
     */
    /* 配置优先级组 2: 4个抢占优先级 4个子优先级 */
    NVIC_SetPriorityGrouping(NVIC_Group2);
    LQ_PinInit(H10, PIN_MODE_OUTPUT, 1);
    system_delay_ms(5);
}

/* 引脚配置
---------------------------------------------------------
LED

GPIO2_IO23  -------->  管脚B12   ----- >  核心板G
GPIO3_IO26  -------->  管脚A7    ----- >  核心板R
GPIO3_IO27  -------->  管脚C7    ----- >  核心板B

GPIO2_IO22  -------->  管脚C12   ----- >  母板D1
GPIO2_IO25  -------->  管脚A13   ----- >  母板D0
---------------------------------------------------------
按键

GPIO3_IO04 ------ 管脚P2   ----- >  母板按键K2
GPIO2_IO27 ------ 管脚C13  ----- >  母板按键K0
GPIO2_IO30 ------ 管脚C14  ----- >  母板按键k1
GPIO5_I000 ------ 管脚L6   ----- >  核心板按键WUP
---------------------------------------------------------
串口
LPUART1_TX ------ 管脚K14
LPUART1_RX ------ 管脚L14
--------------------------------------------------------*/