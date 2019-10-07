/*
 * tk_sns.c
 *
 *  Created on: 2019/04/05
 *      Author: 13539
 */

#include "cmsis_os.h"
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h"


#include "main.h"
#include "lcd_que.h"
#include "grp_lcd.h"
#include "arm_math.h"

#include "task_sns.h"


#define	WAVE_DISPLAY

extern osMessageQId QueSendHandle;

extern osMessageQId QueGLCDHandle;//main.c
extern TIM_HandleTypeDef htim2;
//extern DAC_HandleTypeDef hdac;
extern DAC_HandleTypeDef hdac1;
extern DMA_HandleTypeDef hdma_dac1;


extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel0;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;
extern DMA_HandleTypeDef hdma_dfsdm1_flt0;
extern DMA_HandleTypeDef hdma_dfsdm1_flt1;


GRAP_LCD_QUE	QUE_GLCD_LOCAL;
static bool half_flag = true;
static bool cmplt_flag = true;
static int DMA_CMPLT_CNT;

#ifdef 	STM32L476xx
DFSDM_BUF *pDFSDMBUF = (DFSDM_BUF *)0x10000000;	//~0x10001fff	4kbyte * 2ch	from DFSDM DMA
Q31T_BUF *pSUMBUF =	(Q31T_BUF *)  0x10002000;	//~0x10002fff	4kbyte		sum
Q31T_BUF *pWORK =	(Q31T_BUF *)  0x10003000;	//~0x10003fff	4kbyte		work

W16_BUF *pW16B0 =   (W16_BUF *)0x10004000;
W16_BUF *pW16B1 =   (W16_BUF *)0x10004800;
W16_BUF *pW16BSUM = (W16_BUF *)0x10005000;
W12_BUF	*pDACBUF =  (W12_BUF *)0x10005800;

Q31T_BUF *pSUMBUF2 =     (Q31T_BUF *)  0x10006000;	//~0x10006fff	4kbyte		sum
Q31T_BUF *pWORK2 =    (Q31T_BUF *)  0x10007000;		//~0x10007fff	4kbyte		work

int8_t	MicGainCH0,MicGainCH1;
int8_t	MicGainShiftCH0,MicGainShiftCH1;



#endif

int MicShift = 3;

SNS_StateTypeDef SNS_STS = SNS_INIT;

LCD_BUF *pLCDBUF=(LCD_BUF *)0x20040000;

//int16_t wave[1024];

void disp_wave(int32_t range){

	int16_t ypoint;

    for(int y=0;y<200;y++){
        for(int x=0;x<256;x++){
        	pLCDBUF->BMP[y][x]=RGB565_BLACK;
        }
    }

	for(int i=0;i<DMA_SAMPLE_CNT;i++){
		ypoint = pW16B0->d16[i];	//��
		ypoint /= range;
		ypoint += 25;
		if(ypoint>200){
	    	ypoint=200;
	    }
		if(ypoint<0){
	    	ypoint=0;
	    }
		pLCDBUF->BMP[ypoint][i/(DMA_SAMPLE_CNT/256)]=RGB565_YELLOW;
	}
	for(int i=0;i<DMA_SAMPLE_CNT;i++){
		ypoint = pW16B1->d16[i];	//��
		ypoint /= range;
		ypoint += 75;
		if(ypoint>200){
	    	ypoint=200;
	    }
		if(ypoint<0){
	    	ypoint=0;
	    }
		pLCDBUF->BMP[ypoint][i/(DMA_SAMPLE_CNT/256)]=RGB565_CYAN;
	}

	for(int i=0;i<DMA_SAMPLE_CNT;i++){
		ypoint = pW16BSUM->d16[i];	//��
		ypoint /= range*4;
		ypoint += 100;
		if(ypoint>200){
	    	ypoint=200;
	    }
		if(ypoint<0){
	    	ypoint=0;
	    }
		pLCDBUF->BMP[ypoint][i/(DMA_SAMPLE_CNT/256)]=RGB565_WHITE;
	}



	QUE_GLCD_LOCAL.BITBLT.CMD = GLCDCMD_BITBLT;
	QUE_GLCD_LOCAL.BITBLT.XS = 32;
	QUE_GLCD_LOCAL.BITBLT.YS = 20;
	QUE_GLCD_LOCAL.BITBLT.XE = 287;
	QUE_GLCD_LOCAL.BITBLT.YE = 219;
	QUE_GLCD_LOCAL.BITBLT.src = (uint16_t 	*)pLCDBUF;
	xQueueSendToBack(QueGLCDHandle,&QUE_GLCD_LOCAL,10);
}


void RefreshMaxMinAvg(W16_BUF *pBuf,int16_t *max,int16_t *min,int16_t *avg){

	uint32_t index;
	int16_t work;

	arm_max_q15((q15_t *)pBuf,DMA_SAMPLE_CNT,&work,&index);
    if (work>*max){
    	*max=work;
    }

    arm_min_q15((q15_t *)pBuf,DMA_SAMPLE_CNT,&work,&index);
    if (work<*min){
    	*min=work;
    }

    arm_mean_q15((q15_t *)pBuf,DMA_SAMPLE_CNT,&work);
    *avg = work;
}

void RefreshMaxMinAvgQ31(W32_BUF *pBuf,int32_t *max,int32_t *min,int32_t *avg){

	uint32_t index;
	int32_t work;

	arm_max_q31((q31_t *)pBuf,DMA_SAMPLE_CNT,&work,&index);
    if (work>*max){
    	*max=work;
    }

    arm_min_q31((q31_t *)pBuf,DMA_SAMPLE_CNT,&work,&index);
    if (work<*min){
    	*min=work;
    }

    arm_mean_q31((q31_t *)pBuf,DMA_SAMPLE_CNT,&work);
    *avg = work;
}


bool getAvg32(int ch,int32_t *avg){

	q31_t *pSrc;

	//select channerl
	switch(ch){
	case 0:
		pSrc = (q31_t *)&pDFSDMBUF->d32[0][0];			break;
	case 1:
		pSrc = (q31_t *)&pDFSDMBUF->d32[1][0];			break;
		default:
			return false;
	}

	//get avarage use first half of 32bit wave buffer
    arm_mean_q31(pSrc,DMA_SAMPLE_CNT/2,avg);
	return true;
}

bool getMax32(int ch,int32_t *max){

	q31_t *pSrc;
	uint32_t index;

	//select channerl
	switch(ch){
	case 0:
		pSrc = (q31_t *)&pDFSDMBUF->d32[0][0];			break;
	case 1:
		pSrc = (q31_t *)&pDFSDMBUF->d32[1][0];			break;
	default:
			return false;
	}

	//get avarage use first half of 32bit wave buffer
    arm_max_q31(pSrc,DMA_SAMPLE_CNT/2,max,&index);
	return true;
}

bool getMin32(int ch,int32_t *min){

	q31_t *pSrc;
	uint32_t index;

	//select channerl
	switch(ch){
		case 0:
			pSrc = (q31_t *)&pDFSDMBUF->d32[0][1];			break;
		case 1:
			pSrc = (q31_t *)&pDFSDMBUF->d32[1][1];			break;
		case 2:
			pSrc = (q31_t *)&pDFSDMBUF->d32[2][1];			break;
		case 3:
			pSrc = (q31_t *)&pDFSDMBUF->d32[3][1];			break;
		default:
			return false;
	}

	//get avarage use first half of 32bit wave buffer
    arm_min_q31(pSrc,DMA_SAMPLE_CNT/2,min,&index);
	return true;
}


bool canselOffset(int ch,int32_t avg){

	volatile DFSDM_Channel_HandleTypeDef *pHDFSDM_CHX;
	volatile int32_t offset;

	DFSDM_Channel_TypeDef          *Instance;

	//select channerl
	switch(ch){
		case 0:
			pHDFSDM_CHX = (DFSDM_Channel_HandleTypeDef *)&hdfsdm1_channel0;			break;
		case 1:
			pHDFSDM_CHX = (DFSDM_Channel_HandleTypeDef *)&hdfsdm1_channel1;			break;

		default:
			return false;
	}
	Instance = pHDFSDM_CHX->Instance;

	avg &= 0xffffff00;

	offset = (Instance->CHCFGR2) & 0xffffff00;
	offset += avg;

	Instance->CHCFGR2 &= 0x000000ff;
	Instance->CHCFGR2 |= offset;

	return true;
}


void sencorCalibrate(int *flag){
	int32_t avg;

	if (*flag & 0x01){
		getAvg32(0,&avg);
		if((avg<OFFSET_MAX) && (avg>OFFSET_MIN)){
			*flag &= 0xfffffffe;
		}
		else{
			canselOffset(0,avg);
		}
	}
	if (*flag & 0x02){
		getAvg32(1,&avg);
		if((avg<OFFSET_MAX) && (avg>OFFSET_MIN)){
			*flag &= 0xfffffffd;
		}
		else{
			canselOffset(1,avg);
		}
	}
}



void tk_sns(void const * argument)
{

	volatile int DACSTART_FLAG=0;
//	SNS_StateTypeDef	SNS_STS;
	volatile int CALIB_FLAG=0x3;
	volatile int CALIB_COUNT = 0;


	int	LocalQueue;

	HAL_TIM_Base_Start_IT(&htim2);

	SNS_STS = SNS_INIT;
	osDelay(200);


    MicGainCH0	= 32;
    MicGainCH1	= 32;
    MicGainShiftCH0	= 4;
    MicGainShiftCH1 = 4;

//start sample & DMA transfer
    half_flag = true;
	cmplt_flag = true;

//	DFSDM1_Channel0->CHCFGR1 &= ~0x80000000;

	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);

	if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter1, (int32_t *)pDFSDMBUF->d32[1][0], DMA_SAMPLE_CNT) != HAL_OK)	{
		Error_Handler();
	}
	if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, (int32_t *)pDFSDMBUF->d32[0][0], DMA_SAMPLE_CNT) != HAL_OK)	{
		Error_Handler();
	}

	//	DFSDM1_Channel0->CHCFGR1 |= 0x80000000;
	for(;;)
	{
		while (half_flag==true){	//wait first half DMA complete
    		osDelay(1);
        }
		GetWaveData_FirstHalf();

        ////////////////////////Need Calibration?///////////////////////////////////
        if(SNS_STS == SNS_INIT){
			if((CALIB_FLAG & 0x0f)==0){
				SNS_STS = SNS_READY;
			}
			else if (CALIB_COUNT>CALIB_COUNT_MAX){
        			SNS_STS =SNS_ERR;
        	        SNS_STS = SNS_READY;	//in debug sensor is not conected
			}
        	else if(CALIB_COUNT>3){	//ignore first 3times,and cariblation start
        		sencorCalibrate((int *)&CALIB_FLAG);
        	}
			CALIB_COUNT++;
        }
        ///////////////////////////////////////////////////////////////////////////

        half_flag=true;
        LocalQueue=0;	//first half
    	xQueueSendToBack(QueSendHandle,&LocalQueue,10);


    	while (cmplt_flag==true){		//wait second half DMA complete
    		osDelay(1);
        }
		GetWaveData_SecondHalf();

#define DAout

#ifdef DAout
		if((DACSTART_FLAG==0)&&(SNS_STS == SNS_READY)){
            DACSTART_FLAG = 1;
            if (HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)&pDACBUF->d12[0], DMA_SAMPLE_CNT,DAC_ALIGN_12B_R) != HAL_OK)
            {
              Error_Handler();
            }
        }
#endif
#if 1
        if(DMA_CMPLT_CNT%64==4){

 //       	RefreshMaxMinAvg(pW16B0->d16,&max,&min,&avg);
//    		RefreshMaxMinAvgQ31(pW32B0->d32,&max32,&min32,&avg32);
//        	 disp_wave(100);
        }
#endif
        cmplt_flag=true;
        LocalQueue=1;	//second harf
    	xQueueSendToBack(QueSendHandle,&LocalQueue,10);

	}
}


void GetWaveData_FirstHalf(void){
    int32_t *pSRC;
	q31_t MicGain;

HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);


    //ch0
	MicGain = MicGainCH0 * 0x1000000;
    pSRC=(int32_t *)(&pDFSDMBUF->d32[0][0]);			//DFSDM buffer Block 0

    arm_scale_q31 (pSRC, MicGain, MicGainShiftCH0,(q31_t *) pWORK, (DMA_SAMPLE_CNT/2));
    arm_q31_to_q15((q31_t *)pWORK, (q15_t *)&pW16B0->d16[0],(DMA_SAMPLE_CNT/2));			//to signed 16bit
    arm_copy_q31((q31_t *)pWORK,(q31_t *)pSUMBUF,DMA_SAMPLE_CNT/2);

    //ch1
	MicGain = MicGainCH1 * 0x1000000;
    pSRC=(int32_t *)(&pDFSDMBUF->d32[1][0]);

    arm_scale_q31 (pSRC, MicGain, MicGainShiftCH1, (q31_t *)pWORK, (DMA_SAMPLE_CNT/2));
    arm_q31_to_q15((q31_t *)pWORK, (q15_t *)&pW16B1->d16[0],(DMA_SAMPLE_CNT/2));
    arm_add_q31((q31_t *)pWORK,(q31_t *)pSUMBUF,(q31_t *)pSUMBUF,DMA_SAMPLE_CNT/2);

    arm_q31_to_q15((q31_t *)pSUMBUF,  (q15_t *)&pW16BSUM->d16[0],(DMA_SAMPLE_CNT/2));	//int32_t -> int16_t
    arm_shift_q15( (q15_t *)&pW16BSUM->d16[0],-4,  (q15_t *)&pDACBUF->d12[0],DMA_SAMPLE_CNT/2);				//int16_t -> int12_t
    arm_offset_q15( (q15_t *)&pDACBUF->d12[0],2048 , (q15_t *)&pDACBUF->d12[0],DMA_SAMPLE_CNT/2);			//int12_t -> uint12_t;

HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);


}

void GetWaveData_SecondHalf(void){

    int32_t *pSRC;
	q31_t MicGain;

HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);

	MicGain = MicGainCH0 * 0x1000000;
    pSRC=(int32_t *)(&pDFSDMBUF->d32[0][1]);	//DFSDM buffer Block 1 & Block 2

    arm_scale_q31 (pSRC, MicGain, MicGainShiftCH0, (q31_t *)pWORK, (DMA_SAMPLE_CNT/2));
    arm_q31_to_q15((q31_t *)pWORK, (q15_t *)&pW16B0->d16[DMA_SAMPLE_CNT/2],(DMA_SAMPLE_CNT/2));			//to signed 16bit
    arm_copy_q31(pSRC,(q31_t *)pSUMBUF,DMA_SAMPLE_CNT/2);

    MicGain = MicGainCH1 * 0x1000000;
    pSRC=(int32_t *)(&pDFSDMBUF->d32[1][1]);	//DFSDM buffer Block 1 & Block 2
    arm_scale_q31 (pSRC, MicGain, MicGainShiftCH1, (q31_t *)pWORK, (DMA_SAMPLE_CNT/2));
    arm_q31_to_q15((q31_t *)pWORK, (q15_t *)&pW16B1->d16[DMA_SAMPLE_CNT/2],(DMA_SAMPLE_CNT/2));
    arm_add_q31((q31_t *)pWORK, (q31_t *)pSUMBUF, (q31_t *)pSUMBUF, DMA_SAMPLE_CNT/2);

    arm_q31_to_q15((q31_t *)pSUMBUF,  (q15_t *)&pW16BSUM->d16[DMA_SAMPLE_CNT/2],(DMA_SAMPLE_CNT/2));	//int32_t -> int16_t
    arm_shift_q15( (q15_t *)&pW16BSUM->d16[DMA_SAMPLE_CNT/2],-4, (q15_t *)&pDACBUF->d12[DMA_SAMPLE_CNT/2],DMA_SAMPLE_CNT/2);				//int16_t -> int12_t
    arm_offset_q15( (q15_t *)&pDACBUF->d12[DMA_SAMPLE_CNT/2],2048, (q15_t *)&pDACBUF->d12[DMA_SAMPLE_CNT/2],DMA_SAMPLE_CNT/2);			//int12_t -> uint12_t;
HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);


	if (SNS_STS == SNS_READY){
		DMA_CMPLT_CNT++;
	}
	else{
		DMA_CMPLT_CNT=0;
	}

}

//DMA�]���̑O�����I��
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{

	if(hdfsdm_filter->Instance==DFSDM1_Filter0){

	 //4CH�����Z
 	 half_flag =false;

	}
	if(hdfsdm_filter->Instance==DFSDM1_Filter1){
//	    half_flag =false;
//		HAL_GPIO_TogglePin(DEBP2_GPIO_Port,DEBP2_Pin);
	}
}


/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
//�㔼��DMA�]�����I��
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{

	if(hdfsdm_filter->Instance==DFSDM1_Filter0){

		cmplt_flag = false;

//		HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);
	}
	if(hdfsdm_filter->Instance==DFSDM1_Filter1){
//		cmplt_flag = false;
//		HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);
	}

}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	  HAL_GPIO_TogglePin(DEBP0_GPIO_Port,DEBP0_Pin);

}


