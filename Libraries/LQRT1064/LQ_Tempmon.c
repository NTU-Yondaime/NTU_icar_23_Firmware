/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年03月18日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.30.1及以上版本
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_debug_console.h"
#include "fsl_tempmon.h"
                      
/* 声明外部延时函数 */
extern void delayms(uint16_t ms);

/**
  * @brief    测试读取温度
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/11 星期二
  */
void Test_Tempmon(void)
{
    float temperature = 0U; 
    tempmon_config_t config;
    

    PRINTF("芯片温度测试例程. \r\n");
    

    /* 默认的测量频率 
    config->frequency = 0x03U;
     默认高报警温度
    config->highAlarmTemp = 40U;
     默认紧急报警温度 
    config->panicAlarmTemp = 90U;
     默认低报警温度 
    config->lowAlarmTemp = 20U;*/
    TEMPMON_GetDefaultConfig(&config);

    config.frequency = 0x03U;
    config.highAlarmTemp = 40U;
    config.lowAlarmTemp = 20;
    config.panicAlarmTemp = 80U;
    
    TEMPMON_Init(TEMPMON, &config);
    TEMPMON_StartMeasure(TEMPMON);

    /* 得到温度 */
    temperature = TEMPMON_GetCurrentTemperature(TEMPMON);

    PRINTF("芯片当前温度： %.1f ℃. \r\n", temperature);

    while (1)
    {
   
        delayms(10000);

        temperature = TEMPMON_GetCurrentTemperature(TEMPMON);
        PRINTF("芯片当前温度： %.1f ℃. \r\n", temperature);
    }
    

}




