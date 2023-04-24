/*---------------------------------------------
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
---------------------------------------------------------
LED灯

GPIO2_IO23  -------->  管脚B12   ----- >  核心板G灯
GPIO3_IO26  -------->  管脚A7    ----- >  核心板R灯
GPIO3_IO27  -------->  管脚C7    ----- >  核心板B灯

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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "LQ_UART.h"
#include "LQ_PWM.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_Cfg.h"
#include "LQ_Encoder.h"

#include "fifo.h"
#include "stdio.h"
/* 中断优先级组 */
#define NVIC_Group0 0x07
#define NVIC_Group1 0x06
#define NVIC_Group2 0x05
#define NVIC_Group3 0x04
#define NVIC_Group4 0x03

#define UART_INDEX (LPUART1)   // ?? UART_1
#define UART_BAUDRATE (115200) // ?? 115200

uint8_t uart_get_data[64]; // ?????????
uint8_t fifo_get_data[64]; // fifo ???????

uint8_t get_data = 0;		  // ??????
uint32_t fifo_data_count = 0; // fifo ????

fifo_struct uart_data_fifo;

uint8_t gpio_status;
/**
 * @brief    不精确延时
 *
 * @param    无
 *
 * @return   无
 *
 * @note     无
 *
 * @example  delayms(100);
 *
 * @date     2019/4/22 星期一
 */
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while (ms--)
	{
		for (i = 0; i < 20000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
}

/*!
 * @brief    打开外设电源
 *
 * @param
 *
 * @return
 *
 * @note     IMXRT系列单片机上电时管脚不能为高电平，如果外设模块先上电，
 * @note     单片机与外设相连的管脚就有可能被外设模块拉高导致启动失败
 * @note     因此使用一个管脚作为外设模块电源开关，当单片机启动后，将C10置为高电平，打开外设电源
 *
 * @see
 *
 * @date     2019/9/12 星期四
 */
void POWER_ENABLE(void)
{
	LQ_PinInit(H10, PIN_MODE_OUTPUT, 1);
	delayms(200);
}

/*!
 * @brief Main function
 */
int main(void)
{
	int speed, angle=10;
	short velocity;
	BOARD_ConfigMPU();		  /* 初始化内存保护单元 */
	BOARD_BootClockRUN();	  /* 初始化开发板时钟   */
	BOARD_InitPins();		  /* 串口管脚初始化     */
	BOARD_InitDebugConsole(); /* 初始化串口         */
	/*设置中断优先级组  0: 0个抢占优先级16位个子优先级
	 1: 2个抢占优先级 8个子优先级
	 2: 4个抢占优先级 4个子优先级
	 3: 8个抢占优先级 2个子优先级
	 4: 16个抢占优先级 0个子优先级
	 */
	/* 配置优先级组 2: 4个抢占优先级 4个子优先级 */
	NVIC_SetPriorityGrouping(NVIC_Group2);
	POWER_ENABLE();

	// 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
	NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	LQ_UART_Init(LPUART1, 115200);
	EnableIRQ(LPUART1_IRQn); // 使能LPUART1中断
	// LQ_UART_Init(LPUART8, 115200);
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmB, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B, 50); // M3 M4
	LQ_ENC_Init(ENC4, true);

	fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);
	while (1)
	{
		velocity = (int16_t)ENC_GetPositionDifferenceValue(ENC4);
		printf("%d\n\r", velocity);
		fifo_data_count = fifo_used(&uart_data_fifo);
		if (fifo_data_count != 0)
		{
			fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
			sscanf(fifo_get_data, "%d,%d", &speed, &angle);
			// printf("speed:%d,angle:%d,dir:%d\n\r", speed, angle, dir);
			if (speed>0)
			{
				LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, speed);
				LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, DUTY_MAX);
			}
			else
			{
				LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, -speed);
				LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, DUTY_MAX);
			}
			LQ_SetServoDty(angle);
		}
		delayms(20);
	}
}

uint8_t uart_query_byte(uint8_t *dat)
{
	uint8_t return_state = 0;

	if ((LPUART1->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT)
	{
		*dat = LPUART_ReadByte(LPUART1);
		return_state = 1;
	}
	return return_state;
}

void uart_rx_interrupt_handler(void)
{
	//    get_data = uart_read_byte(UART_INDEX);
	uart_query_byte(&get_data);
	fifo_write_buffer(&uart_data_fifo, &get_data, 1);
}
