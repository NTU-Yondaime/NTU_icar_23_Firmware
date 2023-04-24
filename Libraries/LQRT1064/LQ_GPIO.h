/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include "LQ_GPIO_Cfg.h"


/* GPIO 模式 */
#define PIN_MODE_OUTPUT         0x00         //输出
#define PIN_MODE_INPUT          0x01         //输入
#define PIN_MODE_INPUT_PULLUP   0x02         //上拉输入
#define PIN_MODE_INPUT_PULLDOWN 0x03         //下拉输入
#define PIN_MODE_OUTPUT_OD      0x04         //开漏输入

/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             0x00 //上升沿（下拉）触发中断  
#define PIN_IRQ_MODE_FALLING            0x01 //下降沿（上拉）触发中断
#define PIN_IRQ_MODE_RISING_FALLING     0x02 //双边沿（开漏）触发中断
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03 //高电平（下拉）触发中断
#define PIN_IRQ_MODE_LOW_LEVEL          0x04 //低电平（上拉）触发中断



/*!
  * @brief    GPIO初始化
  *
  * @param    pin      ： 管教标号
  * @param    mode     ： GPIO 模式   LQ_GPIO.h中的宏定义中选择
  * @param    output   ： 输出模式时，输出电平 1：高电平  0：低电平
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinInit(A7, PIN_MODE_OUTPUT, 1);//设置管脚A7为输出模式 高电平
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinInit(GPIO_Name_t pin, uint8_t mode, uint8_t output);



/*!
  * @brief    读取GPIO状态
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO要初始化为输入模式
  *
  * @example  LQ_PinRead(A7); //获取A7状态
  *
  * @date     2019/8/19 星期一
  */
uint8_t LQ_PinRead(GPIO_Name_t pin);



/*!
  * @brief    设置GPIO输出状态
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinWrite(A7, 1);//A7置为高电平
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinWrite(GPIO_Name_t pin, uint8_t output);



/*!
  * @brief    GPIO外部中断初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  LQ_PinExti(C13, PIN_IRQ_MODE_FALLING);//配置C13 下降沿触发中断
  * @example  优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
  * @example  NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
  * @example  EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //使能GPIO2_16~31IO的中断  
  *
  * @date     2019/8/19 星期一
  */
void LQ_PinExti(GPIO_Name_t pin, uint8_t mode);


#endif  
