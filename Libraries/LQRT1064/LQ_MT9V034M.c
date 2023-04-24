/*------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
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

/* 对比度 高对比度0x03c7  低对比度0x01c7  注意 高对比度会使图像变暗 */
#define CAMERA_CONTRAST          0x01c7

/* 自动曝光开关 默认打开  设置为0 关闭自动曝光 */
#define CAMERA_AUTO_EXPOSURE     1

/* 自动曝光模式下 亮度 调节范围 1-64 */
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  30

/* 可以在自动曝光的情况下设置固定帧率 */
/* 自动曝光模式需要设置曝光时间上限 调节范围 1–32765 */
/* 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/* 例如使用100帧时 最大曝光时间超过317 会导致帧率下降 */
/* 曝光时间越长 图像越亮 */
#define CAMERA_MAX_EXPOSURE_TIME  300
#define CAMERA_MIN_EXPOSURE_TIME  1

/* 非自动曝光模式下 可以调节曝光时间来调节图像整体亮度 调节范围 0–32765 */
/* 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/* 曝光时间越长 图像越亮 */
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
//I2C 写16位寄存器函数  address：要写寄存器   Data：要写的值
static unsigned char MTV_IICWriteReg16(unsigned char address, uint16_t Data)  //I2C 写16位寄存器函数
{
  uint8_t buf[2];
  buf[0]=(uint8_t)(Data >> 8);
  buf[1]=(uint8_t)(Data);
  unsigned char result =SCCB_WriteMultiRegs(LPI2C1, MTV_I2C_ADDR, kSCCB_RegAddr8Bit,address,buf,2);
  return result;
}
//I2C 写16位寄存器函数 Device：器件地址  address：要写寄存器   Data：要写的值
static void SCCB_RegWrite(uint8_t Device,uint8_t Address,uint16_t Data)
{
    MTV_IICWriteReg16(Address, Data);
}

//I2C 读16位寄存器函数  address：要读寄存器   Data：读出值的缓冲区
static unsigned char  MTV_IICReadReg16(unsigned char address, uint16_t *Data)
{  
  uint8_t buf[2];
  unsigned char result =SCCB_ReadMultiRegs(LPI2C1, MTV_I2C_ADDR, kSCCB_RegAddr8Bit,address,buf,2); 
  *Data= ((buf[0]<<8)| buf[1]);
  return result;
}
//I2C 读16位寄存器函数 Device：器件地址  address：要读寄存器   *Data：读出的值
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
  * @brief    帧率设置
  *
  * @param    fps : 帧率  
  *
  * @return   
  *
  * @note     帧率其实是通过增加空白行实现的， 下面是实测的帧率与虚拟行之间的关系
  * 帧率	  200    150    100    70.9    50    20    10
  * 虚拟行    39     92     199    330     515   1450  3000
  *
  * @example  
  *
  * @date     2019/7/26 星期五
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
    /* 虚拟行 设置帧率时间 2–32288 */   
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (uint16_t)vertical); 
}

/*------------------------------------------------------------------------------------------------------
【函    数】MT9V034_SetAutoExposure
【功    能】自动曝光
【参    数】enable    0：关闭自动曝光  
【返 回 值】无
【实    例】
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void MT9V034_SetAutoExposure(char enable)
{
    uint16_t reg =0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
    if(enable)
    {
        /* 开启自动曝光自动增益 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
        /* 最大曝光时间 修改这里可以修改比较暗时的图像整体亮度*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* 最小曝光时间 修改这里可以修改遇到强光时的图像整体亮度*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* 最大增益 增大这里 图像偏暗的情况下保留更多的细节 但是可能产生噪点 0-60*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  用于AEC/AGC直方图像素数目,22560 最大44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);
         
    }
    else
    {
        /* 关闭自动曝光 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
        
        /* 0xAB  最大模拟增益     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);  
        
        /* 0x0B 设置曝光时间 0–32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,CAMERA_EXPOSURE_TIME);   
  
    }
    
}

/*------------------------------------------------------------------------------------------------------
【函    数】LQMT9V034_Init
【功    能】
【参    数】
【返 回 值】无
【实    例】
【注意事项】
--------------------------------------------------------------------------------------------------------*/
status_t LQMT9V034_Init(camera_device_handle_t *handle, const camera_config_t *config)
{     
    uint16_t data = 0;  
	 
	if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)  //读取摄像头版本寄存器 
	{     
		if(data != MT9V034_CHIP_ID)                                        //芯片ID不正确，说明没有正确读取导数据，检查接线      
		{ 
#ifdef LQ_TFT1_8
			TFTSPI_P8X8Str(0,0,"V034 NG!!",u16RED,u16BLUE);
#else
			LCD_P6x8Str(2,1,"V034 NG!!");                       
#endif
			while(1); 
		} 
		else                                                               //芯片ID正确
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
		while(1);                                                         //摄像头识别失败，停止运行
	}  
    
    /* 设置摄像头图像4*4分频输出PCLK 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像 */
	MT9V034_SetFrameResolution(IMAGEH, IMAGEW); 
    
    /* 设置帧率 */
    MT9V034_SetFrameRate(CAMERA_FPS);
        
    /* 曝光设置 */
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //参考电压设置   1.4v 
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //反向腐蚀     
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);      //0x1C  here is the way to regulate darkness :)    
    ////  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,CAMERA_CONTRAST);//0x01C7对比度差，发白；0x03C7对比度提高 Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
     
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
    
   
    /* 0xA5  图像亮度  60  1-64  */
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS); 
          
    return kStatus_Success;
}
/*------------------------------------------------------------------------------------------------------
【函    数】MT9V034_SetFrameResolution
【功    能】摄像头窗口大小设置和图像翻转设置
【参    数】height  图像高度
【参    数】width   图像宽度  
【返 回 值】无
【实    例】
【注意事项】
--------------------------------------------------------------------------------------------------------*/
static void MT9V034_SetFrameResolution(uint16_t height,uint16_t width)
{
    uint16_t data = 0;
    
    if((height*4) <= MAX_IMAGE_HEIGHT)   //判断行是否4分频
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if((height*2)<=MAX_IMAGE_HEIGHT )  //判断行是否2分频
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }
    
    if((width*4)<=MAX_IMAGE_WIDTH )   //判断列是否4分频   
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MAX_IMAGE_WIDTH )   //判断列是否2分频
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }
     
    //         水平翻转                     垂直翻转
    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);  //需要翻转的可以打开注释
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //写寄存器，配置行分频
    
    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH,  width);  //读取图像的列数  改变此处也可改变图像输出大小，不过会丢失视角
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //读取图像的行数  改变此处也可改变图像输出大小，不过会丢失视角
    
    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //列开始
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //行开始
     
}


                      
                      
                      
                      

