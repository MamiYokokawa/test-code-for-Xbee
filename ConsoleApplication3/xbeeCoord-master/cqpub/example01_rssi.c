/***************************************************************************************
XBeeのLEDを点滅させてみる：パソコンに接続した親機XBeeのRSSI LEDを点滅

                                                       Copyright (c) 2013 Wataru KUNINO
***************************************************************************************/

#include "../libs/xbee.c"               // XBeeライブラリのインポート

int main(int argc,char **argv){
	
    byte com=0;                         // シリアルCOMポート番号
    
    if(argc==2) com=(byte)atoi(argv[1]);// 引数があれば変数comに代入する
    xbee_init( com );                   // XBee用COMポートの初期化(引数はポート番号)
    
    while(1){                           // 繰り返し処理
        xbee_at("ATP005");              // ローカルATコマンドATP0(DIO10設定)=05(出力'H')
        delay( 1000 );                  // 約1000ms(1秒間)の待ち
        xbee_at("ATP004");              // ローカルATコマンドATP0(DIO10設定)=04(出力'L')
        delay( 1000 );                  // 約1000ms(1秒間)の待ち
    }
}
