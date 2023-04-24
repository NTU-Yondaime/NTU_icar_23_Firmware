/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年6月18日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
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
/* 声明外部延时函数 */
extern void delayms(uint16_t ms);




/* 串口+DMA 标志位 */
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;


/**
  * @brief    串口+DMA 的回调函数  当串口发送完成或者接受完成，会自动调用该函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/15 星期六
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





/* 串口 + DMA 配置结构体 */
lpuart_edma_handle_t g_lpuartEdmaHandle;
edma_handle_t g_lpuartTxEdmaHandle;
edma_handle_t g_lpuartRxEdmaHandle;


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
  * @note     DMA + 空闲中断 处理不定长帧
  *
  * @example  
  *
  * @date     2019/6/6 星期四
  */
void LQ_UART_DMA_Init(LPUART_Type *base, uint32_t bound, uint8_t TX_channel, uint8_t RX_channel)
{
    /* 初始化串口对应的管脚 对应管脚在LQ_UART.h 中通过宏定义确定 */
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

    /* 设置波特率 */
    config.baudRate_Bps = bound;
    
    /* 设置奇偶校验 */
    config.parityMode = kLPUART_ParityDisabled;
    
    /* 设置停止位 */
    config.stopBitCount = kLPUART_OneStopBit;
    
    /* 设置数据位宽 8位 */
    config.dataBitsCount = kLPUART_EightDataBits;
    
    /* 接收水印 */
    config.rxFifoWatermark = 1;
    
    /* 接收空闲中断 开始条件 */
    config.rxIdleType = kLPUART_IdleTypeStartBit;
    
    /* 空闲中断 发生在没有接收到数据的第几个时期 */
    config.rxIdleConfig = kLPUART_IdleCharacter1;
    
    /* 使能收发 */
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(base, &config, BOARD_DebugConsoleSrcFreq());
    
    /* 使能空闲中断 */
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
    
    /* 初始化DMA多路复用器 */
    DMAMUX_Init(DMAMUX);
    
    /* 设置DMA通道源 */
    DMAMUX_SetSource(DMAMUX, TX_channel, (uint8_t)DmaRequestId); 
    DMAMUX_SetSource(DMAMUX, RX_channel, (uint8_t)(DmaRequestId+1)); 
    
    /* 使能DMA通道 */
    DMAMUX_EnableChannel(DMAMUX, TX_channel);
    DMAMUX_EnableChannel(DMAMUX, RX_channel);
    
    
    /* 初始化 EDMA 模块 */
    edma_config_t edma_config;
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(DMA0, &edma_config);
    EDMA_CreateHandle(&g_lpuartTxEdmaHandle, DMA0, TX_channel);
    EDMA_CreateHandle(&g_lpuartRxEdmaHandle, DMA0, RX_channel);

    /* 创建DMA句柄. 将回调函数等信息赋值给 g_lpuartEdmaHandle 结构体 */
    LPUART_TransferCreateHandleEDMA(base, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle,
                                    &g_lpuartRxEdmaHandle);
    
}

/* 接收到的帧长度 */
volatile uint32_t g_rxLen;

/* 串口接收 缓冲区 */
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[100]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[100]) = {0};

/* DMA 传输结构体 */
lpuart_transfer_t sendXfer;
lpuart_transfer_t receiveXfer;

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
void Test_UART_DMA(void)
{
    LED_Init();   
    
    LQ_UART_DMA_Init(LPUART1, 115200, 0, 1);   
    
    //优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //使能LPUART1 的空闲中断
	
    delayms(100);
    PRINTF("\r\n龙邱串口测试例程\r\n");
    PRINTF("串口模块波特率:\n");
    PRINTF("LPUART1 init: 115200!\n"); 
    
    /* 接收配置 */
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = 100;
    
    /* 开始接收 */
    rxOnGoing = true;
    LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
    while(1)
    {
        
        /* 接收到数据，并且已经copy到发送缓冲区发送出去. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
        }

        /* 接收完成后，重新开始接收新帧. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            /* 发送配置. */
            sendXfer.data = g_txBuffer;
            sendXfer.dataSize = g_rxLen; 
            LPUART_SendEDMA(LPUART1, &g_lpuartEdmaHandle, &sendXfer);
        }

        /* 接收到一帧数据后，从接收字符缓冲区copy到发送缓冲区. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            SCB_DisableDCache();
			SCB_EnableDCache();  //刷新D-Cache
            memcpy(g_txBuffer, g_rxBuffer, g_rxLen);
            rxBufferEmpty = true;
            txBufferFull = true;
            LED_ColorReverse(blue);
        }
        
    }  
}


