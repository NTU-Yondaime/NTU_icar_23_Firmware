/*----------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【dev.env.】IAR8.30.1及以上版本

编码器（ENC）
G13--A相或者LSB
F14--B相或者DIR

H2--A相或者LSB
J2--B相或者DIR

J3--A相或者LSB
K1--B相或者DIR

C11--A相或者LSB
B11--B相或者DIR
-------------------------------------------------------------*/
#include "LQ_Encoder.h"
#include "LQ_LED.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "fsl_enc.h"
#include "fsl_xbara.h"
#include "stdio.h"

/* 声明外部延时函数 */
extern void delayms(uint16_t ms);

/**
 * @brief    初始化 ENC模块
 *
 * @param    base        ： ENC1 - ENC4
 * @param    direction   ： 是否反向
 *
 * @return
 *
 * @note     默认 LSB+Dir模式  可以修改
 *
 * @example
 *
 * @date     2019/6/12 星期三
 */
void LQ_ENC_Init(ENC_Type *base, bool direction)
{
    ENC_PinInit(base);

    enc_config_t mEncConfigStruct;           // ENC初始化结构体
    ENC_GetDefaultConfig(&mEncConfigStruct); // 得到默认参数
    /* mEncConfigStruct默认参数
    config->enableReverseDirection = false;
    config->decoderWorkMode = kENC_DecoderWorkAsNormalMode;  //A B两相模式
    config->HOMETriggerMode = kENC_HOMETriggerDisabled;
    config->INDEXTriggerMode = kENC_INDEXTriggerDisabled;
    config->enableTRIGGERClearPositionCounter = false;
    config->enableTRIGGERClearHoldPositionCounter = false;
    config->enableWatchdog = false;
    config->watchdogTimeoutValue = 0U;
    config->filterCount = 0U;
    config->filterSamplePeriod = 0U;
    config->positionMatchMode = kENC_POSMATCHOnPositionCounterEqualToComapreValue;
    config->positionCompareValue = 0xFFFFFFFFU;
    config->revolutionCountCondition = kENC_RevolutionCountOnINDEXPulse;
    config->enableModuloCountMode = false;
    config->positionModulusValue = 0U;
    config->positionInitialValue = 0U;*/

    if (direction)
    {
        mEncConfigStruct.enableReverseDirection = true; // 反方向
    }

    mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode; // LSB+Dir模式   kENC_DecoderWorkAsNormalMode： 普通正交解码
    ENC_Init(base, &mEncConfigStruct);                                         // 初始化正交解码模块
    ENC_DoSoftwareLoadInitialPositionValue(base);                              /* 位置计数器初始值更新成0. */
}

/**
 * @brief    初始化 ENC管脚
 *
 * @param
 *
 * @return
 *
 * @note
 *
 * @example
 *
 * @date     2019/6/12 星期三
 */
void ENC_PinInit(ENC_Type *base)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc); /* 打开io时钟 */
    CLOCK_EnableClock(kCLOCK_Xbar1);

    if (base == ENC1)
    {
        if (ENC1_PHASE_A == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc1PhaseAInput);
        }
        else if (ENC1_PHASE_A == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc1PhaseAInput);
        }
        else if (ENC1_PHASE_A == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc1PhaseAInput);
        }
        else if (ENC1_PHASE_A == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc1PhaseAInput);
        }
        else if (ENC1_PHASE_A == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc1PhaseAInput);
        }
        else if (ENC1_PHASE_A == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_A == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        if (ENC1_PHASE_B == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc1PhaseBInput);
        }
        else if (ENC1_PHASE_B == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc1PhaseBInput);
        }
        else if (ENC1_PHASE_B == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc1PhaseBInput);
        }
        else if (ENC1_PHASE_B == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc1PhaseBInput);
        }
        else if (ENC1_PHASE_B == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc1PhaseBInput);
        }
        else if (ENC1_PHASE_B == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC1_PHASE_B == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
    }
    else if (base == ENC2)
    {
        if (ENC2_PHASE_A == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc2PhaseAInput);
        }
        else if (ENC2_PHASE_A == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc2PhaseAInput);
        }
        else if (ENC2_PHASE_A == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc2PhaseAInput);
        }
        else if (ENC2_PHASE_A == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc2PhaseAInput);
        }
        else if (ENC2_PHASE_A == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc2PhaseAInput);
        }
        else if (ENC2_PHASE_A == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_A == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc2PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        if (ENC2_PHASE_B == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc2PhaseBInput);
        }
        else if (ENC2_PHASE_B == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc2PhaseBInput);
        }
        else if (ENC2_PHASE_B == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc2PhaseBInput);
        }
        else if (ENC2_PHASE_B == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc2PhaseBInput);
        }
        else if (ENC2_PHASE_B == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc2PhaseBInput);
        }
        else if (ENC2_PHASE_B == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC2_PHASE_B == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc2PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
    }
    else if (base == ENC3)
    {
        if (ENC3_PHASE_A == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc3PhaseAInput);
        }
        else if (ENC3_PHASE_A == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc3PhaseAInput);
        }
        else if (ENC3_PHASE_A == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc3PhaseAInput);
        }
        else if (ENC3_PHASE_A == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseAInput);
        }
        else if (ENC3_PHASE_A == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc3PhaseAInput);
        }
        else if (ENC3_PHASE_A == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_A == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc3PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        if (ENC3_PHASE_B == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc3PhaseBInput);
        }
        else if (ENC3_PHASE_B == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc3PhaseBInput);
        }
        else if (ENC3_PHASE_B == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc3PhaseBInput);
        }
        else if (ENC3_PHASE_B == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseBInput);
        }
        else if (ENC3_PHASE_B == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc3PhaseBInput);
        }
        else if (ENC3_PHASE_B == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC3_PHASE_B == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc3PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
    }
    else if (base == ENC4)
    {
        if (ENC4_PHASE_A == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc4PhaseAInput);
        }
        else if (ENC4_PHASE_A == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc4PhaseAInput);
        }
        else if (ENC4_PHASE_A == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc4PhaseAInput);
        }
        else if (ENC4_PHASE_A == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc4PhaseAInput);
        }
        else if (ENC4_PHASE_A == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc4PhaseAInput);
        }
        else if (ENC4_PHASE_A == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_A == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        if (ENC4_PHASE_B == B1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_XBAR1_IN20, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc4PhaseBInput);
        }
        else if (ENC4_PHASE_B == H1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_XBAR1_IN24, 0U);
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc4PhaseBInput);
        }
        else if (ENC4_PHASE_B == J1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc4PhaseBInput);
        }
        else if (ENC4_PHASE_B == K1)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 & (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc4PhaseBInput);
        }
        else if (ENC4_PHASE_B == F2)
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_XBAR1_INOUT06, 0U);
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK))) |
                                IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U));
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc4PhaseBInput);
        }
        else if (ENC4_PHASE_B == H2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, /* GPIO_SD_B0_04 is configured as XBAR1_INOUT08 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == J2)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_05_XBAR1_INOUT09, /* GPIO_SD_B0_05 is configured as XBAR1_INOUT09 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == C3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_36_XBAR1_IN22,                                                             /* GPIO_EMC_36 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_00_XBAR1_XBAR_IN02,                                                        /* GPIO_EMC_00 is configured as XBAR1_XBAR_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == F3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_01_XBAR1_IN03,                                                             /* GPIO_EMC_01 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == H3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_08_XBAR1_INOUT17, /* GPIO_EMC_08 is configured as XBAR1_INOUT17 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == J3)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_01_XBAR1_INOUT05, /* GPIO_SD_B0_01 is configured as XBAR1_INOUT05 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_37_XBAR1_IN23,                                                             /* GPIO_EMC_37 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == F4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_02_XBAR1_INOUT04, /* GPIO_EMC_02 is configured as XBAR1_INOUT04 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_03_XBAR1_INOUT05, /* GPIO_EMC_03 is configured as XBAR1_INOUT05 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_5(0x00U)    /* IOMUXC XBAR_INOUT5 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout05, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT05 output assigned to XBARA1_IN5 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == H4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_07_XBAR1_INOUT09, /* GPIO_EMC_07 is configured as XBAR1_INOUT09 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_9(0x00U)    /* IOMUXC XBAR_INOUT9 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout09, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT09 output assigned to XBARA1_IN9 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == J4)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04, /* GPIO_SD_B0_00 is configured as XBAR1_INOUT04 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_4(0x00U)    /* IOMUXC XBAR_INOUT4 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT04 output assigned to XBARA1_IN4 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == A5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_16_XBAR1_IN21,                                                             /* GPIO_EMC_16 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_35_XBAR1_INOUT18, /* GPIO_EMC_35 is configured as XBAR1_INOUT18 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_05_XBAR1_INOUT07, /* GPIO_EMC_05 is configured as XBAR1_INOUT07 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U)    /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == H5)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_06_XBAR1_INOUT08, /* GPIO_EMC_06 is configured as XBAR1_INOUT08 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_8(0x00U)    /* IOMUXC XBAR_INOUT8 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == A6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_13_XBAR1_IN25,                                                             /* GPIO_EMC_13 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == B6)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_EMC_14_XBAR1_INOUT19, /* GPIO_EMC_14 is configured as XBAR1_INOUT19 */
                0U);                              /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == C10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_12_XBAR1_INOUT10, /* GPIO_B0_12 is configured as XBAR1_INOUT10 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_10(0x00U)    /* IOMUXC XBAR_INOUT10 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT10 output assigned to XBARA1_IN10 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == D10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_13_XBAR1_INOUT11, /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U)    /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT11 output assigned to XBARA1_IN11 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_14_XBAR1_INOUT12, /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U)    /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_11_XBAR1_IN23,                                                           /* GPIO_AD_B0_11 is configured as XBAR1_IN23 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN23 output assigned to XBARA1_IN23 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == H10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, /* GPIO_AD_B0_01 is configured as XBAR1_INOUT15 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == L10)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,                                                           /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN25 output assigned to XBARA1_IN25 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == A11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_00_XBAR1_INOUT14, /* GPIO_B1_00 is configured as XBAR1_INOUT14 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == B11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_01_XBAR1_INOUT15, /* GPIO_B1_01 is configured as XBAR1_INOUT15 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U)    /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT15 output assigned to XBARA1_IN15 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == C11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_02_XBAR1_INOUT16, /* GPIO_B1_02 is configured as XBAR1_INOUT16 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == D11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_03_XBAR1_INOUT17, /* GPIO_B1_03 is configured as XBAR1_INOUT17 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B0_15_XBAR1_INOUT13, /* GPIO_B0_15 is configured as XBAR1_INOUT13 */
                0U);                             /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_13(0x00U)    /* IOMUXC XBAR_INOUT13 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_03_XBAR1_INOUT17, /* GPIO_AD_B0_03 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == M11)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_02_XBAR1_INOUT16, /* GPIO_AD_B0_02 is configured as XBAR1_INOUT16 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_16(0x00U)    /* IOMUXC XBAR_INOUT16 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout16, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT16 output assigned to XBARA1_IN16 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == F12)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_07_XBAR1_INOUT19, /* GPIO_AD_B0_07 is configured as XBAR1_INOUT19 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_19(0x00U)    /* IOMUXC XBAR_INOUT19 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout19, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT19 output assigned to XBARA1_IN19 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == F13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_08_XBAR1_IN20,                                                           /* GPIO_AD_B0_08 is configured as XBAR1_IN20 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn20, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN20 output assigned to XBARA1_IN20 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G13)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_10_XBAR1_IN22,                                                           /* GPIO_AD_B0_10 is configured as XBAR1_IN22 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN22 output assigned to XBARA1_IN22 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == B14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_15_XBAR1_IN03,                                                              /* GPIO_B1_15 is configured as XBAR1_IN03 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN03 output assigned to XBARA1_IN3 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == C14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_B1_14_XBAR1_IN02,                                                              /* GPIO_B1_14 is configured as XBAR1_IN02 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == E14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_06_XBAR1_INOUT18, /* GPIO_AD_B0_06 is configured as XBAR1_INOUT18 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_18(0x00U)    /* IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout18, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT18 output assigned to XBARA1_IN18 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == F14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,                                                           /* GPIO_AD_B0_09 is configured as XBAR1_IN21 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN21 output assigned to XBARA1_IN21 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == G14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_05_XBAR1_INOUT17, /* GPIO_AD_B0_05 is configured as XBAR1_INOUT17 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_17(0x00U)    /* IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout17, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT17 output assigned to XBARA1_IN17 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == H14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_14_XBAR1_IN24,                                                           /* GPIO_AD_B0_14 is configured as XBAR1_IN24 */
                0U);                                                                                       /* Software Input On Field: Input Path is determined by functionality */
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn24, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN24 output assigned to XBARA1_IN24 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
        else if (ENC4_PHASE_B == M14)
        {
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, /* GPIO_AD_B0_00 is configured as XBAR1_INOUT14 */
                0U);                                /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
                                 (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14_MASK))) /* Mask bits to zero which are setting */
                                | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_14(0x00U)    /* IOMUXC XBAR_INOUT14 function direction select: XBAR_INOUT as input */
            );
            XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_INOUT14 output assigned to XBARA1_IN14 input is connected to XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT */
        }
    }
    else
    {
#ifdef DEBUG
        PRINTF("\n请选择正确的ENC模块： ENC1 - ENC4\n");
#endif
    }
}
