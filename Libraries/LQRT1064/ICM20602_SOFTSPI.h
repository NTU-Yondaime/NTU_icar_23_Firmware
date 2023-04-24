#ifndef __ICM20602_SOFTSPI_H
#define __ICM20602_SOFTSPI_H

#include "LQ_GPIO_Cfg.h"
#include "fsl_common.h"

/* ע�� ICM20602 ʱ�ӿ���ʱΪ�ߵ�ƽ  ��ʱ�ӵڶ��������ش�������  ��NRF2401�Ĳ�ͬ������SPI��ͨ��  */
/* 0: ʱ�ӿ���ʱ��Ϊ�͵�ƽ   1��ʱ�ӿ���ʱ��Ϊ�ߵ�ƽ*/ 
#define CPOL  1

/* 0��ʱ�ӵ�һ�������ش�������   1��ʱ�ӵĵڶ��������ش������� */
#define CPHA  1 


#define SOFT_SPI_SCK_PIN    DIR2_22  
#define SOFT_SPI_MISO_PIN   DIR1_4
#define SOFT_SPI_MOSI_PIN   DIR2_25
#define SOFT_SPI_CS_PIN     DIR2_24


#if CPOL
#define SPI_SCK_IDLE      PT2_22 = 1
#define SPI_SCK_ACTIVE    PT2_22 = 0
#else
#define SPI_SCK_IDLE      PT2_22 = 0
#define SPI_SCK_ACTIVE    PT2_22 = 1
#endif

#define SPI_MOSI_OUT      PT2_25
#define SPI_MISO_IN       PT1_4
#define SPI_CS_OUT        PT2_24


/**
  * @brief    ģ��SPI��ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/28 ���ڶ�
  */
void SPI_SoftInit(void);

/**
  * @brief    ģ��SPI��дn �ֽ�
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/28 ���ڶ�
  */
void SPI_SoftRadeWriteNbyte(uint8_t *buff, uint16_t len);

#endif
