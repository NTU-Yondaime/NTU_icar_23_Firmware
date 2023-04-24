#ifndef __ANO_DT_H
#define __ANO_DT_H

#include "fsl_common.h"

/**
  * @brief    向上位机发送发送8个short数据
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 /*,short data7, short data8, short .....可根据需要自行添加 */);

/**
  * @brief    向上位机发送发送PID帧
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);


/**
  * @brief    测试上报匿名上位机
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void Test_ANO_DT(void);


/**
  * @brief    解析匿名上位机发送来的数据包
  *
  * @param    data_buf ：数据包
  * @param    num      ：数据包长度
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num);

/**
  * @brief    用于串口接收中断 一个中断接收一个字符时用这个解析函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ANO_DT_Data_Receive_Prepare(uint8_t data);
	
#endif

