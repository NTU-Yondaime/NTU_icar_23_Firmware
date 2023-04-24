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
#include "include.h"
#include "fsl_qtmr.h"
#include "LQ_LED.h"
#include "LQ_12864.h"
#include "LQ_SGP18T.h"
#include "LQ_QTMR.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "stdio.h"

/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);

/**
  * @brief    QTMR PWM�����ʼ��
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    frequency�� PWM  Ƶ��
  * @param    duty     �� ��ʼռ�ձ�*QTMR_DUTY_MAX   ��Χ��1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     QTMR Ĭ��ʹ�� IPGʱ��Դ  150M   PWMƵ�ʲ�ҪС��18Hz
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PWMInit(TMR_Type *base, qtmr_channel_selection_t channel, uint32_t  frequency, uint16_t duty)
{
    QTMR_PinInit(base, channel);
    
    qtmr_config_t qtmrConfig;    //qtmr���ýṹ��

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;       //ʱ����Ҫ��Դ
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;  //ʱ�ӵڶ���Դ
     */
    QTMR_GetDefaultConfig(&qtmrConfig);                      //�õ�Ĭ�Ͻṹ��
    
    /* ������ѷ�Ƶϵ�� */
    uint32_t pwmSourceClockInHz;  
    pwmSourceClockInHz = CLOCK_GetFreq(kCLOCK_IpgClk);
    
    uint32_t temp = pwmSourceClockInHz / frequency;
    uint8_t div = 0;
    while(temp > 0xFFFF)
    {
        div++;
        temp = temp >> 1;
        pwmSourceClockInHz = pwmSourceClockInHz >> 1;
    }
    assert(div < 8);
    
    qtmrConfig.primarySource = (qtmr_primary_count_source_t)(div + 8);  

    QTMR_Init(base, channel, &qtmrConfig);     
    
    //��ʼ��PWM   TMR    ͨ��     PWMƵ��   ռ�ձ�  ����      ʱ��Ƶ��
    QTMR_SetupPwm(base, channel, frequency, duty,  false,   pwmSourceClockInHz); 
    
    QTMR_PWMSetDuty(base, channel, duty);
        
    QTMR_StartTimer(base, channel, kQTMR_PriSrcRiseEdge); //��primaryʱ��Դ�������ؼ���

}


/**
  * @brief    QTMR ����PWM���ռ�ձ�
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  * @param    duty     �� ռ�ձ�*QTMR_DUTY_MAX   ��Χ��1 - QTMR_DUTY_MAX
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PWMSetDuty(TMR_Type *base, qtmr_channel_selection_t channel, uint16_t duty)
{
    uint32_t period,hightime,lowtime;

    //һ��PWM������Ҫ�ļ���ֵ
    period = base->CHANNEL[channel].CMPLD1 + base->CHANNEL[channel].CMPLD2;
    
    hightime = (period * (QTMR_DUTY_MAX - duty)) / QTMR_DUTY_MAX;     
    lowtime = period - hightime;           
    
    base->CHANNEL[channel].CMPLD1 = hightime;
    base->CHANNEL[channel].CMPLD2 = lowtime;
}


/**
  * @brief    QTMR �������������������ʼ��
  *
  * @param    base       �� TMR1  -  TMR4
  * @param    channel_A  �� kQTMR_Channel_0  -  kQTMR_Channel_3  
  * @param    channel_B  �� kQTMR_Channel_0  -  kQTMR_Channel_3 
  *
  * @return   
  *
  * @note     Ĭ��ʹ�ô��������ģʽ
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_ENCInit(TMR_Type *base, qtmr_channel_selection_t channel_A, qtmr_channel_selection_t channel_B)
{
    QTMR_PinInit(base, channel_A);
    QTMR_PinInit(base, channel_B);
    
    qtmr_config_t qtmrConfig;
     /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);      //�õ�Ĭ�ϵĲ���

    
    qtmrConfig.primarySource = (qtmr_primary_count_source_t)channel_A;    //Counter0InputPin��Ϊ��Ҫ������Դ�������������� ���� A��
    qtmrConfig.secondarySource = (qtmr_input_source_t)channel_B;          //Counter1InputPin��Ϊ��Ҫ������Դ�����ڷ������� ���� B��
    
    QTMR_Init(base, channel_A, &qtmrConfig);       

    QTMR_StartTimer(base, channel_A, kQTMR_PriSrcRiseEdgeSecDir);   //����ģʽʹ�ô��������ģʽ  kQTMR_QuadCountMode ��������

}

/**
  * @brief    ��ȡ QTMR����ֵ
  *
  * @param    base       �� TMR1  -  TMR4
  * @param    channel_A  �� kQTMR_Channel_0  -  kQTMR_Channel_3   
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
inline int16_t QTMR_ENCGetCount(TMR_Type *base, qtmr_channel_selection_t channel_A)
{
    int16_t val = base->CHANNEL[channel_A].CNTR;
    base->CHANNEL[channel_A].CNTR = 0;
    return val;
}


/**
  * @brief    ����QTMR �Ĵ������������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void Test_QTMR_ENC(void)
{
    char txt[32];
    short val = 0;
    LED_Init();
    
    QTMR_ENCInit(TMR3, kQTMR_Channel_0, kQTMR_Channel_1);
    
#ifdef OLED
    LCD_Init();
    LCD_CLS();                     //LCD����
    LCD_P8x16Str(13,0,"LQ QTMR ENC");
#else 
    TFTSPI_Init(1);                //TFT1.8��ʼ��  
    TFTSPI_CLS(u16BLUE);           //����
    TFTSPI_P8X16Str(3,0,"LQ QTMR ENC",u16RED,u16BLUE);
#endif
    
    PRINTF("\n QTMR ENC TEST \n");
    while (1)
    {   
        val = QTMR_ENCGetCount(TMR3, kQTMR_Channel_0);
        sprintf(txt,"ENC :%5d",val);
#ifdef OLED
        LCD_P8x16Str(2,3,txt);
#else
        TFTSPI_P8X16Str(3,3,txt,u16RED,u16BLUE);
#endif
        PRINTF(txt); 
		PRINTF("\n");
        LED_ColorReverse(red); //�����˸
        delayms(500);
    }
    
}


/**
  * @brief    ����QTMR ���PWM����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void Test_QTMR_PWM(void)
{
    LED_Init();
    
    QTMR_PWMInit(TMR1, kQTMR_Channel_0, 100, 1);
    QTMR_PWMInit(TMR1, kQTMR_Channel_1, 200, 1);
    QTMR_PWMInit(TMR1, kQTMR_Channel_2, 300, 1);
    QTMR_PWMInit(TMR1, kQTMR_Channel_3, 400, 1);
    
    QTMR_PWMInit(TMR2, kQTMR_Channel_0, 1000, 1);
    QTMR_PWMInit(TMR2, kQTMR_Channel_1, 2000, 1);
    QTMR_PWMInit(TMR2, kQTMR_Channel_2, 3000, 1);
    QTMR_PWMInit(TMR2, kQTMR_Channel_3, 4000, 1);
    
    QTMR_PWMInit(TMR3, kQTMR_Channel_0, 10000, 1);
    QTMR_PWMInit(TMR3, kQTMR_Channel_1, 20000, 1);
    QTMR_PWMInit(TMR3, kQTMR_Channel_2, 30000, 1);
    QTMR_PWMInit(TMR3, kQTMR_Channel_3, 40000, 1);
    
    QTMR_PWMInit(TMR4, kQTMR_Channel_0, 50, 1);
    QTMR_PWMInit(TMR4, kQTMR_Channel_1, 20, 1);
    QTMR_PWMInit(TMR4, kQTMR_Channel_2, 30, 1);
    QTMR_PWMInit(TMR4, kQTMR_Channel_3, 40, 1);
                               
    delayms(1000);
    uint16_t i = 0;
    while(1)
    {
        QTMR_PWMSetDuty(TMR1, kQTMR_Channel_0, i);
        QTMR_PWMSetDuty(TMR1, kQTMR_Channel_1, i);
        QTMR_PWMSetDuty(TMR1, kQTMR_Channel_2, i);
        QTMR_PWMSetDuty(TMR1, kQTMR_Channel_3, i);
        
        QTMR_PWMSetDuty(TMR2, kQTMR_Channel_0, i);
        QTMR_PWMSetDuty(TMR2, kQTMR_Channel_1, i);
        QTMR_PWMSetDuty(TMR2, kQTMR_Channel_2, i);
        QTMR_PWMSetDuty(TMR2, kQTMR_Channel_3, i);
        
        QTMR_PWMSetDuty(TMR3, kQTMR_Channel_0, i);
        QTMR_PWMSetDuty(TMR3, kQTMR_Channel_1, i);
        QTMR_PWMSetDuty(TMR3, kQTMR_Channel_2, i);
        QTMR_PWMSetDuty(TMR3, kQTMR_Channel_3, i);
        
        QTMR_PWMSetDuty(TMR4, kQTMR_Channel_0, i);
        QTMR_PWMSetDuty(TMR4, kQTMR_Channel_1, i);
        QTMR_PWMSetDuty(TMR4, kQTMR_Channel_2, i);
        QTMR_PWMSetDuty(TMR4, kQTMR_Channel_3, i);

        i += 10;
        delayms(200);
        if(i > QTMR_DUTY_MAX) i = 1;
        LED_ColorReverse(red); //�����˸
    }

}



/**
  * @brief    QTMR ͨ���ܽų�ʼ��
  *
  * @param    base     �� TMR1  -  TMR4
  * @param    channel  �� kQTMR_Channel_0  -  kQTMR_Channel_3
  *
  * @return   
  *
  * @note     �ڲ�����
  *
  * @example  
  *
  * @date     2019/6/12 ������
  */
void QTMR_PinInit(TMR_Type *base, qtmr_channel_selection_t channel)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    
    if(base == TMR1)
    {
        if(channel == kQTMR_Channel_0)
        {
            if(QTMR1_CH0 == D7)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_00_QTIMER1_TIMER0,       /* GPIO_B0_00 is configured as QTIMER1_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL(0x00U) /* QTIMER1 TMR0 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_1)
        {
            if(QTMR1_CH1 == E7)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_01_QTIMER1_TIMER1,       /* GPIO_B0_01 is configured as QTIMER1_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER1_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER1_TRM1_INPUT_SEL(0x00U) /* QTIMER1 TMR1 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_2)
        {
            if(QTMR1_CH2 == E8)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_02_QTIMER1_TIMER2,       /* GPIO_B0_02 is configured as QTIMER1_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER1_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER1_TRM2_INPUT_SEL(0x00U) /* QTIMER1 TMR2 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_3)
        {
            if(QTMR1_CH3 == A12)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B1_08_QTIMER1_TIMER3,       /* GPIO_B1_08 is configured as QTIMER1_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER1_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER1_TRM3_INPUT_SEL(0x00U) /* QTIMER1 TMR3 input select: input from IOMUX */
                                        );
            }
        }
    }
    else if(base == TMR2)
    {
        if(channel == kQTMR_Channel_0)
        {
            if(QTMR2_CH0 == D8)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_03_QTIMER2_TIMER0,       /* GPIO_B0_03 is configured as QTIMER2_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x00U) /* QTIMER2 TMR0 input select: input from IOMUX */
                                        );
            }
            else if(QTMR2_CH0 == B4)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_19_QTIMER2_TIMER0,      /* GPIO_EMC_19 is configured as QTIMER2_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x00U) /* QTIMER2 TMR0 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_1)
        {
            if(QTMR2_CH1 == C8)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_04_QTIMER2_TIMER1,       /* GPIO_B0_04 is configured as QTIMER2_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM1_INPUT_SEL(0x00U) /* QTIMER2 TMR1 input select: input from IOMUX */
                                        );
            }
            else if(QTMR2_CH1 == A3)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_20_QTIMER2_TIMER1,      /* GPIO_EMC_20 is configured as QTIMER2_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM1_INPUT_SEL(0x00U) /* QTIMER2 TMR1 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_2)
        {
            if(QTMR2_CH2 == B8)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_05_QTIMER2_TIMER2,       /* GPIO_B0_05 is configured as QTIMER2_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM2_INPUT_SEL(0x00U) /* QTIMER2 TMR2 input select: input from IOMUX */
                                        );
            }
            else if(QTMR2_CH2 == C1)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_21_QTIMER2_TIMER2,      /* GPIO_EMC_21 is configured as QTIMER2_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM2_INPUT_SEL(0x00U) /* QTIMER2 TMR2 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_3)
        {
            if(QTMR2_CH3 == A13)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B1_09_QTIMER2_TIMER3,       /* GPIO_B1_09 is configured as QTIMER2_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL(0x00U) /* QTIMER2 TMR3 input select: input from IOMUX */
                                        );
            }
            else if(QTMR2_CH3 == F1)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_22_QTIMER2_TIMER3,      /* GPIO_EMC_22 is configured as QTIMER2_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER2_TRM3_INPUT_SEL(0x00U) /* QTIMER2 TMR3 input select: input from IOMUX */
                                        );
            }
        }
    }
    else if(base == TMR3)
    {
        if(channel == kQTMR_Channel_0)
        {
            if(QTMR3_CH0 == A8)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_06_QTIMER3_TIMER0,       /* GPIO_B0_06 is configured as QTIMER3_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL(0x00U) /* QTIMER3 TMR0 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH0 == B1)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_15_QTIMER3_TIMER0,      /* GPIO_EMC_15 is configured as QTIMER3_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL(0x00U) /* QTIMER3 TMR0 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH0 == J11)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0,    /* GPIO_AD_B1_00 is configured as QTIMER3_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM0_INPUT_SEL(0x00U) /* QTIMER3 TMR0 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_1)
        {
            if(QTMR3_CH1 == A5)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_16_QTIMER3_TIMER1,      /* GPIO_EMC_16 is configured as QTIMER3_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL(0x00U) /* QTIMER3 TMR1 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH1 == A9)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_07_QTIMER3_TIMER1,       /* GPIO_B0_07 is configured as QTIMER3_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL(0x00U) /* QTIMER3 TMR1 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH1 == K11)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_AD_B1_01_QTIMER3_TIMER1,    /* GPIO_AD_B1_01 is configured as QTIMER3_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM1_INPUT_SEL(0x00U) /* QTIMER3 TMR1 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_2)
        {
            if(QTMR3_CH2 == A4)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_17_QTIMER3_TIMER2,      /* GPIO_EMC_17 is configured as QTIMER3_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL(0x00U) /* QTIMER3 TMR2 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH2 == B9)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_08_QTIMER3_TIMER2,       /* GPIO_B0_08 is configured as QTIMER3_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL(0x00U) /* QTIMER3 TMR2 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH2 == L11)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_AD_B1_02_QTIMER3_TIMER2,    /* GPIO_AD_B1_02 is configured as QTIMER3_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM2_INPUT_SEL(0x00U) /* QTIMER3 TMR2 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_3)
        {
            if(QTMR3_CH3 == B2)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_EMC_18_QTIMER3_TIMER3,      /* GPIO_EMC_18 is configured as QTIMER3_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL(0x00U) /* QTIMER3 TMR3 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH3 == B13)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B1_10_QTIMER3_TIMER3,       /* GPIO_B1_10 is configured as QTIMER3_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL(0x00U) /* QTIMER3 TMR3 input select: input from IOMUX */
                                        );
            }
            else if(QTMR3_CH3 == M12)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3,    /* GPIO_AD_B1_03 is configured as QTIMER3_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER3_TRM3_INPUT_SEL(0x00U) /* QTIMER3 TMR3 input select: input from IOMUX */
                                        );
            }
        }
    }
    else if(base == TMR4)
    {
        if(channel == kQTMR_Channel_0)
        {
            if(QTMR4_CH0 == C9)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_09_QTIMER4_TIMER0,       /* GPIO_B0_09 is configured as QTIMER4_TIMER0 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER4_TRM0_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER4_TRM0_INPUT_SEL(0x00U) /* QTIMER4 TMR0 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_1)
        {
            if(QTMR4_CH1 == D9)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_10_QTIMER4_TIMER1,       /* GPIO_B0_10 is configured as QTIMER4_TIMER1 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER4_TRM1_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER4_TRM1_INPUT_SEL(0x00U) /* QTIMER4 TMR1 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_2)
        {
            if(QTMR4_CH2 == A10)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B0_11_QTIMER4_TIMER2,       /* GPIO_B0_11 is configured as QTIMER4_TIMER2 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER4_TRM2_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER4_TRM2_INPUT_SEL(0x00U) /* QTIMER4 TMR2 input select: input from IOMUX */
                                        );
            }
        }
        else if(channel == kQTMR_Channel_3)
        {
            if(QTMR4_CH3 == C13)
            {
                IOMUXC_SetPinMux(
                                 IOMUXC_GPIO_B1_11_QTIMER4_TIMER3,       /* GPIO_B1_11 is configured as QTIMER4_TIMER3 */
                                 0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                     (~(IOMUXC_GPR_GPR6_QTIMER4_TRM3_INPUT_SEL_MASK))) /* Mask bits to zero which are setting */
                                    | IOMUXC_GPR_GPR6_QTIMER4_TRM3_INPUT_SEL(0x00U) /* QTIMER4 TMR3 input select: input from IOMUX */
                                        );
            }
        }
    }
}

