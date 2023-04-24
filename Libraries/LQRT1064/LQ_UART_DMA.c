/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��6��18��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
================================================
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "stdio.h"	
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_lpuart_edma.h"
#include "LQ_UART.h"
#include "LQ_LED.h"
#include "LQ_UART_DMA.h"
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);




/* ����+DMA ��־λ */
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;


/**
  * @brief    ����+DMA �Ļص�����  �����ڷ�����ɻ��߽�����ɣ����Զ����øú���
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/15 ������
  */
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }
}





/* ���� + DMA ���ýṹ�� */
lpuart_edma_handle_t g_lpuartEdmaHandle;
edma_handle_t g_lpuartTxEdmaHandle;
edma_handle_t g_lpuartRxEdmaHandle;


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
  * @note     DMA + �����ж� ��������֡
  *
  * @example  
  *
  * @date     2019/6/6 ������
  */
void LQ_UART_DMA_Init(LPUART_Type *base, uint32_t bound, uint8_t TX_channel, uint8_t RX_channel)
{
    /* ��ʼ�����ڶ�Ӧ�Ĺܽ� ��Ӧ�ܽ���LQ_UART.h ��ͨ���궨��ȷ�� */
    UART_PinInit(base);
    
    lpuart_config_t config;
    
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);

    /* ���ò����� */
    config.baudRate_Bps = bound;
    
    /* ������żУ�� */
    config.parityMode = kLPUART_ParityDisabled;
    
    /* ����ֹͣλ */
    config.stopBitCount = kLPUART_OneStopBit;
    
    /* ��������λ�� 8λ */
    config.dataBitsCount = kLPUART_EightDataBits;
    
    /* ����ˮӡ */
    config.rxFifoWatermark = 1;
    
    /* ���տ����ж� ��ʼ���� */
    config.rxIdleType = kLPUART_IdleTypeStartBit;
    
    /* �����ж� ������û�н��յ����ݵĵڼ���ʱ�� */
    config.rxIdleConfig = kLPUART_IdleCharacter1;
    
    /* ʹ���շ� */
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(base, &config, BOARD_DebugConsoleSrcFreq());
    
    /* ʹ�ܿ����ж� */
    LPUART_EnableInterrupts(base, kLPUART_IdleLineInterruptEnable);
    
    
    
    uint8_t DmaRequestId = 0;
    if(base == LPUART1) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART1Tx;
    else if(base == LPUART2) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART2Tx;
    else if(base == LPUART3) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART3Tx;
    else if(base == LPUART4) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART4Tx;
    else if(base == LPUART5) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART5Tx;
    else if(base == LPUART6) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART6Tx;
    else if(base == LPUART7) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART7Tx;
    else if(base == LPUART8) DmaRequestId = (uint8_t)kDmaRequestMuxLPUART8Tx;
    
    /* ��ʼ��DMA��·������ */
    DMAMUX_Init(DMAMUX);
    
    /* ����DMAͨ��Դ */
    DMAMUX_SetSource(DMAMUX, TX_channel, (uint8_t)DmaRequestId); 
    DMAMUX_SetSource(DMAMUX, RX_channel, (uint8_t)(DmaRequestId+1)); 
    
    /* ʹ��DMAͨ�� */
    DMAMUX_EnableChannel(DMAMUX, TX_channel);
    DMAMUX_EnableChannel(DMAMUX, RX_channel);
    
    
    /* ��ʼ�� EDMA ģ�� */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(DMA0, &edma_config);
    EDMA_CreateHandle(&g_lpuartTxEdmaHandle, DMA0, TX_channel);
    EDMA_CreateHandle(&g_lpuartRxEdmaHandle, DMA0, RX_channel);

    /* ����DMA���. ���ص���������Ϣ��ֵ�� g_lpuartEdmaHandle �ṹ�� */
    LPUART_TransferCreateHandleEDMA(base, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle,
                                    &g_lpuartRxEdmaHandle);
    
}

/* ���յ���֡���� */
volatile uint32_t g_rxLen;

/* ���ڽ��� ������ */
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[100]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[100]) = {0};

/* DMA ����ṹ�� */
lpuart_transfer_t sendXfer;
lpuart_transfer_t receiveXfer;

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
void Test_UART_DMA(void)
{
    LED_Init();   
    
    LQ_UART_DMA_Init(LPUART1, 115200, 0, 1);   
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //ʹ��LPUART1 �Ŀ����ж�
	
    delayms(100);
    PRINTF("\r\n���񴮿ڲ�������\r\n");
    PRINTF("����ģ�鲨����:\n");
    PRINTF("LPUART1 init: 115200!\n"); 
    
    /* �������� */
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = 100;
    
    /* ��ʼ���� */
    rxOnGoing = true;
    LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
    while(1)
    {
        
        /* ���յ����ݣ������Ѿ�copy�����ͻ��������ͳ�ȥ. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
        }

        /* ������ɺ����¿�ʼ������֡. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            /* ��������. */
            sendXfer.data = g_txBuffer;
            sendXfer.dataSize = g_rxLen; 
            LPUART_SendEDMA(LPUART1, &g_lpuartEdmaHandle, &sendXfer);
        }

        /* ���յ�һ֡���ݺ󣬴ӽ����ַ�������copy�����ͻ�����. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            SCB_DisableDCache();
			SCB_EnableDCache();  //ˢ��D-Cache
            memcpy(g_txBuffer, g_rxBuffer, g_rxLen);
            rxBufferEmpty = true;
            txBufferFull = true;
            LED_ColorReverse(blue);
        }
        
    }  
}


