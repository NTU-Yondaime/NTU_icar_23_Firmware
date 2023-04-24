#ifndef __ANO_DT_H
#define __ANO_DT_H

#include "fsl_common.h"

/**
  * @brief    ����λ�����ͷ���8��short����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 /*,short data7, short data8, short .....�ɸ�����Ҫ������� */);

/**
  * @brief    ����λ�����ͷ���PID֡
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);


/**
  * @brief    �����ϱ�������λ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void Test_ANO_DT(void);


/**
  * @brief    ����������λ�������������ݰ�
  *
  * @param    data_buf �����ݰ�
  * @param    num      �����ݰ�����
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num);

/**
  * @brief    ���ڴ��ڽ����ж� һ���жϽ���һ���ַ�ʱ�������������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Data_Receive_Prepare(uint8_t data);
	
#endif

