#ifndef __LQ_FLASH_H
#define __LQ_FLASH_H


/**
  * @brief    ��������
  *
  * @param    sector_num �� �������  0 - 4095��16MB FLASH  һ������4K һ��4096��������
  *
  * @return   0:�����ɹ�    ����ʧ��
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
int FLASH_EraseSector(uint16_t sector_num);

/**
  * @brief    ��������flash
  *
  * @param    
  *
  * @return   
  *
  * @note     ʱ��ܳ�
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
int FLASH_EraseAll(void);

/**
  * @brief    ��ȡ����
  *
  * @param    sector_num �� �������      0 - 4095��16MB FLASH  һ������4K һ��4096�������� 
  * @param    page       �� ������ҳ���  0 - 15  ��һ������4K  һҳ256�ֽ� һ��16ҳ�� 
  * @param    offset     �� ҳ�еڼ����ֽ�  0 - 255��1һҳ256���ֽڣ� 
  * @param    buff       �� �������ݴ�ŵ�ַ 
  * @param    len        �� ��������
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
void FLASH_ReadBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len);



/**
  * @brief    д����
  *
  * @param    sector_num �� �������      0 - 4095��16MB FLASH  һ������4K һ��4096�������� 
  * @param    page       �� ������ҳ���  0 - 15  ��һ������4K  һҳ256�ֽ� һ��16ҳ�� 
  * @param    offset     �� ҳ�еڼ����ֽ�  0 - 255��1һҳ256���ֽڣ� 
  * @param    buff       �� д�����ݴ�ŵ�ַ 
  * @param    len        �� д�볤��
  *
  * @return   
  *
  * @note     д֮ǰҪ����
  *           flash Ϊʲôд֮ǰҪ��������Ϊflashֻ��д0, ������ʱ��flash���0xff �Ϳ���д0�ı�������
  * @example  
  *
  * @date     2019/6/20 ������
  */
int FLASH_WriteBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len);



/**
  * @brief    16M  �ⲿFLASH��ʼ��
  *
  * @param    
  *
  * @return   0����ʼ��OK   ��������
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/19 ������
  */
int LQ_FLASH_Init(void);

/**
  * @brief    flash ��д����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 ������
  */
int Test_Flash(void);




/**
  * @brief    flash �ܽų�ʼ�� 
  *
  * @param    
  *
  * @return   
  *
  * @note     �ڲ����� 
  *
  * @example  
  *
  * @date     2019/6/19 ������
  */
void  FLASH_PinInit(void);



#endif
