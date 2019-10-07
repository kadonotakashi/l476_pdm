/*
 * task_main.c
 *
 *  Created on: 2017/09/10
 *      Author: Takashi
 */
#include "main.h"
#include "cmsis_os.h"

#include "ILI9341.h"
#include "grp_lcd.h"
#include "lcd_que.h"


//uint8_t	MSG_FW_OK[]="SDCARD WR OK.\0";
//uint8_t	MSG_FW_NG[]="SDCARD WR N.G..\0";
//uint8_t	MSG_FR_OK[]="SDCARD RD N.G..\0";

extern osMessageQId QueGLCDHandle;//main.c
GRAP_LCD_QUE	QUE_GLCD_LOCAL;
void tk_main(void const * argument)
{
    osDelay(100);

    //LCD������
    QUE_GLCD_LOCAL.INIT.CMD=GLCDCMD_INIT;
    QUE_GLCD_LOCAL.INIT.COLOR=RGB565_BLUE;
	xQueueSendToBack(QueGLCDHandle,&QUE_GLCD_LOCAL,10);

	osDelay(10);

	for(;;)
	{
		osDelay(1);
	}
}

