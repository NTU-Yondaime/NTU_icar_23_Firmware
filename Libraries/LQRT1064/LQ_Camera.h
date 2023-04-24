#ifndef __LQ_SGP18TEST_H
#define __LQ_SGP18TEST_H

#include "LQ_CSI.h"
/* ���ڴ����Ҫʹ������ */
extern pixel_t Image_Use[Use_ROWS][Use_Line]; 

/* ���ڴ��浱ǰ֡�ĵ�ַ */
extern uint32_t fullCameraBufferAddr;
/**
  * @brief    ��ȡ��Ҫʹ�õ�ͼ������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_Use_Image(void);


/**
  * @brief    TFT1.8 ��ʾͼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void Test_SGP18_Camera(void);


/**
  * @brief    OLED ��ʾͼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void Test_OLED_Camera(void);



/**
  * @brief    OLED ��ʾͼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Draw_Road(void);


/**
  * @brief    ��ֵ��
  *
  * @param    mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_01_Value(uint8_t mode);


int GetOSTU(pixel_t tmImage[Use_ROWS][Use_Line]);

#endif

