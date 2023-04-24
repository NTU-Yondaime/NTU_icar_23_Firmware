/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1064���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.30.1�����ϰ汾
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_trng.h"
#include "LQ_TRNG.h"
#include "LQ_LED.h"
#include "LQ_SGP18T.h"
#include "LQ_12864.h"
#include "stdio.h"
#include "include.h"
/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);


#define TRNG_EXAMPLE_RANDOM_NUMBER 5  //ÿ������5��32λ�����

/**
  * @brief    Ӳ�����������
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
#ifdef OLED

void Test_RNG(void)
{
    char txt[16];
    LCD_Init();                  //LCD��ʼ��
    LCD_CLS();                   //LCD����
    LCD_P8x16Str(4,0,"LQ TRNG Test"); 
   
    uint32_t i;
    trng_config_t trngConfig;
    status_t status;
    uint32_t data[TRNG_EXAMPLE_RANDOM_NUMBER];


    PRINTF("TRNG Peripheral Driver Example\r\n");

    /* Initialize TRNG configuration structure to default.*/
    /*
     * trngConfig.lock = TRNG_USER_CONFIG_DEFAULT_LOCK;
     * trngConfig.clockMode = kTRNG_ClockModeRingOscillator;
     * trngConfig.ringOscDiv = TRNG_USER_CONFIG_DEFAULT_OSC_DIV;
     * trngConfig.sampleMode = kTRNG_SampleModeRaw;
     * trngConfig.entropyDelay = TRNG_USER_CONFIG_DEFAULT_ENTROPY_DELAY;
     * trngConfig.sampleSize = TRNG_USER_CONFIG_DEFAULT_SAMPLE_SIZE;
     * trngConfig.sparseBitLimit = TRNG_USER_CONFIG_DEFAULT_SPARSE_BIT_LIMIT;
     * trngConfig.retryCount = TRNG_USER_CONFIG_DEFAULT_RETRY_COUNT;
     * trngConfig.longRunMaxLimit = TRNG_USER_CONFIG_DEFAULT_RUN_MAX_LIMIT;
     * trngConfig.monobitLimit.maximum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MAXIMUM;
     * trngConfig.monobitLimit.minimum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MINIMUM;
     * trngConfig.runBit1Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MAXIMUM;
     * trngConfig.runBit1Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MINIMUM;
     * trngConfig.runBit2Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MAXIMUM;
     * trngConfig.runBit2Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MINIMUM;
     * trngConfig.runBit3Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MAXIMUM;
     * trngConfig.runBit3Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MINIMUM;
     * trngConfig.runBit4Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MAXIMUM;
     * trngConfig.runBit4Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MINIMUM;
     * trngConfig.runBit5Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MAXIMUM;
     * trngConfig.runBit5Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MINIMUM;
     * trngConfig.runBit6PlusLimit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MAXIMUM;
     * trngConfig.runBit6PlusLimit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MINIMUM;
     * trngConfig.pokerLimit.maximum = TRNG_USER_CONFIG_DEFAULT_POKER_MAXIMUM;
     * trngConfig.pokerLimit.minimum = TRNG_USER_CONFIG_DEFAULT_POKER_MINIMUM;
     * trngConfig.frequencyCountLimit.maximum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MAXIMUM;
     * trngConfig.frequencyCountLimit.minimum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MINIMUM;
     */
    TRNG_GetDefaultConfig(&trngConfig);//��ȡĬ������
    
    //����ģʽ����ΪVon Neumann,��ýϼ����ݣ�Ҳ�ɲ�������   
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    //��ʼ��TRNG
    status = TRNG_Init(TRNG, &trngConfig);
    
    if (kStatus_Success == status)
    {
        while (1)
        {
            PRINTF("\r\n����%d��32λ�����������: \r\n", TRNG_EXAMPLE_RANDOM_NUMBER);

            //��������
            status = TRNG_GetRandomData(TRNG, data, sizeof(data));
            if (status == kStatus_Success)
            {
                //������������������
                for (i = 0; i < TRNG_EXAMPLE_RANDOM_NUMBER; i++)
                {
                    PRINTF("Random[%d] = 0x%X\r\n", i, data[i]);
                    sprintf(txt,"n%d : %x ", i, data[i]);
                    LCD_P6x8Str(0,i+2,txt);
                }
            }
            else
            {
                PRINTF("TRNG failed!\r\n");
                LCD_CLS();                //����
                LCD_P8x16Str(2,0,"LQ TRNG Test Fail");
            }            
        LED_ColorReverse(blue);//���Ʒ�ת
        delayms(1000);          //��ʱ1��
        }
    }
    else
    {
        PRINTF("TRNG initialize failed!\r\n");
    }
    return ;
}


#else



void Test_RNG(void)
{
    char txt[16];
    TFTSPI_Init(1);                 //LCD��ʼ��  0:����  1������
    TFTSPI_CLS(u16BLUE);            //����
    TFTSPI_P8X16Str(2,0,"LQ TRNG Test",u16RED,u16BLUE);
   
    uint32_t i;
    trng_config_t trngConfig;
    status_t status;
    uint32_t data[TRNG_EXAMPLE_RANDOM_NUMBER];


    PRINTF("TRNG Peripheral Driver Example\r\n");

    /* Initialize TRNG configuration structure to default.*/
    /*
     * trngConfig.lock = TRNG_USER_CONFIG_DEFAULT_LOCK;
     * trngConfig.clockMode = kTRNG_ClockModeRingOscillator;
     * trngConfig.ringOscDiv = TRNG_USER_CONFIG_DEFAULT_OSC_DIV;
     * trngConfig.sampleMode = kTRNG_SampleModeRaw;
     * trngConfig.entropyDelay = TRNG_USER_CONFIG_DEFAULT_ENTROPY_DELAY;
     * trngConfig.sampleSize = TRNG_USER_CONFIG_DEFAULT_SAMPLE_SIZE;
     * trngConfig.sparseBitLimit = TRNG_USER_CONFIG_DEFAULT_SPARSE_BIT_LIMIT;
     * trngConfig.retryCount = TRNG_USER_CONFIG_DEFAULT_RETRY_COUNT;
     * trngConfig.longRunMaxLimit = TRNG_USER_CONFIG_DEFAULT_RUN_MAX_LIMIT;
     * trngConfig.monobitLimit.maximum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MAXIMUM;
     * trngConfig.monobitLimit.minimum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MINIMUM;
     * trngConfig.runBit1Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MAXIMUM;
     * trngConfig.runBit1Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MINIMUM;
     * trngConfig.runBit2Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MAXIMUM;
     * trngConfig.runBit2Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MINIMUM;
     * trngConfig.runBit3Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MAXIMUM;
     * trngConfig.runBit3Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MINIMUM;
     * trngConfig.runBit4Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MAXIMUM;
     * trngConfig.runBit4Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MINIMUM;
     * trngConfig.runBit5Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MAXIMUM;
     * trngConfig.runBit5Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MINIMUM;
     * trngConfig.runBit6PlusLimit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MAXIMUM;
     * trngConfig.runBit6PlusLimit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MINIMUM;
     * trngConfig.pokerLimit.maximum = TRNG_USER_CONFIG_DEFAULT_POKER_MAXIMUM;
     * trngConfig.pokerLimit.minimum = TRNG_USER_CONFIG_DEFAULT_POKER_MINIMUM;
     * trngConfig.frequencyCountLimit.maximum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MAXIMUM;
     * trngConfig.frequencyCountLimit.minimum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MINIMUM;
     */
    TRNG_GetDefaultConfig(&trngConfig);//��ȡĬ������
    //����ģʽ����ΪVon Neumann,��ýϼ����ݣ�Ҳ�ɲ�������   
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    //��ʼ��TRNG
    status = TRNG_Init(TRNG, &trngConfig);
    if (kStatus_Success == status)
    {
        while (1)
        {
            PRINTF("\r\n����%d��32λ�����������: \r\n", TRNG_EXAMPLE_RANDOM_NUMBER);

            //��������
            status = TRNG_GetRandomData(TRNG, data, sizeof(data));
            if (status == kStatus_Success)
            {
                //������������������
                for (i = 0; i < TRNG_EXAMPLE_RANDOM_NUMBER; i++)
                {
                    PRINTF("Random[%d] = 0x%X\r\n", i, data[i]);
                    sprintf(txt,"n%d : %x ", i, data[i]);
                    TFTSPI_P8X16Str(0,i+1,txt,u16RED,u16BLUE);
                }
            }
            else
            {
                PRINTF("TRNG failed!\r\n");
                TFTSPI_CLS(u16BLUE);                //����
                TFTSPI_P8X16Str(2,0,"LQ TRNG Test Fail",u16RED,u16BLUE);
            }            
        LED_ColorReverse(blue);//���Ʒ�ת
        delayms(1000);          //��ʱ1��
        }
    }
    else
    {
        PRINTF("TRNG initialize failed!\r\n");
    }
    return ;
}
#endif
