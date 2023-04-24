#include "ANO_DT.h"
#include "LQ_UART_DMA.h"
#include "include.h"
#include "LQ_LED.h"
#include "math.h"
#include "LQ_UART.h"
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);

//ʹ������4.3��λ��Э��
/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


/////////////////////////////////////////////////////////////////////////////////////
//Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
//��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���

void ANO_DT_Send_Data(uint8_t *dataToSend , uint8_t length)
{
#if ANO_DT_USE_DMA
     /* If TX is idle and g_txBuffer is full, start to send data. */
    /*ʹ��DMA + ���ڣ�����ռ��CPUʱ�� */
	
	/* ���һ��D-Cache */
	SCB_CleanDCache();
	
    sendXfer.data = dataToSend;   
    sendXfer.dataSize = length;
    if ((!txOnGoing) )
    {
        txOnGoing = true;
        LPUART_SendEDMA(LPUART1, &g_lpuartEdmaHandle, &sendXfer);
    }
#else
    /**ʹ�ô��������������ݣ������Ҫ1.5ms*/
	UART_PutBuff(LPUART1, dataToSend, length);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������
#endif  
   
}
uint8_t data_to_send[50];	//�������ݻ���
static void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum)
{
    
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	ANO_DT_Send_Data(data_to_send, 7);
}

/**
  * @brief    ����λ�����ͷ���8��short����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 /*,short data7, short data8, short .....�ɸ�����Ҫ������� */)
{
    uint8_t _cnt=0;
    data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1  
    data_to_send[_cnt++] = 16;        //8��short ���� 16���ֽ�
    
	data_to_send[_cnt++]=BYTE1(data1);
	data_to_send[_cnt++]=BYTE0(data1);

	data_to_send[_cnt++]=BYTE1(data2);
	data_to_send[_cnt++]=BYTE0(data2);

	data_to_send[_cnt++]=BYTE1(data3);
	data_to_send[_cnt++]=BYTE0(data3);
    
    data_to_send[_cnt++]=BYTE1(data4);
	data_to_send[_cnt++]=BYTE0(data4);

	data_to_send[_cnt++]=BYTE1(data5);
	data_to_send[_cnt++]=BYTE0(data5);

	data_to_send[_cnt++]=BYTE1(data6);
	data_to_send[_cnt++]=BYTE0(data6);
    
    data_to_send[_cnt++]=BYTE1(data7);
	data_to_send[_cnt++]=BYTE0(data7);

	data_to_send[_cnt++]=BYTE1(data8);
	data_to_send[_cnt++]=BYTE0(data8);
    
    uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

///////////////////////////////////////////////////////////////////////////////////////
////Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
////��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
////�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������

void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num);
/**
  * @brief    ���ڴ��ڽ����ж� һ���жϽ���һ���ַ�ʱ�������������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Data_Receive_Prepare(uint8_t data)
{
    static uint8_t RxBuffer[50];
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}

/**
  * @brief    ����λ�����ͷ���PID֡
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	uint8_t _cnt=0;
	short _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = (short)(p1_p * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p1_i  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p1_d  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p2_p  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p2_i  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p2_d * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p3_p  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p3_i  * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(p3_d * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
/**
  * @brief    ����������λ�������������ݰ�
  *
  * @param    data_buf �����ݰ�
  * @param    num      �����ݰ�����
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{
	uint8_t sum = 0;
	for(uint8_t i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		                //�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	
	if(*(data_buf+2)==0X01)        //У׼����
	{
        LED_ColorReverse(red);     //�����˸
		if(*(data_buf+4)==0X01)
			
		if(*(data_buf+4)==0X02)
			
		if(*(data_buf+4)==0X03)
		{
						
		}
	}
	
	if(*(data_buf+2)==0X02)        //��ȡ����
	{
		if(*(data_buf+4)==0X01)    //��ȡpid
		{
            /* ��������û����� */
            //ANO_DT_Send_PID(1, Motor_pid._kp, Motor_pid._ki, Motor_pid._kd, Servo_pid._kp, Servo_pid._ki, Servo_pid._kd, 0,0,0);
		}
		if(*(data_buf+4)==0X02)   //��ȡ����ģʽ
		{
			
		}
		if(*(data_buf+4)==0XA0)   //��ȡ�汾��Ϣ
		{
//			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)	  //�ָ�Ĭ�ϲ���
		{
//			Para_ResetToFactorySetup();
		}
	}

	if(*(data_buf+2)==0X10)								//PID1      д�����
    {
//        Motor_pid._kp  = 0.001*( (short)(*(data_buf+4)<<8)|*(data_buf+5) );   //��λ����ROW PID
//        Motor_pid._ki  = 0.001*( (short)(*(data_buf+6)<<8)|*(data_buf+7) );
//        Motor_pid._kd  = 0.001*( (short)(*(data_buf+8)<<8)|*(data_buf+9) );
//        Speed_pid._kp  = 0.001*( (short)(*(data_buf+10)<<8)|*(data_buf+11) ); //��λ����PITCH PID
//        Speed_pid._ki  = 0.001*( (short)(*(data_buf+12)<<8)|*(data_buf+13) );
//        Speed_pid._kd  = 0.001*( (short)(*(data_buf+14)<<8)|*(data_buf+15) );
//        Dir_pid._kp 	= 0.001*( (short)(*(data_buf+16)<<8)|*(data_buf+17) );
//        Dir_pid._ki 	= 0.001*( (short)(*(data_buf+18)<<8)|*(data_buf+19) );
//        Dir_pid._kd 	= 0.001*( (short)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);    //У��
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
//        ctrl_1.PID[PID4].kp 	= 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_1.PID[PID4].ki 	= 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_1.PID[PID4].kd 	= 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_1.PID[PID5].kp 	= 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_1.PID[PID5].ki 	= 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_1.PID[PID5].kd 	= 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_1.PID[PID6].kp	  = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_1.PID[PID6].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_1.PID[PID6].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
//				Param_SavePID();
    }
    if(*(data_buf+2)==0X12)								//PID3
    {	
//        ctrl_2.PID[PIDROLL].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        ctrl_2.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        ctrl_2.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        ctrl_2.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        ctrl_2.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        ctrl_2.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        ctrl_2.PID[PIDYAW].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        ctrl_2.PID[PIDYAW].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        ctrl_2.PID[PIDYAW].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
//				Param_SavePID();
    }
	if(*(data_buf+2)==0X13)								//PID4
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
}


/**
  * @brief    �����ϱ�������λ��
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 ����һ
  */
#if ANO_DT_USE_DMA
void Test_ANO_DT(void)    
{
    float data1 = 0.0f;
    float data2 = 45.0f; 
    float data3 = 90.0f;   
    float data4 = 135.0f;   
    float data5 = 60.0f;
    const float PI = 3.1415926;
    
    LED_Init();   

    LQ_UART_DMA_Init(LPUART1, 115200, 0, 1);   
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //ʹ��LPUART1 �Ŀ����ж�
    
    /* �������� */
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = 100;
    
    /* ��ʼ���� */
    rxOnGoing = true;
    LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
    
    while(1)
    {
        /* �������� */
        if ((!rxOnGoing))
        {
			/* ���һ��D-Cache */
			SCB_CleanDCache();
	
            /* �����յ���֡ */
            ANO_DT_Data_Receive_Anl(g_rxBuffer, g_rxLen);
            
            /* ����������֡ */
            rxOnGoing = true;
            LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);
            
        }
        
        if(txOnGoing == false)
        {
            
            ANO_DT_send_int16((short)(sin(data1/180.0f * PI) * 100),
                          (short)(sin(data2/180.0f * PI) * 100),
                          (short)(sin(data3/180.0f * PI) * 100),
                          (short)(sin(data4/180.0f * PI) * 100), 
                          (short)(sin(data5/180.0f * PI) * 100), 
                          0, 
                          0,
                          0);  
        }
         
        delayms(20);
        if(data1 > 180) data1 = -180;
        if(data2 > 180) data2 = -180;
        if(data3 > 180) data3 = -180;
        if(data4 > 180) data4 = -180;
        if(data5 > 180) data5 = -180;
        
        data1 += 1;
        data2 += 2;
        data3 += 3;
        data4 += 4;
        data5 += 5;
    }
}

#else

void Test_ANO_DT(void)    
{
    float data1 = 0.0f;
    float data2 = 45.0f; 
    float data3 = 90.0f;   
    float data4 = 135.0f;   
    float data5 = 60.0f;
    const float PI = 3.1415926;
    
    LED_Init();   

    LQ_UART_Init(LPUART1, 115200);   
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    EnableIRQ(LPUART1_IRQn);	                            //ʹ��LPUART1 �Ľ����ж�
    
  
    
    while(1)
    {
        
        

            
            ANO_DT_send_int16((short)(sin(data1/180.0f * PI) * 100),
                          (short)(sin(data2/180.0f * PI) * 100),
                          (short)(sin(data3/180.0f * PI) * 100),
                          (short)(sin(data4/180.0f * PI) * 100), 
                          (short)(sin(data5/180.0f * PI) * 100), 
                          0, 
                          0,
                          0);  

         
        delayms(20);
        if(data1 > 180) data1 = -180;
        if(data2 > 180) data2 = -180;
        if(data3 > 180) data3 = -180;
        if(data4 > 180) data4 = -180;
        if(data5 > 180) data5 = -180;
        
        data1 += 1;
        data2 += 2;
        data3 += 3;
        data4 += 4;
        data5 += 5;
    }
}
#endif


