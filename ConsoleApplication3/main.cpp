#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include < locale.h >

bool Ret;
HANDLE arduino;

void main(void){
	BYTE data = 1;
	/*
	//----------受信----------------
	//----------受信----------------
	//----------受信----------------
	setlocale(LC_ALL, "japanese");
	//1.ポートをオープン
	arduino = CreateFile("COM3", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//2014/01/22追記　これでつながらない場合には"\\\\.\\COM7"とするとつながるかもしれません。

	if (arduino == INVALID_HANDLE_VALUE){
		printf("PORT COULD NOT OPEN\n");
		system("PAUSE");
		exit(0);
	}
	//2.送受信バッファ初期化
	Ret = SetupComm(arduino, 1024, 1024);
	if (!Ret){
		printf("SET UP FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}
	Ret = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (!Ret){
		printf("CLEAR FAILED\n");
		CloseHandle(arduino);
		exit(0);
	}
	//3.基本通信条件の設定
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 57600;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	Ret = SetCommState(arduino, &dcb);
	if (!Ret){
		printf("SetCommState FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}
	//3.5受信
	
	DWORD dwErrors;  // エラー情報 
	COMSTAT ComStat; // デバイスの状態 
	DWORD dwCount;   // 受信データのバイト数 
	char* pszBuf[100];    // 読み出しデータバッファ 
	DWORD dwRead;    // ポートから読み出したバイト数 

	while (1){
	ClearCommError(arduino, &dwErrors, &ComStat);
	dwCount = ComStat.cbInQue;
	ReadFile(arduino, pszBuf, dwCount, &dwRead, NULL);
	printf("%s\n",pszBuf);
	}

	*/
	//----------送信----------------
	//----------送信----------------
	//----------送信----------------
	//1.ポートをオープン
	arduino = CreateFile("COM5", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//2014/01/22追記　これでつながらない場合には"\\\\.\\COM7"とするとつながるかもしれません。

	if (arduino == INVALID_HANDLE_VALUE){
		printf("PORT COULD NOT OPEN\n");
		system("PAUSE");
		exit(0);
	}
	//2.送受信バッファ初期化
	Ret = SetupComm(arduino, 1024, 1024);
	if (!Ret){
		printf("SET UP FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}
	Ret = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (!Ret){
		printf("CLEAR FAILED\n");
		CloseHandle(arduino);
		exit(0);
	}
	//3.基本通信条件の設定
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 57600;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	Ret = SetCommState(arduino, &dcb); 
	if (!Ret){
		printf("SetCommState FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}	
	//4.送信
	DWORD dwSendSize;
	DWORD dwErrorMask;
	int num = 1; //シーケンス番号
	int lPwm = 16, rPwm = 32; // 左右のpwm
	int buf[] = { num, lPwm, rPwm }; //送信するデータの配列

	Ret = WriteFile(arduino, buf, sizeof(buf), &dwSendSize, NULL);
	if (!Ret){
		printf("SEND FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}
	printf("FINISH num:%d left:%d right:%d\n", buf[0], buf[1], buf[2]);
	CloseHandle(arduino);
	system("PAUSE");


}