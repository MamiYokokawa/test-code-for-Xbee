#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include < locale.h >

bool Ret;
HANDLE arduino;

void main(void){
	BYTE data = 1;
	/*
	//----------��M----------------
	//----------��M----------------
	//----------��M----------------
	setlocale(LC_ALL, "japanese");
	//1.�|�[�g���I�[�v��
	arduino = CreateFile("COM3", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//2014/01/22�ǋL�@����łȂ���Ȃ��ꍇ�ɂ�"\\\\.\\COM7"�Ƃ���ƂȂ��邩������܂���B

	if (arduino == INVALID_HANDLE_VALUE){
		printf("PORT COULD NOT OPEN\n");
		system("PAUSE");
		exit(0);
	}
	//2.����M�o�b�t�@������
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
	//3.��{�ʐM�����̐ݒ�
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
	//3.5��M
	
	DWORD dwErrors;  // �G���[��� 
	COMSTAT ComStat; // �f�o�C�X�̏�� 
	DWORD dwCount;   // ��M�f�[�^�̃o�C�g�� 
	char* pszBuf[100];    // �ǂݏo���f�[�^�o�b�t�@ 
	DWORD dwRead;    // �|�[�g����ǂݏo�����o�C�g�� 

	while (1){
	ClearCommError(arduino, &dwErrors, &ComStat);
	dwCount = ComStat.cbInQue;
	ReadFile(arduino, pszBuf, dwCount, &dwRead, NULL);
	printf("%s\n",pszBuf);
	}

	*/
	//----------���M----------------
	//----------���M----------------
	//----------���M----------------
	//1.�|�[�g���I�[�v��
	arduino = CreateFile("COM5", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//2014/01/22�ǋL�@����łȂ���Ȃ��ꍇ�ɂ�"\\\\.\\COM7"�Ƃ���ƂȂ��邩������܂���B

	if (arduino == INVALID_HANDLE_VALUE){
		printf("PORT COULD NOT OPEN\n");
		system("PAUSE");
		exit(0);
	}
	//2.����M�o�b�t�@������
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
	//3.��{�ʐM�����̐ݒ�
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
	//4.���M
	DWORD dwSendSize;
	DWORD dwErrorMask;
	int num = 1; //�V�[�P���X�ԍ�
	int lPwm = 16, rPwm = 32; // ���E��pwm
	int buf[] = { num, lPwm, rPwm }; //���M����f�[�^�̔z��

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