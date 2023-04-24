#ifndef __LQ_ICM20602_H
#define __LQ_ICM20602_H

#include "fsl_common.h"


//****************************************
// 定义ICM20602内部地址
//****************************************
//MPU6500的内部寄存器
#define ICM_SELF_TESTX_REG		0X0D	//自检寄存器X
#define ICM_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define ICM_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define ICM_SELF_TESTA_REG		0X10	//自检寄存器A
#define ICM_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define ICM_CFG_REG				0X1A	//配置寄存器
#define ICM_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define ICM_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define ICM_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define ICM_FIFO_EN_REG			0X23	//FIFO使能寄存器

#define ICM_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define ICM_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define ICM_INT_EN_REG			0X38	//中断使能寄存器
#define ICM_INT_STA_REG			0X3A	//中断状态寄存器

#define ICM_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define ICM_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define ICM_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define ICM_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define ICM_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define ICM_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define ICM_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define ICM_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define ICM_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define ICM_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define ICM_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define ICM_USER_CTRL_REG		0X6A	//用户控制寄存器
#define ICM_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define ICM_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define ICM_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define ICM_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define ICM_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define WHO_AM_I		        0X75	//器件ID寄存器



/**
  * @brief    读取ICM20602或者ICM20602原始数据 测试
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void Test_ICM20602(void);



/**
  * @brief    初始化ICM20602 或者 ICM20602
  *
  * @param    
  *
  * @return   
  *
  * @note    0：初始化成功   1：失败  
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM20602_Init(void);




/**
  * @brief    设置陀螺仪测量范围
  *
  * @param    fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
  *
  * @return   0 ：设置成功  
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Set_Gyro_Fsr(uint8_t fsr);





/**
  * @brief    设置加速度计测量范围
  *
  * @param    fsr:0,±2g;1,±4g;2,±8g;3,±16g
  *
  * @return   0：设置成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Set_Accel_Fsr(uint8_t fsr);





/**
  * @brief    设置数字低通滤波
  *
  * @param    lpf:数字低通滤波频率(Hz) 
  *
  * @return   0：设置成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Set_LPF(uint16_t lpf);




/**
  * @brief    设置采样率
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0：设置成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Set_Rate(uint16_t rate);





/**
  * @brief    获取温度值
  *
  * @param    
  *
  * @return   温度值(扩大了100倍)
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
short ICM_Get_Temperature(void);




/**
  * @brief    获取陀螺仪值
  *
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Get_Gyroscope(short *gx,short *gy,short *gz);



/**
  * @brief    获取加速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Get_Accelerometer(short *ax,short *ay,short *az);




/**
  * @brief    获取 加速度值 角速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)  
  *
  * @return   0：读取成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz);



/**
  * @brief    SPI 连续读
  *
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);




/**
  * @brief    SPI 写一个寄存器
  *
  * @param    reg   :要写的寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Write_Byte(uint8_t reg,uint8_t value);




/**
  * @brief    SPI 读一个寄存器
  *
  * @param    reg   :要写的寄存器地址
  *
  * @return   读取的值
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
uint8_t ICM_Read_Byte(uint8_t reg);

#endif

