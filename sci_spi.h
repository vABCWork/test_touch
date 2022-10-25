

// タッチコントローラ(XPT2046)のT_CS用　 (SCI5 簡易SPIのセレクト信号)   
//  常に　Low

// タッチコントローラ(XPT2046)のT_IRQ用　P14 (タッチがあるとLowとなる)    
// 今回は未使用
#define T_IRQ_PMR      (PORT1.PMR.BIT.B4)   // 0:汎用入出力ポート
#define T_IRQ_PDR      (PORT1.PDR.BIT.B4)   // 0:入力ポートに指定
#define T_IRQ_PIDR     (PORT1.PIDR.BIT.B4)  //   入力データ
#define T_IRQ_PCR      (PORT1.PCR.BIT.B4)   // 1:入力プルアップ抵抗有効	


extern volatile uint8_t  xpt_rcv_data[16];
extern volatile uint8_t  xpt_rcv_cnt;

extern volatile uint8_t xpt_sd_data[16];
extern volatile uint8_t xpt_send_num;
extern volatile uint8_t xpt_send_pt;

extern volatile uint8_t sci5_rxi_cnt;
extern volatile uint8_t sci5_tei_cnt;

void initSCI_5(void);

void xpt2046_cal_average(void);
void xpt2046_cal_pressure(void);


void xpt2046_cmd_set(void);

void xpt2046_press_cmd_set(void);




