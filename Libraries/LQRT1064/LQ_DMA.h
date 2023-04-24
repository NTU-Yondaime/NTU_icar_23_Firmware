#ifndef __LQ_DMA_H__
#define __LQ_DMA_H__

#include "fsl_edma.h"
/*!
  * @brief    DMA传输句柄
  *
  * @note     
  */ 
extern edma_handle_t g_EDMA_Handle; 


/*!
  * @brief    DMA传输配置结构体
  *
  * @note     
  */ 
extern edma_transfer_config_t transferConfig;

/*!
  * @brief    DMA内存 TO 内存初始化
  *
  * @param    DMA_channel:  DMA通道号  0-31
  * @param    srcAddr    :  源地址
  * @param    destAddr   :  目的地址
  * @param    len        :  传输长度
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/26 星期一
  */
void DMA_MemoryToMemoryInit(uint8_t DMA_channel, uint32_t srcAddr, uint32_t destAddr, uint32_t len);


/*!
  * @brief    测试DMA内存到内存
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/26 星期一
  */
void Test_DMA(void);

#endif 

