#ifndef __LQ_KEY_H__
#define __LQ_KEY_H__

/**
  * @brief    初始化龙邱母板上的按键
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO3_IO04 ------ 管脚P2   ----- >  母板按键K2
  * @note     GPIO2_IO27 ------ 管脚C13  ----- >  母板按键K0
  * @note     GPIO2_IO30 ------ 管脚C14  ----- >  母板按键k1
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void KEY_Init(void);


/**
  * @brief    读取按键状态
  *
  * @param    mode： 0：不支持连续按;   1：支持连续按;
  *
  * @return   0：无按键按下  1：KEY0按下  2:KEY1按下  3:KEY2按下
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
uint8_t KEY_Read(uint8_t mode);


/**
  * @brief    测试按键
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void Test_KEY(void);



/*!
  * @brief    测试GPIIO外部中断
  *
  * @param    
  *
  * @return   
  *
  * @note     按下母板上不同的按键 不同颜色的灯闪烁
  *
  * @example  
  *
  * @date     2019/8/19 星期一
  */
void Test_ExtiKEY(void);



#endif 
