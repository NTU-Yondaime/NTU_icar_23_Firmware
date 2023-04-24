/*-----------------------------------------------------------------
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年03月13日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【dev.env.】IAR8.30.1及以上版本
-------------------------------------------------------------------*/
#include "LQ_Rtwdog.h"
#include "fsl_rtwdog.h"
#include "fsl_debug_console.h"
#include "LQ_SYSTICK.h"




/**
  * @brief    窗口看门狗
  *
  * @param    ofus  : 看门狗计数器溢出时间 ms(超过该值将产生复位).范围:0~1985 ms
  *           winus : 看门狗计数器窗口值.  ms 范围:0~1985 ms,0表示不使用窗口模式.
  * @return   
  *
  * @note     喂狗要在 win - toval 之间
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void MYRTWDOG_Init(uint16_t ofms,uint16_t winms)
{
    uint16_t overtime = (uint16_t)(ofms * 32.768);
    uint16_t wintime  = (uint16_t)(winms * 32.768);
    rtwdog_config_t rtwdog_config;	
    systime.delay_ms(100);
    RTWDOG_GetDefaultConfig(&rtwdog_config);		//获取默认参数
    rtwdog_config.testMode=kRTWDOG_UserModeEnabled;	//用户模式
    rtwdog_config.clockSource=kRTWDOG_ClockSource1; //时钟源选择  32.768KHZ
    rtwdog_config.prescaler=kRTWDOG_ClockPrescalerDivide1;				//不分频
    rtwdog_config.timeoutValue=overtime;	        //超时值
    rtwdog_config.enableWindowMode =true;			//开启窗口功能
	rtwdog_config.enableInterrupt=false;		    //关中断
    rtwdog_config.enableRtwdog=true;				//使能看门狗
    rtwdog_config.windowValue=wintime;              //窗口值
    RTWDOG_Init(RTWDOG,&rtwdog_config);				//初始化看门狗
    
	//优先级配置 抢占优先级2  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(RTWDOG_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,2));
    NVIC_EnableIRQ(RTWDOG_IRQn);				    //使能RTWDOG中断
}


/**
  * @brief    测试窗口看门狗
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void Test_Rtwdog(void)
{
    systime.init();
    PRINTF("\nRTWdog 看门狗例程\n");
    PRINTF("喂狗时间要在0 ms 到 50ms 之间\n");
    PRINTF("大于50ms 喂狗，都会导致复位\n");
    MYRTWDOG_Init(100, 0);	            //初始化RT看门狗,100ms溢出,非窗口模式
    while(1)
    {
        systime.delay_ms(10);
        RTWDOG_Refresh(RTWDOG);         //喂狗
        PRINTF("喂狗成功，系统继续运行\n");
        systime.delay_ms(20);
        RTWDOG_Refresh(RTWDOG);         //喂狗
        PRINTF("喂狗成功，系统继续运行\n");
        systime.delay_ms(200);
        RTWDOG_Refresh(RTWDOG);         //喂狗
        PRINTF("喂狗成功，系统继续运行\n");
        while(1);
    }
    

}


