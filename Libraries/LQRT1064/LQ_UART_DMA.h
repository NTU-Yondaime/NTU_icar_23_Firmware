/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年10月18日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
================================================
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_UART_DMA_H
#define __LQ_UART_DMA_H

#include "fsl_lpuart_edma.h"
/* 串口+DMA 标志位 */
extern volatile bool rxBufferEmpty;
extern volatile bool txBufferFull;
extern volatile bool txOnGoing;
extern volatile bool rxOnGoing;

/* 接收到的帧长度 */
extern volatile uint32_t g_rxLen;

/* 串口接收 缓冲区 */
extern uint8_t g_rxBuffer[100];
extern uint8_t g_txBuffer[100];

/* DMA 传输结构体 */
extern lpuart_transfer_t sendXfer;
extern lpuart_transfer_t receiveXfer;

/* 串口 + DMA 配置结构体 */
extern lpuart_edma_handle_t g_lpuartEdmaHandle;


/**
  * @brief    串口DMA传输初始化
  *
  * @param    base  ： LPUART1 - LPUART8
  * @param    bound ： 串口波特率
  * @param    TX_channel  ： DMA TX通道号
  * @param    RX_channel  ： DMA RX通道号
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void LQ_UART_DMA_Init(LPUART_Type *base, uint32_t bound, uint8_t TX_channel, uint8_t RX_channel);







/**
  * @brief    串口测试函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void Test_UART_DMA(void);


#endif
