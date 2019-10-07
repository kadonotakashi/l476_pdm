/*
 * task_sns.h
 *
 *  Created on: 2019/04/24
 *      Author: 13539
 */

#ifndef TASK_SNS_H_
#define TASK_SNS_H_

#define	CH_MAX			2

#define	SAMPLE_CNT	1024
#define	DMA_SAMPLE_CNT	SAMPLE_CNT

#define	DISP_CNT	FFT_SampleNum

//#define OFFSET_MAX	32768*32
//#define OFFSET_MIN	-32768*32

#define OFFSET_MAX	32768*32
#define OFFSET_MIN	-32768*32


#define	CALIB_COUNT_MAX	100
typedef struct{
	volatile q31_t d32[DMA_SAMPLE_CNT];
}W32_BUF;

typedef struct{
//	volatile int16_t d16[4][DMA_SAMPLE_CNT/2];
	volatile q15_t d16[DMA_SAMPLE_CNT];
}W16_BUF;

typedef struct{
	volatile q15_t d12[DMA_SAMPLE_CNT];
}W12_BUF;


typedef struct{
	uint16_t BMP[200][256];
}LCD_BUF;


//DFSDM output buffer(DMA transfer)
//    4ch
// x 3(copy ofprevious second half,first half ,second  half )
typedef struct{
	q31_t d32[2][2][SAMPLE_CNT/2];
}DFSDM_BUF;

typedef struct{
	q31_t d32[SAMPLE_CNT];
}Q31T_BUF;



void GetWaveData_FirstHalf(void);
void GetWaveData_SecondHalf(void);


typedef enum
{
	  SNS_INIT ,  	/*	sensor(mic) is in Calibration  */
	  SNS_READY ,  	/*  sensor(mic) is Ready  */
	  SNS_ERR		/*  sensor(mic) is ERROR  */
}SNS_StateTypeDef;



#endif /* TASK_SNS_H_ */
