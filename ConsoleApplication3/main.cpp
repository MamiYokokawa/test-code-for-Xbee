#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include < locale.h >
#include <iostream>

bool Ret;
HANDLE arduino;

/*
//���{�b�g�̓}�j���A�����[�h�̍ۂ̃p�P�b�g�𗘗p�����s������B
//���̓R�}���h
//0�`5:�}�j���A�����[�h���̑��s�R�}���h
//8:�X�^���o�C���[�h
//9:�}�j���A�����[�h
*/

//���M���Xbee�̃A�h���X
byte const robotAddr[] = { byte(0x00), byte(0x13), byte(0xA2), byte(0x00), byte(0x40), byte(0x99), byte(0x37), byte(0x7A) };
//�e�����o�C�g
byte const A = byte(0x41), B = byte(0x42), C = byte(0x43), D = byte(0x44), E = byte(0x45), F = byte(0x46),
G = byte(0x47), H = byte(0x48), I = byte(0x49), J = byte(0x4a), K = byte(0x4b), L = byte(0x4c),
M = byte(0x4d), N = byte(0x4e), O = byte(0x4f), P = byte(0x50), Q = byte(0x51), R = byte(0x52),
S = byte(0x53), T = byte(0x54), U = byte(0x55), V = byte(0x56), W = byte(0x57), X = byte(0x58),
Y = byte(0x59), Z = byte(0x5a);

//���[�h���Ƃ�left��right��pwm
byte lPwm[] = { byte(0x20), byte(0x10), byte(0x08), byte(0x20), byte(0x20), byte(0x00) };
byte rPwm[] = { byte(0x20), byte(0x20), byte(0x20), byte(0x10), byte(0x08), byte(0x00) };

//�p�P�b�g�쐬�E���M
//command:�V�[�P���X�ԍ�0�`5
void sentAigamoCommand(int command){

	DWORD dwSendSize;
	DWORD dwErrorMask;
	byte checksum = 0;

	//�p�P�b�g����
	byte requestPacket[] = { byte(0x7E), byte(0x00), byte(0x1F), byte(0x10), byte(0x01),
		robotAddr[0], robotAddr[1], robotAddr[2], robotAddr[3],
		robotAddr[4], robotAddr[5], robotAddr[6], robotAddr[7],
		byte(0xFF), byte(0xFE), byte(0x00), byte(0x00), A, G, S,
		M, F, A, T, A, L, 1, lPwm[byte(command)], R, 1, rPwm[byte(command)], A, G, E, byte(0x00) };

	//�`�F�b�N�T���̌v�Z
	for (int i = 3; i < 34; i++){
		checksum += requestPacket[i];
	}
	checksum = 0xFF - (checksum & 0x00FF);
	requestPacket[34] = byte(checksum);

	//�p�P�b�g�̑��M
	Ret = WriteFile(arduino, requestPacket, sizeof(requestPacket), &dwSendSize, NULL);

	if (!Ret){
		printf("SEND FAILED\n");
		CloseHandle(arduino);
		system("PAUSE");
		exit(0);
	}

}

//�}�j���A�����[�h�ɕύX�R�}���h�̑��M
//8:�X�^���o�C
//9:�}�j���A��
void sentManualCommand(int command){

	DWORD dwSendSize;
	DWORD dwErrorMask;
	byte checksum = 0;

	byte mode = byte(command) - 0x08; //�ύX���[�h

	//�p�P�b�g����
	byte requestPacket[] = { byte(0x7E), byte(0x00), byte(0x1A), byte(0x10), byte(0x01),
		robotAddr[0], robotAddr[1], robotAddr[2], robotAddr[3],
		robotAddr[4], robotAddr[5], robotAddr[6], robotAddr[7],
		byte(0xFF), byte(0xFE), byte(0x00), byte(0x00), A, G, S, C, F, A, T, A, mode, A, G, E, byte(0x00) };

	//�`�F�b�N�T���̌v�Z
	for (int i = 3; i < 29; i++){
		checksum += requestPacket[i];
	}
	checksum = 0xFF - (checksum & 0x00FF);
	requestPacket[29] = byte(checksum);

	//�p�P�b�g�̑��M
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

	while (1){
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
		char id = A;
		int command;

		std::cin >> command;

		//�}�j���A�����[�h�ɕύX�R�}���h�̑��M
		//8:�X�^���o�C
		//9:�}�j���A��
		if (command == 8 || command == 9 ){
			sentManualCommand(command);
		}
		//�p�P�b�g�쐬�E���M
		//command:�V�[�P���X�ԍ�0�`5
		else{
			sentAigamoCommand(command);
		}
	}
	//	printf("FINISH\n");
		CloseHandle(arduino);
		system("PAUSE");

}