#ifndef __LQ_LED_H__
#define __LQ_LED_H__

//������ɫö��
typedef enum
{
  white = 0,  //��ɫ
  black = 1,  //��ɫ
  red   = 2,  //��
  green = 3,  //��
  blue  = 4,  //��ɫ 
  yellow= 5,  //��ɫ
  violet= 6,  //��ɫ
  cyan  = 7,  //��ɫ
}LED_t;

/**
  * @brief    ����LED��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void Test_LED(void);


/**
  * @brief    ָ����ɫ��˸
  *
  * @param    color  �� ö��LED_t�е���ɫ
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void LED_ColorReverse(LED_t color);


/**
  * @brief    ָ����ɫ�ĵ���
  *
  * @param    color  �� ö��LED_t�е���ɫ
  *
  * @return   
  *
  * @note     
  *
  * @example  LED_Color(red);   //�����
  *
  * @date     2019/6/4 ���ڶ�
  */
void LED_Color(LED_t color);


/**
  * @brief    RGB�����ų�ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO2_IO23  -------->  �ܽ�B12   ----- >  ���İ�G��
  * @note     GPIO3_IO26  -------->  �ܽ�A7    ----- >  ���İ�R��
  * @note     GPIO3_IO27  -------->  �ܽ�C7    ----- >  ���İ�B��
  * @note    
  * @note     GPIO2_IO22  -------->  �ܽ�C12   ----- >  ĸ��D0
  * @note     GPIO2_IO25  -------->  �ܽ�A13   ----- >  ĸ��D1
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void LED_Init(void);

#endif 
