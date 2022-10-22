#include	<machine.h>
#include	 "iodefine.h"
#include	 "misratypes.h"
#include	"delay.h"
#include	"sci_spi.h"

void clear_module_stop(void);

void main(void)
{
	
	clear_module_stop();	//  ���W���[���X�g�b�v�̉���
	
	initSCI_5();		// SCI5(�Ȉ�SPI)  �����ݒ�
	
	while(1){
	
	   delay_msec(1);		// 1msec �҂�
		
	   xpt2046_cmd_set();     // �^�b�`�R���g���[�� XPT2046�Ƒ���M�J�n  (2Mbps��25[usec]������ )
	   
	   IEN(SCI5,TXI5) = 1;    // ���M���荞�݋���
     
	     
	}
	
}





// ���W���[���X�g�b�v�̉���
//  �V���A���R�~���j�P�[�V�����C���^�t�F�[�X5(SCI5)(�^�b�`�R���g���[�� XPT2046 �Ƃ̒ʐM�p)(�Ȉ�SPI�ʐM)
//
void clear_module_stop(void)
{
	SYSTEM.PRCR.WORD = 0xA50F;	// �N���b�N�����A����d�͒ጸ�@�\�֘A���W�X�^�̏������݋���	

	MSTP(SCI5) = 0;			// SCI5    ���W���[���X�g�b�v�̉���:

	SYSTEM.PRCR.WORD = 0xA500;	// �N���b�N�����A����d�͒ጸ�@�\�֘A���W�X�^�������݋֎~
}

