#ifndef __VL53L0X_H__
#define __VL53L0X_H__


#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53_REG_DIS                                0x1E
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define VL53ADDR                                    0x29    //0x52   默认地址
#define VL53NEWADDR                                 0x30   

/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53(void);

/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     测试一路IIC接两个VL53  VL53模块的IIC地址可以用程序修改，但是上电的时候
  *           默认IIC地址都是0x29    所以修改地址的时候先失能一个模块，修改使能模块的
  *           IIC地址，修改完后，两个模块的地址不同了，就可以按不同地址操作了。
  *
  * @example  
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53_TWO(void);

/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data);



/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);

#endif
