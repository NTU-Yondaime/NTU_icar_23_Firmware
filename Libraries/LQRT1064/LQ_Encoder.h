#ifndef __LQ_ENCODER_H__
#define __LQ_ENCODER_H__

#include "LQ_GPIO_Cfg.h"
#include "fsl_enc.h"
/*--------------------------------------------------------------------------------------------
//ENC是没有固定的复用管脚的，只可以通过 XBARA 连接到 内部ENC触发器上
//有46个管脚可以连接在 XBARA上 但是XBARA只有24个通道  也就是一些通道可以映射在不同的管脚上
//XBARA 功能很强大  ENC  FlexPWM  QTMR ...都可以使用 可以自行研究
//B1  H1  F2  C3  E3  F3  H3  E4  F4  G4  H4  A5  E5  G5  H5  A6  B6  接外部SDRAM
//F12 F13 E14 J1  K1  H2  J2  J3  J4  C10 D10 E10 G10 H10 L10 A11 B11 C11 D11 E11 G11 M11 G13 B14 C14 F14 G14 H14 M14

//其中 F12 为SWD调试口  E14 为SWD调试口  F13 下载调试口选择管脚（上电时，低电平用SWD调试，上电时，高电平用JTAG调试）
// XBARA 24个通道如下 24列  每列可以选出一个管脚作为 XBARA的管脚
//| J1 | J3 | J2 | J4 | K1 | H2 | G10| L10| H10 | D11| C11| F12| F13| G13| B14| C14| E14| F14| H14| A11| C10| D10| E10| E11
  | F2 | G4 | H4 | F4 | G5 | H5 | E4 | A6 | B11 | H3 | M11| B6 | B1 | C3 | F3 | E3 | E5 | A5 | H1 | M14
                                                | G11|
                                                | G14|   不可以同时使用
-----------------------------------------------------------------------------------------------*/
/**********************************  ENC(引脚复用) *******************************************************/
//     ENC输入通道        端口    可选范围                       建议
#define ENC1_PHASE_A H2 // XBARA
#define ENC1_PHASE_B J2 // XBARA

#define ENC2_PHASE_A G13 // XBARA
#define ENC2_PHASE_B F14 // XBARA

#define ENC3_PHASE_A J3 // XBARA
#define ENC3_PHASE_B K1 // XBARA

#define ENC4_PHASE_A C11 // XBARA
#define ENC4_PHASE_B B11 // XBARA

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
void LQ_ENC_Init(ENC_Type *base, bool direction);

/**
 * @brief    初始化 ENC管脚
 *
 * @param    base        ： ENC1 - ENC4
 *
 * @return
 *
 * @note     管脚选择从 头文件中的宏定义修改
 *
 * @example
 *
 * @date     2019/6/12 星期三
 */
void ENC_PinInit(ENC_Type *base);

#endif
