/*--------------------------------------------------------------
【平    台】北京龙邱智能科技RT1064核心板
【编    写】Z
【备    注】
【软件版本】V1.0
【最后更新】2018年12月17日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
----------------------------------------------------------------*/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "LQ_IIC.h"


/**
  * @brief    IIC 初始化函数
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    baudrate:   IIC波特率
  *
  * @return   
  *
  * @note     IIC 默认时钟源 10M
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void LQ_IIC_Init(LPI2C_Type *base, uint32_t baudrate)
{
    IIC_PinInit(base);
    
    LPI2C_MasterDeinit(base);
    
    /* 配置IIC 时钟 使用 60M / 6 */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 5);
    
    lpi2c_master_config_t i2cConfig;
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    
    LPI2C_MasterGetDefaultConfig(&i2cConfig);         
    
    /* 配置波特率 */
    i2cConfig.baudRate_Hz = baudrate;
 
//    i2cConfig.debugEnable = true;
    
    LPI2C_MasterInit(base, &i2cConfig, 10000000UL);   

}




lpi2c_master_transfer_t master_transfer; //传输结构体   
    
    
    
/**
  * @brief    IIC 读n个寄存器
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要读的寄存器 
  * @param    val     ：  存放读出数据的地址 
  * @param    len     ：  读取长度  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_ReadReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t *val, uint8_t len)  
{
    
    status_t status = kStatus_Success;
    
    /* 配置IIC传输结构体 */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //寄存器地址
    master_transfer.subaddressSize = 1U;                     //按8位读
    master_transfer.data = val;                              //存放读出的数据
    master_transfer.dataSize = len;                          //要读多少个数据
    master_transfer.direction = kLPI2C_Read;                 //I2C读模式
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(base,&master_transfer) == kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}


/**
  * @brief    IIC 写一个寄存器
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要写的寄存器 
  * @param    val     ：  要写入的值  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_WriteReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t val)    
{   
    status_t status = kStatus_Success;
    
    uint8_t buff[1];

    buff[0] = val;
    /* 配置IIC传输结构体 */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //寄存器地址
    master_transfer.subaddressSize = 1U;                     //按8位读
    master_transfer.data = buff;                             //要写入的数据
    master_transfer.dataSize = 1;                            //要写多少个数据
    master_transfer.direction = kLPI2C_Write;                //I2C写模式
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(base,&master_transfer) == kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}


    
    

/**
  * @brief    IIC 管脚初始化
  *
  * @param    base    ：  LPI2C1~ LPI2C4  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 星期三
  */
void IIC_PinInit(LPI2C_Type *base)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    
    if(base == LPI2C1)
    {
        if(IIC1_SCL == J11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,0xD8B0u);
        }
        else if(IIC1_SCL == P2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_LPI2C1_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_LPI2C1_SCL,0xD8B0u);
        }
        
        if(IIC1_SDA == K11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,0xD8B0u);
        }
        else if(IIC1_SDA == N3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_LPI2C1_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_LPI2C1_SDA,0xD8B0u);
        }
    }
    else if(base == LPI2C2)
    {
        if(IIC2_SCL == C8)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LPI2C2_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LPI2C2_SCL,0xD8B0u);
        }
        else if(IIC2_SCL == P5)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_LPI2C2_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_LPI2C2_SCL,0xD8B0u);
        }
        
        if(IIC2_SDA == B8)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LPI2C2_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LPI2C2_SDA,0xD8B0u);
        }
        else if(IIC2_SDA == P4)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_LPI2C2_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_LPI2C2_SDA,0xD8B0u);
        }
    }
    else if(base == LPI2C3)
    {
        if(IIC3_SCL == F1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_22_LPI2C3_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_22_LPI2C3_SCL,0xD8B0u);
        }
        else if(IIC3_SCL == J4)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_LPI2C3_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_LPI2C3_SCL,0xD8B0u);
        }
        else if(IIC3_SCL == K10)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL,0xD8B0u);
        }
        
        if(IIC3_SDA == C1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_21_LPI2C3_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_21_LPI2C3_SDA,0xD8B0u);
        }
        else if(IIC3_SDA == J3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_LPI2C3_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_LPI2C3_SDA,0xD8B0u);
        }
        else if(IIC3_SDA == J12)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA,0xD8B0u);
        }
    }
    else if(base == LPI2C4)
    {
        if(IIC4_SCL == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_LPI2C4_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_12_LPI2C4_SCL,0xD8B0u);
        }
        else if(IIC4_SCL == K14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPI2C4_SCL,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPI2C4_SCL,0xD8B0u);
        }
        
        if(IIC4_SDA == G3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_LPI2C4_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_11_LPI2C4_SDA,0xD8B0u);
        }
        else if(IIC4_SDA == L14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPI2C4_SDA,1U);                                   
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPI2C4_SDA,0xD8B0u);
        }
    }
}








