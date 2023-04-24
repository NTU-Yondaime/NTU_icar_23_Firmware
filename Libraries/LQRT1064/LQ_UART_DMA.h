/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��10��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
================================================
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_UART_DMA_H
#define __LQ_UART_DMA_H

#include "fsl_lpuart_edma.h"
/* ����+DMA ��־λ */
extern volatile bool rxBufferEmpty;
extern volatile bool txBufferFull;
extern volatile bool txOnGoing;
extern volatile bool rxOnGoing;

/* ���յ���֡���� */
extern volatile uint32_t g_rxLen;

/* ���ڽ��� ������ */
extern uint8_t g_rxBuffer[100];
extern uint8_t g_txBuffer[100];

/* DMA ����ṹ�� */
extern lpuart_transfer_t sendXfer;
extern lpuart_transfer_t receiveXfer;

/* ���� + DMA ���ýṹ�� */
extern lpuart_edma_handle_t g_lpuartEdmaHandle;


/**
  * @brief    ����DMA�����ʼ��
  *
  * @param    base  �� LPUART1 - LPUART8
  * @param    bound �� ���ڲ�����
  * @param    TX_channel  �� DMA TXͨ����
  * @param    RX_channel  �� DMA RXͨ����
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void LQ_UART_DMA_Init(LPUART_Type *base, uint32_t bound, uint8_t TX_channel, uint8_t RX_channel);







/**
  * @brief    ���ڲ��Ժ���
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void Test_UART_DMA(void);


#endif
