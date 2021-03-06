/***************************************************************************************
LEDをリモート制御する�B様々なポートに出力

                                                       Copyright (c) 2013 Wataru KUNINO
***************************************************************************************/

#include "../libs/xbee.c"

// お手持ちのXBeeモジュール子機のIEEEアドレスに変更する↓
byte dev[] = {0x00,0x13,0xA2,0x00,0x40,0x30,0xC1,0x6F};

int main(int argc,char **argv){
    byte com=0;                         // シリアルCOMポート番号
    char s[3];                          // 入力用(2文字まで)
    byte port;                          // リモート子機のポート番号
    byte value;                         // リモート子機への設定値
    
    if(argc==2) com=(byte)atoi(argv[1]);// 引数があれば変数comに代入する
    xbee_init( com );                   // XBee用COMポートの初期化
    xbee_atnj( 0xFF );                  // 親機XBeeを常にジョイン許可状態にする

    while(1){                           // 繰り返し処理
    
        /* 子機のポート番号と制御値の入力 */
        printf("Port  =");              // ポート番号入力のための表示
        gets( s );                      // キーボードからの入力
        port = atoi( s );               // 入力文字を数字に変換してportに代入
        printf("Value =");              // 値の入力のための表示
        gets( s );                      // キーボードからの入力
        value = atoi( s );              // 入力文字を数字に変換してvalueに代入

        /* XBee通信 */
        xbee_gpo(dev,port,value);       // リモート子機ポート(port)に制御値(value)を設定
    }
}
