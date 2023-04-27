/* LED灯

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
#include "headfiles.h"

#define UART_INDEX (LPUART1)
#define UART_BAUDRATE (115200)

uint8_t uart_get_data[64];
uint8_t fifo_get_data[64];

uint8_t get_data = 0;
uint32_t fifo_data_count = 0;
uint8_t gpio_status;

fifo_struct uart_data_fifo;

#define MAX_PWM 8000 // PWM输出的最大值
#define MIN_PWM 2000 // PWM输出的最小值


double v_raw;
struct pid_coeffs vPID = {V_kP, V_kI, V_kD};
void main(void)
{
	int speed_target, angle_target = 10;
	double speed_c;
	
	// 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
	NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	LQ_UART_Init(LPUART1, 115200);
	EnableIRQ(LPUART1_IRQn); // 使能LPUART1中断
	// LQ_UART_Init(LPUART8, 115200);
	PIT_InitConfig(kPIT_Chnl_1, 20000); // 定时器中断
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmB, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B, 50); // M3 M4
	LQ_ENC_Init(ENC4, true);

	fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);

	while (1)
	{
		double v = v_raw / V_k_20ms;
		printf("%f\n\r", v);
		fifo_data_count = fifo_used(&uart_data_fifo);
		if (fifo_data_count != 0)
		{
			fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
			sscanf(fifo_get_data, "%d,%d", &speed_target, &angle_target);
			// printf("speed:%d,angle:%d,dir:%d\n\r", speed, angle, dir);
		}
		system_delay_ms(20);

		speed_c = incrPID(speed_target, v, &vPID);
		if (speed_c > MAX_PWM)
		{
			speed_c = MAX_PWM;
		}
		else if (speed_c < MIN_PWM)
		{
			speed_c = MIN_PWM;
		}
		if (speed_c > 0)
		{
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, speed_c);
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, DUTY_MAX);
		}
		else
		{
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, -speed_c);
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, DUTY_MAX);
		}
		LQ_SetServoDty(angle_target);
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

void pit_handler(void)
{
	// **_raw 从传感器得到的原始数据
	// **_f 完成滤波
	// **_p 估计值
	// **_k 系数
	// **_c 控制量
	v_raw = (int16_t)ENC_GetPositionDifferenceValue(ENC4);
}
