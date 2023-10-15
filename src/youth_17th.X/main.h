/* -----------------------------------------------------------------------------
| 第17回若年者ものづくり競技大会「電子回路組立て」職種
| ファイル名 [ main.h ] 
----------------------------------------------------------------------------- */

#ifndef MAIN_H
#define	MAIN_H

/* --------------------------------- 定義文 --------------------------------- */
#define	SER			LATAbits.LATA4				/*シフトレジスタ SER*/
#define	SRCLK		LATCbits.LATC1				/*シフトレジスタ SRCLK*/
#define	RCLK		LATCbits.LATC0				/*シフトレジスタ RCLK*/
#define	SRCLR		LATAbits.LATA5				/*シフトレジスタ SRCLR*/
#define	SW1			PORTBbits.RB4				/*SW1（プッシュスイッチ）*/
#define	SW2			PORTBbits.RB3				/*SW2（トグルスイッチ）*/
#define	SW3			PORTBbits.RB2				/*SW3（ロータリーエンコーダ）*/
#define	ROTARY_LED	LATBbits.LATB5				/*ロータリーエンコーダLED*/
#define	BZ_CON		TRISCbits.TRISC2			/*ブザーコントロール*/
#define BZ_ON		0							/*ブザーON*/
#define BZ_OFF		1							/*ブザーOFF*/

union flag {
	unsigned char all_flag;						/*全フラグ*/
	struct flag_bit {
		unsigned char ad_check_flag : 1;		/*A/D変換動作用フラグ*/
		unsigned char rotary_l_start_flag : 1;	/*ロータリーSW左回転開始フラグ*/
		unsigned char rotary_r_start_flag : 1;	/*ロータリーSW右回転開始フラグ*/
		unsigned char rotary_l_fix_flag : 1;	/*ロータリーSW左回転確定フラグ*/
		unsigned char rotary_r_fix_flag : 1;	/*ロータリーSW右回転確定フラグ*/
		unsigned char sw1_flag : 1;				/*SW1押下検知フラグ*/
		unsigned char sw2_flag : 1;				/*SW2上下検知フラグ*/
		unsigned char sw3_flag : 1;				/*SW3押下検知フラグ*/
	};
};

/* -------------------------------- 関数宣言 -------------------------------- */
void high_timer(void);							/*高位割り込み*/
void low_timer(void);							/*低位割り込み*/
void init(void);								/*PIC初期設定*/
void HardCheck(void);							/*ハードチェックモード処理分岐*/
void HardCheck_Mode1(void);						/*ハードチェックモード１*/
void HardCheck_Mode2(void);						/*ハードチェックモード２*/
void HardCheck_Mode3(void);						/*ハードチェックモード３*/
void HardCheck_Mode4(void);						/*ハードチェックモード４*/
void HardCheck_Mode5(void);						/*ハードチェックモード５*/
void Move(void);
 
#endif	/* MAIN_H */

