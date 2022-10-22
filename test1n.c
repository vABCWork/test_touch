#include	<machine.h>
#include	 "iodefine.h"
#include	 "misratypes.h"
#include	"delay.h"
#include	"sci_spi.h"

void clear_module_stop(void);

void main(void)
{
	
	clear_module_stop();	//  モジュールストップの解除
	
	initSCI_5();		// SCI5(簡易SPI)  初期設定
	
	while(1){
	
	   delay_msec(1);		// 1msec 待つ
		
	   xpt2046_cmd_set();     // タッチコントローラ XPT2046と送受信開始  (2Mbpsで25[usec]かかる )
	   
	   IEN(SCI5,TXI5) = 1;    // 送信割り込み許可
     
	     
	}
	
}





// モジュールストップの解除
//  シリアルコミュニケーションインタフェース5(SCI5)(タッチコントローラ XPT2046 との通信用)(簡易SPI通信)
//
void clear_module_stop(void)
{
	SYSTEM.PRCR.WORD = 0xA50F;	// クロック発生、消費電力低減機能関連レジスタの書き込み許可	

	MSTP(SCI5) = 0;			// SCI5    モジュールストップの解除:

	SYSTEM.PRCR.WORD = 0xA500;	// クロック発生、消費電力低減機能関連レジスタ書き込み禁止
}

