#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include < locale.h >
#include <iostream>

bool Ret;
HANDLE arduino;

/*
//ロボットはマニュアルモードの際のパケットを利用し走行させる。
//入力コマンド
//0～5:マニュアルモード時の走行コマンド
//8:スタンバイモード
//9:マニュアルモード
*/

//送信先のXbeeのアドレス
byte const robotAddr[] = { byte(0x00), byte(0x13), byte(0xA2), byte(0x00), byte(0x40), byte(0x99), byte(0x37), byte(0x7A) };
//各文字バイト
byte const A = byte(0x41), B = byte(0x42), C = byte(0x43), D = byte(0x44), E = byte(0x45), F = byte(0x46),
G = byte(0x47), H = byte(0x48), I = byte(0x49), J = byte(0x4a), K = byte(0x4b), L = byte(0x4c),
M = byte(0x4d), N = byte(0x4e), O = byte(0x4f), P = byte(0x50), Q = byte(0x51), R = byte(0x52),
S = byte(0x53), T = byte(0x54), U = byte(0x55), V = byte(0x56), W = byte(0x57), X = byte(0x58),
Y = byte(0x59), Z = byte(0x5a);

//モードごとのleftとrightのpwm
byte lPwm[] = { byte(0x20), byte(0x10), byte(0x08), byte(0x20), byte(0x20), byte(0x00) };
byte rPwm[] = { byte(0x20), byte(0x20), byte(0x20), byte(0x10), byte(0x08), byte(0x00) };

//パケット作成・送信
//command:シーケンス番号0～5
void sentAigamoCommand(int command){

	DWORD dwSendSize;
	DWORD dwErrorMask;
	byte checksum = 0;

	//パケット生成
	byte requestPacket[] = { byte(0x7E), byte(0x00), byte(0x1F), byte(0x10), byte(0x01),
		robotAddr[0], robotAddr[1], robotAddr[2], robotAddr[3],
		robotAddr[4], robotAddr[5], robotAddr[6], robotAddr[7],
		byte(0xFF), byte(0xFE), byte(0x00), byte(0x00), A, G, S,
		M, F, A, T, A, L, 1, lPwm[byte(command)], R, 1, rPwm[byte(command)], A, G, E, byte(0x00) };

	//チェックサムの計算
	for (int i = 3; i < 34; i++){
		checksum += requestPacket[i];
	}
	checksum = 0xFF - (checksum & 0x00FF);
	requestPacket[34] = byte(checksum);

	//パケットの送信
	Ret = WriteFile(arduino, requestPacket, sizeof(requestPacket), &dwSendSize, NULL);

	if (!Ret){
		printf("SEND FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}

}

//マニュアルモードに変更コマンドの送信
//8:スタンバイ
//9:マニュアル
void sentManualCommand(int command){

	DWORD dwSendSize;
	DWORD dwErrorMask;
	byte checksum = 0;

	byte mode = byte(command) - 0x08; //変更モード

	//パケット生成
	byte requestPacket[] = { byte(0x7E), byte(0x00), byte(0x1A), byte(0x10), byte(0x01),
		robotAddr[0], robotAddr[1], robotAddr[2], robotAddr[3],
		robotAddr[4], robotAddr[5], robotAddr[6], robotAddr[7],
		byte(0xFF), byte(0xFE), byte(0x00), byte(0x00), A, G, S, C, F, A, T, A, mode, A, G, E, byte(0x00) };

	//チェックサムの計算
	for (int i = 3; i < 29; i++){
		checksum += requestPacket[i];
	}
	checksum = 0xFF - (checksum & 0x00FF);
	requestPacket[29] = byte(checksum);

	//パケットの送信
	Ret = WriteFile(arduino, requestPacket, sizeof(requestPacket), &dwSendSize, NULL);

	if (!Ret){
		printf("SEND FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}

}

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

	while (1){
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
		char id = A;
		int command;

		std::cin >> command;

		//マニュアルモードに変更コマンドの送信
		//8:スタンバイ
		//9:マニュアル
		if (command == 8 || command == 9 ){
			sentManualCommand(command);
		}
		//パケット作成・送信
		//command:シーケンス番号0～5
		else{
			sentAigamoCommand(command);
		}
	}
	//	printf("FINISH\n");
		CloseHandle(arduino);
		system("PAUSE");

}