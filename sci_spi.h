

// �^�b�`�R���g���[��(XPT2046)��T_CS�p�@ (SCI5 �Ȉ�SPI�̃Z���N�g�M��)   
//  ��Ɂ@Low

// �^�b�`�R���g���[��(XPT2046)��T_IRQ�p�@P14 (�^�b�`�������Low�ƂȂ�)    
// ����͖��g�p
#define T_IRQ_PMR      (PORT1.PMR.BIT.B4)   // 0:�ėp���o�̓|�[�g
#define T_IRQ_PDR      (PORT1.PDR.BIT.B4)   // 0:���̓|�[�g�Ɏw��
#define T_IRQ_PIDR     (PORT1.PIDR.BIT.B4)  //   ���̓f�[�^
#define T_IRQ_PCR      (PORT1.PCR.BIT.B4)   // 1:���̓v���A�b�v��R�L��	


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




