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
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "LQ_FlexCAN.h"
#include "LQ_LED.h"

/* �����ⲿ��ʱ���� */
extern void delayms(uint16_t ms);

void CAN_PinInit(CAN_Type *base);

/*!
  * @brief    CAN ��ʼ��
  *
  * @param    base     :CANģ�� CAN1 CAN2 CAN3
  * @param    baudrate :CAN����
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void CAN_Init(CAN_Type *base, uint32_t baudrate)
{
    /* IO��ʼ�� */
    CAN_PinInit(base);

    /* CAN��ʼ���ṹ�� */
    flexcan_config_t can_config;
    
    FLEXCAN_GetDefaultConfig(&can_config); 
    
    can_config.baudRate = baudrate;
    can_config.enableSelfWakeup = true;
    
    /* ��ʼ��CAN  Ĭ��ʱ��Դ 40M */
    FLEXCAN_Init(base, &can_config, 40000000UL);       
    
}


/*!
  * @brief    ����CAN��������
  *
  * @param    base     :CANģ�� CAN1 CAN2 CAN3  
  * @param    RxMb_num :ʹ�õ������ 
  * @param    id       :����֡��ID 
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void CAN_RxConfig(CAN_Type *base, uint8_t RxMb_num, uint32_t id)
{
    /* ʹ��CAN�Ľ����ж� */
    FLEXCAN_EnableMbInterrupts(base, 1<<RxMb_num);
    
    /* ����Ҫ���յ�ID */
    FLEXCAN_SetRxMbGlobalMask(base, FLEXCAN_RX_MB_STD_MASK(id, 0, 0));
    
    flexcan_rx_mb_config_t mbConfig;
    
    /* ����RX���� ��׼֡������֡ ID  */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type = kFLEXCAN_FrameTypeData;
    mbConfig.id = FLEXCAN_ID_STD(id);
    
    FLEXCAN_SetRxMbConfig(base, RxMb_num, &mbConfig, true);
}


/*!
  * @brief    ����CAN��������
  *
  * @param    base     :CANģ�� CAN1 CAN2 CAN3  
  * @param    TxMb_num :ʹ�õ������  
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void CAN_TxConfig(CAN_Type *base, uint8_t TxMb_num)
{
    FLEXCAN_SetTxMbConfig(base, TxMb_num, true);  
}


/*!
  * @brief    CAN����֡
  *
  * @param    base     :CANģ�� CAN1 CAN2 CAN3  
  * @param    TxMb_num :ʹ�õ������   
  * @param    id       :����֡ID
  * @param    msg      :��ŷ������ݵ�ַ 
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void CAN_Tx(CAN_Type *base, uint8_t TxMb_num, uint32_t id, uint8_t *msg)
{
    flexcan_frame_t can_txframe;
    
    can_txframe.format = kFLEXCAN_FrameFormatStandard;    //��׼��ʽ
    can_txframe.type   = kFLEXCAN_FrameTypeData;          //����֡
    can_txframe.id     = FLEXCAN_ID_STD(id);              //��׼ID
    can_txframe.length = 8;                               //����8
    
    
    can_txframe.dataByte0 = msg[0];
    can_txframe.dataByte1 = msg[1];
    can_txframe.dataByte2 = msg[2];
    can_txframe.dataByte3 = msg[3];
    can_txframe.dataByte4 = msg[4];
    can_txframe.dataByte5 = msg[5];
    can_txframe.dataByte6 = msg[6];
    can_txframe.dataByte7 = msg[7];
    
    FLEXCAN_TransferSendBlocking(base, TxMb_num, &can_txframe);
    
}


/*!
  * @brief    ����CAN
  *
  * @param    
  *
  * @return   
  *
  * @note     �շ�IDһ�� CAN����յ��Լ����͵�����
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void Test_CAN(void)
{
	/* CAN��ʼ�� */
    CAN_Init(CAN2, 1000000UL);
	
	/* ���ý������� */
    CAN_RxConfig(CAN2, 1, 0x12);
    
	/* ���ý��������ж����ȼ� */
    NVIC_SetPriority(CAN2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
    EnableIRQ(CAN2_IRQn);
    
	/* ���÷������� */
    CAN_TxConfig(CAN2, 2);
    
    LED_Init();
    
    uint8_t txbuf[8];
    while(1)
    {
        delayms(1000);
        LED_ColorReverse(red);
		
		/* �������� */
        CAN_Tx(CAN2, 2, 0x12, txbuf);
        txbuf[0]++;
    }

}


/*!
  * @brief    CAN �ܽų�ʼ��
  *
  * @param    base     :CANģ�� CAN1 CAN2 CAN3
  *
  * @return   
  *
  * @note     �ڲ�����
  *
  * @see      
  *
  * @date     2019/8/21 ������
  */
void CAN_PinInit(CAN_Type *base)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);
    
    switch((uint32_t)base)
    {
      case CAN1_BASE:
        if(FLEX_CAN1_TX == M3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXCAN1_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXCAN1_TX,0x10B0);
        }
        else if(FLEX_CAN1_TX == A4)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_17_FLEXCAN1_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_17_FLEXCAN1_TX,0x10B0);
        }
        else if(FLEX_CAN1_TX == E8)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_FLEXCAN1_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_FLEXCAN1_TX,0x10B0);
        }
        else if(FLEX_CAN1_TX == H13)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_FLEXCAN1_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_FLEXCAN1_TX,0x10B0);
        }
        
        if(FLEX_CAN1_RX == B2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_FLEXCAN1_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_18_FLEXCAN1_RX,0x10B0);
        }
        else if(FLEX_CAN1_RX == M4)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXCAN1_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXCAN1_RX,0x10B0);
        }
        else if(FLEX_CAN1_RX == D8)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_FLEXCAN1_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_FLEXCAN1_RX,0x10B0);
        }
        else if(FLEX_CAN1_RX == M13)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_FLEXCAN1_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_FLEXCAN1_RX,0x10B0);
        }
        break;
        
      case CAN2_BASE:
        if(FLEX_CAN2_TX == H14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX,0x10B0);
        }
        else if(FLEX_CAN2_TX == C2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_FLEXCAN2_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_09_FLEXCAN2_TX,0x10B0);
        }
        else if(FLEX_CAN2_TX == M11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_FLEXCAN2_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_FLEXCAN2_TX,0x10B0);
        }
        else if(FLEX_CAN2_TX == A12)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_FLEXCAN2_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_FLEXCAN2_TX,0x10B0);
        }
        
        if(FLEX_CAN2_RX == G1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_10_FLEXCAN2_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_10_FLEXCAN2_RX,0x10B0);
        }
        else if(FLEX_CAN2_RX == L10)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX,0x10B0);
        }
        else if(FLEX_CAN2_RX == G11)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_FLEXCAN2_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_FLEXCAN2_RX,0x10B0);
        }
        else if(FLEX_CAN2_RX == A13)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_FLEXCAN2_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_FLEXCAN2_RX,0x10B0);
        }
        break;
        
      case CAN3_BASE:
        if(FLEX_CAN3_TX == C3)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_36_FLEXCAN3_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_36_FLEXCAN3_TX,0x10B0);
        }
        else if(FLEX_CAN3_TX == G13)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_FLEXCAN3_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_FLEXCAN3_TX,0x10B0);
        }
        else if(FLEX_CAN3_TX == H14)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_FLEXCAN3_TX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_14_FLEXCAN3_TX,0x10B0);
        }
        
        if(FLEX_CAN3_RX == E4)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_37_FLEXCAN3_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_37_FLEXCAN3_RX,0x10B0);
        }
        else if(FLEX_CAN3_RX == G10)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_FLEXCAN3_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_FLEXCAN3_RX,0x10B0);
        }
        else if(FLEX_CAN3_RX == L10)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_FLEXCAN3_RX,0U);
            IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_FLEXCAN3_RX,0x10B0);
        }
        
        break;
    } 

}


