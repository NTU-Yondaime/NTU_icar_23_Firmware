/*-----------------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【dev.env.】IAR8.30.1及以上版本
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

/* 用于存放需要使用数据 */
pixel_t Image_Use[Use_ROWS][Use_Line]; 

/* 存放二值化后的数据 */
uint8_t Pixle[Use_ROWS][Use_Line];     

/* 用于储存当前帧的地址 */
uint32_t fullCameraBufferAddr;  

/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


/**
  * @brief    获取需要使用的图像数组
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Get_Use_Image(void)
{
    //注意，使用csiFrameBuf数组时，最好刷新一下D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }
    SCB_EnableDCache();
    
	uint8_t div_h, div_w;
    uint32_t temp_h = 0;
    uint32_t temp_w = 0;
    uint32_t row_start = 0;
    uint32_t lin_start = 0;

#ifdef LQMT9V034    //使用神眼
	div_h = IMAGEH/Use_ROWS;
	div_w = IMAGEW/Use_Line;
    
    /* 从中心取图像 */
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
#ifdef LQOV7725YUV  //使用7725  UYVY
	div_h = 2*APP_CAMERA_HEIGHT/Use_ROWS;
	div_w = 2*APP_CAMERA_WIDTH/Use_Line;
        /* 从中心取图像 */
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
    
#else               //使用7725  RGB
    
    div_h = APP_CAMERA_HEIGHT/Use_ROWS;
	div_w = APP_CAMERA_WIDTH/Use_Line;
    /* 从中心取图像 */
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
    /* 将空帧提交到缓冲区 */
	CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);    
}





/**
  * @brief    TFT1.8 显示图像
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
void Test_SGP18_Camera(void)
{

    TFTSPI_Init(0);               //TFT1.8初始化  0：横屏显示  1：竖屏显示

    TFTSPI_CLS(u16BLUE);          //蓝色屏幕	
    LED_Init();
     
    LQ_Camera_Init();  
    while (1)
    {     
        //等待摄像头CSI缓存区满
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }   

        Get_Use_Image();                      //获取想要使用的大小
        
        /* 如果不要要显示 则不需要停止接收 */
        CAMERA_RECEIVER_Stop(&cameraReceiver);//显示时间比较长 所以先暂停csi接收 防止图像还没有显示完就刷新了下一帧图像
            
        /* 设置显示范围 */
        TFTSPI_Set_Pos(0, 0, Use_Line - 1, Use_ROWS);   
        
#ifdef LQOV7725RGB  //7725  彩色显示     
        /* 显示图像 */
        for(int i = 0; i < Use_ROWS; i++)   
        {
            for(int j = 0; j < Use_Line; j++)
            {                       
                TFTSPI_Write_Word (Image_Use[i][j].rgb565);    //显示数据
            }
        }
#else //灰度显示
        uint16_t color;
        /* 显示图像 */
        for(int i = 0; i < Use_ROWS; i++)   
        {
            for(int j = 0; j < Use_Line; j++)
            {
                /* 将灰度转化为 RGB565 */
                color = 0;
                color = (Image_Use[i][j].gray[0] >> 3) << 11;
                color |= (Image_Use[i][j].gray[0] >> 2) << 5;
                color |= Image_Use[i][j].gray[0] >> 3;
                /* 显示 */
                TFTSPI_Write_Word (color);    
            }
        }
        
#endif
        /* 重新启动接收camera数据 */
        CAMERA_RECEIVER_Start(&cameraReceiver);   
        /* 重新启动接收后接收一帧数据 */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }
        /* 将空帧提交到缓冲区 */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
        LED_ColorReverse(red); 
    }
}



/**
  * @brief    OLED 显示图像
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
        //等待摄像头CSI缓存区满
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  
        {
        }   

        Get_Use_Image();                      //获取想要使用的大小
        Get_01_Value(0);
        
        Draw_Road();
        
        LED_ColorReverse(red); 
    }
}



/**
  * @brief    二值化
  *
  * @param    mode  ：  0：使用大津法阈值    1：使用平均阈值
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Get_01_Value(uint8_t mode)
{
  int i = 0,j = 0;
  uint8_t Threshold;
  uint32_t  tv=0;
  char txt[16];
  if(mode)
  {
      //累加
      for(i = 0; i <Use_ROWS; i++)
      {    
          for(j = 0; j <Use_Line; j++)
          {                            
              tv+=Image_Use[i][j].gray[0];   //累加  
          } 
      }
      Threshold=tv/Use_ROWS/Use_Line;        //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
      Threshold=Threshold*7/10+10;   //此处阈值设置，根据环境的光线来设定 
  }
  else
  {
      Threshold = GetOSTU(Image_Use);//大津法阈值
//      Threshold = (uint8_t)(Threshold * 0.5) + 70;
  } 
#ifdef OLED
  sprintf(txt,"%03d",Threshold);//前者为大津法求得的阈值，后者为平均值  
  LCD_P6x8Str(80,0,txt);
#endif
  
  for(i = 0; i < Use_ROWS; i++)
  {
    for(j = 0; j < Use_Line; j++)
    {                                
      if(Image_Use[i][j].gray[0] >Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}



/**
  * @brief    OLED 显示图像
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/25 星期二
  */
void Draw_Road(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)// 56行 
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
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：求阈值大小 
* 参数说明： 
* 函数返回：阈值大小 
* 修改时间：2018年3月27日 
* 备 注： 
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代	1
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7) 将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
解决光照不均匀  https://blog.csdn.net/kk55guang2/article/details/78475414
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
	float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差; 
	int16_t MinValue, MaxValue; 
	uint8_t Threshold = 0;
	uint8_t HistoGram[256];              //  
	
	for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图 
	
	for (j = 0; j < Use_ROWS; j++) 
	{ 
		for (i = 0; i < Use_Line; i++) 
		{ 
			HistoGram[tmImage[j][i].gray[0]]++; //统计灰度级中每个像素在整幅图像中的个数
		} 
	} 
	
	for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
	for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
	
	if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色    
	if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色
	
	for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数
	
	PixelIntegral = 0;
	for (j = MinValue; j <= MaxValue; j++)
	{
		PixelIntegral += HistoGram[j] * j;//灰度值总数
	}
	SigmaB = -1;
	for (j = MinValue; j < MaxValue; j++)
	{
		PixelBack = PixelBack + HistoGram[j];   //前景像素点数
		PixelFore = Amount - PixelBack;         //背景像素点数
		OmegaBack = (float)PixelBack / Amount;//前景像素百分比
		OmegaFore = (float)PixelFore / Amount;//背景像素百分比
		PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
		PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
		MicroBack = (float)PixelIntegralBack / PixelBack;   //前景灰度百分比
		MicroFore = (float)PixelIntegralFore / PixelFore;   //背景灰度百分比
		Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
		if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
		{
			SigmaB = Sigma;
			Threshold = j;
		}
	}
	return Threshold;                        //返回最佳阈值;
} 

