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
#include "stdio.h"
#include "LQ_IIC.h"
#include "LQ_LED.h"
#include "LQ_CSI.h"
#include "LQ_Camera.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
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
#include "LQ_CSI.h"
#ifdef LQMT9V034
#include "LQ_MT9V034M.h"
#else
#include "LQ_OV7725M.h"
#endif

/* ���ڴ����Ҫʹ������ */
pixel_t Image_Use[Use_ROWS][Use_Line]; 

/* ��Ŷ�ֵ��������� */
uint8_t Pixle[Use_ROWS][Use_Line];     

/* ���ڴ��浱ǰ֡�ĵ�ַ */
uint32_t fullCameraBufferAddr;  

/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);


/**
  * @brief    ��ȡ��Ҫʹ�õ�ͼ������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_Use_Image(void)
{
    //ע�⣬ʹ��csiFrameBuf����ʱ�����ˢ��һ��D-Cache ��Ȼ�ϴ����ݿ��ܻ�����cache���棬������ݴ���
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }
    SCB_EnableDCache();
    
	uint8_t div_h, div_w;
    uint32_t temp_h = 0;
    uint32_t temp_w = 0;
    uint32_t row_start = 0;
    uint32_t lin_start = 0;

#ifdef LQMT9V034    //ʹ������
	div_h = IMAGEH/Use_ROWS;
	div_w = IMAGEW/Use_Line;
    
    /* ������ȡͼ�� */
    if(Use_ROWS * div_h != IMAGEH)
    {
        row_start = (IMAGEH - Use_ROWS * div_h)/2;
        temp_h = (uint32_t)(row_start/2) * APP_CAMERA_WIDTH;
    }
    if(Use_Line * div_w != IMAGEW)
    {
        lin_start = (IMAGEW - Use_Line * div_w)/2;       
    }
    for(int i = 0; i < Use_ROWS; i += 2)
    {

        temp_w = lin_start;
        for(int j = 0; j < Use_Line; j++)
        {  
            Image_Use[i][j].gray[0] = (*((uint8_t *)fullCameraBufferAddr +  temp_h + temp_w));
            Image_Use[i+1][j].gray[0] = (*((uint8_t *)fullCameraBufferAddr +  temp_h + temp_w + APP_CAMERA_WIDTH));
            temp_w += div_w;
        }
        temp_h += div_h * 2 * APP_CAMERA_WIDTH;
    }
    
#else
#ifdef LQOV7725YUV  //ʹ��7725  UYVY
	div_h = 2*APP_CAMERA_HEIGHT/Use_ROWS;
	div_w = 2*APP_CAMERA_WIDTH/Use_Line;
        /* ������ȡͼ�� */
    if(Use_ROWS * div_h != 2*APP_CAMERA_HEIGHT)
    {
        row_start = APP_CAMERA_HEIGHT - Use_ROWS * (div_h/2);
        temp_h = row_start * APP_CAMERA_WIDTH;
    }
    if(Use_Line * div_w != 2*APP_CAMERA_WIDTH)
    {
        lin_start = APP_CAMERA_WIDTH - Use_Line * (div_w/2); 
        if(lin_start % 2 == 0)
        {
            lin_start += 1;
        }
    }
    else
    {
        lin_start = 1;
    }
    
	for(int i = Use_ROWS-1; i >= 0; i--)  
	{
        temp_w = lin_start;
		for(int j = Use_Line-1; j >= 0; j--)
		{
			Image_Use[i][j].gray[0] = (*((uint8_t *)fullCameraBufferAddr +  temp_h + temp_w)); 
            temp_w += div_w;
		}
        temp_h += div_h * APP_CAMERA_WIDTH;
	}
    
#else               //ʹ��7725  RGB
    
    div_h = APP_CAMERA_HEIGHT/Use_ROWS;
	div_w = APP_CAMERA_WIDTH/Use_Line;
    /* ������ȡͼ�� */
    if(Use_ROWS * div_h != APP_CAMERA_HEIGHT)
    {
        row_start = (APP_CAMERA_HEIGHT - Use_ROWS * div_h)/2;
        row_start = row_start * APP_CAMERA_WIDTH;
    }
    if(Use_Line * div_w != APP_CAMERA_WIDTH)
    {
        lin_start = (APP_CAMERA_WIDTH - Use_Line * div_w)/2; 

    }
    for(int i = Use_ROWS-1; i >= 0; i--)
    {
        temp_h = (Use_ROWS - 1 - i) * div_h * APP_CAMERA_WIDTH - row_start;
        temp_w = lin_start;
        for(int j = Use_Line-1; j >= 0; j--)
        {
            Image_Use[i][j].rgb565 = (*((uint16_t *)fullCameraBufferAddr +  temp_h + temp_w));
            temp_w += div_w;
        }
    }
#endif
#endif
    /* ����֡�ύ�������� */
	CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);    
}





/**
  * @brief    TFT1.8 ��ʾͼ��
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
void Test_SGP18_Camera(void)
{

    TFTSPI_Init(0);               //TFT1.8��ʼ��  0��������ʾ  1��������ʾ

    TFTSPI_CLS(u16BLUE);          //��ɫ��Ļ	
    LED_Init();
     
    LQ_Camera_Init();  
    while (1)
    {     
        //�ȴ�����ͷCSI��������
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }   

        Get_Use_Image();                      //��ȡ��Ҫʹ�õĴ�С
        
        /* �����ҪҪ��ʾ ����Ҫֹͣ���� */
        CAMERA_RECEIVER_Stop(&cameraReceiver);//��ʾʱ��Ƚϳ� ��������ͣcsi���� ��ֹͼ��û����ʾ���ˢ������һ֡ͼ��
            
        /* ������ʾ��Χ */
        TFTSPI_Set_Pos(0, 0, Use_Line - 1, Use_ROWS);   
        
#ifdef LQOV7725RGB  //7725  ��ɫ��ʾ     
        /* ��ʾͼ�� */
        for(int i = 0; i < Use_ROWS; i++)   
        {
            for(int j = 0; j < Use_Line; j++)
            {                       
                TFTSPI_Write_Word (Image_Use[i][j].rgb565);    //��ʾ����
            }
        }
#else //�Ҷ���ʾ
        uint16_t color;
        /* ��ʾͼ�� */
        for(int i = 0; i < Use_ROWS; i++)   
        {
            for(int j = 0; j < Use_Line; j++)
            {
                /* ���Ҷ�ת��Ϊ RGB565 */
                color = 0;
                color = (Image_Use[i][j].gray[0] >> 3) << 11;
                color |= (Image_Use[i][j].gray[0] >> 2) << 5;
                color |= Image_Use[i][j].gray[0] >> 3;
                /* ��ʾ */
                TFTSPI_Write_Word (color);    
            }
        }
        
#endif
        /* ������������camera���� */
        CAMERA_RECEIVER_Start(&cameraReceiver);   
        /* �����������պ����һ֡���� */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }
        /* ����֡�ύ�������� */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
        LED_ColorReverse(red); 
    }
}



/**
  * @brief    OLED ��ʾͼ��
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
void Test_OLED_Camera(void)
{
    LCD_Init();          
    LCD_CLS();   
    LED_Init();
#ifdef LQOV7725
    LCD_Show_Frame80();
#else
    LCD_Show_Frame94();
#endif
    LQ_Camera_Init();  
    while (1)
    {     
        //�ȴ�����ͷCSI��������
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }   

        Get_Use_Image();                      //��ȡ��Ҫʹ�õĴ�С
        Get_01_Value(0);
        
        Draw_Road();
        
        LED_ColorReverse(red); 
    }
}



/**
  * @brief    ��ֵ��
  *
  * @param    mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_01_Value(uint8_t mode)
{
  int i = 0,j = 0;
  uint8_t Threshold;
  uint32_t  tv=0;
  char txt[16];
  if(mode)
  {
      //�ۼ�
      for(i = 0; i <Use_ROWS; i++)
      {    
          for(j = 0; j <Use_Line; j++)
          {                            
              tv+=Image_Use[i][j].gray[0];   //�ۼ�  
          } 
      }
      Threshold=tv/Use_ROWS/Use_Line;        //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
      Threshold=Threshold*7/10+10;   //�˴���ֵ���ã����ݻ����Ĺ������趨 
  }
  else
  {
      Threshold = GetOSTU(Image_Use);//�����ֵ
//      Threshold = (uint8_t)(Threshold * 0.5) + 70;
  } 
#ifdef OLED
  sprintf(txt,"%03d",Threshold);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  LCD_P6x8Str(80,0,txt);
#endif
  
  for(i = 0; i < Use_ROWS; i++)
  {
    for(j = 0; j < Use_Line; j++)
    {                                
      if(Image_Use[i][j].gray[0] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}



/**
  * @brief    OLED ��ʾͼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 ���ڶ�
  */
void Draw_Road(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56�� 
  {
#ifdef LQOV7725
    LCD_Set_Pos(24,i/8+1);
#else 
    LCD_Set_Pos(17,i/8+1);
#endif
    for(j=0;j<Use_Line;j++) 
    { 
      temp=0;
      if(Pixle[0+i][j]) temp|=1;
      if(Pixle[1+i][j]) temp|=2;
      if(Pixle[2+i][j]) temp|=4;
      if(Pixle[3+i][j]) temp|=8;
      if(Pixle[4+i][j]) temp|=0x10;
      if(Pixle[5+i][j]) temp|=0x20;
      if(Pixle[6+i][j]) temp|=0x40;
      if(Pixle[7+i][j]) temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}


/*************************************************************** 
* 
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����	1
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7) �����g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
������ղ�����  https://blog.csdn.net/kk55guang2/article/details/78475414
                https://blog.csdn.net/kk55guang2/article/details/78490069
				https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
***************************************************************/ 
int GetOSTU(pixel_t tmImage[Use_ROWS][Use_Line]) 
{ 
	int16_t i,j; 
	uint32_t Amount = 0; 
	uint32_t PixelBack = 0; 
	uint32_t PixelIntegralBack = 0; 
	uint32_t PixelIntegral = 0; 
	int32_t PixelIntegralFore = 0; 
	int32_t PixelFore = 0; 
	float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
	int16_t MinValue, MaxValue; 
	uint8_t Threshold = 0;
	uint8_t HistoGram[256];              //  
	
	for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
	
	for (j = 0; j < Use_ROWS; j++) 
	{ 
		for (i = 0; i < Use_Line; i++) 
		{ 
			HistoGram[tmImage[j][i].gray[0]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
		} 
	} 
	
	for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
	for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
	
	if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
	if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
	
	for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
	
	PixelIntegral = 0;
	for (j = MinValue; j <= MaxValue; j++)
	{
		PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
	}
	SigmaB = -1;
	for (j = MinValue; j < MaxValue; j++)
	{
		PixelBack = PixelBack + HistoGram[j];   //ǰ�����ص���
		PixelFore = Amount - PixelBack;         //�������ص���
		OmegaBack = (float)PixelBack / Amount;//ǰ�����ذٷֱ�
		OmegaFore = (float)PixelFore / Amount;//�������ذٷֱ�
		PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
		PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
		MicroBack = (float)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
		MicroFore = (float)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
		Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
		if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
		{
			SigmaB = Sigma;
			Threshold = j;
		}
	}
	return Threshold;                        //���������ֵ;
} 

