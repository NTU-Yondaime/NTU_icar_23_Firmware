#ifndef  __LQ_SPI_H
#define  __LQ_SPI_H

#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_lpspi.h"


/**********************************  IIC(���Ÿ���) *******************************************************/
//     IIC�ܽ�        �˿�        ��ѡ��Χ                   ����
#define SPI1_SCK      J4          //A2  J4                   A2 ���ⲿSDRAM                 
#define SPI1_SDI      K1          //E1  K1                   E1 ���ⲿSDRAM                  
#define SPI1_SDO      J1          //D1  J1                   D1 ���ⲿSDRAM
#define SPI1_CS0      J3          //J3  C6                   C6 ���ⲿSDRAM
#define SPI1_CS1      C5          //C5                       C5 ���ⲿSDRAM
#define SPI1_CS2      A7          //A7                       A7 ���İ�R��
#define SPI1_CS3      C7          //C7                       C7 ���İ�B��

#define SPI2_SCK      L4          //E3  L4                   E3 ���ⲿSDRAM    L4 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��             
#define SPI2_SDI      N4          //G4  N4                   G4 ���ⲿSDRAM    N4 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��                        
#define SPI2_SDO      P3          //F4  P3                   F4 ���ⲿSDRAM    P3 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��
#define SPI2_CS0      L3          //F3  L3                   F3 ���ⲿSDRAM    L3 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��
#define SPI2_CS1      B6          //B6                       B6 ���ⲿSDRAM
#define SPI2_CS2      P4          //P4                       P4 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��
#define SPI2_CS3      P5          //P5                       P5 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��

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
  * @brief    SPI ����ģʽ��ʼ��
  *
  * @param    base      ��  LPSPI1 - LPSPI4
  * @param    cs_num    ��  Ƭѡ���ű�� 
  * @param      mode            0: CLK����ʱ��Ϊ�͵�ƽ �ڵ�һ��CLK�����أ��½��أ���������
  * @param      mode            1: CLK����ʱ��Ϊ�͵�ƽ �ڵڶ���CLK�����أ������أ���������
  * @param      mode            2: CLK����ʱ��Ϊ�ߵ�ƽ �ڵ�һ��CLK�����أ������أ���������
  * @param      mode            3: CLK����ʱ��Ϊ�ߵ�ƽ �ڵڶ���CLK�����أ��½��أ���������
  * @param    baudrate  ��  SPI ������    
  *
  * @return   ע�� ICM20602 ʱ�ӿ���ʱΪ�ߵ�ƽ ʱ����ЧʱΪ�͵�ƽ ��ʱ�ӵڶ��������ش�������    
  *
  * @note     SPI Ĭ��ʱ��Դ 66M
  *
  * @example  
  *
  * @date     2019/6/3 ����һ
  */
void LQ_SPI_Master_Init(LPSPI_Type * base, uint8_t cs_num, uint8_t mode, uint32_t baudrate);


/**
  * @brief    SPI��дN���ֽ�
  *
  * @param    base    ��  LPSPI1 - LPSPI4
  * @param    cs_num  ��  Ƭѡ���ű��  
  * @param    txData  ��  д���buff
  * @param    rxData  ��  ������buff
  * @param    len     ��  ����
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/3 ����һ
  */
uint8_t SPI_ReadWriteNByte(LPSPI_Type * base, uint8_t cs_num, uint8_t *txData, uint8_t *rxData, uint32_t len);



/**
  * @brief    SPI �ܽų�ʼ��
  *
  * @param    base      ��  LPSPI1 - LPSPI4
  * @param    cs_num    ��  Ƭѡ���ű��  
  *
  * @return   
  *
  * @note     ��4�¹ܽŸ���
  *
  * @example  
  *
  * @date     2019/6/3 ����һ
  */
void SPI_PinInit(LPSPI_Type * base, uint8_t cs_num);


#endif
