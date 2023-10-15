/* -----------------------------------------------------------------------------
| ��17���N�҂��̂Â��苣�Z���u�d�q��H�g���āv�E��
| �t�@�C���� [ main.c ] 
----------------------------------------------------------------------------- */

/* ---------------------- �w�b�_�t�@�C���̃C���N���[�h ---------------------- */
#include <p18f4620.h>
#include <adc.h>
#include <timers.h>
#include <usart.h>
#include <delays.h>
#include <portb.h>
#include <pwm.h>
#include <math.h>
#include "main.h"
#include "move.h"
#include "common_func.h"
#include "glcdlib_c18_v03.h"

/* ----------------------------- �R���t�B�O�ݒ� ----------------------------- */
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF						/*CONFIG1H*/
#pragma config PWRT = ON, BOREN = OFF, BORV = 0							/*CONFIG2L*/
#pragma config WDT = OFF, WDTPS = 1										/*CONFIG2H*/
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC	/*CONFIG3H*/
#pragma config STVREN = ON, LVP = OFF, XINST = OFF, DEBUG = OFF			/*CONFIG4L*/
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF				/*CONFIG5L*/
#pragma config CPB = OFF, CPD = OFF										/*CONFIG5H*/
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF			/*CONFIG6L*/
#pragma config WRTB = OFF, WRTC = OFF, WRTD = OFF						/*CONFIG6H*/
#pragma config EBTRB = OFF												/*CONFIG7H*/

/* --------------------------- �O���[�o���ϐ��錾�E��` ----------------------- */
unsigned int red_data = 0;						/*��LED�f�[�^�i�[�p*/
unsigned int green_data = 0;						/*��LED�f�[�^�i�[�p*/
unsigned int blue_data = 0;						/*��LED�f�[�^�i�[�p*/
signed int rotary_cnt = 0;						/*���[�^���[SW��]���J�E���g*/
unsigned char sw_cnt = 0;						/*SW1�`���^�����O���m�p*/
unsigned int cnt_10ms = 0;						/*10ms�J�E���^*/
unsigned int cnt_100ms = 0;						/*100ms�J�E���^*/
unsigned int mode = 0;							/*���݃��[�h���*/
unsigned int old_mode = 0;						/*�ߋ����[�h���*/
union flag flg;									/*�e��t���O�Ǘ��p*/
const unsigned int bz_frq = 149;					/*�����u�U�[���g���ݒ�*/

/* -----------------------------------------------------------------------------
/* ���ʊ��荞��
----------------------------------------------------------------------------- */
#pragma interrupt high_timer
void high_timer(void)
{
	/*10ms���荞��*/
	if ((PIR1bits.TMR1IF & PIE1bits.TMR1IE) == 1)
	{
		PIR1bits.TMR1IF = 0;
		WriteTimer1(53036);

		cnt_10ms++;								/*10ms�J�E���^*/
		sw_cnt++;								/*SW1�`���^�����O�h�~�p�J�E���^*/
		flg.ad_check_flag = 1;					/*A/D�ϊ�����p�t���OON*/
		
		if (cnt_10ms >= 10)
		{
			cnt_10ms = 0;
			cnt_100ms++;						/*100ms�J�E���^*/
		}
	}
	
	/*INT0�O�����荞��*/
	if ((INTCONbits.INT0IE & INTCONbits.INT0IF) == 1)
	{
		INTCONbits.INT0IF = 0;

		flg.rotary_l_start_flag = 1;			/*����]���m�t���O�Z�b�g*/

		/*�E��]���o*/
		if ((PORTBbits.RB1 == 0) && (flg.rotary_r_start_flag == 1))
		{
			flg.rotary_r_start_flag = 0;
			flg.rotary_r_fix_flag = 1;
			flg.rotary_l_fix_flag = 0;
		}
	}
	
	/*INT1�O��������*/
	if ((INTCON3bits.INT1E & INTCON3bits.INT1IF) == 1)
	{
		INTCON3bits.INT1IF = 0;

		flg.rotary_r_start_flag = 1;			/*�E��]���m�t���O�Z�b�g*/

		/*����]���o*/
		if ((PORTBbits.RB0 == 0) && (flg.rotary_l_start_flag == 1))
		{
			flg.rotary_l_start_flag = 0;
			flg.rotary_l_fix_flag = 1;
			flg.rotary_r_fix_flag = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* ��ʊ��荞��
----------------------------------------------------------------------------- */
#pragma interruptlow low_timer
void low_timer(void)
{
	unsigned int i;
	static unsigned char row_cnt = 0;
	unsigned int led_out = 0;
	const unsigned int ini_row = 0x0008;

	/*1ms���荞�݁i�_�C�i�~�b�N�_�������j*/
	if ((INTCONbits.TMR0IE & INTCONbits.TMR0IF) == 1)
	{
		INTCONbits.TMR0IF = 0;
		WriteTimer0(55536);

		/*��LED�f�[�^����*/
		led_out |=	(red_data >> row_cnt) & 0x0001;
		/*��LED�f�[�^����*/
		led_out |=	((green_data >> row_cnt) << 1) & 0x0002;
		/*��LED�f�[�^����*/
		led_out |=	((blue_data >> row_cnt) << 2) & 0x0004;
		/*�c�R�����f�[�^����*/
		led_out |=	(ini_row << row_cnt) & 0x1FF8;

		/*���W�X�^�N���A �O�f�[�^�̉f�荞�ݑΉ�*/
		SRCLK = 0;
		RCLK = 0;
		for ( i = 0; i < 13; i++)
		{
			SRCLK = 0;
			SER = 0;
			SRCLK = 1;
		}
		RCLK = 1;

		/*�V�t�g���W�X�^�f�[�^���M*/
		SRCLK = 0;
		RCLK = 0;
		for (i = 0; i < 13; i++)
		{
			SRCLK = 0;
			SER = led_out & 0x01;
			led_out >>= 1;
			SRCLK = 1;
		}
		RCLK = 1;

		/*���̍s�֐i�߂�*/
		row_cnt++;
		if (row_cnt > 9)
		{
			row_cnt = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* ���C���֐�
----------------------------------------------------------------------------- */
void main(void)
{
	init();										/*PIC�����ݒ�*/
	flg.all_flag = 0;							/*�t���O�̃N���A*/

	/*�I�[�v�j���O���*/
	glcd_message(1, 5);

	while (1)
	{
		/*���샂�[�h�E�n�[�h�`�F�b�N���[�h����*/
		if (SW1 == 1)
		{
			Move();								/*���샂�[�h*/
		}
		else
		{
			HardCheck();						/*�n�[�h�`�F�b�N���[�h*/
		}
	}
}

/* -----------------------------------------------------------------------------
/* ���샂�[�h��������
----------------------------------------------------------------------------- */
void Move(void)
{
	/*�I�[�v�j���O���*/
	glcd_message(3, 7);
	/*wait 1s*/
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);

	old_mode = 5;
	PIE1bits.TMR1IE = 1;						/*TIMER1���荞�݋���*/

	while (1)
	{
		/*���[�h����*/
		switch (mode)
		{
            case 0:
                Move_Mode1();					/*���샂�[�h�P*/
                break;
            case 1:
                Move_Mode2();					/*���샂�[�h�Q*/
                break;
            case 2:
                Move_Mode3();					/*���샂�[�h�R*/
                break;
            case 3:
                Move_Mode4();					/*���샂�[�h�S*/
                break;
            case 4:
                Move_Mode5();					/*���샂�[�h�T*/
                break;
            default:
                /*�z��O*/
                break;
		}

		old_mode = mode;						/*���[�h��ԋL��*/
		sw1_check();							/*SW1��Ԋm�F*/

		/*���[�h�Ǘ��ϐ��̕␳����*/
		if (mode > 4)
		{
			mode = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h��������
----------------------------------------------------------------------------- */
void HardCheck(void)
{
	/*�I�[�v�j���O���*/
	glcd_message(3, 6);
	/*wait 1s*/
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	
	while (SW1 == 0)							/*SW1������Ă���ԃ��[�v*/
	{
	}
	Delay10KTCYx(200);							/*�`���^�����O�h�~�p*/

	old_mode = 4;								/*����N����LCD�\�������p*/
	PIE1bits.TMR1IE = 1;						/*TIMER1���荞�݋���*/

	while (1)
	{
		/*���[�h����*/
		switch (mode)
		{
			case 0:
				HardCheck_Mode1();				/*�n�[�h�`�F�b�N���[�h�P*/
				break;
			case 1:
				HardCheck_Mode2();				/*�n�[�h�`�F�b�N���[�h�Q*/
				break;
			case 2:
				HardCheck_Mode3();				/*�n�[�h�`�F�b�N���[�h�R*/
				break;
			case 3:
				HardCheck_Mode4();				/*�n�[�h�`�F�b�N���[�h�S*/
				break;
			case 4:
				HardCheck_Mode5();				/*�n�[�h�`�F�b�N���[�h�T*/
				break;
			default:
				/*�z��O*/
				break;
		}

		old_mode = mode;						/*���[�h��ԋL��*/
		sw1_check();							/*SW1��Ԋm�F*/
		
		/*���[�h�Ǘ��ϐ��̕␳����*/
		if (mode > 4)
		{
			mode = 0;
		}

	}
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h1�@LED�`�F�b�N���[�h
/* �o�[LED�̓_����SW2(�g�O���X�C�b�`)�̌����ɍ��킹�Ĉړ�����
/* SW2�㑤�F������@�@SW2�����F�と��
----------------------------------------------------------------------------- */
void HardCheck_Mode1(void)
{
	const unsigned int check_data = 0x0001;		/*�\���`�F�b�N�p�f�[�^*/
	static signed int shift = 0;				/*�\���V�t�g�ʒu*/

	/*�����[�h���ڍs���A������Ԃ�蓮��J�n������*/
	if (mode != old_mode)
	{
		mode_init();
		/* HARD_CHECK1�\�� */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'1');
		/*�_���J�n�ʒu�̏�����*/
		if (SW2 == 1)
		{
			shift = 11;
		}
		else
		{
			shift = 0;
		}
	}

	/*500ms�P�ʂŕ\���ړ�*/
	if (cnt_100ms >= 5) 
    {
		cnt_100ms = 0;

		barled_clear();							/*�o�[LED�S����*/

		/*��\��*/
		if (shift <= 9)
		{
			blue_data = check_data << shift;
		}
		/*�Η�\��*/
		if ((shift >= 1) && (shift <= 10))
		{
			green_data = check_data << (shift - 1);
		}
		/*�ԗ�\��*/
		if ((shift >= 2) && (shift <= 11))
		{
			red_data = check_data << (shift - 2);
		}

		/*SW2�ɂ��ړ������؂�ւ�*/
		/*SW2�㑤�F������*/
		if (SW2 == 1)
		{
			shift--;
			if (shift < 0)
			{
				shift = 12;
			}
		}
		/*SW2�����F�と��*/
		else
		{
			shift++;
			if (shift > 12)
			{
				shift = 0;
			}
		}
	}
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h2�@���[�^���[�`�F�b�N���[�h
/* SW3(���[�^���[�G���R�[�_)���񂷂�GLCD�o�[�ʒu���ω�
----------------------------------------------------------------------------- */
void HardCheck_Mode2(void)
{
	unsigned char bar_pos = 0;

	/*�����[�h���ڍs���A������Ԃ�蓮��J�n������*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK2�\�� */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'2');
		/* �w����ʕ\�� */
		glcd_write_num(4, 0,'1');
		glcd_write_num(4, 5,'5');
		glcd_write_num(4, 60,'0');
		glcd_write_num(4, 115,'1');
		glcd_write_num(4, 120,'5');
	}

	/*�E��]�ɂ��C���N�������g*/
	if (flg.rotary_r_fix_flag == 1)
	{
		flg.rotary_r_fix_flag = 0;
		if (rotary_cnt < 30)					/*�G���R�[�_�J�E���g����ݒ�*/
		{
			rotary_cnt+=2;
		}
	}

	/*����]�ɂ��f�N�������g*/
	if (flg.rotary_l_fix_flag == 1)
	{
		flg.rotary_l_fix_flag = 0;
		if (rotary_cnt > -30)					/*�G���R�[�_�J�E���g�����ݒ�*/
		{
			rotary_cnt-=2;
		}
	}
	
	bar_pos = (rotary_cnt + 31) << 1;
	/*GLCD�փo�[�ʒu�o��*/
	glcd_bar(5, bar_pos, bar_pos+3);

	ROTARY_LED = 1;								/*���[�^���[SW LED�̓_��*/
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h3�@A/D�`�F�b�N���[�h
/* VR2���񂷂�LCD�ɕ\�����Ă���A/D�l���ω�����
----------------------------------------------------------------------------- */
void HardCheck_Mode3(void)
{
	unsigned int ad_data = 0;					/*A/D�f�[�^�i�[*/

	/*�����[�h���ڍs���A������Ԃ�蓮��J�n������*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK3�\�� */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'3');
		/* �w����ʕ\�� */
		glcd_write_num(4, 0,'0');
		glcd_write_num(4, 22,'1');
		glcd_write_num(4, 48,'2');
		glcd_write_num(4, 74,'3');
		glcd_write_num(4, 99,'4');
		glcd_write_num(4, 120,'5');
		
		/*�A�i���O�`�����l����AN0�ɐݒ�*/
		SetChanADC(ADC_CH0);

	}

	/*10ms�Ԋu�ŕ\���X�V*/
	if (flg.ad_check_flag == 1)
	{
		flg.ad_check_flag = 0;
		
		/*A/D�l�擾*/
		ConvertADC();
		while (BusyADC() == 1)
		{
		}
		ad_data = ReadADC();
		ad_data >>= 3;							/*0-127�ɕϊ�*/			

		/*GLCD��A/D�l�o��*/
		glcd_bar(5, 0, ad_data);
	}
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h4�@�u�U�[�`�F�b�N���[�h
/* SW3(���[�^�������{�^���X�C�b�`)�Ńu�U�[��ON/OFF�𐧌�
----------------------------------------------------------------------------- */
void HardCheck_Mode4(void)
{
	/*�����[�h���ڍs���A������Ԃ�蓮��J�n������*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK4�\�� */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'4');
	}

	/*SW3(���[�^���[�X�C�b�`)�������������A�u�U�[��炷*/
	if (SW3 == 1)
	{
		BZ_CON = BZ_OFF;
	}
	else
	{
		BZ_CON = BZ_ON;
	}
	ROTARY_LED = 1;								/*���[�^���[SW LED�̓_��*/
}

/* -----------------------------------------------------------------------------
/* �n�[�h�`�F�b�N���[�h5�@�����x�Z���T�`�F�b�N���[�h
/* ��̌X���ɍ��킹�O���t�B�b�NLCD�̊e���̃o�[�\�����ω�����
----------------------------------------------------------------------------- */
void HardCheck_Mode5(void)
{
	unsigned int ax_ad = 0;
	unsigned int ax_pos = 0;
	static unsigned char ch = 1;

	/*�����[�h���ڍs���A������Ԃ�蓮��J�n������*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK5�\�� */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'5');
		/* �w����ʕ\�� */
		glcd_message(2, 2);
		glcd_message(4, 3);
		glcd_message(6, 4);
		glcd_write_num(1, 27,'-');
		glcd_write_num(1, 60,'0');
		glcd_write_num(1, 94,'1');	
	}

	/*10ms�Ԋu�ŕ\���X�V*/
	if (flg.ad_check_flag == 1)
	{
		flg.ad_check_flag = 0;
		
		SetChanADC(0x87 | ((ch << 3) & 0x78));
		
		/*A/D�l�擾*/
		ConvertADC();
		while (BusyADC() == 1)
		{
		}
		ax_ad = ReadADC();
		ax_ad >>= 2;							/*0-255�ɕϊ�*/			
		ax_pos = ax_ad - 21;					/*1.65v���̃I�t�Z�b�g*/
		
		/*GLCD�֎��ʒu�o��*/
		if (ax_pos < 64)
		{
			glcd_bar(((ch << 1) + 1), ax_pos, 64);
		}
		else
		{
			glcd_bar(((ch << 1) + 1), 63, ax_pos);
		}
		
		ch++;									/*AD�`�����l���ؑ�*/
		
		if (ch > 3)
		{
			ch = 1;
		}
	}
}

/* -----------------------------------------------------------------------------
/* PIC�����ݒ�
----------------------------------------------------------------------------- */
void init(void)
{
	ADCON1 = 0x0B;								/*AN0-3���A�i���O�|�[�g��*/
	
	/*�S�|�[�g�o�͂�L��*/
	LATA = 0x00;
	LATB = 0x00;
	LATC = 0x00;
	LATD = 0x00;
	LATE = 0x00;
	
	/*�|�[�g���o�͐ݒ�*/
	TRISA = 0x0F;								/*RA�|�[�g*/
	TRISB = 0x1F;								/*RB�|�[�g*/
	TRISC = 0xB8;								/*RC�|�[�g*/
	TRISD = 0x00;								/*RD�|�[�g*/
	TRISE = 0x00;								/*RE�|�[�g*/

	/*RC4�|�[�g�듮��h�~����*/
	LATDbits.LATD2 = 1;
	TRISDbits.TRISD2 = 0;
	
	/*TIMER0�����ݒ�*/
	OpenTimer0(
			TIMER_INT_ON		&
			T0_16BIT			&
			T0_SOURCE_INT		&
			T0_EDGE_FALL		&
			T0_PS_1_1);
	INTCON2bits.TMR0IP = 0;
	
	/*TIMER1�����ݒ�*/
	OpenTimer1(
			TIMER_INT_OFF		&
			T1_16BIT_RW			&
			T1_SOURCE_INT		&
			T1_OSC1EN_OFF		&
			T1_SYNC_EXT_OFF		&
			T1_PS_1_8);
	IPR1bits.TMR1IP = 1;
	
	/*TIMER2�����ݒ�*/
	OpenTimer2(
			TIMER_INT_OFF		&
			T2_POST_1_1			&
			T2_PS_1_16);
	
	/*INT0�����ݒ�*/
	OpenRB0INT(
			PORTB_CHANGE_INT_ON	&
			PORTB_PULLUPS_OFF	&
			FALLING_EDGE_INT);
	
	/*INT1�����ݒ�*/
	OpenRB1INT(
			PORTB_CHANGE_INT_ON	&
			PORTB_PULLUPS_OFF	&
			FALLING_EDGE_INT);
	INTCON3bits.INT1IP = 1;
	
	/*ADC�����ݒ�*/
	OpenADC(
			ADC_FOSC_64			&
			ADC_RIGHT_JUST		&
			ADC_8_TAD,
			ADC_CH0				&
			ADC_INT_OFF			&
			ADC_REF_VDD_VSS,
			ADC_4ANA);
	
	/*USART�����ݒ�*/
	OpenUSART(
			USART_TX_INT_OFF	&
			USART_RX_INT_OFF	&
			USART_BRGH_HIGH		&
			USART_CONT_RX		&
			USART_EIGHT_BIT		&
			USART_ASYNCH_MODE,
			21);

	/*CCP1�����ݒ�*/
	OpenPWM1(bz_frq);
	SetDCPWM1((bz_frq >> 1) & 0x3FF);
	BZ_CON = BZ_OFF;							/*�u�U�[OFF*/
	
	/*�����ݗD�揇�ʋ���*/
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	/*LCD������*/
	GLCD_Init();
	SRCLR = 1;									/*�V�t�g���W�X�^ CLR = 1*/
}

/* --------------------------- ���ʊ��荞�݃x�N�^�̋L�q --------------------------- */
#pragma code interrupt_high = 0x08				/*�J�n�Ԓn�w��*/
void interrupt_high_timer(void)
{
	_asm goto high_timer _endasm				/*���荞�݊֐��փW�����v*/
}

/* --------------------------- ��ʊ��荞�݃x�N�^�̋L�q --------------------------- */
#pragma code interrupt_low = 0x18				/*�J�n�Ԓn�w��*/
void interrupt_low_timer(void)
{
	_asm goto low_timer _endasm					/*���荞�݊֐��փW�����v*/
}
