/*!
  * @file     LQ_FlexCAN.h
  *
  * @brief    CAN 测试文件
  *
  * @author   LQ-005
  *
  * @note     
  *
  * @version  
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     
  */ 
#ifndef __LQFLEXCAN_H__
#define __LQFLEXCAN_H__

#include "LQ_GPIO_Cfg.h"

/**  @brief    CAN 可选管脚  */
#define FLEX_CAN1_TX   M3         //M3  A4  E8  H13    A4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define FLEX_CAN1_RX   B2         //B2  M4  D8  M13    B2 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）

#define FLEX_CAN2_TX   H14        //H14 C2  M11 A12    C2 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define FLEX_CAN2_RX   L10        //L10 G1  G11 A13    G1 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）

#define FLEX_CAN3_TX   C3         //C3  G13 H14        C3 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define FLEX_CAN3_RX   E4         //E4  G10 L10        E4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）

      

/*!
  * @brief    CAN 初始化
  *
  * @param    base     :CAN模块 CAN1 CAN2 CAN3
  * @param    baudrate :CAN速率
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 星期三
  */
void CAN_Init(CAN_Type *base, uint32_t baudrate);


/*!
  * @brief    设置CAN接收邮箱
  *
  * @param    base     :CAN模块 CAN1 CAN2 CAN3  
  * @param    RxMb_num :使用的邮箱号 
  * @param    id       :接收帧的ID 
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 星期三
  */
void CAN_RxConfig(CAN_Type *base, uint8_t RxMb_num, uint32_t id);


/*!
  * @brief    设置CAN发送邮箱
  *
  * @param    base     :CAN模块 CAN1 CAN2 CAN3  
  * @param    TxMb_num :使用的邮箱号  
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 星期三
  */
void CAN_TxConfig(CAN_Type *base, uint8_t TxMb_num);



/*!
  * @brief    CAN发送帧
  *
  * @param    base     :CAN模块 CAN1 CAN2 CAN3  
  * @param    TxMb_num :使用的邮箱号   
  * @param    id       :发送帧ID
  * @param    msg      :存放发送数据地址 
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 星期三
  */
void CAN_Tx(CAN_Type *base, uint8_t TxMb_num, uint32_t id, uint8_t *msg);



/*!
  * @brief    测试CAN
  *
  * @param    
  *
  * @return   
  *
  * @note     收发ID一致 CAN会接收到自己发送的数据
  *
  * @see      
  *
  * @date     2019/8/21 星期三
  */
void Test_CAN(void);
      
      
#endif 



