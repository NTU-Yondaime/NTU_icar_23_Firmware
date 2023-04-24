#ifndef __LQ_CSI_H
#define __LQ_CSI_H

#include "include.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_sccb.h"
#include "LQ_GPIO_Cfg.h"
#include "LQ_MT9V034M.h"




#define APP_CAMERA_FRAME_BUFFER_COUNT 4
#define FRAME_BUFFER_ALIGN 64           //对齐 
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿

typedef struct
{ 
    uint16_t b         : 5;
    uint16_t g         : 6;
    uint16_t r         : 5;
}rgb565_t;


typedef union 
{
    uint16_t rgb565;
    uint8_t  gray[2];
    rgb565_t rgb;
}pixel_t;

extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
extern volatile pixel_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];
extern camera_config_t cameraConfig;   //摄像头配置结构体



/**
  * @brief    上位机看图
  *
  * @param    
  *
  * @return   
  *
  * @note     串口传输数据比较慢
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void Test_Camera_Reprot(void);


/**
  * @brief    摄像头初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 星期一
  */
void LQ_Camera_Init(void);


#endif

