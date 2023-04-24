/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
�������汾��V1.0
�������¡�2019��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "stdio.h"
#include "fsl_debug_console.h"
#include "LQ_9AX.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
#include "LQ_LED.h"

/* ����ʹ�õĽ��߷�ʽ��ѡ��ͬͷ�ļ� */
#if (IMU_USE_SELECT == USE_HAL_IIC)
#include "LQ_IIC.h"

#elif (IMU_USE_SELECT == USE_SOFT_IIC) 
#include "LQ_SOFTIIC.h"

#endif


/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);



/**
  * @brief    �������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
#ifdef TFT1_8
void Test_9AX(void)
{
    TFTSPI_Init(1);                //TFT1.8��ʼ��  
    TFTSPI_CLS(u16BLUE);           //����
    
#if (IMU_USE_SELECT == USE_HAL_IIC)
    LQ_IIC_Init(LPI2C1, 400000);
#elif (IMU_USE_SELECT == USE_SOFT_IIC) 
    IIC_Init(); 
#endif
    
    char  txt[30];
	short aacx,aacy,aacz;	         //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;        //������ԭʼ���� 
    short magx, magy, magz;         //�شż�ԭʼ���� 
    
    TFTSPI_P8X16Str(2,0,"LQ 9AX Test",u16RED,u16BLUE);

    if(LQ_9AX_Init())
    {
        TFTSPI_P8X16Str(2,0,"9AX Test Fail",u16RED,u16BLUE);
        while(1);   
    }
        
    while(1)
    {
        FX_8700_GetACCRaw(&aacx,&aacy,&aacz);	  //�õ����ٶȴ���������  
        FX_8700_GetMAGRaw(&magx, &magy, &magz);
        FX_21002_GetRaw(&gyrox, &gyroy, &gyroz);
        sprintf((char*)txt,"ax:%06d",aacx);
        TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"ay:%06d",aacy);
        TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"az:%06d",aacz);
        TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"gx:%06d",gyrox);
        TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"gy:%06d",gyroy);
        TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"gz:%06d",gyroz);
        TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"MX:%06d ",magx);  
        TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"MY:%06d ",magy); 
        TFTSPI_P8X16Str(0,8,txt,u16RED,u16BLUE);
        sprintf((char*)txt,"MZ:%06d ",magz);
        TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLUE);

        PRINTF("\r\nAX: %d  ",aacx); 
        PRINTF("\r\nAY: %d  ",aacy);
        PRINTF("\r\nAZ: %d  ",aacz); 
        PRINTF("\r\nGX: %d  ",gyrox);
        PRINTF("\r\nGY: %d  ",gyroy); 
        PRINTF("\r\nGZ: %d  ",gyroz);
        PRINTF("\r\nMX: %d  ",magx);
        PRINTF("\r\nMY: %d  ",magy); 
        PRINTF("\r\nMZ: %d  ",magz);
        delayms(100);
        PRINTF("\r\n*********************\r\n");
    }
    
}


#else

void Test_9AX(void)
{
    char  txt[30];
	short aacx,aacy,aacz;	         //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;        //������ԭʼ����
    short magx, magy, magz;         //�شż�ԭʼ����
    
    LCD_Init();                      //LCD��ʼ��
    LCD_CLS();                       //LCD����
    
#if (IMU_USE_SELECT == USE_HAL_IIC)
    LQ_IIC_Init(LPI2C1, 400000);
#elif (IMU_USE_SELECT == USE_SOFT_IIC) 
    IIC_Init(); 
#endif
    
    LCD_P8x16Str(15,0,"LQ 9AX Test"); 
    PRINTF("\r\nLQ 9AX Test\n");
    
    if(LQ_9AX_Init())
    {
    
        LCD_P8x16Str(15,2,"\n9AX Test Fail\n");
        while(1);
    }
        
    while(1)
    {
        FX_8700_GetACCRaw(&aacx,&aacy,&aacz);	  //�õ����ٶȴ���������  
        FX_8700_GetMAGRaw(&magx, &magy, &magz);
        FX_21002_GetRaw(&gyrox, &gyroy, &gyroz);
        
        sprintf((char*)txt,"ax:%06d",aacx);
        LCD_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",aacy);
        LCD_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",aacz);
        LCD_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyrox);
        LCD_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyroy);
        LCD_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyroz);
        LCD_P6x8Str(0,7,txt);
        sprintf((char*)txt,"MX:%5d ",magx);  
        LCD_P6x8Str(60,5,txt);
        sprintf((char*)txt,"MY:%5d ",magy);
        LCD_P6x8Str(60,6,txt); 
        sprintf((char*)txt,"MZ:%5d ",magz); 
        LCD_P6x8Str(60,7,txt);

        PRINTF("\r\nAX: %d  ",aacx); 
        PRINTF("\r\nAY: %d  ",aacy);
        PRINTF("\r\nAZ: %d  ",aacz); 
        PRINTF("\r\nGX: %d  ",gyrox);
        PRINTF("\r\nGY: %d  ",gyroy); 
        PRINTF("\r\nGZ: %d  ",gyroz);
        PRINTF("\r\nMX: %d  ",magx);
        PRINTF("\r\nMY: %d  ",magy); 
        PRINTF("\r\nMZ: %d  ",magz);
        delayms(100);
        PRINTF("\r\n*********************\r\n");
    }
    
}
#endif


/**
  * @brief    ����Ĵ�����ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t LQ_9AX_Init(void)
{
    FX_8700_Active(0);
    delayms(10);
    
    if(FX_8700_Check())
    {
        PRINTF("\n �����ʼ��ʧ�� \n");
        return 1;
    }
    FX_8700_SetRate(800, 2);   // 800Hz  �شźͼ��ٶȼƶ�ʹ��
    FX_8700_SetRange(4);       //�� 4g
    FX_8700_Active(1);
    
    
    FX_21002_Active(0);
    delayms(10);
    
    if(FX_21002_Check())
    {
        PRINTF("\n �����ʼ��ʧ�� \n");
        return 2;
    }
    FX_21002_SetRate(800);     // 800Hz  ���ٶȼ�
    FX_21002_SetRange(2000);   //�� 2000dps
    FX_21002_SetLP(2);         //��ͨ�˲�
    FX_21002_Active(1);
    delayms(10);
    return 0;
}










/**
  * @brief    ����FXOS8700 �Ƿ񼤻�
  *
  * @param    enable :  1������  0������ 
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_Active(bool enable)
{
    /* �����߷ֱ���ģʽ */
    FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG2, 0x02);
    
    uint8_t reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1);
    if(enable)
    {
        reg = reg | FXOS8701CQ_CTRL_REG1_ACTIVE;
    }
    else
    {
        reg = reg & ~FXOS8701CQ_CTRL_REG1_ACTIVE;
    }
    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1, reg);
}

/**
  * @brief    ��ȡ���ٶȼ�ԭʼ����
  *
  * @param    ax,ay,az:���ٶȼ�x,y,z���ԭʼ����(������)
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_GetACCRaw(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res = FX_Read_Len(FXOS8700_ADDR, FXOS8701CQ_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*ax = -((uint16_t)buf[0]<<8)|buf[1];  
		*ay = -((uint16_t)buf[2]<<8)|buf[3];  
		*az = ((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}


/**
  * @brief    ��ȡ�شżƼ�ԭʼ����
  *
  * @param    mx,my,mz:�شż�x,y,z���ԭʼ����(������)
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_GetMAGRaw(short *mx,short *my,short *mz)
{
    uint8_t buf[6],res;
    res = FX_Read_Len(FXOS8700_ADDR, FXOS8701CQ_M_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*mx = -((uint16_t)buf[0]<<8)|buf[1];  
		*my = -((uint16_t)buf[2]<<8)|buf[3];  
		*mz = ((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}



/**
  * @brief    ����FXOS8700 ����
  *
  * @param    dps :  �������� ��λ ��ÿ��
  *
  * @return   
  *
  * @note     �شżƺͼ��ٶȼ� ������һ���趨�� ��2g;��0.244 mg/LSB   ��4g;��0.488 mg/LSB   ��8g ��0.976 mg/LSB
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_SetRange(uint8_t mg)
{
    uint8_t reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_XYZ_DATA_CFG);
    reg = reg & ~XYZ_DATA_CFG_FS_MASK;
    if(mg <= 2)
    {
        reg |= XYZ_DATA_CFG_FS_2G;
    }
    else if(mg <= 4)
    {
        reg |= XYZ_DATA_CFG_FS_4G;
    }
    else 
    {
        reg |= XYZ_DATA_CFG_FS_8G;
    }
    
    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_XYZ_DATA_CFG, reg);

}


/**
  * @brief    ����FXOS8700 ��������
  *
  * @param    fps  ����������� ��λ 1��ˢ�¶��ٴ�
  * @param    mode 0: ��ʹ�ü��ٶȼ�  1����ʹ�õشż�  2�����ٶȼƺ͵شżƶ��ã��ᵼ��������ʼ��룩
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_SetRate(uint16_t fps, uint8_t mode)
{
    if(mode == 0)
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x00);
    }
    else if(mode == 1)
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x01);
    }
    else 
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x40|0x1C|0x03);
    }
    uint8_t reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1);
    reg &= ~CTRL_REG_DR_MASK;
    
    if(fps <= 6)
    {
        reg |= CTRL_REG1_DR(6);
    }
    else if(fps <= 12)
    {
        reg |= CTRL_REG1_DR(5);
    }
    else if(fps <= 50)
    {
        reg |= CTRL_REG1_DR(4);
    }
    else if(fps <= 100)
    {
        reg |= CTRL_REG1_DR(3);
    }
    else if(fps <= 200)
    {
        reg |= CTRL_REG1_DR(2);
    }
    else if(fps <= 400)
    {
        reg |= CTRL_REG1_DR(1);
    }
    else 
    {
        reg |= CTRL_REG1_DR(0);
    }
    reg &= ~0x02;
    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1, reg);
}


/**
  * @brief    FXOS8700 �Լ�
  *
  * @param    
  *
  * @return   0 : ����   ���������� 
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_8700_Check(void)
{
    uint8_t ID = 0;
    
    ID = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_WHOAMI);
    if(FXOS8700CQ_WHOAMI_VAL == ID || FXOS8701CQ_WHOAMI_VAL == ID)
    {
        PRINTF("\n FXOS8700 is OK\n");
        return 0;
    }
    else 
    {
        PRINTF("\n FXOS8700 ���ʧ�� ��⵽ID = %x \n" , ID);
    }
    return 1;
}


/*--------------------------------------------------------------------------------------
----------------------------------FXAS21002---------------------------------------------
----------------------------------------------------------------------------------------*/




/**
  * @brief    ��ȡ���ٶȼ�ԭʼ����
  *
  * @param    gyrox,gyroy,gyroz:������x,y,z���ԭʼ����(������)
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_GetRaw(short * gyrox, short *gyroy, short *gyroz)
{
    uint8_t buf[6],res;
    res = FX_Read_Len(FXAS21002C_ADDR, FXAS21002C_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*gyrox = ((uint16_t)buf[0]<<8)|buf[1];  
		*gyroy = ((uint16_t)buf[2]<<8)|buf[3];  
		*gyroz = ((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}


/**
  * @brief    ����FXAS21002 �Ƿ񼤻�
  *
  * @param    enable :  1������  0������
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_Active(bool enable)
{
    uint8_t reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1);
    if(enable)
    {
        reg = reg | CTRL_REG1_ACTIVE;
    }
    else
    {
        reg = reg & ~CTRL_REG1_READY;
    }
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1, reg);
}



/**
  * @brief    ����FXAS21002 ��ͨ �˲�
  *
  * @param    BW :  ���ý�ֹƵ�ʣ�����Ƶ�ʿ����� 
  *
  * @return   
  *
  * @note     ��ͬ����Ƶ����  ��ͨ�˲��������õ�ֵ���±�
  *    
  *    mode        ��������    800 Hz 400 Hz 200 Hz  100 Hz 50 Hz  25 Hz 12.5 Hz
  *     0          ��ֹƵ��      15    7.5    3.75   1.875  0.937 0.468  0.234
  *     1          ��ֹƵ��     7.7   3.85   1.925   0.963  0.481 0.241  0.120
  *     2          ��ֹƵ��     3.9   1.95   0.975   0.488  0.244 0.122  0.061
  *     3          ��ֹƵ��    1.98   0.99   0.495   0.248  0.124 0.062  0.031
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_SetHP(uint8_t mode)
{
    uint8_t reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg | CTRL_REG0_HPF_EN;
    reg |= (mode << CTRL_REG0_SEL_SHIFTS);
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);
}



/**
  * @brief    ����FXAS21002 ��ͨ �˲�
  *
  * @param    BW :  ���ý�ֹƵ�ʣ�����Ƶ�ʿ����� 
  *
  * @return   
  *
  * @note     ��ͬ����Ƶ����  ��ͨ�˲��������õ�ֵ���±�
  *  mode     ��������  800 Hz  400 Hz  200 Hz  100 Hz  50 Hz  25 Hz  12.5 Hz
  *  0        ��ֹƵ��  256     128      64      32     16      8        4
  *  1        ��ֹƵ��  128      64      32      16      8      4       ��
  *  2        ��ֹƵ��   64      32      16       8      4     ��       ��
  *
  * @example  FX_21002_SetLP(0) //����������� 800Hz ���ͨ�˲���ֹƵ�� 256
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_SetLP(uint8_t mode)
{
    uint8_t reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg & ~CTRL_REG0_BW_MASK;
    reg |= (mode << CTRL_REG0_BW_SHIFTS);
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);
}



/**
  * @brief    ����FXAS21002 ����
  *
  * @param    dps :  �������� ��λ ��ÿ��
  *
  * @return   
  *
  * @note     ��250dps;1,��500dps;2,��1000dps;3,��2000dps
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_SetRange(uint16_t dps)
{
    uint8_t reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg & ~CTRL_REG0_FS_MASK;
    if(dps <= 250)
    {
        reg |= CTRL_REG0_FS_250_DPS;
    }
    else if(dps <= 500)
    {
        reg |= CTRL_REG0_FS_500_DPS;
    }
    else if(dps <= 1000)
    {
        reg |= CTRL_REG0_FS_1000_DPS;
    }
    else 
    {
        reg |= CTRL_REG0_FS_2000_DPS;
    }
    
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);

}


/**
  * @brief    ����FXAS21002 ��������
  *
  * @param    fps ����������� ��λ 1��ˢ�¶��ٴ�
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_SetRate(uint16_t fps)
{
    uint8_t reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1);
    reg &= ~CTRL_REG_DR_MASK;
    
    if(fps <= 13)
    {
        reg |= CTRL_REG_DR_12_5;
    }
    else if(fps <= 25)
    {
        reg |= CTRL_REG_DR_25HZ;
    }
    else if(fps <= 50)
    {
        reg |= CTRL_REG_DR_50HZ;
    }
    else if(fps <= 100)
    {
        reg |= CTRL_REG_DR_100HZ;
    }
    else if(fps <= 200)
    {
        reg |= CTRL_REG_DR_200HZ;
    }
    else if(fps <= 400)
    {
        reg |= CTRL_REG_DR_400HZ;
    }
    else 
    {
        reg |= CTRL_REG_DR_800HZ;
    }
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1, reg);
}



/**
  * @brief    FXAS21002 �Լ�
  *
  * @param    
  *
  * @return   0 : ����   ���������� 
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/13 ������
  */
uint8_t FX_21002_Check(void)
{
    uint8_t ID = 0;
    
    ID = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_WHO_AM_I);
    if(FXAS21002C_ID == ID)
    {
        PRINTF("\n FXAS21002 is OK\n");
        return 0;
    }
    else
    {
        PRINTF("\n FXAS21002 ���ʧ�� ��⵽ID = %x \n" , ID);
    }
    return 1;
}






/*--------------------------------------------------------------------------------------
----------------------------------�ײ���������------------------------------------------
----------------------------------------------------------------------------------------*/



#if (IMU_USE_SELECT == USE_HAL_IIC)

/**
  * @brief    IIC ������
  *
  * @param    addr:������ַ 
  * @param    reg :Ҫ��ȡ�ļĴ�����ַ
  * @param    len :Ҫ��ȡ�ĳ���
  * @param    buf :��ȡ�������ݴ洢��
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadReg(LPI2C1, addr, reg, buf, len);     
}



/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ 
  * @param    reg   :Ҫд�ļĴ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteReg(LPI2C1, addr, reg, value);    
}

/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ 
  * @param    reg   :Ҫд�ļĴ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value[1];
    IIC_ReadReg(LPI2C1, addr, reg, value, 1);
    return value[0];
}



#elif (IMU_USE_SELECT == USE_SOFT_IIC) 


/**
  * @brief    IIC ������
  *
  * @param    addr:������ַ 
  * @param    reg :Ҫ��ȡ�ļĴ�����ַ
  * @param    len :Ҫ��ȡ�ĳ���
  * @param    buf :��ȡ�������ݴ洢��
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);     
}



/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ 
  * @param    reg   :Ҫд�ļĴ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);    
}


/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ 
  * @param    reg   :Ҫд�ļĴ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
uint8_t FX_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value[1];
    FX_Read_Len(addr, reg, 1, value);
    return value[0];
}

#endif