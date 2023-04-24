/*------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
��dev.env.��IAR8.30.1�����ϰ汾
��Target �� i.MX RT1064
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 600MHz
��USB PLL�� 480MHz
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
---------------------------------------------------------*/
#include "LQ_MT9V034M.h"
#include "include.h"
#include "LQ_SGP18T.h"
#include "LQ_12864.h"
#include "fsl_sccb.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_csi_camera_adapter.h"

/* �Աȶ� �߶Աȶ�0x03c7  �ͶԱȶ�0x01c7  ע�� �߶ԱȶȻ�ʹͼ��䰵 */
#define CAMERA_CONTRAST          0x01c7

/* �Զ��ع⿪�� Ĭ�ϴ�  ����Ϊ0 �ر��Զ��ع� */
#define CAMERA_AUTO_EXPOSURE     1

/* �Զ��ع�ģʽ�� ���� ���ڷ�Χ 1-64 */
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  30

/* �������Զ��ع����������ù̶�֡�� */
/* �Զ��ع�ģʽ��Ҫ�����ع�ʱ������ ���ڷ�Χ 1�C32765 */
/* ע�� ��֡�ʹ���ʱ ������õ��ع�ʱ����� ֡�ʿ��ܻ�����Ӧ�µ� */
/* ����ʹ��100֡ʱ ����ع�ʱ�䳬��317 �ᵼ��֡���½� */
/* �ع�ʱ��Խ�� ͼ��Խ�� */
#define CAMERA_MAX_EXPOSURE_TIME  300
#define CAMERA_MIN_EXPOSURE_TIME  1

/* ���Զ��ع�ģʽ�� ���Ե����ع�ʱ��������ͼ���������� ���ڷ�Χ 0�C32765 */
/* ע�� ��֡�ʹ���ʱ ������õ��ع�ʱ����� ֡�ʿ��ܻ�����Ӧ�µ� */
/* �ع�ʱ��Խ�� ͼ��Խ�� */
#define CAMERA_EXPOSURE_TIME  150


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t LQMT9V034_Init(camera_device_handle_t *handle, const camera_config_t *config);
status_t LQMT9V034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);
status_t LQMT9V034_Deinit(camera_device_handle_t *handle);
status_t LQMT9V034_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);
status_t LQMT9V034_Start(camera_device_handle_t *handle);
status_t LQMT9V034_Stop(camera_device_handle_t *handle);
/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t LQMT9V034_ops = {
    .init    = LQMT9V034_Init,   
    .deinit  = LQMT9V034_Deinit,
    .start   = LQMT9V034_Start,
    .stop    = LQMT9V034_Stop,
    .control = LQMT9V034_Control,
    .init_ext= LQMT9V034_InitExt,
};
//I2C д16λ�Ĵ�������  address��Ҫд�Ĵ���   Data��Ҫд��ֵ
static unsigned char MTV_IICWriteReg16(unsigned char address, uint16_t Data)  //I2C д16λ�Ĵ�������
{
  uint8_t buf[2];
  buf[0]=(uint8_t)(Data >> 8);
  buf[1]=(uint8_t)(Data);
  unsigned char result =SCCB_WriteMultiRegs(LPI2C1, MTV_I2C_ADDR, kSCCB_RegAddr8Bit,address,buf,2);
  return result;
}
//I2C д16λ�Ĵ������� Device��������ַ  address��Ҫд�Ĵ���   Data��Ҫд��ֵ
static void SCCB_RegWrite(uint8_t Device,uint8_t Address,uint16_t Data)
{
    MTV_IICWriteReg16(Address, Data);
}

//I2C ��16λ�Ĵ�������  address��Ҫ���Ĵ���   Data������ֵ�Ļ�����
static unsigned char  MTV_IICReadReg16(unsigned char address, uint16_t *Data)
{  
  uint8_t buf[2];
  unsigned char result =SCCB_ReadMultiRegs(LPI2C1, MTV_I2C_ADDR, kSCCB_RegAddr8Bit,address,buf,2); 
  *Data= ((buf[0]<<8)| buf[1]);
  return result;
}
//I2C ��16λ�Ĵ������� Device��������ַ  address��Ҫ���Ĵ���   *Data��������ֵ
static unsigned char SCCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data)
{
    return MTV_IICReadReg16(Address, Data);
}

status_t LQMT9V034_Deinit(camera_device_handle_t *handle)    
{
    ((LQMT9V034_resource_t *)(handle->resource))->pullPowerDownPin(true);
    return kStatus_Success;
}

status_t LQMT9V034_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{   
    return kStatus_InvalidArgument;
}

status_t LQMT9V034_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t LQMT9V034_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t LQMT9V034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return LQMT9V034_Init(handle, config);
}
/*@@
  * @brief    ֡������
  *
  * @param    fps : ֡��  
  *
  * @return   
  *
  * @note     ֡����ʵ��ͨ�����ӿհ���ʵ�ֵģ� ������ʵ���֡����������֮��Ĺ�ϵ
  * ֡��	  200    150    100    70.9    50    20    10
  * ������    39     92     199    330     515   1450  3000
  *
  * @example  
  *
  * @date     2019/7/26 ������
  */
void MT9V034_SetFrameRate(uint8_t fps)
{
    float vertical = 0;
    if(fps > 200)
    {
        vertical = 2;
    }
    else if(fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if(fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if(fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if(fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if(fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else 
    {
        vertical = -155 * fps + 4550;
    }
    /* ������ ����֡��ʱ�� 2�C32288 */   
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (uint16_t)vertical); 
}

/*------------------------------------------------------------------------------------------------------
����    ����MT9V034_SetAutoExposure
����    �ܡ��Զ��ع�
����    ����enable    0���ر��Զ��ع�  
���� �� ֵ����
��ʵ    ����
��ע�����
--------------------------------------------------------------------------------------------------------*/
void MT9V034_SetAutoExposure(char enable)
{
    uint16_t reg =0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
    if(enable)
    {
        /* �����Զ��ع��Զ����� */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
        /* ����ع�ʱ�� �޸���������޸ıȽϰ�ʱ��ͼ����������*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* ��С�ع�ʱ�� �޸���������޸�����ǿ��ʱ��ͼ����������*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* ������� �������� ͼ��ƫ��������±��������ϸ�� ���ǿ��ܲ������ 0-60*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  ����AEC/AGCֱ��ͼ������Ŀ,22560 ���44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);
         
    }
    else
    {
        /* �ر��Զ��ع� */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
        
        /* 0xAB  ���ģ������     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);  
        
        /* 0x0B �����ع�ʱ�� 0�C32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,CAMERA_EXPOSURE_TIME);   
  
    }
    
}

/*------------------------------------------------------------------------------------------------------
����    ����LQMT9V034_Init
����    �ܡ�
����    ����
���� �� ֵ����
��ʵ    ����
��ע�����
--------------------------------------------------------------------------------------------------------*/
status_t LQMT9V034_Init(camera_device_handle_t *handle, const camera_config_t *config)
{     
    uint16_t data = 0;  
	 
	if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)  //��ȡ����ͷ�汾�Ĵ��� 
	{     
		if(data != MT9V034_CHIP_ID)                                        //оƬID����ȷ��˵��û����ȷ��ȡ�����ݣ�������      
		{ 
#ifdef LQ_TFT1_8
			TFTSPI_P8X8Str(0,0,"V034 NG!!",u16RED,u16BLUE);
#else
			LCD_P6x8Str(2,1,"V034 NG!!");                       
#endif
			while(1); 
		} 
		else                                                               //оƬID��ȷ
		{
#ifdef LQ_TFT1_8
			TFTSPI_P8X8Str(0,0,"V034 OK",u16RED,u16BLUE);
#else
			LCD_P6x8Str(2,1,"V034 OK");                              
#endif
		}
	} 
	else 
	{ 
#ifdef LQ_TFT1_8
		TFTSPI_P8X8Str(0,0,"V034 NG!!",u16RED,u16BLUE);
#else
		LCD_P6x8Str(2,1,"V034 NG!!");                      
#endif
		while(1);                                                         //����ͷʶ��ʧ�ܣ�ֹͣ����
	}  
    
    /* ��������ͷͼ��4*4��Ƶ���PCLK 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� */
	MT9V034_SetFrameResolution(IMAGEH, IMAGEW); 
    
    /* ����֡�� */
    MT9V034_SetFrameRate(CAMERA_FPS);
        
    /* �ع����� */
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //�ο���ѹ����   1.4v 
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //����ʴ     
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);      //0x1C  here is the way to regulate darkness :)    
    ////  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,CAMERA_CONTRAST);//0x01C7�ԱȶȲ���ף�0x03C7�Աȶ���� Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
     
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
    
   
    /* 0xA5  ͼ������  60  1-64  */
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS); 
          
    return kStatus_Success;
}
/*------------------------------------------------------------------------------------------------------
����    ����MT9V034_SetFrameResolution
����    �ܡ�����ͷ���ڴ�С���ú�ͼ��ת����
����    ����height  ͼ��߶�
����    ����width   ͼ����  
���� �� ֵ����
��ʵ    ����
��ע�����
--------------------------------------------------------------------------------------------------------*/
static void MT9V034_SetFrameResolution(uint16_t height,uint16_t width)
{
    uint16_t data = 0;
    
    if((height*4) <= MAX_IMAGE_HEIGHT)   //�ж����Ƿ�4��Ƶ
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if((height*2)<=MAX_IMAGE_HEIGHT )  //�ж����Ƿ�2��Ƶ
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }
    
    if((width*4)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�4��Ƶ   
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�2��Ƶ
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }
     
    //         ˮƽ��ת                     ��ֱ��ת
    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);  //��Ҫ��ת�Ŀ��Դ�ע��
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //д�Ĵ����������з�Ƶ
    
    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH,  width);  //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�
    
    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //�п�ʼ
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //�п�ʼ
     
}


                      
                      
                      
                      

