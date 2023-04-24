#ifndef __LQ_IIC_H
#define __LQ_IIC_H

#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_lpi2c.h"


/**********************************  IIC(引脚复用) *******************************************************/
//     IIC管脚        端口        可选范围                   建议
#define IIC1_SCL      J11         //P2  J11                  
#define IIC1_SDA      K11         //K11 N3                   

#define IIC2_SCL      C8          //C8  P5                   P5 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）  
#define IIC2_SDA      B8          //B8  P4                   P4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）  

#define IIC3_SCL      J4          //F1  J4  K10              F1 接外部SDRAM
#define IIC3_SDA      J3          //C1  J3  J12              C1 接外部SDRAM

#define IIC4_SCL      K14         //H1  K14                  H1 接外部SDRAM
#define IIC4_SDA      L14         //G3  L14                  G3 接外部SDRAM





/**
  * @brief    IIC 初始化函数
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    baudrate:   IIC波特率
  *
  * @return   
  *
  * @note     IIC 默认时钟源 10M
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void LQ_IIC_Init(LPI2C_Type *base, uint32_t baudrate);


/**
  * @brief    IIC 管脚初始化
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void IIC_PinInit(LPI2C_Type *base);


/**
  * @brief    IIC 写一个寄存器
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要写的寄存器 
  * @param    val     ：  要写入的值  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_WriteReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t val);



/**
  * @brief    IIC 读n个寄存器
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要读的寄存器 
  * @param    val     ：  存放读出数据的地址 
  * @param    len     ：  读取长度  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_ReadReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t *val, uint8_t len);

#endif

