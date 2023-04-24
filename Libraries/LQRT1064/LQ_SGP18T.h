/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	


/*******************�ӿڶ���******************************/
#define APP_LCD_WIDTH  LCD_WIDTH//Һ�������
#define APP_LCD_HEIGHT LCD_HEIGHT//Һ�����߶�

#define TFT18W        162
#define TFT18H        132

#define	u16RED		0xf800
#define	u16GREEN	0x07e0
#define	u16BLUE		0x001f
#define	u16PURPLE	0xf81f
#define	u16YELLOW	0xffe0
#define	u16CYAN		0x07ff 		//����ɫ
#define	u16ORANGE	0xfc08
#define	u16BLACK	0x0000
#define	u16WHITE	0xffff

extern uint8_t u16ftflogo[];
extern uint8_t lqLCD12864[16384];
extern uint8_t gImage_lo[17280];
extern uint8_t gImage_Battery_B[900];
extern uint8_t gImage_title2[5152];
extern uint8_t gImage_title[8320];
extern uint8_t gImage_1[40960];
extern uint8_t gImage_2[34560];
/*****************˽�к�������*********************************/							
extern void TFTSPI_Init(uint8_t type);									 //LCD��ʼ��  0:����  1������
extern void TFTSPI_Write_Cmd(uint8_t cmd);						         //���Ϳ�����
extern void TFTSPI_Write_Byte(uint8_t dat);						         //�������ݲ���
extern void TFTSPI_Write_Word(uint16_t dat);						         //����������ʾ����
extern void TFTSPI_Addr_Rst(void);							         //DDRAM��ַ����
extern void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);	                         //��λ��ʾ����λ��
extern void TFTSPI_CLS(uint16_t color);					                         //ȫ����ʾĳ����ɫ
extern void TFTSPI_Draw_Part(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //��������������ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
extern void TFTSPI_Draw_Line(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //���ߣ�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
extern void TFTSPI_Draw_Rectangle(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);//�����α߿�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
extern void TFTSPI_Draw_Circle(uint8_t x,uint8_t y,uint8_t r,uint16_t color_dat);                 //��Բ�α߿�Բ�ĺ����ꡢ�����꣬�뾶����ɫ
extern void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat);	                         //���㣬�����꣬�����꣬��ɫ
extern void TFTSPI_P8X16(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X16Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x16Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x12Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_Show_Pic(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint8_t *ppic);
extern void TFTSPI_Show_Pic2(uint8_t xs,uint8_t ys,uint8_t w,uint8_t h,uint8_t *ppic) ;
extern void TFTSPI_Show_Battery_Icon(void);
extern void TFTSPI_Show_Title(void);
extern void TFTSPI_Show_Logo(uint8_t xs,uint8_t ys);
extern void TFTSPI_Test(void);
extern void ColumnarSetting(uint8_t x,uint8_t y, uint8_t w,uint8_t h,uint16_t fclolr,uint16_t bcolor);
extern void DrawingColumnar(uint8_t t,uint8_t duty,uint8_t cl);
extern void DrawSpectrum(void);
extern void Clear_Columnar(void);
#endif /*SGP18T_ILI9163B.h*/
