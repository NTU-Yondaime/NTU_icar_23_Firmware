#ifndef __LQ_ENCODER_H__
#define __LQ_ENCODER_H__

#include "LQ_GPIO_Cfg.h"
#include "fsl_enc.h"
/*--------------------------------------------------------------------------------------------
//ENC��û�й̶��ĸ��ùܽŵģ�ֻ����ͨ�� XBARA ���ӵ� �ڲ�ENC��������
//��46���ܽſ��������� XBARA�� ����XBARAֻ��24��ͨ��  Ҳ����һЩͨ������ӳ���ڲ�ͬ�Ĺܽ���
//XBARA ���ܺ�ǿ��  ENC  FlexPWM  QTMR ...������ʹ�� ���������о�
//B1  H1  F2  C3  E3  F3  H3  E4  F4  G4  H4  A5  E5  G5  H5  A6  B6  ���ⲿSDRAM
//F12 F13 E14 J1  K1  H2  J2  J3  J4  C10 D10 E10 G10 H10 L10 A11 B11 C11 D11 E11 G11 M11 G13 B14 C14 F14 G14 H14 M14

//���� F12 ΪSWD���Կ�  E14 ΪSWD���Կ�  F13 ���ص��Կ�ѡ��ܽţ��ϵ�ʱ���͵�ƽ��SWD���ԣ��ϵ�ʱ���ߵ�ƽ��JTAG���ԣ�
// XBARA 24��ͨ������ 24��  ÿ�п���ѡ��һ���ܽ���Ϊ XBARA�Ĺܽ�
//| J1 | J3 | J2 | J4 | K1 | H2 | G10| L10| H10 | D11| C11| F12| F13| G13| B14| C14| E14| F14| H14| A11| C10| D10| E10| E11
  | F2 | G4 | H4 | F4 | G5 | H5 | E4 | A6 | B11 | H3 | M11| B6 | B1 | C3 | F3 | E3 | E5 | A5 | H1 | M14
                                                | G11|
                                                | G14|   ������ͬʱʹ��
-----------------------------------------------------------------------------------------------*/
/**********************************  ENC(���Ÿ���) *******************************************************/
//     ENC����ͨ��        �˿�    ��ѡ��Χ                       ����
#define ENC1_PHASE_A H2 // XBARA
#define ENC1_PHASE_B J2 // XBARA

#define ENC2_PHASE_A G13 // XBARA
#define ENC2_PHASE_B F14 // XBARA

#define ENC3_PHASE_A J3 // XBARA
#define ENC3_PHASE_B K1 // XBARA

#define ENC4_PHASE_A C11 // XBARA
#define ENC4_PHASE_B B11 // XBARA

/**
 * @brief    ��ʼ�� ENCģ��
 *
 * @param    base        �� ENC1 - ENC4
 * @param    direction   �� �Ƿ���
 *
 * @return
 *
 * @note     Ĭ�� LSB+Dirģʽ  �����޸�
 *
 * @example
 *
 * @date     2019/6/12 ������
 */
void LQ_ENC_Init(ENC_Type *base, bool direction);

/**
 * @brief    ��ʼ�� ENC�ܽ�
 *
 * @param    base        �� ENC1 - ENC4
 *
 * @return
 *
 * @note     �ܽ�ѡ��� ͷ�ļ��еĺ궨���޸�
 *
 * @example
 *
 * @date     2019/6/12 ������
 */
void ENC_PinInit(ENC_Type *base);

#endif
