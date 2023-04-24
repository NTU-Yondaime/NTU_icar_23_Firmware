/*-----------------------------------------------------------------
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2019��3��12��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
��dev.env.��IAR8.30.1�����ϰ汾
--------------------------------------------------------------------*/
#include "fsl_common.h"
#include "include.h"
#include "LQ_IIC.h"
#include "LQ_LED.h"
#include "LQ_CSI.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_debug_console.h"
#include "fsl_sccb.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "LQ_UART.h"
#ifdef LQMT9V034
#include "LQ_MT9V034M.h"
#else
#include "LQ_OV7725M.h"
#endif

/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);

//��������ͷ���ݻ�����
SDK_ALIGN(volatile pixel_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH], FRAME_BUFFER_ALIGN); 



#ifdef LQMT9V034 //LQMT9V034ģ��
/* ����ͷʹ��Ӳ����Դ ����û���õ� */
static LQMT9V034_resource_t LQMT9V034Resource;

/* ����ͷ�������ýṹ�� */
camera_device_handle_t cameraDevice = {          
    .resource = &LQMT9V034Resource,
    .ops = &LQMT9V034_ops,
};

#else           //LQOV7725ģ��

static ov7725_resource_t ov7725Resource;

/* ����ͷ�������ýṹ�� */
camera_device_handle_t cameraDevice = {  
    .resource = &ov7725Resource,
    .ops = &ov7725_ops,
};
#endif

/* CSI ������ؽṹ��. */
static csi_resource_t csiResource = {   
    .csiBase = CSI,
};

static csi_private_data_t csiPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, 
    .ops = &csi_ops, 
    .privateData = &csiPrivateData,
};


extern void CSI_DriverIRQHandler(void);


/**
  * @brief    CSI�жϷ�����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
}


/**
  * @brief    ����ͷCSI�ӿڳ�ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void LQ_CSI_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);        
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK,0U);                                                                      
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_CSI_DATA09,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_CSI_DATA08,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_CSI_DATA07,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_CSI_DATA06,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_CSI_DATA05,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_CSI_DATA04,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_CSI_DATA03,0U);                                   
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_CSI_DATA02,0U);                                   
                                             
    /* CSI MCLK select 120M. */
   /*
    * CSI clock source: 
    * 00 derive clock from osc_clk (24M)
    * 01 derive clock from PLL2 PFD2
    * 10 derive clock from pll3_120M
    * 11 derive clock from PLL3 PFD1
    */
    CLOCK_SetMux(kCLOCK_CsiMux, 2);

    CLOCK_SetDiv(kCLOCK_CsiDiv, 1);
    
}


camera_config_t cameraConfig;   //����ͷ���ýṹ��
#ifdef LQMT9V034 //LQMT9V034ģ��
    camera_config_t cameraConfig = {
        .pixelFormat   = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
        .bytesPerPixel = APP_BPP,//   ÿ�����ص㼸������
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //�ֱ���
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //�м��
        .interface     = kCAMERA_InterfaceGatedClock,                            //������ӿ�����
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec   = CAMERA_FPS,                                             //fps 
    };
#else            //LQOV7725ģ��
    camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatYUYV,
        .bytesPerPixel = APP_BPP,
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT),  //�ֱ���
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                 //�м��
        .interface =  kCAMERA_InterfaceCCIR656,                                   //������ӿ�����
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec = CAMERA_FPS,                                                //fps 
    };
#endif
    
    
/**
  * @brief    ����ͷ��ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void LQ_Camera_Init(void)  
{
#if (defined LQOV7725) && (defined LQOV7725YUV)
    cameraConfig.pixelFormat = kVIDEO_PixelFormatYUYV;
#endif
    LQ_IIC_Init(LPI2C1, 10000);
    LQ_CSI_Init();
    
    delayms(20);
    
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //��ʼ��csi
    
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //��ʼ������ͷ
    
    CAMERA_DEVICE_Start(&cameraDevice);                                //�������
    
    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++)       //����֡�������ύ������������
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }
    
    CAMERA_RECEIVER_Start(&cameraReceiver);   // ��������camera����
    delayms(200);        
}


/**
  * @brief    ��λ����ͼ
  *
  * @param    
  *
  * @return   
  *
  * @note     ���ڴ������ݱȽ��� ��DAP�Դ��Ĵ���ʱ����Ҫһ�ߵ��ԣ�һ�߷�����ݣ��Ῠ��
  *
  * @example  
  *
  * @date     2019/6/24 ����һ
  */
void Test_Camera_Reprot(void)
{
    uint32_t fullCameraBufferAddr;  

    LQ_UART_Init(LPUART1, 115200);
    LQ_Camera_Init();
    LED_Init();       
    uint8_t count = 0;

    while (1)
    {     

        //����ͷCSI����������
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }  
        SCB_DisableDCache();
        SCB_EnableDCache();  //ˢ��D-Cache
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//��������������ύ���������
        CAMERA_RECEIVER_Stop(&cameraReceiver);//����ʱ��Ƚϳ� ��������ͣcsi���� ��ֹͼ��û�з������ˢ������һ֡ͼ��
        
        for(count = 0; count < APP_CAMERA_FRAME_BUFFER_COUNT; count++)    //��ȡ��ǰ����������������
        {
            if(fullCameraBufferAddr == (uint32_t )csiFrameBuf[count])
            {
                break;
            }
            
        }
        
        UART_PutChar(LPUART1, 0xfe);  //֡ͷ
        UART_PutChar(LPUART1, 0xef);  //֡ͷ

        for(int i = 0; i < APP_CAMERA_HEIGHT; i++)  //�ֱ���Խ�� ����ͼԽ��
        {
//��λ���� 7725 RGB565ͼ�� �� ���ۻҶ�ͼ�� 
#if  (defined LQMT9V034) || (defined LQOV7725RGB)   
            for(int j = 0; j < APP_CAMERA_WIDTH; j++)
            {
                if(csiFrameBuf[count][i][j].gray[0] == 0xfe )  //��ֹ������֡β
                {
                    csiFrameBuf[count][i][j].gray[0] = 0xff;  
                }
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[0]); //��������
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[1]); //��������
            }
#else     //��λ���� 7725 �ĻҶ�ͼ�� 
            for(int j = 0; j < APP_CAMERA_WIDTH; j++)  //UYVY ��ʽͼ�� ��Y�ǻҶȣ����Ҷ�ͼ��ֻ���Yֵ
            {
                if(csiFrameBuf[count][i][j].gray[1] == 0xfe )  //��ֹ������֡β
                {
                    csiFrameBuf[count][i][j].gray[1] = 0xff;  
                }
                UART_PutChar(LPUART1, csiFrameBuf[count][i][j].gray[1]); //��������
            }
#endif
        }  
        UART_PutChar(LPUART1, 0xef);  //֡β
        UART_PutChar(LPUART1, 0xfe);  //֡β
        
        
        /* ������������camera���� */
        CAMERA_RECEIVER_Start(&cameraReceiver);
		SCB_DisableDCache();
        SCB_EnableDCache();  //ˢ��D-Cache		
        /* �����������պ����һ֡���� */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }
        /* ����֡�ύ�������� */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
        LED_ColorReverse(blue);

        delayms(1000);
    }
}



