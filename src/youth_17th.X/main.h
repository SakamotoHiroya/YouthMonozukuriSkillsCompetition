/* -----------------------------------------------------------------------------
| ��17���N�҂��̂Â��苣�Z���u�d�q��H�g���āv�E��
| �t�@�C���� [ main.h ] 
----------------------------------------------------------------------------- */

#ifndef MAIN_H
#define	MAIN_H

/* --------------------------------- ��`�� --------------------------------- */
#define	SER			LATAbits.LATA4				/*�V�t�g���W�X�^ SER*/
#define	SRCLK		LATCbits.LATC1				/*�V�t�g���W�X�^ SRCLK*/
#define	RCLK		LATCbits.LATC0				/*�V�t�g���W�X�^ RCLK*/
#define	SRCLR		LATAbits.LATA5				/*�V�t�g���W�X�^ SRCLR*/
#define	SW1			PORTBbits.RB4				/*SW1�i�v�b�V���X�C�b�`�j*/
#define	SW2			PORTBbits.RB3				/*SW2�i�g�O���X�C�b�`�j*/
#define	SW3			PORTBbits.RB2				/*SW3�i���[�^���[�G���R�[�_�j*/
#define	ROTARY_LED	LATBbits.LATB5				/*���[�^���[�G���R�[�_LED*/
#define	BZ_CON		TRISCbits.TRISC2			/*�u�U�[�R���g���[��*/
#define BZ_ON		0							/*�u�U�[ON*/
#define BZ_OFF		1							/*�u�U�[OFF*/

union flag {
	unsigned char all_flag;						/*�S�t���O*/
	struct flag_bit {
		unsigned char ad_check_flag : 1;		/*A/D�ϊ�����p�t���O*/
		unsigned char rotary_l_start_flag : 1;	/*���[�^���[SW����]�J�n�t���O*/
		unsigned char rotary_r_start_flag : 1;	/*���[�^���[SW�E��]�J�n�t���O*/
		unsigned char rotary_l_fix_flag : 1;	/*���[�^���[SW����]�m��t���O*/
		unsigned char rotary_r_fix_flag : 1;	/*���[�^���[SW�E��]�m��t���O*/
		unsigned char sw1_flag : 1;				/*SW1�������m�t���O*/
		unsigned char sw2_flag : 1;				/*SW2�㉺���m�t���O*/
		unsigned char sw3_flag : 1;				/*SW3�������m�t���O*/
	};
};

/* -------------------------------- �֐��錾 -------------------------------- */
void high_timer(void);							/*���ʊ��荞��*/
void low_timer(void);							/*��ʊ��荞��*/
void init(void);								/*PIC�����ݒ�*/
void HardCheck(void);							/*�n�[�h�`�F�b�N���[�h��������*/
void HardCheck_Mode1(void);						/*�n�[�h�`�F�b�N���[�h�P*/
void HardCheck_Mode2(void);						/*�n�[�h�`�F�b�N���[�h�Q*/
void HardCheck_Mode3(void);						/*�n�[�h�`�F�b�N���[�h�R*/
void HardCheck_Mode4(void);						/*�n�[�h�`�F�b�N���[�h�S*/
void HardCheck_Mode5(void);						/*�n�[�h�`�F�b�N���[�h�T*/
void Move(void);
 
#endif	/* MAIN_H */

