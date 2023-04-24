#ifndef __LQ_SGP18TEST_H
#define __LQ_SGP18TEST_H

#include "LQ_CSI.h"
/* 用于存放需要使用数据 */
extern pixel_t Image_Use[Use_ROWS][Use_Line]; 

/* 用于储存当前帧的地址 */
extern uint32_t fullCameraBufferAddr;
/**
  * @brief    获取需要使用的图像数组
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Get_Use_Image(void);


/**
  * @brief    TFT1.8 显示图像
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void Test_SGP18_Camera(void);


/**
  * @brief    OLED 显示图像
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void Test_OLED_Camera(void);



/**
  * @brief    OLED 显示图像
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Draw_Road(void);


/**
  * @brief    二值化
  *
  * @param    mode  ：  0：使用大津法阈值    1：使用平均阈值
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Get_01_Value(uint8_t mode);


int GetOSTU(pixel_t tmImage[Use_ROWS][Use_Line]);

#endif

