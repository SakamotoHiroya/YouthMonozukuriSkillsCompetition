/* -----------------------------------------------------------------------------
|	若年者ものづくり競技大会「電子回路組立て」職種
|	GLCD display Library for PIC18F4620  ==>  filename [ glcdlib_c18_v03.h ]
|	
|	変更履歴
|	(glcdlib_c18_v01)
|	    2017. 4. 19 : 新規作成 lcdlib_c18_v01.h by D.A
|	(glcdlib_c18_v02)
|	    2018. 4.19 : GLCD_Pixel関数追加			by D.A
|	    2018. 4.19 : GLCD_Line関数追加			by D.A
|	(glcdlib_c18_v03)
|	    2019. 5.01 : GLCD_Char関数追加			by D.A
|	    2019. 5.01 : GLCD_String関数追加		by D.A
|	    2019. 5.01 : GLCD_String_p関数追加		by D.A
	
----------------------------------------------------------------------------- */

#ifndef _GLCDLIB_C18_V01_H
#define _GLCDLIB_C18_V01_H

/*PICのOSCに合わせて変更する*/
#ifndef	CLOCK_FREQ
#define CLOCK_FREQ 40000000UL
#endif

#define GLCD_DB	    PORTD				//GLCDデータバス(DB7_DB0)
#define GLCD_DBDR   TRISD				//GLCDデータバス方向設定
#define CS			LATEbits.LATE0		//GLCD画面セレクト 1:Left,0:Right
#define RS          LATEbits.LATE2		//GLCDデータ/コマンド切り替え
#define EBL         LATEbits.LATE1		//GLCDイネーブル
#define R_W         LATAbits.LATA5		//GLCDリード/ライト切り替え

#define XDOT_MAX	128					//GLCD_X軸最大ピクセル数
#define XDOT_HALF	64					//GLCD_X軸画面境界ピクセル数
#define YDOT_MAX	64					//GLCD_Y軸最大ピクセル数
#define PAGE_MAX	8					//GLCD_ページピクセル数

void GLCD_Init(void);
void GLCD_ClearScreen(void);
void GLCD_SetDisplayOnOff(unsigned char display);
void GLCD_SetDisp(unsigned char disp);
void GLCD_SetLine(unsigned char line_number);
void GLCD_SetPage(unsigned char page_number);
void GLCD_SetColumn(unsigned char column_number);
void GLCD_WriteDisplayData(unsigned char val);
unsigned char GLCD_ReadDisplayData(void);
void GLCD_Pixel(unsigned char column,unsigned char row,unsigned char light);
void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char light);
void GLCD_Char(unsigned char x_pos,unsigned char y_pos,unsigned char ascii);
void GLCD_String(unsigned char x_pos,unsigned char y_pos,const rom char *code);
void GLCD_String_p(unsigned char x_pos,unsigned char y_pos,unsigned char *code);
#endif
