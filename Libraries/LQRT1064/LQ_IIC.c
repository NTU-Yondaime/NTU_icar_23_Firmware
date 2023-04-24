/*--------------------------------------------------------------
��ƽ    ̨�������������ܿƼ�RT1064���İ�
����    д��Z
����    ע��
������汾��V1.0
�������¡�2018��12��17��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
���������䡿chiusir@163.com
----------------------------------------------------------------*/
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "LQ_IIC.h"


/**
  * @brief    IIC ��ʼ������
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    baudrate:   IIC������
  *
  * @return   
  *
  * @note     IIC Ĭ��ʱ��Դ 10M
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void LQ_IIC_Init(LPI2C_Type *base, uint32_t baudrate)
{
    IIC_PinInit(base);
    
    LPI2C_MasterDeinit(base);
    
    /* ����IIC ʱ�� ʹ�� 60M / 6 */
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
    
    /* ���ò����� */
    i2cConfig.baudRate_Hz = baudrate;
 
//    i2cConfig.debugEnable = true;
    
    LPI2C_MasterInit(base, &i2cConfig, 10000000UL);   

}




lpi2c_master_transfer_t master_transfer; //����ṹ��   
    
    
    
/**
  * @brief    IIC ��n���Ĵ���
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    addr    ��  ������ַ����7λ�� 
  * @param    reg     ��  Ҫ���ļĴ��� 
  * @param    val     ��  ��Ŷ������ݵĵ�ַ 
  * @param    len     ��  ��ȡ����  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
status_t IIC_ReadReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t *val, uint8_t len)  
{
    
    status_t status = kStatus_Success;
    
    /* ����IIC����ṹ�� */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //�Ĵ�����ַ
    master_transfer.subaddressSize = 1U;                     //��8λ��
    master_transfer.data = val;                              //��Ŷ���������
    master_transfer.dataSize = len;                          //Ҫ�����ٸ�����
    master_transfer.direction = kLPI2C_Read;                 //I2C��ģʽ
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(base,&master_transfer) == kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}


/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  * @param    addr    ��  ������ַ����7λ�� 
  * @param    reg     ��  Ҫд�ļĴ��� 
  * @param    val     ��  Ҫд���ֵ  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
status_t IIC_WriteReg(LPI2C_Type *base, uint16_t addr, uint8_t reg, uint8_t val)    
{   
    status_t status = kStatus_Success;
    
    uint8_t buff[1];

    buff[0] = val;
    /* ����IIC����ṹ�� */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //�Ĵ�����ַ
    master_transfer.subaddressSize = 1U;                     //��8λ��
    master_transfer.data = buff;                             //Ҫд�������
    master_transfer.dataSize = 1;                            //Ҫд���ٸ�����
    master_transfer.direction = kLPI2C_Write;                //I2Cдģʽ
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(base,&master_transfer) == kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}


    
    

/**
  * @brief    IIC �ܽų�ʼ��
  *
  * @param    base    ��  LPI2C1~ LPI2C4  
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
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








