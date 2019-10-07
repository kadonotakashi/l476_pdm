/*
 * task_qvga.c
 *
 *  Created on: 2017/09/12
 *      Author: 13539
 */

#include "cmsis_os.h"

#include "ILI9341.h"
#include "grp_lcd.h"
#include "lcd_que.h"

extern osMessageQId QueGLCDHandle;
void tk_disp(void const * argument)
{
  /* USER CODE BEGIN tk_qvga */

	portBASE_TYPE result;
	GRAP_LCD_QUE LocQue;

	/* Infinite loop */
	for(;;)
	{

#if 1	///////////////////Graphic LCD邵ｺ�ｽｸ邵ｺ�ｽｮQueue
		result = xQueueReceive(QueGLCDHandle,&LocQue,0);
		if(result==pdTRUE){

			if(LocQue.data[0]==GLCDCMD_INIT){
				glcd_Init(LocQue.INIT.COLOR);	//�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽF�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｾ讎顔ｧ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽ�ｿｽ�ｽｿ�ｽｽ�ｿｽ�ｽｽ�ｽｿ�ｿｽ�ｽｽ�ｽｽA
			}
			if(LocQue.data[0]==GLCDCMD_PSET){
				glcd_PointSet( LocQue.PSET.XS,LocQue.PSET.YS,LocQue.PSET.COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_LINE){
				glcd_drawLine(LocQue.LINE.XS,LocQue.LINE.YS,LocQue.LINE.XE,LocQue.LINE.YE,LocQue.LINE.COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_VLINE){
				glcd_drawVline(LocQue.VLINE.XS,LocQue.VLINE.YS,LocQue.VLINE.LENGTH,LocQue.VLINE.COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_HLINE){
				glcd_drawHline(LocQue.HLINE.XS,LocQue.HLINE.YS,LocQue.HLINE.LENGTH,LocQue.HLINE.COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_RECT){
				glcd_drawRectangle(LocQue.RECT.XS,LocQue.RECT.YS,LocQue.RECT.XE,LocQue.RECT.YE,LocQue.RECT.COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_RECT_FILL){
				glcd_drawRectangleFill(LocQue.RECT_FILL.XS,LocQue.RECT_FILL.YS,LocQue.RECT_FILL.XE,LocQue.RECT_FILL.YE,
						LocQue.RECT_FILL.LINE_COLOR,LocQue.RECT_FILL.FILL_COLOR);
			}
			if(LocQue.data[0]==GLCDCMD_PRINT_STRING){
				glcd_put_string_fixed(LocQue.PRINT_STRING.XS,LocQue.PRINT_STRING.YS,LocQue.PRINT_STRING.str,
						LocQue.PRINT_STRING.COLOR,LocQue.PRINT_STRING.FONT_SIZE);
			}
			if(LocQue.data[0]==GLCDCMD_PRINT_STRING_ADA){
				glcd_put_string_Adafruit(LocQue.PRINT_STRING_ADA.XS,LocQue.PRINT_STRING_ADA.YS,LocQue.PRINT_STRING_ADA.str,
												LocQue.PRINT_STRING_ADA.COLOR,LocQue.PRINT_STRING_ADA.FONT_SEL);

			}
			if(LocQue.data[0]==GLCDCMD_BITBLT){
				glcd_BitBLT(LocQue.BITBLT.XS,LocQue.BITBLT.YS, LocQue.BITBLT.XE,LocQue.BITBLT.YE,LocQue.BITBLT.src);
			}else{
				//邵ｺ阮呻ｽ瑚脂�ｽ･陞滓じ�ｿｽ�ｽｯ隴幢ｽｪ陞ｳ螟ゑｽｾ�ｽｩ
			}
		}
#endif


    osDelay(1);
  }
  /* USER CODE END tk_qvga */
}
