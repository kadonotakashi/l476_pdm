/*
 * task_re.c
 *
 *  Created on: 2019/02/13
 *      Author: 13539
 */


#include "cmsis_os.h"
#include "main.h"
#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef htim3;

static int16_t RE_POSITION;

TIM_Encoder_InitTypeDef sConfig = {0};
TIM_MasterConfigTypeDef sMasterConfig = {0};



void task_re(void const * argument)
{
	int16_t prevPos;
	RE_POSITION=0;
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	TIM3->CNT=0x8000;

	for(;;)
	{
		RE_POSITION=(int16_t)(TIM3->CNT - 0x8000);
		if (prevPos!=RE_POSITION){
			prevPos=RE_POSITION;
		}
		osDelay(100);
	}
}



