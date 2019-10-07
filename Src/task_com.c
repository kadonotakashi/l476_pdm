/*
 * task_com.c
 *
 *  Created on: Jun 18, 2019
 *      Author: 13539
 */


#include "cmsis_os.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "arm_math.h"

#include "task_com.h"
#include "task_sns.h"


void CmndSeq(char recv_char);

extern UART_HandleTypeDef huart1;

#define	HAL_UART	huart1

extern DMA_HandleTypeDef hdma_usart1_tx;
extern osMessageQId QueSendHandle;
CmdRespBuf	CmdBuf;
CmdRespBuf	RspBuf;

RecvBuf rx_buf;
uint8_t recv_char;

extern DFSDM_BUF *pDFSDMBUF;
extern W12_BUF	*pDACBUF;
extern W16_BUF *pW16BSUM;

extern	W16_BUF *pW16B0;
extern	W16_BUF *pW16B1;

extern int8_t	MicGainCH0,MicGainCH1;
extern int8_t	MicGainShiftCH0,MicGainShiftCH1;



volatile int TxCmpleteFlag=1;

int send_block_max;
int send_block;
int DataTxEn;

int AsciiHex2uint16( char *src,uint16_t *val)
{
	int i;
	uint16_t temp=0;

	for(i=0;i<4;i++,src++){
		temp = temp<<4;
		if((*src>='0')&&(*src<='9')){
			temp += (*src-'0');
		}else if((*src>='A')&&(*src<='F')){
			temp += (*src-55);
		}else{
			return -1;
		}
	}
	*val=temp;
	return 0;
}

void uint16toAsciiHex( uint16_t val,char *dst)
{
	uint16_t v;
	int i;
	char temp;

	v=val;
	for(i=3;i>=0;i--){
		temp = v & 0x0f;
		if (temp>9){
			*(dst+i)=(temp + 55);
		}else{
			*(dst+i)=(temp + '0');
		}
		v = v >>4;
	}
	*(dst+4)=0;

}



int AsciiHex2char( char *src,uint8_t *val)
{
	uint8_t temp=0;
	if((*src>='0')&&(*src<='9')){
		temp=(*src-'0')*0x10;
	}else if((*src>='A')&&(*src<='F')){
		temp=(*src-55)*0x10;
	}else{
		return -1;
	}

	if((*(src+1)>='0')&&(*(src+1)<='9')){
		temp+=(*(src+1)-'0');
	}else if((*(src+1)>='A')&&(*(src+1)<='F')){
		temp+=(*(src+1)-55);
	}else{
		return -1;
	}
	*val = temp;
	return 0;
}


void char2AsciiHex( char val,char *dst)
{
	char temp;

	temp=(val>>4)&0x0f;
	if (temp>9){
		*dst=(temp + 55);
	}else{
		*dst=(temp + '0');
	}

	temp=val&0x0f;
	if (temp>9){
		*(dst+1)=(temp + 55);
	}else{
		*(dst+1)=(temp + '0');
	}
	*(dst+2)=0;
}


//PCとの通信開始
void HostIfOpen(void)
{	//1character目受信開始
	HAL_UART_Receive_IT(&HAL_UART , &recv_char,1);
}



//UART 1character 受信
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	CmndSeq(recv_char);
	HAL_UART_Receive_IT(&HAL_UART , &recv_char,1);	//allow recieve next character
}


void send_resp(int length,char *buf)
{
	HAL_UART_Transmit_IT(&HAL_UART,(uint8_t *)buf,(uint16_t)length);
}

HAL_StatusTypeDef send_data(char *txbuf,int data_count)
{
	HAL_StatusTypeDef STS;
	STS=HAL_UART_Transmit_DMA(&HAL_UART,(uint8_t *)txbuf,data_count);
	return	STS;
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	TxCmpleteFlag=1;
}


int HostStsChk(void)
{
	if( rx_buf.RecvSeq==COMPLETE	){
		//コマンド処理
		rx_buf.OverFlow=0;
		rx_buf.RcvCnt=0;
		rx_buf.RecvSeq=IDLE;
		return 1;
	}
	else if( rx_buf.RecvSeq==CMDERROR	){
		rx_buf.OverFlow=0;
		rx_buf.RcvCnt=0;
		rx_buf.RecvSeq=IDLE;
		//通信エラー解除
	}
	else if( rx_buf.RecvSeq==CMND	){
		//コマンド受信中
	}
	else if( rx_buf.RecvSeq==IDLE	){
	}
	return 0;
}



void CmndSeq(char recv_char){

	if (recv_char=='(') {	//強制的にコマンド受信開始
		rx_buf.RcvCnt=0;
		rx_buf.RecvSeq=CMND;
	}
	else if (recv_char==')') {
		if (rx_buf.RcvCnt==8){
			rx_buf.RecvSeq=COMPLETE;
			CmdBuf.command		=rx_buf.RecvBuf[0];
			CmdBuf.sub_command	=rx_buf.RecvBuf[1];
			CmdBuf.param[0]		=rx_buf.RecvBuf[2];
			CmdBuf.param[1]		=rx_buf.RecvBuf[3];
			CmdBuf.param[2]		=rx_buf.RecvBuf[4];
			CmdBuf.param[3]		=rx_buf.RecvBuf[5];
			CmdBuf.param[4]		=rx_buf.RecvBuf[6];
			CmdBuf.param[5]		=rx_buf.RecvBuf[7];
		}
		else{
			rx_buf.RecvSeq=CMDERROR;
		}
	}else if (rx_buf.RcvCnt>=8){
		rx_buf.RecvSeq=CMDERROR;
	}else{
		rx_buf.RecvBuf[rx_buf.RcvCnt]=recv_char;
		rx_buf.RcvCnt++;
	}

}



int CommandChk(int *STS)
{
#if 1
	volatile int CmfChkFlag,ParmChkFlag	;
	volatile int flag;
	uint8_t val;
	uint16_t	val16;

	CmfChkFlag =-1;
	ParmChkFlag=-2;
	//////////////
	//command check
	//////////////

	//STS=0 idle,STS!=0 Busy
	if (('I'==CmdBuf.command) &&('S'==CmdBuf.sub_command)){				//Initial sensor
		CmfChkFlag=1;
		*STS=0;
	}
	else if (('G'==CmdBuf.command) &&('W'==CmdBuf.sub_command) && (*STS==0)){		//write register command
		CmfChkFlag=1;
	}

	if (1!=CmfChkFlag)	return CmfChkFlag;	//コマンド未定義

	//////////////
	//parameter check
	//////////////
	if(('I'==CmdBuf.command)&&('S'==CmdBuf.sub_command))		//reset command
	{
		//not need parameter
		//parameter="000000"
//		flag=AsciiHex2char((char *)&CmdBuf.param[0],(char *)&val);
//		if ((flag!=0)||(val!=0))	return ParmChkFlag;
//		flag=AsciiHex2char( (char *)&CmdBuf.param[2],(char *)&val);
//		if ((flag!=0)||(val!=0))	return ParmChkFlag;
//		flag=AsciiHex2char( (char *)&CmdBuf.param[4],(char *)&val);
//		if ((flag!=0)||(val!=0))	return ParmChkFlag;
		ParmChkFlag=1;
		return 0;
	}
	else if(('G'==CmdBuf.command)&&('W'==CmdBuf.sub_command))		//GetWave command
	{
		//"abcd00"	abcdはブロック数　max256 1bock=4kbyte=2k sample = 46ms
		flag=AsciiHex2uint16( (char *)&CmdBuf.param[0],&val16);
		if (flag!=0)				return ParmChkFlag;
		if((val<0) || (val>4096))	return ParmChkFlag;

		ParmChkFlag=1;
		return 0;
	}
	else if(('S'==CmdBuf.command)&&('G'==CmdBuf.sub_command))		//GetWave command
	{
		//"abcd00"	abcdはブロック数　max256 1bock=4kbyte=2k sample = 46ms
		flag=AsciiHex2uint16( (char *)&CmdBuf.param[0],&val16);
		if (flag!=0)				return ParmChkFlag;
//		if((val<0) || (val>256))	return ParmChkFlag;

		ParmChkFlag=1;
		return 0;
	}

	return 0;
#endif
}


void Execute(void)
{
	uint16_t val16;
	//////////////
	//execute command
	//////////////
	if(('I'==CmdBuf.command)&&('S'==CmdBuf.sub_command))		//reset command
	{
		DataTxEn=0;
		RspBuf.command='I';
		RspBuf.sub_command='S';
		RspBuf.param[0]='0';
		RspBuf.param[1]='0';
		RspBuf.param[2]='0';
		RspBuf.param[3]='0';
		RspBuf.param[4]='0';
		RspBuf.param[5]='0';
		send_block_max = 0;
		send_block=0;
		DataTxEn=0;
		send_resp(10,(char *)&RspBuf);
	}
	else if(('G'==CmdBuf.command)&&('W'==CmdBuf.sub_command))		//read register command
	{
		RspBuf.command='G';
		RspBuf.sub_command='W';
		RspBuf.param[0]=CmdBuf.param[0];
		RspBuf.param[1]=CmdBuf.param[1];
		RspBuf.param[2]=CmdBuf.param[2];
		RspBuf.param[3]=CmdBuf.param[3];
		RspBuf.param[4]='0';
		RspBuf.param[5]='0';

		AsciiHex2uint16((char *)&CmdBuf.param[0],&val16);	//採取ブロック数

		send_block_max = (int)val16;
		send_block=0;
		DataTxEn=1;

	}
	else if(('S'==CmdBuf.command)&&('G'==CmdBuf.sub_command))		//read register command
	{
		RspBuf.command='S';
		RspBuf.sub_command='G';
		RspBuf.param[0]=CmdBuf.param[0];
		RspBuf.param[1]=CmdBuf.param[1];
		RspBuf.param[2]=CmdBuf.param[2];
		RspBuf.param[3]=CmdBuf.param[3];
		RspBuf.param[4]='0';
		RspBuf.param[5]='0';

		AsciiHex2char((char *)&CmdBuf.param[0], (uint8_t *)&MicGainCH0);
		AsciiHex2char((char *)&CmdBuf.param[2], (uint8_t *)&MicGainCH1);
		send_resp(10,(char *)&RspBuf);
	}
}

void tk_com(void const * argument)
{
  /* USER CODE BEGIN tk_com */
  /* Infinite loop */

	int LocalQue;
	portBASE_TYPE result;
	int flag=0;
	RspBuf.stx='(';
	RspBuf.etx=')';

	HostIfOpen();
	for(;;)
	{
		TxCmpleteFlag=1;
		flag = HostStsChk();
		if(flag!=0){	//有効なコマンド
			Execute();
		}

		if (1==DataTxEn){
			result = xQueueReceive(QueSendHandle,&LocalQue,0);
			if(result==pdTRUE){
				///////////////////////////first half
				if(0==LocalQue){

//HAL_GPIO_TogglePin(DEBP0_GPIO_Port,DEBP0_Pin);
					while(TxCmpleteFlag==0){	// wait for ch1.second half complete
						osDelay(1);
					}

					TxCmpleteFlag=0;
					send_data( (char *)&pW16B0->d16[0], 1024);
					send_block++;

//HAL_GPIO_TogglePin(DEBP0_GPIO_Port,DEBP0_Pin);
					while(TxCmpleteFlag==0){	// wait for ch1.second half complete
						osDelay(1);
					}

					TxCmpleteFlag=0;
					send_data( (char *)&pW16B1->d16[0], 1024);
					send_block++;

				}
				////////////////////////////second half
				else if(1==LocalQue){
//HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);
				while(TxCmpleteFlag==0){	// wait for ch1.second half complete
					osDelay(1);
				}

					TxCmpleteFlag=0;
					send_data((char *) &pW16B0->d16[(DMA_SAMPLE_CNT/2)], 1024);
					send_block++;

//HAL_GPIO_TogglePin(DEBP1_GPIO_Port,DEBP1_Pin);
				while(TxCmpleteFlag==0){	// wait for ch1.second half complete
					osDelay(1);
				}

					TxCmpleteFlag=0;
					send_data( (char *)&pW16B1->d16[(DMA_SAMPLE_CNT/2)], 1024);
					send_block++;

				}
				else{

				}
				if(	send_block >= send_block_max *4 ){
					DataTxEn=0;
				}
				osDelay(1);
			}
		}else{
			result = xQueueReceive(QueSendHandle,&LocalQue,0);
			if(result==pdTRUE){
				osDelay(1);		//不要なQueueは読み飛ばす。
			}
			else{
				osDelay(1);
			}
		}
  }
  /* USER CODE END tk_com */
}



