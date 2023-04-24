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
#ifndef _LQ_GPT_H_
#define _LQ_GPT_H_

#include "fsl_gpt.h"
/**
  * @brief    GPT 通用定时器初始化为程序计时器
  *
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     使用GPT程序计时器前请先调用该函数进行初始化
  *
  * @example  GPT_TimeInitConfig(GPT1);  //初始化GPT1为程序计时器
  *
  * @date     2019/6/10 星期一
  */
void GPT_TimeInitConfig(GPT_Type *base);


/**
  * @brief    gpt 计时器开始计时
  *
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     GPT 是32为计数器 最大延时4294967295 / 1 000000 = 4290s 
  *
  * @example  GPT_TimeStart(GPT1);  //开始计数
  *
  * @date     2019/6/10 星期一
  */
void GPT_TimeStart(GPT_Type *base);


/**
  * @brief    gpt 计时器开始计时
  *
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     GPT 是32为计数器 最大延时4294967295 / 1 000000 = 4290s 
  *
  * @example  GPT_TimeGetUs(GPT1); //获取从GPT_TimeStart(GPT1)开始至现在的us数
  *
  * @date     2019/6/10 星期一
  */
uint32_t GPT_TimeGetUs(GPT_Type *base);


/**
  * @brief    GPT 通用定时器初始化为延时计时器
  *
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     使用GPT延时前请先调用该函数进行初始化
  *
  * @example  GPT_DelayInitConfig(GPT1);  //初始化
  *
  * @date     2019/6/10 星期一
  */
void GPT_DelayInitConfig(GPT_Type *base);


/**
  * @brief    gpt 定时器精确延时
  *
  * @param    ms ： 延时时间 ms
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     GPT 是32为计数器 最大延时4294967295 / 5 000000 = 868s
  *
  * @example  GPT_DelayMs(GPT1, 1000); //延时1s
  *
  * @date     2019/6/10 星期一
  */
void GPT_DelayMs(GPT_Type *base, uint32_t ms);


/**
  * @brief    gpt 定时器精确延时
  *
  * @param    us ： 延时时间 us
  * @param    base： GPT1、GPT2
  *
  * @return   无
  *
  * @note     GPT 是32为计数器 最大延时4294967295 / 5 000000 = 868s
  *
  * @example  GPT_DelayUs(GPT1, 1000); //延时1ms
  *
  * @date     2019/6/10 星期一
  */
void GPT_DelayUs(GPT_Type *base, uint32_t us);






#endif  
