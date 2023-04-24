#ifndef __LQ_FLASH_H
#define __LQ_FLASH_H


/**
  * @brief    擦出扇区
  *
  * @param    sector_num ： 扇区编号  0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区）
  *
  * @return   0:擦出成功    其他失败
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_EraseSector(uint16_t sector_num);

/**
  * @brief    擦出整块flash
  *
  * @param    
  *
  * @return   
  *
  * @note     时间很长
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_EraseAll(void);

/**
  * @brief    读取数据
  *
  * @param    sector_num ： 扇区编号      0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区） 
  * @param    page       ： 扇区内页编号  0 - 15  （一个扇区4K  一页256字节 一共16页） 
  * @param    offset     ： 页中第几个字节  0 - 255（1一页256个字节） 
  * @param    buff       ： 读出数据存放地址 
  * @param    len        ： 读出长度
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
void FLASH_ReadBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len);



/**
  * @brief    写数据
  *
  * @param    sector_num ： 扇区编号      0 - 4095（16MB FLASH  一个扇区4K 一共4096个扇区） 
  * @param    page       ： 扇区内页编号  0 - 15  （一个扇区4K  一页256字节 一共16页） 
  * @param    offset     ： 页中第几个字节  0 - 255（1一页256个字节） 
  * @param    buff       ： 写入数据存放地址 
  * @param    len        ： 写入长度
  *
  * @return   
  *
  * @note     写之前要擦除
  *           flash 为什么写之前要擦除，因为flash只能写0, 擦除的时候flash变成0xff 就可以写0改变数据了
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int FLASH_WriteBuff(uint16_t sector_num, uint16_t page, uint16_t offset, uint8_t *buff, uint32_t len);



/**
  * @brief    16M  外部FLASH初始化
  *
  * @param    
  *
  * @return   0：初始化OK   其他错误
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
int LQ_FLASH_Init(void);

/**
  * @brief    flash 读写测试
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/20 星期四
  */
int Test_Flash(void);




/**
  * @brief    flash 管脚初始化 
  *
  * @param    
  *
  * @return   
  *
  * @note     内部调用 
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
void  FLASH_PinInit(void);



#endif
