/*
 * grp_lcd.h
 *
 *  Created on: 2017/10/06
 *      Author: 13539
 */

#ifndef GRP_LCD_H_
#define GRP_LCD_H_

#ifdef STM32L4R5xx
#include "stm32l4xx_hal.h"
#endif

#ifdef SSD1331
#include "ssd1331.h"
#endif

#ifdef ST7789
#include "st7789.h"
#endif


#ifdef ILI9341_SPI
#include "ILI9341.h"
#endif


#ifdef ILI9341_PARA
#include "ILI9341.h"
#endif

#ifdef ILI9481
#include "ILI9481.h"
#endif

#include "../src/AdafruitFonts/AdafruitFont.h"

#ifdef ST7789
#define	GLCD_IMG_HEIGHT	ST7789_TFTHEIGHT
#define	GLCD_IMG_WIDTH	ST7789_TFTWIDTH
#endif



#ifdef SSD1331
#define	GLCD_IMG_HEIGHT	SSD1331_IMGHEIGHT_MAX
#define	GLCD_IMG_WIDTH	SSD1331_IMGWIDTH_MAX
#endif


#ifdef ILI9341_SPI
#define	GLCD_IMG_HEIGHT	ILI9341_IMG_HEIGHT
#define	GLCD_IMG_WIDTH	ILI9341_IMG_WIDTH
#endif

#ifdef ILI9341_PARA
#define	GLCD_IMG_HEIGHT	ILI9341_IMG_HEIGHT
#define	GLCD_IMG_WIDTH	ILI9341_IMG_WIDTH
#endif

#ifdef ILI9481
#define	GLCD_IMG_HEIGHT	ILI9481_IMG_HEIGHT
#define	GLCD_IMG_WIDTH	ILI9481_IMG_WIDTH
#endif


#define	RGB565_BLACK	0x0000
#define	RGB565_BLUE		0x001f
#define	RGB565_GREEN	0x07e0
#define	RGB565_CYAN		0x07ff
#define	RGB565_RED		0xf800
#define	RGB565_YELLOW	0xffe0
#define	RGB565_MAGENTA	0xf81f
#define	RGB565_WHITE	0xffff

//STM32CUBEMX�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽfont�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽp
//FontSize�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譴ｧ谺��ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
#define	FNT12	0
#define	FNT16	1
#define	FNT20	2
#define	FNT24	3

uint16_t encodeRGB565(uint8_t RED,uint8_t GREEN,uint8_t BLUE);
int glcd_Init(uint16_t color);
int glcd_PointSet(uint16_t x,uint16_t y,uint16_t color);
int glcd_drawLine(uint16_t xs,uint16_t ys, uint16_t xe,uint16_t ye,uint16_t color);
int glcd_drawHline(uint16_t x,uint16_t y,uint16_t length,uint16_t color);
int glcd_drawVline(uint16_t x,uint16_t y,uint16_t length,uint16_t color);
int glcd_drawRectangle(uint16_t xs,uint16_t ys, uint16_t xe,uint16_t ye,uint16_t color);
int glcd_drawRectangleFill(uint16_t xs,uint16_t ys, uint16_t xe,uint16_t ye,uint16_t line_color,uint16_t fill_color);
int glcd_BitBLT(uint16_t xs,uint16_t ys, uint16_t xe,uint16_t ye,uint16_t *pSRC);
int glcd_put_string_fixed(int x,int y,char *string,uint16_t CharColor,int FontSize);
int glcd_put_string_Adafruit(int x,int y,char *string,uint16_t CharColor,int FontSel);

int glcd_put_string_Adafruit(int x,int y,char *string,uint16_t CharColor,int FontSel);

int put_charPattern_Adafruit(char CharCode,uint16_t CHAR_COLOR,uint16_t xs,uint16_t ys,int FontSel);


#endif /* GRP_LCD_H_ */
