#ifndef __LQ_LED_H__
#define __LQ_LED_H__

//定义颜色枚举
typedef enum
{
  white = 0,  //白色
  black = 1,  //黑色
  red   = 2,  //红
  green = 3,  //绿
  blue  = 4,  //蓝色 
  yellow= 5,  //黄色
  violet= 6,  //紫色
  cyan  = 7,  //青色
}LED_t;

/**
  * @brief    测试LED灯
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
void Test_LED(void);


/**
  * @brief    指定颜色闪烁
  *
  * @param    color  ： 枚举LED_t中的颜色
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void LED_ColorReverse(LED_t color);


/**
  * @brief    指定颜色的灯亮
  *
  * @param    color  ： 枚举LED_t中的颜色
  *
  * @return   
  *
  * @note     
  *
  * @example  LED_Color(red);   //红灯亮
  *
  * @date     2019/6/4 星期二
  */
void LED_Color(LED_t color);


/**
  * @brief    RGB灯引脚初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO2_IO23  -------->  管脚B12   ----- >  核心板G灯
  * @note     GPIO3_IO26  -------->  管脚A7    ----- >  核心板R灯
  * @note     GPIO3_IO27  -------->  管脚C7    ----- >  核心板B灯
  * @note    
  * @note     GPIO2_IO22  -------->  管脚C12   ----- >  母板D0
  * @note     GPIO2_IO25  -------->  管脚A13   ----- >  母板D1
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void LED_Init(void);

#endif 
