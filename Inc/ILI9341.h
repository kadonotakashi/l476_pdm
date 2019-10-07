/*
 * ILI9341.h
 *
 *  Created on: 2017/09/04
 *      Author: 13539
 */

#ifndef ILI9341_H_
#define ILI9341_H_

#include "main.h"	//define LCD Control Port

#define LANDSCAPE



#if defined(LANDSCAPE)
# define MAC_CONFIG MAC_PORTRAIT
# define ILI9341_IMG_WIDTH 320
# define ILI9341_IMG_HEIGHT 240
#else
# define MAC_CONFIG MAC_LANDSCAPE
# define ILI9341_IMG_WIDTH 320
# define ILI9341_IMG_HEIGHT 240
#endif




#define CMD_SLEEP_OUT 0x11
#define CMD_DISPLAY_ON 0x29
#define CMD_COLUMN_ADDRESS_SET 0x2a
#define CMD_PAGE_ADDRESS_SET 0x2b
#define CMD_MEMORY_WRITE 0x2c
#define CMD_MEMORY_ACCESS_CONTROL 0x36
#define CMD_COLMOD 0x3a



#define ILI9341_TFTWIDTH   240
#define ILI9341_TFTHEIGHT  320

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09

#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13

#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29

#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E

#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_VSCRSADD   0x37
#define ILI9341_PIXFMT     0x3A

#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6

#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7

#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD

#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1




#define MAC_PORTRAIT 0xe8
#define MAC_LANDSCAPE 0x48
#define COLMOD_16BIT 0x55
#define COLMOD_18BIT 0x66


//#define	RGB565_WHITE	0xffff
//#define	RGB565_BLACK	0x0000
//#define	RGB565_RED		0xf800
//#define	RGB565_GREEN	0x07e0
//#define	RGB565_BLUE		0x001f
//#define	RGB565_MAGENTA	0xf81f
//#define	RGB565_YELLOW	0xffe0
//#define	RGB565_CYAN		0x07FF




//Nucleo64 F401,F446�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽp
//#define VGA_RSTn_Pin GPIO_PIN_1
//#define VGA_RSTn_GPIO_Port GPIOC
//#define VGA_RDn_Pin GPIO_PIN_0
//#define VGA_RDn_GPIO_Port GPIOA
//#define VGA_WRn_Pin GPIO_PIN_1
//#define VGA_WRn_GPIO_Port GPIOA
//#define VGA_CnD_Pin GPIO_PIN_4
//#define VGA_CnD_GPIO_Port GPIOA
//#define VGA_CSn_Pin GPIO_PIN_0
//#define VGA_CSn_GPIO_Port GPIOB
//
//#define VGA_D7_Pin GPIO_PIN_8
//#define VGA_D7_GPIO_Port GPIOA
//#define VGA_D6_Pin GPIO_PIN_10
//#define VGA_D6_GPIO_Port GPIOB
//#define VGA_D5_Pin GPIO_PIN_4
//#define VGA_D5_GPIO_Port GPIOB
//#define VGA_D4_Pin GPIO_PIN_5
//#define VGA_D4_GPIO_Port GPIOB
//#define VGA_D3_Pin GPIO_PIN_3
//#define VGA_D3_GPIO_Port GPIOB
//#define VGA_D2_Pin GPIO_PIN_10
//#define VGA_D2_GPIO_Port GPIOA
//#define VGA_D1_Pin GPIO_PIN_7
//#define VGA_D1_GPIO_Port GPIOC
//#define VGA_D0_Pin GPIO_PIN_9
//#define VGA_D0_GPIO_Port GPIOA


//�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽLmain.h�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譴ｧ諷｣�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ`�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ諛岩�堤ｹｧ竏夲ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ

//QUBE MX�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ�ｿｽ邵ｲ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽepin�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ謌托ｽｻ�ｽ･闔ｨ螟ｲ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ魃会ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽx�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽU�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ鬯ｩ謳ｾ�ｽｽ�ｽｱ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
//VGA_RSTn,VGA_RDn,VGA_WRn,VGA_CnD,VGA_CSn
//VGA_D0,VGA_D1,......VGA_D7

#define VGA_DBUS_PORT0_PIN	(GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8)	//D7,D2,D0
#define VGA_DBUS_PORT0 GPIOA

#define VGA_DBUS_PORT1_PIN	(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10)	//D3,D4,D5,D6
#define VGA_DBUS_PORT1 GPIOB

#define VGA_DBUS_PORT2_PIN GPIO_PIN_7	//D1
#define VGA_DBUS_PORT2 GPIOC

#define VGA_DBUS_DIR_WR 0
#define VGA_DBUS_DIR_RD 1

#define	VGA_DATA_ADDR	1
#define	VGA_CMND_ADDR	0

//�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽM�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｨ滓慣�ｽｽ�ｽｮ郢ｧ謇假ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ貅假ｿｽ�ｽｮ郢晄ｩｸ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽN�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
#define	LCD_RST_LOW		HAL_GPIO_WritePin(LCD_RSTn_GPIO_Port,LCD_RSTn_Pin,GPIO_PIN_RESET)
#define	LCD_RST_HIGH	HAL_GPIO_WritePin(LCD_RSTn_GPIO_Port,LCD_RSTn_Pin,GPIO_PIN_SET)

#define	LCD_RS_LOW		HAL_GPIO_WritePin(LCD_CnD_GPIO_Port,LCD_CnD_Pin,GPIO_PIN_RESET)
#define	LCD_RS_HIGH		HAL_GPIO_WritePin(LCD_CnD_GPIO_Port,LCD_CnD_Pin,GPIO_PIN_SET)

#define	LCD_CSn_LOW		HAL_GPIO_WritePin(LCD_CSn_GPIO_Port,LCD_CSn_Pin,GPIO_PIN_RESET)
#define	LCD_CSn_HIGH	HAL_GPIO_WritePin(LCD_CSn_GPIO_Port,LCD_CSn_Pin,GPIO_PIN_SET)


#ifndef ILI9341_SPI
#define	LCD_WRn_LOW		HAL_GPIO_WritePin(LCD_WRn_GPIO_Port,LCD_WRn_Pin,GPIO_PIN_RESET)
#define	LCD_WRn_HIGH	HAL_GPIO_WritePin(LCD_WRn_GPIO_Port,LCD_WRn_Pin,GPIO_PIN_SET)

#define	LCD_RDn_LOW		HAL_GPIO_WritePin(LCD_RDn_GPIO_Port,LCD_RDn_Pin,GPIO_PIN_RESET)
#define	LCD_RDn_HIGH	HAL_GPIO_WritePin(LCD_RDn_GPIO_Port,LCD_RDn_Pin,GPIO_PIN_SET)
#endif

//�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽf�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ[�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ^�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽo�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽX�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ迹夲ｽｪ�ｽｭ邵ｺ�ｽｿ髮懶ｽ｣�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
#define	ILI_D7IN		HAL_GPIO_ReadPin(VGA_D7_GPIO_Port,VGA_D7_Pin)
#define	ILI_D6IN		HAL_GPIO_ReadPin(VGA_D6_GPIO_Port,VGA_D6_Pin)
#define	ILI_D5IN		HAL_GPIO_ReadPin(VGA_D5_GPIO_Port,VGA_D5_Pin)
#define	ILI_D4IN		HAL_GPIO_ReadPin(VGA_D4_GPIO_Port,VGA_D4_Pin)
#define	ILI_D3IN		HAL_GPIO_ReadPin(VGA_D3_GPIO_Port,VGA_D3_Pin)
#define	ILI_D2IN		HAL_GPIO_ReadPin(VGA_D2_GPIO_Port,VGA_D2_Pin)
#define	ILI_D1IN		HAL_GPIO_ReadPin(VGA_D1_GPIO_Port,VGA_D1_Pin)
#define	ILI_D0IN		HAL_GPIO_ReadPin(VGA_D0_GPIO_Port,VGA_D0_Pin)

#define	LCD_D7OUT(x)	HAL_GPIO_WritePin(LCD_D7_GPIO_Port,LCD_D7_Pin,x)
#define	LCD_D6OUT(x)	HAL_GPIO_WritePin(LCD_D6_GPIO_Port,LCD_D6_Pin,x)
#define	LCD_D5OUT(x)	HAL_GPIO_WritePin(LCD_D5_GPIO_Port,LCD_D5_Pin,x)
#define	LCD_D4OUT(x)	HAL_GPIO_WritePin(LCD_D4_GPIO_Port,LCD_D4_Pin,x)
#define	LCD_D3OUT(x)	HAL_GPIO_WritePin(LCD_D3_GPIO_Port,LCD_D3_Pin,x)
#define	LCD_D2OUT(x)	HAL_GPIO_WritePin(LCD_D2_GPIO_Port,LCD_D2_Pin,x)
#define	LCD_D1OUT(x)	HAL_GPIO_WritePin(LCD_D1_GPIO_Port,LCD_D1_Pin,x)
#define	LCD_D0OUT(x)	HAL_GPIO_WritePin(LCD_D0_GPIO_Port,LCD_D0_Pin,x)

void init_ILI9341(int16_t color);
void BitBlt_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t *data);
int Rectangle_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t color);
int RectangleFill_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t fill_color,uint16_t line_color);
void PSET_ILI9341(uint16_t x, uint16_t y,uint16_t color);
int VLINE_ILI9341(int sx,int sy,int length,int16_t color);
int HLINE_ILI9341(int sx,int sy,int length,int16_t color);
int LINE_ILI9341(int sx,int sy,int ex,int ey,int mode);
void ClearScreen_ILI9341(uint16_t color);



#endif /* ILI9341_H_ */
