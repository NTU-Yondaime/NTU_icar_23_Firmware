#ifndef _delay_h_
#define _delay_h_
#include "fsl_gpt.h"
#include "typedef.h"
#define SYSTEM_DELAY_GPT GPT2
#define BOARD_XTAL_FREQ (24000000)

static uint8 system_init_flag;
void system_delay_init(void);
void system_delay(uint32 time, uint32 num);
#define system_delay_ms(time) system_delay(BOARD_XTAL_FREQ / 1000, time)            // 设置延时时间 单位 ms
#define system_delay_us(time) system_delay(BOARD_XTAL_FREQ / 1000 / 1000 * time, 1) // 设置延时时间 单位 us

#endif
