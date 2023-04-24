#ifndef __LQ_IIC_H
#define __LQ_IIC_H

#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"
#include "fsl_lpi2c.h"


/**********************************  IIC(���Ÿ���) *******************************************************/
//     IIC�ܽ�        �˿�        ��ѡ��Χ                   ����
#define IIC1_SCL      J11         //P2  J11                  
#define IIC1_SDA      K11         //K11 N3                   

#define IIC2_SCL      C8          //C8  P5                   P5 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��  
#define IIC2_SDA      B8          //B8  P4                   P4 ���ⲿQSPI FLASH��RT1064 �ڲ���4M FLASH  ���Բ�ʹ���ⲿQSPI FLASH��  

#define IIC3_SCL      J4          //F1  J4  K10              F1 ���ⲿSDRAM
#define IIC3_SDA      J3          //C1  J3  J12              C1 ���ⲿSDRAM

#define IIC4_SCL      K14         //H1  K14                  H1 ���ⲿSDRAM
#define IIC4_SDA      L14         //G3  L14                  G3 ���ⲿSDRAM





/**
  * @brief    IIC ��ʼ������
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    baudrate:   IIC������
  *
  * @return   
  *
  * @note     IIC Ĭ��ʱ��Դ 10M
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void LQ_IIC_Init(LPI2C_Type *base, uint32_t baudrate);


/**
  * @brief    IIC �ܽų�ʼ��
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void IIC_PinInit(LPI2C_Type *base);


/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    addr    ��  ������ַ����7λ�� 
  * @param    reg     ��  Ҫд�ļĴ��� 
  * @param    val     ��  Ҫд���ֵ  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
status_t IIC_WriteReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t val);



/**
  * @brief    IIC ��n���Ĵ���
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    addr    ��  ������ַ����7λ�� 
  * @param    reg     ��  Ҫ���ļĴ��� 
  * @param    val     ��  ��Ŷ������ݵĵ�ַ 
  * @param    len     ��  ��ȡ����  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
status_t IIC_ReadReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t *val, uint8_t len);

#endif

