/*
 * task_com.h
 *
 *  Created on: Jun 18, 2019
 *      Author: 13539
 */

#ifndef TASK_COM_H_
#define TASK_COM_H_


//コマンド受信ステータス
typedef enum _RCV_STS{
	IDLE,
	CMND,		//コマンド受信中
	COMPLETE,	//コマンド正常受信完了
	CMDERROR		//エラー状態
}RecievSeq;

typedef	volatile struct{
	//受信
	char			RecvBuf[16];
	int				RcvCnt;
	int				OverFlow;
	RecievSeq		RecvSeq;

}RecvBuf;


typedef	volatile struct{
	//受信
	char			stx;
	char			command;
	char			sub_command;
	char			param[6];
	char			etx;

}CmdRespBuf;






#endif /* TASK_COM_H_ */
