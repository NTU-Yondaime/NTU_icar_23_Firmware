/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_debug_console.h"
#include "fsl_wdog.h"
#include "LQ_WDog.h"
#include "LQ_LED.h"

/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


/**
  * @brief    看门狗初始化
  *
  * @param    base  ： WDOG1、WDOG2
  * @param    time  ： 实际时间为(time+1)*0.5s
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void LQ_WDOG_Init(WDOG_Type *base, uint8_t time)
{
    wdog_config_t config;
    /*
    * wdogConfig->enableWdog = true;            //使能看门狗
    * wdogConfig->workMode.enableWait = true;   //等待模式下仍然开启
    * wdogConfig->workMode.enableStop = false;  //停止模式下关闭
    * wdogConfig->workMode.enableDebug = false; //debug模式关闭
    * wdogConfig->enableInterrupt = false;      //不使能中断
    * wdogConfig->enablePowerdown = false;      
    * wdogConfig->resetExtension = flase;
    * wdogConfig->timeoutValue = 0xFFU;         //设置喂狗时间
    * wdogConfig->interruptTimeValue = 0x04u;   //在喂狗前 多长时间进入中断 方便喂狗
    */
    WDOG_GetDefaultConfig(&config);             //获取默认配置参数
    config.workMode.enableDebug = true;        //debug模式开启
    config.timeoutValue = time;                 //看门狗超时时间，实际时间为(timeoutValue+1)*0.5s
    WDOG_Init(base, &config);
}


/**
  * @brief    测试看门狗
  *
  * @param    
  *
  * @return   
  *
  * @note     看门狗的作用是定时喂狗，如果程序跑飞没有喂狗，系统复位
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void Test_WDOG(void)
{
    LED_Init();
    delayms(3000);
    LED_Color(blue);
    LQ_WDOG_Init(WDOG1, 9);
    PRINTF("--- 看门狗测试开始 ---\r\n");
    
    for (uint32_t i = 0; i < 10; i++)
    {
        WDOG_Refresh(WDOG1);     //喂狗
        PRINTF("--- 第 %d 次喂狗 ---\r\n", i + 1);
        delayms(1000 * i);
    }

    PRINTF("\r\n不喂狗 程序复位\r\n");
    while (1)
    {
    }
}


