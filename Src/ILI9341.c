/*
 * ILI9341.c
 *
 *  Created on: 2017/09/04
 *      Author: 13539
 */
//#include <math.h>

#ifdef STM32L4R5xx
#include "stm32l4xx_hal.h"
#endif


#include "cmsis_os.h"
#include "./FONT/fonts.h"

#include "ILI9341.h"


extern SPI_HandleTypeDef hspi3;
#define H_SPI_LCD	hspi3


#ifdef ILI9341_SPI

#define INSERT_OSDELAY

volatile int LCD_SPI_CMPLETEFLAG;
volatile int LCD_SPI_DMA_FLAG;
volatile int TP_SPI_CMPLT;

static uint16_t QVGA_Buf[512];// max 14(width)*20(height)*16bit=280,1line=320;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	LCD_SPI_CMPLETEFLAG=1;
	LCD_SPI_DMA_FLAG=1;
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	TP_SPI_CMPLT=1;
}


void Write_data_SPI(uint8_t data)
{
	LCD_CSn_LOW;
	LCD_SPI_CMPLETEFLAG=0;
	HAL_SPI_Transmit_IT(&H_SPI_LCD, &data,1);
	while(LCD_SPI_CMPLETEFLAG==0){  /*wait*/  }
	LCD_CSn_HIGH;
}


void Write_datax_SPI(uint8_t *data,int size)
{
	LCD_CSn_LOW;
	LCD_SPI_CMPLETEFLAG=0;
	HAL_SPI_Transmit_IT(&H_SPI_LCD, data,size);
	while(LCD_SPI_CMPLETEFLAG==0){  /*wait*/  }
	LCD_CSn_HIGH;
}

#endif


void RESET_ILI9341(void)
{
	LCD_RST_LOW;
    osDelay(2);
	LCD_RST_HIGH;
}


#ifndef ILI9341_SPI
void Write_Data8bit(uint8_t data)
{
	LCD_CSn_HIGH;
	LCD_RDn_HIGH;
	LCD_WRn_HIGH;

	LCD_CSn_LOW;
	LCD_WRn_LOW;

	if(data&0x80){	LCD_D7OUT(1);}	else{		LCD_D7OUT(0);}
	if(data&0x40){	LCD_D6OUT(1);}	else{		LCD_D6OUT(0);}
	if(data&0x20){	LCD_D5OUT(1);}	else{		LCD_D5OUT(0);}
	if(data&0x10){	LCD_D4OUT(1);}	else{		LCD_D4OUT(0);}
	if(data&0x08){	LCD_D3OUT(1);}	else{		LCD_D3OUT(0);}
	if(data&0x04){	LCD_D2OUT(1);}	else{		LCD_D2OUT(0);}
	if(data&0x02){	LCD_D1OUT(1);}	else{		LCD_D1OUT(0);}
	if(data&0x01){	LCD_D0OUT(1);}	else{		LCD_D0OUT(0);}
	LCD_WRn_HIGH;
	LCD_CSn_HIGH;
}
#endif

void Write_Command(uint8_t command)
{
	LCD_RS_LOW;
#ifdef ILI9341_SPI
	Write_data_SPI(command);
#else
	Write_Data8bit(command);
#endif
}

void Write_Data(uint8_t data)
{
	LCD_RS_HIGH;
#ifdef ILI9341_SPI
	Write_data_SPI(data);
#else
	Write_Data8bit(data);
#endif
}

void init_ILI9341(int16_t color){
	RESET_ILI9341();
    osDelay(20);


    Write_Command(0xEF);
    Write_Data(0x03);
    Write_Data(0x80);
    Write_Data(0x02);

    Write_Command(0xCF);
    Write_Data(0x00);
    Write_Data(0XC1);
    Write_Data(0X30);

    Write_Command(0xED);
    Write_Data(0x64);
    Write_Data(0x03);
    Write_Data(0X12);
    Write_Data(0X81);

    Write_Command(0xE8);
    Write_Data(0x85);
    Write_Data(0x00);
    Write_Data(0x78);

    Write_Command(0xCB);
    Write_Data(0x39);
    Write_Data(0x2C);
    Write_Data(0x00);
    Write_Data(0x34);
    Write_Data(0x02);

    Write_Command(0xF7);
    Write_Data(0x20);

    Write_Command(0xEA);
    Write_Data(0x00);
    Write_Data(0x00);

    Write_Command(ILI9341_PWCTR1);    //Power control
    Write_Data(0x23);   //VRH[5:0]

    Write_Command(ILI9341_PWCTR2);    //Power control
    Write_Data(0x10);   //SAP[2:0];BT[3:0]

    Write_Command(ILI9341_VMCTR1);    //VCM control
    Write_Data(0x3e);
    Write_Data(0x28);

    Write_Command(ILI9341_VMCTR2);    //VCM control2
    Write_Data(0x86);  //--

    Write_Command(ILI9341_MADCTL);    // Memory Access Control
    Write_Data(MAC_PORTRAIT);

    Write_Command(ILI9341_VSCRSADD); // Vertical scroll
    Write_Data(0);                 // Zero

    Write_Command(ILI9341_PIXFMT);
    Write_Data(0x55);

    Write_Command(ILI9341_FRMCTR1);
    Write_Data(0x00);
    Write_Data(0x18);

    Write_Command(ILI9341_DFUNCTR);    // Display Function Control
    Write_Data(0x08);
    Write_Data(0x82);
    Write_Data(0x27);

    Write_Command(0xF2);    // 3Gamma Function Disable
    Write_Data(0x00);

    Write_Command(ILI9341_GAMMASET);    //Gamma curve selected
    Write_Data(0x01);

    Write_Command(ILI9341_GMCTRP1);    //Set Gamma
    Write_Data(0x0F);
    Write_Data(0x31);
    Write_Data(0x2B);
    Write_Data(0x0C);
    Write_Data(0x0E);
    Write_Data(0x08);
    Write_Data(0x4E);
    Write_Data(0xF1);
    Write_Data(0x37);
    Write_Data(0x07);
    Write_Data(0x10);
    Write_Data(0x03);
    Write_Data(0x0E);
    Write_Data(0x09);
    Write_Data(0x00);

    Write_Command(ILI9341_GMCTRN1);    //Set Gamma
    Write_Data(0x00);
    Write_Data(0x0E);
    Write_Data(0x14);
    Write_Data(0x03);
    Write_Data(0x11);
    Write_Data(0x07);
    Write_Data(0x31);
    Write_Data(0xC1);
    Write_Data(0x48);
    Write_Data(0x08);
    Write_Data(0x0F);
    Write_Data(0x0C);
    Write_Data(0x31);
    Write_Data(0x36);
    Write_Data(0x0F);

    Write_Command(ILI9341_SLPOUT);    //Exit Sleep
    osDelay(60);
    Write_Command(ILI9341_DISPON);    //Display on
	vTaskDelay( 50/ portTICK_PERIOD_MS);
	osDelay(5);

	RectangleFill_ILI9341(0,319, 0, 239,color,color);
}


void BitBlt_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t *data) {
  // VRAM�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譴ｧ蠕暦ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譎擾ｿｽ�ｽｷ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ`�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ迹夊か�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譎擾ｽｮ螢ｹ笘��ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽCMD_MEMORY_WRITE�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ逕ｻ豎夲ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽf�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ[�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ^�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ遒�ｿｽ蛟ｩ閧ｩ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ逕ｻ蠕暦ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ

	int i;
	uint16_t *sp;

	sp=data;
	Write_Command(CMD_COLUMN_ADDRESS_SET);
	Write_Data((uint8_t)(sx>>8));
	Write_Data((uint8_t)(sx&0xff));
	Write_Data((uint8_t)(ex>>8));
	Write_Data((uint8_t)(ex&0xff));


	Write_Command(CMD_PAGE_ADDRESS_SET);
	Write_Data((uint8_t)(sy>>8));
	Write_Data((uint8_t)(sy&0xff));
	Write_Data((uint8_t)(ey>>8));
	Write_Data((uint8_t)(ey&0xff));
	Write_Command(CMD_MEMORY_WRITE);

#ifdef ILI9341_SPI
	uint8_t *p;
	int count;
	int size,remain;

	remain = (ex-sx+1)*(ey-sy+1)*2;
	p=(uint8_t *)data;

while(remain > 0){

	if(remain > 0x8000){
		size=0x8000;
	}
	else{
		size=remain;
	}

	LCD_RS_HIGH;
	LCD_SPI_DMA_FLAG=0;
	LCD_CSn_LOW;

	HAL_SPI_Transmit_DMA(&H_SPI_LCD,p,size);
	count=0;
	do{
#ifdef INSERT_OSDELAY
		LCD_CSn_LOW;
		osDelay(1);
		if(count>1000){
			LCD_CSn_HIGH;
			return -1;
		}
		count++;
#endif
	}
	while(LCD_SPI_DMA_FLAG==0);
//	osDelay(2);
	LCD_CSn_HIGH;
	remain -= size;
	p += size;
}

#else
	for(i=0;i<(ex-sx+1)*(ey-sy+1);i++,sp++){
		Write_Data((uint8_t)( (*sp) & 0xff));	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
		Write_Data((uint8_t)(*sp>>8));	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	}
#endif
}
int Rectangle_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t color) {
  // VRAM�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譴ｧ蠕暦ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譎擾ｿｽ�ｽｷ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ`�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ迹夊か�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ譎擾ｽｮ螢ｹ笘��ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽCMD_MEMORY_WRITE�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ逕ｻ豎夲ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽf�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ[�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ^�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ遒�ｿｽ蛟ｩ閧ｩ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ逕ｻ蠕暦ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ

	HLINE_ILI9341(sx,sy,ex-sx+1,color);
	HLINE_ILI9341(sx,ey,ex-sx+1,color);
	VLINE_ILI9341(sx,sy,ey-sy+1,color);
	VLINE_ILI9341(ex,sy,ey-sy+1,color);
	return 0;
}



int RectangleFill_ILI9341(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey,uint16_t fill_color,uint16_t line_color) {
	int size,remain;

	if((sx<0) ||(sx>ex) || (ex>=ILI9341_IMG_WIDTH)){
		return -1;
	}
	if((sy<0) ||(sy>ey) || (ey>=ILI9341_IMG_HEIGHT)){
		return -1;
	}

	Write_Command(CMD_COLUMN_ADDRESS_SET);
	Write_Data((uint8_t)(sx>>8));
	Write_Data((uint8_t)(sx&0xff));
	Write_Data((uint8_t)(ex>>8));
	Write_Data((uint8_t)(ex&0xff));

	Write_Command(CMD_PAGE_ADDRESS_SET);
	Write_Data((uint8_t)(sy>>8));
	Write_Data((uint8_t)(sy&0xff));
	Write_Data((uint8_t)(ey>>8));
	Write_Data((uint8_t)(ey&0xff));

	Write_Command(CMD_MEMORY_WRITE);


#ifdef ILI9341_SPI
	int count,i;
	uint16_t COL;
	remain = (ex-sx+1)*(ey-sy+1);
	COL = (fill_color&0xff)<<8;
	COL += fill_color>>8;
	if (remain <= 512){
		for(i=0;i<remain;i++)
			QVGA_Buf[i] = COL;
	}
	else{
		for(i=0;i<512;i++)
			QVGA_Buf[i] = COL;
	}

	LCD_RS_HIGH;

while(remain > 0){
	LCD_CSn_LOW;
	if(remain > 512){
		size=512;
	}
	else{
		size=remain;
	}

	LCD_SPI_DMA_FLAG=0;
	HAL_SPI_Transmit_DMA(&H_SPI_LCD,QVGA_Buf,size*2);
	count=0;
	do{
		osDelay(1);
		if(count>1000){
			LCD_CSn_HIGH;
			return -1;
		}
		count++;
	}
	while(LCD_SPI_DMA_FLAG==0);
//	osDelay(2);
	LCD_CSn_HIGH;
	remain -= size;

}
#else
	uint8_t COL_U =(uint8_t)(fill_color>>8);
	uint8_t COL_L =(uint8_t)(fill_color & 0xff);

	size=(ex-sx+1)*(ey-sy+1);
	for(;size>0;size--){
		Write_Data(COL_U);
		Write_Data(COL_L);
	}
#endif
	Rectangle_ILI9341(sx,ex,sy,ey,line_color);

	return 0;
}

#ifdef ILI9341_SPI
void PSET_ILI9341(uint16_t x, uint16_t y,uint16_t color)
{
	uint8_t data[4];

	Write_Command(CMD_COLUMN_ADDRESS_SET);
	data[0]= (uint8_t)(x>>8);
	data[1]= (uint8_t)(x&0xff);
	data[2]= (uint8_t)(x>>8);
	data[3]= (uint8_t)(x&0xff);

	LCD_RS_HIGH;
	Write_datax_SPI(data,4);

	Write_Command(CMD_PAGE_ADDRESS_SET);
	data[0]= (uint8_t)(y>>8);
	data[1]= (uint8_t)(y&0xff);
	data[2]= (uint8_t)(y>>8);
	data[3]= (uint8_t)(y&0xff);
	LCD_RS_HIGH;
	Write_datax_SPI(data,4);

	Write_Command(CMD_MEMORY_WRITE);
	data[0]= (uint8_t)(color >> 8);
	data[1]= (uint8_t)(color & 0xff);
	LCD_RS_HIGH;
	Write_datax_SPI(data,2);

}
#else
void PSET_ILI9341(uint16_t x, uint16_t y,uint16_t color)
{
	Write_Command(CMD_COLUMN_ADDRESS_SET);
	Write_Data((uint8_t)(x>>8));
	Write_Data((uint8_t)(x&0xff));
	Write_Data((uint8_t)(x>>8));
	Write_Data((uint8_t)(x&0xff));

	Write_Command(CMD_PAGE_ADDRESS_SET);
	Write_Data((uint8_t)(y>>8));
	Write_Data((uint8_t)(y&0xff));
	Write_Data((uint8_t)(y>>8));
	Write_Data((uint8_t)(y&0xff));

	Write_Command(CMD_MEMORY_WRITE);
	Write_Data((uint8_t)(color>>8));	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	Write_Data((uint8_t)( color & 0xff));	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
}
#endif



int VLINE_ILI9341(int sx,int sy,int length,int16_t color)
{
	uint8_t	COL_U,COL_L;
	Write_Command(CMD_COLUMN_ADDRESS_SET);
	Write_Data((uint8_t)(sx>>8));
	Write_Data((uint8_t)(sx&0xff));
	Write_Data((uint8_t)(sx>>8));
	Write_Data((uint8_t)(sx&0xff));

	Write_Command(CMD_PAGE_ADDRESS_SET);
	Write_Data((uint8_t)(sy>>8));
	Write_Data((uint8_t)(sy&0xff));
	Write_Data((uint8_t)((sy+length-1)>>8));
	Write_Data((uint8_t)((sy+length-1)&0xff));

	Write_Command(CMD_MEMORY_WRITE);
	COL_U =(uint8_t)(color>>8);
	COL_L =(uint8_t)( color & 0xff);
	for(int i=0;i<length;i++){
		Write_Data(COL_U);	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
		Write_Data(COL_L);	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	}
	return 0;
}

int HLINE_ILI9341(int sx,int sy,int length,int16_t color)
{
	uint8_t	COL_U,COL_L;
	Write_Command(CMD_COLUMN_ADDRESS_SET);
	Write_Data((uint8_t)(sx>>8));
	Write_Data((uint8_t)(sx&0xff));
	Write_Data((uint8_t)((sx+length-1)>>8));
	Write_Data((uint8_t)((sx+length-1)&0xff));

	Write_Command(CMD_PAGE_ADDRESS_SET);
	Write_Data((uint8_t)(sy>>8));
	Write_Data((uint8_t)(sy&0xff));
	Write_Data((uint8_t)(sy>>8));
	Write_Data((uint8_t)(sy&0xff));

	Write_Command(CMD_MEMORY_WRITE);
	COL_U =(uint8_t)(color>>8);
	COL_L =(uint8_t)( color & 0xff);
	for(int i=0;i<length;i++){
		Write_Data(COL_U);	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
		Write_Data(COL_L);	//8bit�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	}
	return 0;
}

int LINE_ILI9341(int sx,int sy,int ex,int ey,int mode)
{
	float a,b;
	int x,y;

	//2�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ_�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ閧ｴ諤擾ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ迹夊か�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ蟲ｨ�ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ鬯ｩ謳ｾ�ｽｽ�ｽｱ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ�ｿｽ邵ｺ�ｽｫ郢ｧ謇假ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	if((sx<0)||(sx>=ILI9341_IMG_WIDTH))		return -1;
	if((sy<0)||(sy>=ILI9341_IMG_HEIGHT))	return -1;
	if((ex<0)||(ex>=ILI9341_IMG_WIDTH))		return -1;
	if((ey<0)||(ey>=ILI9341_IMG_HEIGHT))	return -1;

	if(sx==ex){
		if(sy<ey){
			VLINE_ILI9341(sx,sy,(ey-sy+1),mode);
		}
		else if(ey<sy){
			VLINE_ILI9341(sx,ey,(sy-ey+1),mode);
		}
		else{
			PSET_ILI9341(sx,sy,mode);
		}
	}

	if(sy==ey){
		if(sx<ex){
			HLINE_ILI9341(sx,sy,(ex-sx+1),mode);
		}
		else if(ex<sx){
			HLINE_ILI9341(ex,sy,(sx-ex+1),mode);
		}
		else{
			PSET_ILI9341(sx,sy,mode);
		}
	}

	if(abs(ex-sx)>abs(ey-sy))	//x�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽW�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ荳茨ｽｼ螟ｲ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ蛹ｻ�ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ髴難ｽ､陷ｻ�ｽｻ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽW�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ貅假ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
	{	//y = ax + b
		b=(float)(sy*ex-ey*sx)/(float)(ex-sx);
		if(ex!=0){
			a=((float)ey-b)/(float)ex;
		}else{
			a=((float)sy-b)/(float)sx;
		}

		if (sx<ex){
			for(x=sx;x<=ex;x++){
				y=(int)(((x*a)+b)+0.5);
				PSET_ILI9341(x,y,mode);
			}
		}
		else{
			for(x=ex;x<=sx;x++){
				y=(int)(((x*a)+b)+0.5);
				PSET_ILI9341(x,y,mode);
			}

		}
	}else{						//y�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽW�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ荳茨ｽｼ螟ｲ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ蛹ｻ�ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ髴難ｽ､陷ｻ�ｽｻ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽW�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ貅假ｽ托ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ
		//x = ay + b
		b=(float)(sx*ey-ex*sy)/(float)(ey-sy);
		if(ey!=0){
			a=((float)ex-b)/(float)ey;
		}else{
			a=((float)sx-b)/(float)sy;
		}
		if (sy<ey){
			for(y=sy;y<=ey;y++){
				x=(int)(((y*a)+b)+0.5);
				PSET_ILI9341(x,y,mode);
			}
		}
		else{
			for(y=ey;y<=sy;y++){
				x=(int)(((y*a)+b)+0.5);
				PSET_ILI9341(x,y,mode);
			}
		}
	}
	return 0;
}


void ClearScreen_ILI9341(uint16_t color) {
	Rectangle(0,319, 0, 239,color);
}

