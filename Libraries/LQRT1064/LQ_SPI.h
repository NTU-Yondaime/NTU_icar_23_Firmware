#ifndef  __LQ_SPI_H
#define  __LQ_SPI_H

#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_lpspi.h"


/**********************************  IIC(引脚复用) *******************************************************/
//     IIC管脚        端口        可选范围                   建议
#define SPI1_SCK      J4          //A2  J4                   A2 接外部SDRAM                 
#define SPI1_SDI      K1          //E1  K1                   E1 接外部SDRAM                  
#define SPI1_SDO      J1          //D1  J1                   D1 接外部SDRAM
#define SPI1_CS0      J3          //J3  C6                   C6 接外部SDRAM
#define SPI1_CS1      C5          //C5                       C5 接外部SDRAM
#define SPI1_CS2      A7          //A7                       A7 核心板R灯
#define SPI1_CS3      C7          //C7                       C7 核心板B灯

#define SPI2_SCK      L4          //E3  L4                   E3 接外部SDRAM    L4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）             
#define SPI2_SDI      N4          //G4  N4                   G4 接外部SDRAM    N4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）                        
#define SPI2_SDO      P3          //F4  P3                   F4 接外部SDRAM    P3 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define SPI2_CS0      L3          //F3  L3                   F3 接外部SDRAM    L3 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define SPI2_CS1      B6          //B6                       B6 接外部SDRAM
#define SPI2_CS2      P4          //P4                       P4 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）
#define SPI2_CS3      P5          //P5                       P5 接外部QSPI FLASH（RT1064 内部有4M FLASH  可以不使用外部QSPI FLASH）

#define SPI3_SCK      J14         //J14 M14               
#define SPI3_SDI      H11         //H11 M11                                     
#define SPI3_SDO      H10         //H10 G12              
#define SPI3_CS0      H12         //G11 H12            
#define SPI3_CS1      F11         //F11                       
#define SPI3_CS2      G14         //G14                       
#define SPI3_CS3      E14         //E14                       

#define SPI4_SCK      D8          //D8  B12              
#define SPI4_SDI      E7          //E7  D12                                     
#define SPI4_SDO      E8          //E8  C12             
#define SPI4_CS0      D7          //D7  E12           
#define SPI4_CS1      D11         //D11                       
#define SPI4_CS2      C11         //C11                      
#define SPI4_CS3      C13         //C13   

/**
  * @brief    SPI 主机模式初始化
  *
  * @param    base      ：  LPSPI1 - LPSPI4
  * @param    cs_num    ：  片选引脚编号 
  * @param      mode            0: CLK空闲时刻为低电平 在第一个CLK跳边沿（下降沿）捕获数据
  * @param      mode            1: CLK空闲时刻为低电平 在第二个CLK跳边沿（上升沿）捕获数据
  * @param      mode            2: CLK空闲时刻为高电平 在第一个CLK跳边沿（上升沿）捕获数据
  * @param      mode            3: CLK空闲时刻为高电平 在第二个CLK跳边沿（下降沿）捕获数据
  * @param    baudrate  ：  SPI 波特率    
  *
  * @return   注意 ICM20602 时钟空闲时为高电平 时钟有效时为低电平 在时钟第二个跳边沿传输数据    
  *
  * @note     SPI 默认时钟源 66M
  *
  * @example  
  *
  * @date     2019/6/3 星期一
  */
void LQ_SPI_Master_Init(LPSPI_Type * base, uint8_t cs_num, uint8_t mode, uint32_t baudrate);


/**
  * @brief    SPI读写N个字节
  *
  * @param    base    ：  LPSPI1 - LPSPI4
  * @param    cs_num  ：  片选引脚编号  
  * @param    txData  ：  写入的buff
  * @param    rxData  ：  读出的buff
  * @param    len     ：  长度
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/3 星期一
  */
uint8_t SPI_ReadWriteNByte(LPSPI_Type * base, uint8_t cs_num, uint8_t *txData, uint8_t *rxData, uint32_t len);



/**
  * @brief    SPI 管脚初始化
  *
  * @param    base      ：  LPSPI1 - LPSPI4
  * @param    cs_num    ：  片选引脚编号  
  *
  * @return   
  *
  * @note     第4章管脚复用
  *
  * @example  
  *
  * @date     2019/6/3 星期一
  */
void SPI_PinInit(LPSPI_Type * base, uint8_t cs_num);


#endif
