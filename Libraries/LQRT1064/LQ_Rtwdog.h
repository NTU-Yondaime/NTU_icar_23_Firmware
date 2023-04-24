#ifndef __LQ_RTWDOG_H
#define __LQ_RTWDOG_H

#include "fsl_common.h"
/**
  * @brief    窗口看门狗
  *
  * @param   toval: 看门狗计数器溢出最大值(cnt超过该值将产生复位).范围:0~65535 
  *           win : 看门狗计数器窗口值.范围:0~65535,0表示不使用窗口模式.
  * @return   
  *
  * @note     喂狗要在 win - toval 之间
  *
  * @example  
  *
  * @date     2019/6/10 星期一
  */
void MYRTWDOG_Init(uint16_t toval,uint16_t win);


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
void Test_Rtwdog(void);




#endif

