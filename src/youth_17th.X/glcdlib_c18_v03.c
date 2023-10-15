/* -----------------------------------------------------------------------------
|	若年者ものづくり競技大会「電子回路組立て」職種
|	GLCD display Library for PIC18F4620  ==>  filename [ glcdlib_c18_v03.c ]
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
|	
----------------------------------------------------------------------------- */

#include <delays.h>
#include "glcdlib_c18_v03.h"
#include "font.h"

//----------------------------------------------------------------------
// * Function	: void GLCD_Init(void)
// * Overview	: ＧＬＣＤ画面の初期設定
// * Argument	: なし
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_Init(void)
{
	Delay1KTCYx(5*(CLOCK_FREQ/10000000UL));			//20ms待ち 20m/(4/40MHz)=20000cycle
	EBL = 1 ;
	GLCD_SetDisplayOnOff(0) ;						// 画面表示:OFF
	GLCD_SetDisplayOnOff(1) ;						// 画面表示:ON
	GLCD_ClearScreen() ;     		 				// 画面クリア
}

//----------------------------------------------------------------------
// * Function	: void GLCD_ClearScreen(void)
// * Overview	: 画面をクリアする
// * Argument	: なし
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_ClearScreen(void)
{
	int x ;
	int y ;

	GLCD_SetDisp(1);								//左画面
	GLCD_SetLine(0);								//Line指定
	for (y = 0; y < 8; y++)
	{
		GLCD_SetPage(y);							//Page指定
		GLCD_SetColumn(0);							//Column指定
		for (x = 0; x < 64; x++)
		{
			GLCD_WriteDisplayData(0x00) ;			//1列分の1バイトデータ
		}
	}

	GLCD_SetDisp(0);								//右画面
	GLCD_SetLine(0);								//Line指定
	for (y = 0; y < 8; y++)
	{
		GLCD_SetPage(y);							//Page指定
		GLCD_SetColumn(0);							//Column指定
		for (x = 0; x < 64; x++)
		{
			GLCD_WriteDisplayData(0x00) ;			//1列分の1バイトデータ
		}
	}
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetDisplayOnOff(unsigned char display)
// * Overview	: 画面表示コマンド
// * Argument	: 表示設定：display＝1：On　display＝0：Off
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_SetDisplayOnOff(unsigned char display)
{
	EBL = 0;
	RS  = 0;
	GLCD_SetDisp(1);								//左画面
	GLCD_DB = 0x3E + display;
	EBL = 1;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
	
	GLCD_SetDisp(0);								//右画面
	GLCD_DB = 0x3E + display;
	EBL = 1;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetDisp(unsigned char disp)
// * Overview	: GLCDの画面指定コマンド
// * Argument	: 画面選択 : disp : 1-Left,0-Right
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_SetDisp(unsigned char disp)
{
	CS = disp;
}


//--------------------------------------------------------------------------------
// * Function	: void GLCD_SetLine(unsigned char line_number)
// * Overview	: GLCDの表示開始ラインコマンド
// * Argument	: ライン番号：line_number : 表示開始ライン(0_63)
// * Return		: なし
// * Note		: ライン0-7がページ0
//--------------------------------------------------------------------------------

void GLCD_SetLine(unsigned char line_number) 
{
	EBL = 0 ;
	R_W  = 0 ;
	RS  = 0 ;
	GLCD_DB = 0xC0 + line_number ;
	EBL = 1 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetPage(unsigned char page_number)
// * Overview	: GLCDのページ設定:行コマンド(縦方向)
// * Argument	: ページ番号：page_number : Yアドレス(0-7)
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_SetPage(unsigned char page_number) 
{
	EBL = 0 ;
	R_W  = 0 ;
	RS  = 0 ;
	GLCD_DB = 0xB8 + (page_number & 0x07) ;
	EBL = 1 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetColumn(unsigned char column_number)
// * Overview	: GLCDのカラム設定:列コマンド(横方向)
// * Argument	: カラム番号：column_number : Xアドレス(0-63)
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_SetColumn(unsigned char column_number)
{
	EBL = 0 ;
	R_W  = 0 ;
	RS  = 0 ;
	GLCD_DB = 0x40 + (column_number & 0x3F) ;
	EBL = 1 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_WriteDisplayData(unsigned char val)
// * Overview	: GLCDのデータ書き込み
// * Argument	: 書込みデータ（8bit）
// * Return		: なし
// * Note		: なし
//----------------------------------------------------------------------

void GLCD_WriteDisplayData(unsigned char val)
{
	EBL = 0 ;
	R_W  = 0 ;
	RS  = 1 ;
	GLCD_DB = val ;
	EBL = 1 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0 ;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us待ち =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: unsigned char GLCD_ReadDisplayData(void)
// * Overview	: GLCDからのデータ読み取り関数
// * Argument	: なし
// * Return		: 該当アドレスのレジスタ値
// * Note		: dummyデータ読み出しのため2回読み出し
//----------------------------------------------------------------------

unsigned char GLCD_ReadDisplayData(void)
{
	unsigned char value	= 0 ;						//読み込みデータ
	unsigned char i;								//ループ用
	
	/*dummyデータの読み出しのための2回ループ*/
	for (i=0; i<2; i++){
		TRISD = 0xFF;								//一時的にGLCD_DBラインを入力設定に
		Delay10TCYx(CLOCK_FREQ/10000000UL);
		EBL = 0 ;
		R_W  = 1 ;
		RS  = 1 ;
		EBL = 1 ;
		Delay10TCYx(CLOCK_FREQ/10000000UL);
		EBL = 0 ;
		Delay10TCYx(CLOCK_FREQ/10000000UL);
		value = PORTD ;
		Delay10TCYx(CLOCK_FREQ/10000000UL);
		TRISD = 0x00;								//ポートDを出力設定に戻す
	}
	return value ;
}

//--------------------------------------------------------------------------------------------
// * Function	: void GLCD_Pixel(unsigned char column,unsigned char row,unsigned char light)
// * Overview	: 1Pixel表示関数
// * Argument	: column:X座標 row：Y座標 light：点灯・消灯（0：消灯，1：点灯）
// * Return		: なし
// * Note		: なし
//--------------------------------------------------------------------------------------------

void GLCD_Pixel(unsigned char column,unsigned char row,unsigned char light)
{
	unsigned char conversion_disp = 0;				//GLCD制御用css格納用
	unsigned char conversion_page = 0;				//GLCD制御用page格納用
	unsigned char conversion_column = 0;			//GLCD制御用column格納用
	unsigned char conversion_row = 0;				//GLCD制御用row格納用
	unsigned char data = 0;							//点灯データ格納用

	if (column >= XDOT_MAX){						//X軸の上限値処理
		column = XDOT_MAX - 1;
	}
	if (row >= YDOT_MAX){							//Y軸の上限値処理
		row = YDOT_MAX - 1;
	}
	if (light >= 2){								//点灯/消灯の上限値処理
		light = 1;
	}

	conversion_disp = column / XDOT_HALF;			//Disp判断

	conversion_page = (PAGE_MAX - 1) - (row / PAGE_MAX);	//ページの位置判断
	if (row >= 8){									//上から0，1，2・・・7までの処理
		conversion_row = row % 8;
	}
	else{
		conversion_row = row;
	}

	if (column >= XDOT_HALF){						//column判断
		conversion_column = column - XDOT_HALF;
	}
	else{
		conversion_column = column;
	}

	switch (conversion_disp){						//画面設定
		case 0:
			GLCD_SetDisp(1);						//左画面
			break;
		case 1:
			GLCD_SetDisp(0);						//右画面
			break;
		default:
													//処理しない
			break;
	}

	GLCD_SetPage(conversion_page);					//既存データへの上書き
	GLCD_SetColumn(conversion_column);
	data = GLCD_ReadDisplayData();
	if (light != 0){
		data = data | (0x80 >> conversion_row);
	}
	else{
		data = data & ~(0x80 >> conversion_row);
	}

	GLCD_SetPage(conversion_page);					//データの書込み
	GLCD_SetColumn(conversion_column);
	GLCD_WriteDisplayData(data);	   
}

//------------------------------------------------------------------------------------------------------------------------
// * Function	: void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char light)
// * Overview	: 直線描画関数
// * Argument	: x1：X1座標1 y1：Y1座標 x2：X2座標 y2：Y2座標 light：点灯・消灯（0：消灯，1：点灯）
// * Return		: なし
// * Note		: なし
//------------------------------------------------------------------------------------------------------------------------

void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char light)
{
	int time;										//ループ用
	int xerr = 0;									//x軸移動判断用
	int yerr = 0;									//y軸移動判断用
	int deltax, deltay, distance;					//移動量格納用
	int incx, incy;									//移動方向格納
	unsigned char uRow;								//現在位置：Y
	unsigned char uCol;								//現在位置：X

	deltax = (int)x2 - x1;							//最小処理ステップ判断
	deltay = (int)y2 - y1;
	uCol = x1;
	uRow = y1;
	if (deltax > 0){
		incx = 1;
	}else if (deltax == 0){
		incx = 0;
	}else {
		incx = -1;
		deltax = -deltax;
	}
	if (deltay > 0){
		incy = 1;
	}else if (deltay == 0){
		incy = 0;
	}else {
		incy = -1;
		deltay = -deltay;
	}
	if (deltax > deltay){
		distance = deltax;
	}else {
		distance = deltay;
	}
												
	for (time = 0; time <= distance+1; time++){		//1ドットずつ描画処理

		GLCD_Pixel(uCol, uRow, (unsigned char)light);

		xerr += deltax;
		yerr += deltay;
		if (xerr>distance){
			xerr -= distance;
			uCol += incx;
		}
		if (yerr > distance){
			yerr -= distance;
			uRow += incy;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
// * Function	: void GLCD_Char(unsigned char x_pos,unsigned char y_pos,unsigned char ascii)
// * Overview	: １文字表示関数（ドット座標指定）
// * Argument	: x_pos:X座標(0-127)　y_pos：Y座標(0-63)　ascii：文字コード
// * Return		: なし
// * Note		: なし
//------------------------------------------------------------------------------------------------------------------------

void GLCD_Char(unsigned char x_pos,unsigned char y_pos,unsigned char ascii)
{
	unsigned char conversion_disp = 0;		/*GLCD制御用css格納用*/
			 char conversion_page = 0;		/*GLCD制御用page格納用*/
	unsigned char conversion_column = 0;	/*GLCD制御用column格納用*/
	unsigned char conversion_row = 0;		/*GLCD制御用row格納用*/
	unsigned char i;						/*ループ用*/
	unsigned char data_h,data_l;			/*文字データ上下オフセット用*/
	unsigned char data;						/*現行描画データ取得用*/

	
	/*X軸の上下限値，Y軸の上下限判断処理*/
	if ((x_pos < XDOT_MAX) && (y_pos < YDOT_MAX))
	{
		/*Pixel座標から対象となる画面方向を取得する*/
		conversion_disp = x_pos / XDOT_HALF;

		/*Pixel座標からページの位置を取得する*/
		conversion_page = (PAGE_MAX - 1) - (y_pos / PAGE_MAX);

		/*Pixel座標からページ内の縦位置を取得する*/
		/*上から0，1，2・・・7まで*/
		if (y_pos >= 8){
			conversion_row = y_pos % 8;
		}
		else{
			conversion_row = y_pos;
		}

		/*Pixel座標からcolumnを取得する*/
		if (x_pos >= XDOT_HALF)
		{
			conversion_column = x_pos - XDOT_HALF ;
		}
		else
		{
			conversion_column = x_pos;
		}

		/*対象の画面を取得する*/
		switch (conversion_disp){
			case 0:
				GLCD_SetDisp(1);			/*左画面*/
				break;
			case 1:
				GLCD_SetDisp(0);			/*右画面*/
				break;
			default:
				/*処理しない*/
				break;
		}

		/*ascii値から文字データを取得し描画*/
		for (i = 0;i < 8;i++)
		{
			data_l = 0;
			data_h = 0;
			GLCD_SetColumn(conversion_column);
			GLCD_SetPage(conversion_page);
			data_l = GLCD_ReadDisplayData();
			data_l |= font_data[((unsigned int)ascii * 8) + i] >> conversion_row;
			GLCD_SetColumn(conversion_column);
			GLCD_WriteDisplayData(data_l);
			if (((conversion_page - 1) >= 0) && (conversion_row != 0)) 
			{
				GLCD_SetColumn(conversion_column);
				GLCD_SetPage(conversion_page - 1);
				data_h = GLCD_ReadDisplayData();
				data_h |= font_data[((unsigned int)ascii * 8) + i] << (8 - conversion_row);
				GLCD_SetColumn(conversion_column);
				GLCD_WriteDisplayData(data_h);
			}
			conversion_column++;
			if ((conversion_disp == 1) && (conversion_column >= XDOT_HALF))
			{
				break;
			}
			if (conversion_column == XDOT_HALF)
			{
				conversion_disp = 1;
				GLCD_SetDisp(0);			/*右画面*/
				conversion_column = 0;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------
// * Function	: void GLCD_String(unsigned char x_pos,unsigned char y_pos,const rom char *code)
// * Overview	: 文字列表示関数(定数渡し)
// * Argument	: x_pos:X座標(0-127) y_pos：Y座標(0-63) *code：文字列ポインタ
// * Return		: なし
// * Note		: なし
//------------------------------------------------------------------------------------------------

void GLCD_String(unsigned char x_pos,unsigned char y_pos,const rom char *code)
{
	unsigned char x_now,y_now;
	
	x_now = x_pos;
	y_now = y_pos;
	
	while(*code){
		switch(*code){
			case '\n':	y_now = y_pos - 8;			//CR+LF:復帰改行
						x_now = x_pos;
						break;
			case '\r':	x_now = x_pos;				//CR:復帰
						break;
			default  :	GLCD_Char(x_now,y_now,*code);
						break;
		}
		code++;
		x_now += 8;
	}
	return;
}

//------------------------------------------------------------------------------------------------
// * Function	: void GLCD_String_p(unsigned char x_pos,unsigned char y_pos,unsigned char *code)
// * Overview	: 文字列表示関数(変数渡し)
// * Argument	: x_pos:X座標(0-127) y_pos：Y座標(0-63) *code：文字列ポインタ
// * Return		: なし
// * Note		: なし
//------------------------------------------------------------------------------------------------

void GLCD_String_p(unsigned char x_pos,unsigned char y_pos,unsigned char *code)
{
	unsigned char x_now,y_now;
	
	x_now = x_pos;
	y_now = y_pos;
	
	while(*code){
		switch(*code){
			case '\n':	y_now = y_pos - 8;			//CR+LF:復帰改行
						x_now = x_pos;
						break;
			case '\r':	x_now = x_pos;				//CR:復帰
						break;
			default  :	GLCD_Char(x_now,y_now,*code);
						break;
		}
		code++;
		x_now += 8;
	}
	return;
}

