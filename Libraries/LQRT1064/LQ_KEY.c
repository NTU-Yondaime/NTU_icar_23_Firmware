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
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "LQ_LED.h"
#include "LQ_KEY.h"
#include "LQ_GPIO.h"
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);


#if  1   //ʹ��fsl�� ����GPIO

/**
  * @brief    ��ʼ������ĸ���ϵİ���
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO3_IO04 ------ �ܽ�P2   ----- >  ĸ�尴��K2
  * @note     GPIO2_IO27 ------ �ܽ�C13  ----- >  ĸ�尴��K0
  * @note     GPIO2_IO30 ------ �ܽ�C14  ----- >  ĸ�尴��k1
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void KEY_Init(void)
{  
    CLOCK_EnableClock(kCLOCK_Iomuxc);                    // IO��ʱ��ʹ��
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);//�ܽ�P2
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,0U);   //�ܽ�C13
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,0U);   //�ܽ�C14
    
    /*
    �ֶν��ͣ� 
    2.0��SRE ����λ��0 
    SRE(Slew Rate Field)��ת���ٶ��ֶ� ����һ�����Ե����ߵ͵�ƽ�л��������½�ʱ���ٶȵ������
    ��һ���Ӧ����ʹ��оƬĬ�ϵ����þͿ����ˡ�����Ҫ΢��SI��EMI����ʱ���Գ����޸����� 
    ��ѡ��ֵ�� 
    0 SRE_0_Slow_Slew_Rate �� Slow Slew Rate��ת���ٶ��� 
    1 SRE_1_Fast_Slew_Rate �� Fast Slew Rate�� ת���ٶȿ�
    
    2.1��DSE ����λ5-3 
    DSE(Drive Strength Field)�����������ֶΣ���Ϊ���ʱ��Ч 
    ��ѡ��ֵ�� 
    000 DSE_0_output_driver_disabled_ �� output driver disabled; ��ֹ��� 
    001 DSE_1_R0_260_Ohm___3_3V__150_Ohm_1_8V__240_Ohm_for_DDR_ �� R0(260 Ohm 
    @ 3.3V, 150 Ohm@1.8V, 240 Ohm for DDR) -R0��260ŷķ@3.3v��150ŷķ@1.8v 
    010 DSE_2_R0_2 �� R0/2 -R0�Ķ���֮һ��һ����ͬ 
    011 DSE_3_R0_3 �� R0/3 
    100 DSE_4_R0_4 �� R0/4 
    101 DSE_5_R0_5 �� R0/5 
    110 DSE_6_R0_6 �� R0/6 
    111 DSE_7_R0_7 �� R0/7
    
    2.2��SPEED ����λ7-6 
    Speed Field���ٶ��ֶ� 
    ��ѡ��ֵ 
    00 SPEED_0_low_50MHz_ �� low(50MHz) ���50MHz 
    01 SPEED_1_medium_100MHz_ �� medium(100MHz)�м�ֵ100MHz 
    10 SPEED_2_medium_100MHz_ �� medium(100MHz)�м�ֵ100MHz 
    11 SPEED_3_max_200MHz_ �� max(200MHz) ���200MHz
    
    2.3��ODE ����λ11 
    ODE(Open Drain Enable Field)��©����·��������Ϊ���ʱ��Ч�������ⲿ��ѹ������������ 
    ��ѡ��ֵ�� 
    0 ODE_0_Open_Drain_Disabled �� Open Drain Disabled ����ֹ©����· 
    1 ODE_1_Open_Drain_Enabled �� Open Drain Enabled�� ʹ��©����·
    
    2.4��PKE ����λ12 
    PKE(Pull / Keep Enable Field)����/����ʹ���ֶ� 
    ��ѡ��ֵ�� 
    0 PKE_0_Pull_Keeper_Disabled �� Pull/Keeper Disabled�� ��ֹ��/���� 
    1 PKE_1_Pull_Keeper_Enabled �� Pull/Keeper Enabled�� ʹ����/����
    
    2.5��PUE ����λ13 
    PUE(Pull / Keep Select Field)����/����ѡ���ֶΣ��������߲���ͬʱ���ã� 
    ���֣�ʹ������״̬������������IO����NVCC_xxx�ص�֮��ʹ���뻺����������Զ�ά���ڹص�֮ǰ���߼�״̬�����ֵ�����ڵ͹��ĵ�Ӧ���еõ����֡� 
    ʹ�����״̬�������������ں˹���ص�֮��ʹIO������Զ�ά���ڹص�֮ǰ���߼�״̬(��Ҫע��������״̬������������������ͬʱ����)�����ֵͬ�����������ڵ͹��ĵ�Ӧ���� 
    ��ѡ��ֵ�� 
    0 PUE_0_Keeper �� Keeper ������ 
    1 PUE_1_Pull �� Pull ����
    
    2.6��PUS ����λ15-14 
    PUS(Pull Up / Down Config. Field)������/�������ã�������Ҫ�������������źŵ�����������ȷ�������źŵĵ�ƽ����ֹ���ţ� 
    ��ѡ��ֵ�� 
    00 PUS_0_100K_Ohm_Pull_Down �� 100K Ohm Pull Down ��100Kŷķ������ 
    01 PUS_1_47K_Ohm_Pull_Up �� 47K Ohm Pull Up ��47Kŷķ������ 
    10 PUS_2_100K_Ohm_Pull_Up �� 100K Ohm Pull Up ��100Kŷķ������ 
    11 PUS_3_22K_Ohm_Pull_Up �� 22K Ohm Pull Up ��22Kŷķ������
    
    2.77��HYS ����λ16 
    HYS(Hyst. Enable Field)������ʹ���ֶΣ�������Ϊ����ʱ��Ч 
    0 HYS_0_Hysteresis_Disabled �� Hysteresis Disabled����ֹ���� 
    1 HYS_1_Hysteresis_Enabled �� Hysteresis Enabled�� ʹ�ܴ���*/
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0xF080); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0xF080);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0xF080);
    
    //һ������ڹܽŶ���
    gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIOΪ���뷽��
                                            1,                    //�ߵ�ƽ
                                            kGPIO_NoIntmode,      //�������ж�
                                            };
    
    GPIO_PinInit(GPIO2, 27, &GPIO_Input_Config);        // GPIO����ڣ����ж�
    GPIO_PinInit(GPIO2, 30, &GPIO_Input_Config);        // GPIO����ڣ����ж�
    GPIO_PinInit(GPIO3,  4, &GPIO_Input_Config);        // GPIO����ڣ����ж�
    
}


/**
  * @brief    ��ȡ����״̬
  *
  * @param    mode�� 0����֧��������;   1��֧��������;
  *
  * @return   0���ް�������  1��KEY0����  2:KEY1����  3:KEY2����
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //�����ɿ���־
    if(mode==1) 
    {
        key_up=1;      //֧������
    }
    if(key_up && (GPIO_PinRead(GPIO2, 27)==0 || GPIO_PinRead(GPIO2, 30)==0 || GPIO_PinRead(GPIO3, 4)==0))
    {
        delayms(10);   //����
        key_up=0;
        if(GPIO_PinRead(GPIO2, 27)==0)      
        {
            return 1;
        }
        
        else if(GPIO_PinRead(GPIO2, 30)==0) 
        {
            return 2;    
        }
        
        else if(GPIO_PinRead(GPIO3,  4)==0) 
        {
            return 3;    
        }
        
    }
    if(GPIO_PinRead(GPIO2, 27)==1 && GPIO_PinRead(GPIO2, 30)==1 && GPIO_PinRead(GPIO3,  4)==1) 
    {
        key_up=1;   
    }
    return 0;   //�ް�������
    
}



/*!
  * @brief    ����GPIIO�ⲿ�ж�
  *
  * @param    
  *
  * @return   
  *
  * @note     ����ĸ���ϲ�ͬ�İ��� ��ͬ��ɫ�ĵ���˸
  *
  * @example  
  *
  * @date     2019/8/19 ����һ
  */
void Test_ExtiKEY(void)
{
    LED_Init();
    
    /*GPIO �ⲿ�ж����ÿ�ʼ*/
    CLOCK_EnableClock(kCLOCK_Iomuxc);                         // IO��ʱ��ʹ��
    gpio_pin_config_t GPIO_Input_Config =                     // GPIO��ʼ���ṹ��
    {
        .direction = kGPIO_DigitalInput,                      // ����
        .outputLogic = 0U,                                  
        .interruptMode = kGPIO_IntFallingEdge                 // �½����ж�
    };
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,0U);        // ���ùܽŸ��ù���                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0x90B0U);// ���ùܽ� ���� 
    GPIO_PinInit(GPIO2, 27U, &GPIO_Input_Config);             // GPIO����ڣ����� �½��ش����ж�
    GPIO_PortEnableInterrupts(GPIO2,1<<27);			          // C13 GPIO2_27�ж�ʹ��
    
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,0U);        // ���ùܽŸ��ù���                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0x90B0U);// ���ùܽ� ����
    GPIO_PinInit(GPIO2, 30U, &GPIO_Input_Config);             // GPIO����ڣ����� �½��ش����ж�   
    GPIO_PortEnableInterrupts(GPIO2,1<<30);			          // C14 GPIO2_30�ж�ʹ��
 
    
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);        // ���ùܽŸ��ù���                          
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0x90B0U);// ���ùܽ� ����
    GPIO_PinInit(GPIO3, 4U, &GPIO_Input_Config);                 // GPIO����ڣ����� �½��ش����ж�   
    GPIO_PortEnableInterrupts(GPIO3,1<<4);			             // P2 GPIO3_4�ж�ʹ��
    
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //ʹ��GPIO2_16~31IO���ж�  
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(GPIO3_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO3_Combined_0_15_IRQn);			          //ʹ��GPIO3_0~15IO���ж� 
    /*GPIO �ⲿ�ж����ý���*/  
    
    /* ��irq.c �� �����ж� */
    while(1);
}


#else //ʹ�÷�װ����fsl�����GPIO

/**
  * @brief    ��ʼ������ĸ���ϵİ���
  *
  * @param    
  *
  * @return   
  *
  * @note     GPIO3_IO04 ------ �ܽ�P2   ----- >  ĸ�尴��K2
  * @note     GPIO2_IO27 ------ �ܽ�C13  ----- >  ĸ�尴��K0
  * @note     GPIO2_IO30 ------ �ܽ�C14  ----- >  ĸ�尴��k1
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void KEY_Init(void)
{  
    /* ������������ */
    LQ_PinInit(P2,  PIN_MODE_INPUT_PULLUP, 1);
    LQ_PinInit(C13, PIN_MODE_INPUT_PULLUP, 1);
    LQ_PinInit(C14, PIN_MODE_INPUT_PULLUP, 1);  
}


/**
  * @brief    ��ȡ����״̬
  *
  * @param    mode�� 0����֧��������;   1��֧��������;
  *
  * @return   0���ް�������  1��KEY0����  2:KEY1����  3:KEY2����
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //�����ɿ���־
    if(mode==1) 
    {
        key_up=1;      //֧������
    }
    if(key_up && (LQ_PinRead(C13)==0 || LQ_PinRead(C14)==0 || LQ_PinRead(P2)==0))
    {
        delayms(10);   //����
        key_up=0;
        if(LQ_PinRead(C13)==0)      
        {
            return 1;
        }
        
        else if(LQ_PinRead(C14)==0) 
        {
            return 2;    
        }
        
        else if(LQ_PinRead(P2)==0) 
        {
            return 3;    
        }
        
    }
    if(LQ_PinRead(C13)==1 && LQ_PinRead(C14)==1 && LQ_PinRead(P2)==1) 
    {
        key_up=1;   
    }
    return 0;   //�ް�������
    
}



/*!
  * @brief    ����GPIIO�ⲿ�ж�
  *
  * @param    
  *
  * @return   
  *
  * @note     ����ĸ���ϲ�ͬ�İ��� ��ͬ��ɫ�ĵ���˸
  *
  * @example  
  *
  * @date     2019/8/19 ����һ
  */
void Test_ExtiKEY(void)
{
    LED_Init();
    
    /* �����½��ش����ж� */
    LQ_PinExti(C13, PIN_IRQ_MODE_FALLING);
    LQ_PinExti(C14, PIN_IRQ_MODE_FALLING);
    LQ_PinExti(P2,  PIN_IRQ_MODE_FALLING);
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO2_Combined_16_31_IRQn);			          //ʹ��GPIO2_16~31IO���ж�  
    
    //���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(GPIO3_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
	EnableIRQ(GPIO3_Combined_0_15_IRQn);			          //ʹ��GPIO3_0~15IO���ж� 
    /*GPIO �ⲿ�ж����ý���*/  
    
    /* ��irq.c �� �����ж� */
    while(1);



}
#endif

/**
  * @brief    ���԰���
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/4 ���ڶ�
  */
void Test_KEY(void)
{ 
    LED_Init();
    KEY_Init(); 
    while (1)
    {  
        switch(KEY_Read(1))  
        {
            case 1:
                LED_Color(red);     
                break;           
            case 2:      
                LED_Color(green);   
                break;
            case 3:      
                LED_Color(blue);    
                break;
            default:
                LED_Color(white);   
                break;
        }

        delayms(50);
    }
}


