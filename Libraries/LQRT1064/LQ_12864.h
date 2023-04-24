/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQOLED_H
#define _LQOLED_H
#include "include.h"
extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(unsigned char x,unsigned char y, char ch[]);
extern void LCD_P8x16Str(unsigned char x,unsigned char y, char ch[]);
extern void LCD_P14x16Str(unsigned char x,unsigned char y, char ch[]);
extern void LCD_Print(unsigned char x, unsigned char y,  char ch[]);
extern void LCD_PutPixel(unsigned char x,unsigned char y);
extern void LCD_Set_Pos(unsigned char x, unsigned char y);
extern void LCD_WrDat(unsigned char data);
extern void LCD_WrCmd(unsigned char cmd);
extern void LCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
extern void LCD_Show_LQLogo(void);
extern void LCD_Show_LibLogo(void);
extern void LCD_Show_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
extern void LCD_Show_Road(void);
extern void LCD_Show_Frame80(void);
void LCD_Show_Frame94(void);
extern void LCD_Show_Frame100(void);
extern void LCD_Set_Pos(unsigned char x, unsigned char y);
extern void LCD_WrDat(unsigned char data);
extern void LCD_WrCmd(unsigned char cmd);
void LCD_Fill(unsigned char bmp_data);
extern void Test_OLED(void);
#endif

