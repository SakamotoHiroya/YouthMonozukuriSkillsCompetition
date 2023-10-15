/* -----------------------------------------------------------------------------
| 第17回若年者ものづくり競技大会「電子回路組立て」職種
| ファイル名 [ main.c ] 
----------------------------------------------------------------------------- */

/* ---------------------- ヘッダファイルのインクルード ---------------------- */
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

/* ----------------------------- コンフィグ設定 ----------------------------- */
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

/* --------------------------- グローバル変数宣言・定義 ----------------------- */
unsigned int red_data = 0;						/*赤LEDデータ格納用*/
unsigned int green_data = 0;						/*緑LEDデータ格納用*/
unsigned int blue_data = 0;						/*青LEDデータ格納用*/
signed int rotary_cnt = 0;						/*ロータリーSW回転数カウント*/
unsigned char sw_cnt = 0;						/*SW1チャタリング検知用*/
unsigned int cnt_10ms = 0;						/*10msカウンタ*/
unsigned int cnt_100ms = 0;						/*100msカウンタ*/
unsigned int mode = 0;							/*現在モード状態*/
unsigned int old_mode = 0;						/*過去モード状態*/
union flag flg;									/*各種フラグ管理用*/
const unsigned int bz_frq = 149;					/*初期ブザー周波数設定*/

/* -----------------------------------------------------------------------------
/* 高位割り込み
----------------------------------------------------------------------------- */
#pragma interrupt high_timer
void high_timer(void)
{
	/*10ms割り込み*/
	if ((PIR1bits.TMR1IF & PIE1bits.TMR1IE) == 1)
	{
		PIR1bits.TMR1IF = 0;
		WriteTimer1(53036);

		cnt_10ms++;								/*10msカウンタ*/
		sw_cnt++;								/*SW1チャタリング防止用カウンタ*/
		flg.ad_check_flag = 1;					/*A/D変換動作用フラグON*/
		
		if (cnt_10ms >= 10)
		{
			cnt_10ms = 0;
			cnt_100ms++;						/*100msカウンタ*/
		}
	}
	
	/*INT0外部割り込み*/
	if ((INTCONbits.INT0IE & INTCONbits.INT0IF) == 1)
	{
		INTCONbits.INT0IF = 0;

		flg.rotary_l_start_flag = 1;			/*左回転検知フラグセット*/

		/*右回転検出*/
		if ((PORTBbits.RB1 == 0) && (flg.rotary_r_start_flag == 1))
		{
			flg.rotary_r_start_flag = 0;
			flg.rotary_r_fix_flag = 1;
			flg.rotary_l_fix_flag = 0;
		}
	}
	
	/*INT1外部割込み*/
	if ((INTCON3bits.INT1E & INTCON3bits.INT1IF) == 1)
	{
		INTCON3bits.INT1IF = 0;

		flg.rotary_r_start_flag = 1;			/*右回転検知フラグセット*/

		/*左回転検出*/
		if ((PORTBbits.RB0 == 0) && (flg.rotary_l_start_flag == 1))
		{
			flg.rotary_l_start_flag = 0;
			flg.rotary_l_fix_flag = 1;
			flg.rotary_r_fix_flag = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* 低位割り込み
----------------------------------------------------------------------------- */
#pragma interruptlow low_timer
void low_timer(void)
{
	unsigned int i;
	static unsigned char row_cnt = 0;
	unsigned int led_out = 0;
	const unsigned int ini_row = 0x0008;

	/*1ms割り込み（ダイナミック点灯処理）*/
	if ((INTCONbits.TMR0IE & INTCONbits.TMR0IF) == 1)
	{
		INTCONbits.TMR0IF = 0;
		WriteTimer0(55536);

		/*赤LEDデータ結合*/
		led_out |=	(red_data >> row_cnt) & 0x0001;
		/*緑LEDデータ結合*/
		led_out |=	((green_data >> row_cnt) << 1) & 0x0002;
		/*青LEDデータ結合*/
		led_out |=	((blue_data >> row_cnt) << 2) & 0x0004;
		/*縦コモンデータ結合*/
		led_out |=	(ini_row << row_cnt) & 0x1FF8;

		/*レジスタクリア 前データの映り込み対応*/
		SRCLK = 0;
		RCLK = 0;
		for ( i = 0; i < 13; i++)
		{
			SRCLK = 0;
			SER = 0;
			SRCLK = 1;
		}
		RCLK = 1;

		/*シフトレジスタデータ送信*/
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

		/*次の行へ進める*/
		row_cnt++;
		if (row_cnt > 9)
		{
			row_cnt = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* メイン関数
----------------------------------------------------------------------------- */
void main(void)
{
	init();										/*PIC初期設定*/
	flg.all_flag = 0;							/*フラグのクリア*/

	/*オープニング画面*/
	glcd_message(1, 5);

	while (1)
	{
		/*動作モード・ハードチェックモード分岐*/
		if (SW1 == 1)
		{
			Move();								/*動作モード*/
		}
		else
		{
			HardCheck();						/*ハードチェックモード*/
		}
	}
}

/* -----------------------------------------------------------------------------
/* 動作モード処理分岐
----------------------------------------------------------------------------- */
void Move(void)
{
	/*オープニング画面*/
	glcd_message(3, 7);
	/*wait 1s*/
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);

	old_mode = 5;
	PIE1bits.TMR1IE = 1;						/*TIMER1割り込み許可*/

	while (1)
	{
		/*モード分岐*/
		switch (mode)
		{
            case 0:
                Move_Mode1();					/*動作モード１*/
                break;
            case 1:
                Move_Mode2();					/*動作モード２*/
                break;
            case 2:
                Move_Mode3();					/*動作モード３*/
                break;
            case 3:
                Move_Mode4();					/*動作モード４*/
                break;
            case 4:
                Move_Mode5();					/*動作モード５*/
                break;
            default:
                /*想定外*/
                break;
		}

		old_mode = mode;						/*モード状態記憶*/
		sw1_check();							/*SW1状態確認*/

		/*モード管理変数の補正処理*/
		if (mode > 4)
		{
			mode = 0;
		}
	}
}

/* -----------------------------------------------------------------------------
/* ハードチェックモード処理分岐
----------------------------------------------------------------------------- */
void HardCheck(void)
{
	/*オープニング画面*/
	glcd_message(3, 6);
	/*wait 1s*/
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	Delay10KTCYx(250);
	
	while (SW1 == 0)							/*SW1押されている間ループ*/
	{
	}
	Delay10KTCYx(200);							/*チャタリング防止用*/

	old_mode = 4;								/*初回起動時LCD表示処理用*/
	PIE1bits.TMR1IE = 1;						/*TIMER1割り込み許可*/

	while (1)
	{
		/*モード分岐*/
		switch (mode)
		{
			case 0:
				HardCheck_Mode1();				/*ハードチェックモード１*/
				break;
			case 1:
				HardCheck_Mode2();				/*ハードチェックモード２*/
				break;
			case 2:
				HardCheck_Mode3();				/*ハードチェックモード３*/
				break;
			case 3:
				HardCheck_Mode4();				/*ハードチェックモード４*/
				break;
			case 4:
				HardCheck_Mode5();				/*ハードチェックモード５*/
				break;
			default:
				/*想定外*/
				break;
		}

		old_mode = mode;						/*モード状態記憶*/
		sw1_check();							/*SW1状態確認*/
		
		/*モード管理変数の補正処理*/
		if (mode > 4)
		{
			mode = 0;
		}

	}
}

/* -----------------------------------------------------------------------------
/* ハードチェックモード1　LEDチェックモード
/* バーLEDの点灯列がSW2(トグルスイッチ)の向きに合わせて移動する
/* SW2上側：下→上　　SW2下側：上→下
----------------------------------------------------------------------------- */
void HardCheck_Mode1(void)
{
	const unsigned int check_data = 0x0001;		/*表示チェック用データ*/
	static signed int shift = 0;				/*表示シフト位置*/

	/*他モードより移行時、初期状態より動作開始させる*/
	if (mode != old_mode)
	{
		mode_init();
		/* HARD_CHECK1表示 */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'1');
		/*点灯開始位置の初期化*/
		if (SW2 == 1)
		{
			shift = 11;
		}
		else
		{
			shift = 0;
		}
	}

	/*500ms単位で表示移動*/
	if (cnt_100ms >= 5) 
    {
		cnt_100ms = 0;

		barled_clear();							/*バーLED全消去*/

		/*青列表示*/
		if (shift <= 9)
		{
			blue_data = check_data << shift;
		}
		/*緑列表示*/
		if ((shift >= 1) && (shift <= 10))
		{
			green_data = check_data << (shift - 1);
		}
		/*赤列表示*/
		if ((shift >= 2) && (shift <= 11))
		{
			red_data = check_data << (shift - 2);
		}

		/*SW2による移動方向切り替え*/
		/*SW2上側：下→上*/
		if (SW2 == 1)
		{
			shift--;
			if (shift < 0)
			{
				shift = 12;
			}
		}
		/*SW2下側：上→下*/
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
/* ハードチェックモード2　ロータリーチェックモード
/* SW3(ロータリーエンコーダ)を回すとGLCDバー位置が変化
----------------------------------------------------------------------------- */
void HardCheck_Mode2(void)
{
	unsigned char bar_pos = 0;

	/*他モードより移行時、初期状態より動作開始させる*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK2表示 */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'2');
		/* 指示画面表示 */
		glcd_write_num(4, 0,'1');
		glcd_write_num(4, 5,'5');
		glcd_write_num(4, 60,'0');
		glcd_write_num(4, 115,'1');
		glcd_write_num(4, 120,'5');
	}

	/*右回転によるインクリメント*/
	if (flg.rotary_r_fix_flag == 1)
	{
		flg.rotary_r_fix_flag = 0;
		if (rotary_cnt < 30)					/*エンコーダカウント上限設定*/
		{
			rotary_cnt+=2;
		}
	}

	/*左回転によるデクリメント*/
	if (flg.rotary_l_fix_flag == 1)
	{
		flg.rotary_l_fix_flag = 0;
		if (rotary_cnt > -30)					/*エンコーダカウント下限設定*/
		{
			rotary_cnt-=2;
		}
	}
	
	bar_pos = (rotary_cnt + 31) << 1;
	/*GLCDへバー位置出力*/
	glcd_bar(5, bar_pos, bar_pos+3);

	ROTARY_LED = 1;								/*ロータリーSW LEDの点灯*/
}

/* -----------------------------------------------------------------------------
/* ハードチェックモード3　A/Dチェックモード
/* VR2を回すとLCDに表示しているA/D値が変化する
----------------------------------------------------------------------------- */
void HardCheck_Mode3(void)
{
	unsigned int ad_data = 0;					/*A/Dデータ格納*/

	/*他モードより移行時、初期状態より動作開始させる*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK3表示 */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'3');
		/* 指示画面表示 */
		glcd_write_num(4, 0,'0');
		glcd_write_num(4, 22,'1');
		glcd_write_num(4, 48,'2');
		glcd_write_num(4, 74,'3');
		glcd_write_num(4, 99,'4');
		glcd_write_num(4, 120,'5');
		
		/*アナログチャンネルをAN0に設定*/
		SetChanADC(ADC_CH0);

	}

	/*10ms間隔で表示更新*/
	if (flg.ad_check_flag == 1)
	{
		flg.ad_check_flag = 0;
		
		/*A/D値取得*/
		ConvertADC();
		while (BusyADC() == 1)
		{
		}
		ad_data = ReadADC();
		ad_data >>= 3;							/*0-127に変換*/			

		/*GLCDへA/D値出力*/
		glcd_bar(5, 0, ad_data);
	}
}

/* -----------------------------------------------------------------------------
/* ハードチェックモード4　ブザーチェックモード
/* SW3(ロータリ押しボタンスイッチ)でブザーのON/OFFを制御
----------------------------------------------------------------------------- */
void HardCheck_Mode4(void)
{
	/*他モードより移行時、初期状態より動作開始させる*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK4表示 */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'4');
	}

	/*SW3(ロータリースイッチ)を押下した時、ブザーを鳴らす*/
	if (SW3 == 1)
	{
		BZ_CON = BZ_OFF;
	}
	else
	{
		BZ_CON = BZ_ON;
	}
	ROTARY_LED = 1;								/*ロータリーSW LEDの点灯*/
}

/* -----------------------------------------------------------------------------
/* ハードチェックモード5　加速度センサチェックモード
/* 基板の傾きに合わせグラフィックLCDの各軸のバー表示が変化する
----------------------------------------------------------------------------- */
void HardCheck_Mode5(void)
{
	unsigned int ax_ad = 0;
	unsigned int ax_pos = 0;
	static unsigned char ch = 1;

	/*他モードより移行時、初期状態より動作開始させる*/
	if (old_mode != mode)
	{
		mode_init();
		/* HARD_CHECK5表示 */
		glcd_message(0, 0);
		glcd_write_num(0, 71,'5');
		/* 指示画面表示 */
		glcd_message(2, 2);
		glcd_message(4, 3);
		glcd_message(6, 4);
		glcd_write_num(1, 27,'-');
		glcd_write_num(1, 60,'0');
		glcd_write_num(1, 94,'1');	
	}

	/*10ms間隔で表示更新*/
	if (flg.ad_check_flag == 1)
	{
		flg.ad_check_flag = 0;
		
		SetChanADC(0x87 | ((ch << 3) & 0x78));
		
		/*A/D値取得*/
		ConvertADC();
		while (BusyADC() == 1)
		{
		}
		ax_ad = ReadADC();
		ax_ad >>= 2;							/*0-255に変換*/			
		ax_pos = ax_ad - 21;					/*1.65v分のオフセット*/
		
		/*GLCDへ軸位置出力*/
		if (ax_pos < 64)
		{
			glcd_bar(((ch << 1) + 1), ax_pos, 64);
		}
		else
		{
			glcd_bar(((ch << 1) + 1), 63, ax_pos);
		}
		
		ch++;									/*ADチャンネル切替*/
		
		if (ch > 3)
		{
			ch = 1;
		}
	}
}

/* -----------------------------------------------------------------------------
/* PIC初期設定
----------------------------------------------------------------------------- */
void init(void)
{
	ADCON1 = 0x0B;								/*AN0-3をアナログポートに*/
	
	/*全ポート出力をLに*/
	LATA = 0x00;
	LATB = 0x00;
	LATC = 0x00;
	LATD = 0x00;
	LATE = 0x00;
	
	/*ポート入出力設定*/
	TRISA = 0x0F;								/*RAポート*/
	TRISB = 0x1F;								/*RBポート*/
	TRISC = 0xB8;								/*RCポート*/
	TRISD = 0x00;								/*RDポート*/
	TRISE = 0x00;								/*REポート*/

	/*RC4ポート誤動作防止処理*/
	LATDbits.LATD2 = 1;
	TRISDbits.TRISD2 = 0;
	
	/*TIMER0初期設定*/
	OpenTimer0(
			TIMER_INT_ON		&
			T0_16BIT			&
			T0_SOURCE_INT		&
			T0_EDGE_FALL		&
			T0_PS_1_1);
	INTCON2bits.TMR0IP = 0;
	
	/*TIMER1初期設定*/
	OpenTimer1(
			TIMER_INT_OFF		&
			T1_16BIT_RW			&
			T1_SOURCE_INT		&
			T1_OSC1EN_OFF		&
			T1_SYNC_EXT_OFF		&
			T1_PS_1_8);
	IPR1bits.TMR1IP = 1;
	
	/*TIMER2初期設定*/
	OpenTimer2(
			TIMER_INT_OFF		&
			T2_POST_1_1			&
			T2_PS_1_16);
	
	/*INT0初期設定*/
	OpenRB0INT(
			PORTB_CHANGE_INT_ON	&
			PORTB_PULLUPS_OFF	&
			FALLING_EDGE_INT);
	
	/*INT1初期設定*/
	OpenRB1INT(
			PORTB_CHANGE_INT_ON	&
			PORTB_PULLUPS_OFF	&
			FALLING_EDGE_INT);
	INTCON3bits.INT1IP = 1;
	
	/*ADC初期設定*/
	OpenADC(
			ADC_FOSC_64			&
			ADC_RIGHT_JUST		&
			ADC_8_TAD,
			ADC_CH0				&
			ADC_INT_OFF			&
			ADC_REF_VDD_VSS,
			ADC_4ANA);
	
	/*USART初期設定*/
	OpenUSART(
			USART_TX_INT_OFF	&
			USART_RX_INT_OFF	&
			USART_BRGH_HIGH		&
			USART_CONT_RX		&
			USART_EIGHT_BIT		&
			USART_ASYNCH_MODE,
			21);

	/*CCP1初期設定*/
	OpenPWM1(bz_frq);
	SetDCPWM1((bz_frq >> 1) & 0x3FF);
	BZ_CON = BZ_OFF;							/*ブザーOFF*/
	
	/*割込み優先順位許可*/
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	/*LCD初期化*/
	GLCD_Init();
	SRCLR = 1;									/*シフトレジスタ CLR = 1*/
}

/* --------------------------- 高位割り込みベクタの記述 --------------------------- */
#pragma code interrupt_high = 0x08				/*開始番地指定*/
void interrupt_high_timer(void)
{
	_asm goto high_timer _endasm				/*割り込み関数へジャンプ*/
}

/* --------------------------- 低位割り込みベクタの記述 --------------------------- */
#pragma code interrupt_low = 0x18				/*開始番地指定*/
void interrupt_low_timer(void)
{
	_asm goto low_timer _endasm					/*割り込み関数へジャンプ*/
}
