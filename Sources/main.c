#include "headfiles.h"
// 全局数据组
// 串口
uint8_t uart_get_data[64];
uint8_t fifo_get_data[64];
uint8_t get_data = 0;
uint32_t fifo_data_count = 0;
fifo_struct uart_data_fifo;
// 传感器
double v_raw;
// 算法
pid_coeffs_struct vPID = {V_kP, V_kI, V_kD};
// 板载设置初始化
void BoardSettingsInit(void)
{
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
	LQ_PinInit(H10, PIN_MODE_OUTPUT, 1);
}

// 系统设置初始化
void SystemSettingsInit(void)
{
	systime.init();
	NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
	LQ_UART_Init(LPUART1, 115200);
	EnableIRQ(LPUART1_IRQn); // 使能LPUART1
	// LQ_UART_Init(LPUART8, 115200);
	PIT_InitConfig(kPIT_Chnl_1, 20000); // 定时器中断
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmB, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA, 1000);
	LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B, 50); // M3 M4
	LQ_ENC_Init(ENC4, true);
	fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);
}

int main(void)
{
	BoardSettingsInit();
	SystemSettingsInit();
	systime.init();
	int v_target, angle_target = 750;
	double v_ctrl;
	while (1)
	{
		fifo_data_count = fifo_used(&uart_data_fifo);
		if (fifo_data_count != 0)
		{
			fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);
			sscanf(fifo_get_data, "%d,%d", &v_target, &angle_target);
		}
		v_ctrl = incrPID(v_target, v_raw / V_k_20ms, &vPID);
		if (v_ctrl > MAX_PWM || -v_ctrl > MAX_PWM)
		{
			v_ctrl = MAX_PWM;
		}
		else if ((v_ctrl > 0 && v_ctrl < MIN_PWM) || (v_ctrl < 0 && -v_ctrl < MIN_PWM))
		{
			v_ctrl = MIN_PWM;
		}
		if (v_ctrl > 0)
		{
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, v_ctrl);
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, DUTY_MAX);
		}
		else
		{
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmA, -v_ctrl);
			LQ_PWM_SetDuty(PWM2, kPWM_Module_1, kPWM_PwmB, DUTY_MAX);
		}
		LQ_SetServoDty(angle_target);
		systime.delay_ms(20);
	}
}

void uart_rx_interrupt_handler(void)
{
	// uart_query_byte(&get_data);
	if ((LPUART1->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT)
	{
		// *dat = LPUART_ReadByte(LPUART1);
		get_data = LPUART_ReadByte(LPUART1);
	}
	fifo_write_buffer(&uart_data_fifo, &get_data, 1);
}

void pit_handler(void)
{
	// **_raw 传感器原始数据 Sensor original data
	// **_f 完成滤波 filtered data
	// **_p 预测值 prediction
	// **_k 系数 coefficients
	// **_ctrl 控制量（输入量） inputs
	v_raw = (int16_t)ENC_GetPositionDifferenceValue(ENC4);
	printf("%f\n\r", v_raw / V_k_20ms);
}
