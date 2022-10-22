
#include "iodefine.h"
#include "misratypes.h"
#include "sci_spi.h"


//
//  SCI5 �^�b�`�R���g���[�� (XPT2046)�Ƃ̊Ȉ�SPI�ʐM
//

// �^�b�`�R���g���[�� XPT2046�Ƃ̒ʐM�@��M�p
volatile uint8_t  xpt_rcv_data[16];
volatile uint8_t  xpt_rcv_cnt;

volatile uint8_t  xpt_ovr_run_err;

//  �^�b�`�R���g���[�� XPT2046�Ƃ̒ʐM�@���M�p
volatile uint8_t xpt_sd_data[16];
volatile uint8_t xpt_send_num;
volatile uint8_t xpt_send_pt;

volatile uint8_t sci5_rxi_cnt;	// �f�o�b�N�p
volatile uint8_t sci5_tei_cnt;  // �f�o�b�N�p


//
// ��M���荞��(SCI5) (�^�b�`�R���g���[�� XPT2046 �ʐM�p)
// 28.5.6 �V���A���f�[�^�̑���M��������( �N���b�N���������[�h)
//  ��M���瓯������M�֐؂�ւ���Ƃ��ɂ́ASCI ����M������Ԃł��邱�Ƃ��m�F������ASCR ���W�X�^��RIE�ARE �r�b�g���g0�h �ɂ��Ă���A
//  �G���[�t���O(SSR.ORER, FER, PER) ���g0�h �ł��邱�Ƃ��m�F������ASCR ���W�X�^��TIE�ARIE�ATE�ARE �r�b�g��1 ���߂œ����Ɂg1�h �ɂ��Ă��������B
//
#pragma interrupt (Excep_SCI5_RXI5(vect=223))
void Excep_SCI5_RXI5(void)
{
      xpt_rcv_data[xpt_rcv_cnt] =  SCI5.RDR;	// ��M�f�[�^�ǂݏo��

      xpt_ovr_run_err = SCI5.SSR.BIT.ORER;  // �I�[�o�����G���[�t���O�ǂݏo��
      
      sci5_rxi_cnt++;		// �f�o�b�N�p
      
      if ( xpt_rcv_cnt < (xpt_send_num - 1) ){  
	     xpt_rcv_cnt++;      // ��M�f�[�^�i�[�ʒu�̃C���N�������g
      }
      else {                     // �ŏI�f�[�^��M�ς�
	   
      	    xpt_rcv_cnt = 0;
      }
}


//
// ���M���荞��(SCI5) (�^�b�`�R���g���[�� XPT2046 �ʐM�p)
// 28.5.6 �V���A���f�[�^�̑���M��������( �N���b�N���������[�h)
//   ���M���瓯������M�֐؂�ւ���Ƃ��ɂ́ASCI �����M�I����Ԃł��邱�Ƃ�SSR.TEND �t���O���g1�h �� �Ȃ��Ă��邱�ƂŊm�F���Ă��������B
//    ���̌�ASCR ���W�X�^�����������Ă���SCR ���W�X�^��TIE,RIE, TE,RE �r�b�g��1 ���߂œ����Ɂg1�h �ɂ��Ă��������B
//
#pragma interrupt (Excep_SCI5_TXI5(vect=224))
void Excep_SCI5_TXI5(void){
		
	SCI5.TDR = xpt_sd_data[xpt_send_pt];     // ���M�f�[�^�����C�g
	
	xpt_send_pt++;		// ���M�f�[�^�̊i�[�ꏊ�C���N�������g
	
	if ( xpt_send_pt == xpt_send_num ) {  // �ŏI�f�[�^�̑��M����
	
	     IEN(SCI5,TXI5) = 0;		// ���M���荞�݋֎~
	     SCI5.SCR.BIT.TEIE = 1;  		// TEI ���M�������荞�ݗv���̋���
	
	}				
	
}


//
// ���M�I�����荞��(SCI5) (�^�b�`�R���g���[�� XPT2046 �ʐM�p)
//
#pragma interrupt (Excep_SCI5_TEI5(vect=225))
void Excep_SCI5_TEI5(void){
       
	SCI5.SCR.BIT.TEIE = 0;  	// TEI���荞��(���M�I�����荞��)�֎~
	
	sci5_tei_cnt++;			// �f�o�b�N�p
}



// 
// SCI5 �����ݒ� �i�Ȉ�SPI�ʐM) (�^�b�`�R���g���[�� XPT2046�p)
// 
//�|�[�g: �p�r      :XPT2046�M����
//  P14: ���̓|�[�g ; T_IRQ (Low�Ń^�b�`����)
//
//  PH2; SCK5       : T_CLK (�N���b�N�@max:��2MHz)
//  PH1: SMOSI5     : T_DIN (�}�C�R������̃R�}���h����)
//  PH0: SMISO5     : T_DO (A/D�ϊ��f�[�^�o��)
//
//�ESPI���[�h
//  CKPH = 1 , CKPOL = 0
//  �A�C�h���� Clock = Low, �N���b�N�����オ�莞�Ƀf�[�^�T���v��
//�@
// �E�{�[���[�g
// �N���b�N�Z���N�g�r�b�g=0 (PCKLB=32MHz)�@
//  SCI5.SMR.BYTE = 0x80;	// PCLKB(=32MHz)  n = 0
//
//  N�̌v�Z��:	(B=�{�[���[�g bps)			
//    N= (32 x 1000000/(8/2)xB)-1
//     = (32 x 1000000/(4xB)) - 1
//�@�@
//   B=250Kbps �Ƃ���ƁAN= ( 32x 1000000 / 1000000 ) - 1 = 31
//   B=500Kbps �Ƃ���ƁAN= ( 32x 1000000 / 2000000 )  -1 = 15
//   B=1000Kbps �Ƃ���ƁAN= ( 32x 1000000 / 4000000 )  -1 = 7
//   B=2000kbps  �Ƃ���ƁAN = ( 32x1000000 / 8000000 ) - 1 = 3
// (�^�b�`�R���g���[�� XPT2046�� Max 2.5MHz)
//
// �E����M�̃r�b�g�̏���
//   MSB�t�@�[�X�g�ő���M (b7���瑗�M�A��M)
//  SCI5.SCMR.BIT.SDIR = 1 : MSB�t�@�[�X�g�ő���M
//
void initSCI_5(void)
{
	
	MPC.PWPR.BIT.B0WI = 0;   // �}���`�t�@���N�V�����s���R���g���[���@�v���e�N�g����
	MPC.PWPR.BIT.PFSWE = 1;  // PmnPFS ���C�g�v���e�N�g����
	
	
	MPC.PH2PFS.BYTE = 0x0A;  // PH2 = SCK5
	MPC.PH1PFS.BYTE = 0x0A;  // PH1 = SMOSI5
	MPC.PH0PFS.BYTE = 0x0A;  // PH0 = SMISO5
	
	MPC.PWPR.BYTE = 0x80;    //  PmnPFS ���C�g�v���e�N�g �ݒ�
			
	PORTH.PMR.BIT.B2 = 1;   // PH2  ���Ӌ@�\�Ƃ��Ďg�p
	PORTH.PMR.BIT.B1 = 1;   // PH1   :
	PORTH.PMR.BIT.B0 = 1;   // PH0   :
	
	T_IRQ_PDR = 0;		// P14 T_IRQ ���̓|�[�g�ɐݒ�
	T_IRQ_PMR = 0;		// P14 �ėp���o�̓|�[�g�Ƃ��Ďg�p
	T_IRQ_PCR = 0;		// P14 ���̓v���A�b�v��R �L��
	
	
	SCI5.SCR.BYTE = 0;	// ����M�֎~ 
	
	SCI5.SIMR1.BIT.IICM =0; // �����������A�N���b�N���������[�h�܂��͊Ȉ�SPI���[�h

	SCI5.SPMR.BIT.SSE = 0;	// SSn#�[�q�@�\�֎~
	SCI5.SPMR.BIT.CTSE =0;  // CTS�@�\�֎~
	SCI5.SPMR.BIT.MSS = 0;  // SMOSIn�[�q�F���M�ASMISOn�[�q�F��M(�}�X�^���[�h)
	SCI5.SPMR.BIT.MFF = 0;  // ���[�h�t�H���g�G���[�Ȃ�
		
      	SCI5.SPMR.BIT.CKPH = 1;  // �N���b�N�x�ꂠ��
	SCI5.SPMR.BIT.CKPOL = 0; // �N���b�N�ɐ����]�Ȃ�
	
	SCI5.SMR.BIT.CM = 1;	//�N���b�N���������[�h�A�܂��͊Ȉ�SPI���[�h�œ���
	SCI5.SMR.BIT.CKS = 0;	// �N���b�N�Z���N�g�r�b�g PCLKB(=32MHz)(n=0)
	
	SCI5.SCMR.BIT.SMIF = 0;	// ��X�}�[�g�J�[�h�C���^�t�F�[�X���[�h(�������������[�h�A�N���b�N���������[�h�A�Ȉ�SPI���[�h�A�Ȉ�I2C���[�h)
	SCI5.SCMR.BIT.SDIR = 1;  //  MSB�t�@�[�X�g�ő���M
	
	//SCI5.BRR = 15;	 // 500Kbps  
	SCI5.BRR = 3;		 // 2000Kbps= 2Mbps

	
	
	IPR(SCI5,RXI5) = 14;		// ��M ���荞�݃��x�� = 14�i15���ō����x��)
	IEN(SCI5,RXI5) = 1;		// ��M���荞�݋���
	
	
	IPR(SCI5,TXI5) = 13;		// ���M ���荞�݃��x�� = 13 �i15���ō����x��)  
	IEN(SCI5,TXI5) = 1;		// ���M���荞�݋���
	
	IPR(SCI5,TEI5) = 13;		// ���M���� ���荞�݃��x�� = 13 �i15���ō����x��)
	IEN(SCI5,TEI5) = 1;		// ���M�������荞�݋���
	
	
	xpt_rcv_cnt = 0;		// ��M�f�[�^��
	
	

}


// XPT2046�ւ̑��M�R�}���h�ݒ�ƍŏ��̑��M�J�n
// 
// 8bit,�f�B�t�@�����V����,�I�[�g�p���[�_�E��
//
void xpt2046_cmd_set_0(void)
{
	xpt_sd_data[0] = 0xdc;	// X���ǂݏo��,8bit,�f�B�t�@�����V����,�I�[�g�p���[�_�E��(�y�����荞�ݗL��)
	xpt_sd_data[1] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	xpt_sd_data[2] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	
	xpt_sd_data[3] = 0x9c;	// Y���ǂݏo��,8bit,�f�B�t�@�����V����,�I�[�g�p���[�_�E��(�y�����荞�ݗL��)
	xpt_sd_data[4] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	xpt_sd_data[5] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	
	xpt_send_num = 6;	// ���M�f�[�^�� (XPT2046)
	xpt_send_pt = 0;	// ���M�f�[�^�̈ʒu�̏�����
  
	sci5_rxi_cnt = 0;	// �f�o�b�N�p
        sci5_tei_cnt = 0;	// �f�o�b�N�p

		
	SCI5.SCR.BYTE = 0xf0;   // ����M����(TE,RE)�Ƒ���M���荞��(TIE,RIE)������
	 			 // �ŏ��̑��M���荞�݂���������B
}


// XPT2046�ւ̑��M�R�}���h�ݒ�ƍŏ��̑��M�J�n
// 
// 12bit,�f�B�t�@�����V����,�p���[�_�E������
//
void xpt2046_cmd_set(void)
{
	xpt_sd_data[0] = 0xd7;	// X���ǂݏo��,12bit,�f�B�t�@�����V����,�p���[�_�E������
	xpt_sd_data[1] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	xpt_sd_data[2] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	
	xpt_sd_data[3] = 0x97;	// Y���ǂݏo��,12bit,�f�B�t�@�����V����,�p���[�_�E������
	xpt_sd_data[4] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	xpt_sd_data[5] = 0x00;	// �_�~�[�f�[�^�@(�Ǐo���̂��߂̃N���b�N�����p)
	
	xpt_send_num = 6;	// ���M�f�[�^�� (XPT2046)
	xpt_send_pt = 0;	// ���M�f�[�^�̈ʒu�̏�����
  
	sci5_rxi_cnt = 0;	// �f�o�b�N�p
        sci5_tei_cnt = 0;	// �f�o�b�N�p

		
	SCI5.SCR.BYTE = 0xf0;   // ����M����(TE,RE)�Ƒ���M���荞��(TIE,RIE)������
	 			 // �ŏ��̑��M���荞�݂���������B
}
