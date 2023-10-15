/* -----------------------------------------------------------------------------
| 第17回若年者ものづくり競技大会「電子回路組立て」職種
| 競技用Cソースファイル
| ファイル名 [ move.c ]
----------------------------------------------------------------------------- */

/* ---------------------- ヘッダファイルのインクルード ---------------------- */
#include <p18f4620.h>
#include <adc.h>
#include <timers.h>
#include <usart.h>
#include <delays.h>
#include <portb.h>
#include <math.h>
#include <pwm.h>
#include <stdio.h>
#include "main.h"
#include "move.h"
#include "common_func.h"
#include "glcdlib_c18_v03.h"
#include "font.h"

/* --------------------------- グローバル変数宣言---------------------------- */
extern unsigned int red_data;		/*赤LEDデータ格納用*/
extern unsigned int green_data;		/*緑LEDデータ格納用*/
extern unsigned int blue_data;		/*青LEDデータ格納用*/
extern signed int rotary_cnt;		/*ロータリーSW回転数カウント*/
extern unsigned char sw_cnt;		/*SW1チャタリング検知用*/
extern unsigned int cnt_10ms;		/*10msカウンタ*/
extern unsigned int cnt_100ms;		/*100msカウンタ*/
extern unsigned int mode;			/*現在モード状態*/
extern unsigned int old_mode;		/*過去モード状態*/
extern union flag flg;				/*各種フラグ管理用*/
extern const unsigned int bz_frq;	/*初期ブザー周波数設定*/

/* -------------------------------- 定数宣言--------------------------------- */
#define JOYA PORTCbits.RC5			/*課題1,2用*/
#define JOYB PORTCbits.RC4			/*課題1,2用*/
/*設問4用 内臓音源*/
enum scale { DO,DOs,RE,REs,MI,FA,FAs,SO,SOs,RA,RAs,SI,DO_ };
unsigned char musical_scale[13] = { 0xFF,0xF1,0xE3,0xD7,0xCA,0xBF,0xB4,0xAA,0xA1,0x98,0x8F,0x87,0x80 };	/*音階データ*/

#define RED 0b100 //フルカラー１０バーLED 赤色
#define GREEN 0b010 //フルカラー１０バーLED 緑色
#define BLUE 0b001 //フルカラー１０バーLED 青色

/* -------------------------------- プロトタイプ宣言--------------------------------- */

int map(int value, int in_min, int in_max, int out_min, int out_max);//入力した値を別の範囲に割り当てる
void write_barled_data(int data, int color); //引数の色で１０バーLEDを点灯
int read_analog_value(int channel); //引数のチャンネルをの値を10ビットにAD変換した値を返す
int read_joystick(int channel); //ジョイスティックの値を読み取り傾けていないとき０、傾いているとき-1, 1を返す
void glcd_cursor(int location);//動作モード５のカーソルを描画
void glcd_clear_cursor();//動作モード５のカーソルを消去

int map(int value, int in_min, int in_max, int out_min, int out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void write_barled_data(int data, int color)
{
    if(color & RED)
    {
        red_data = data;
    }
    else
    {
        red_data = 0;
    }
    
    if(color & GREEN)
    {
        green_data = data;
    }
    else
    {
        green_data = 0;
    }
    
    if(color & BLUE)
    {
        blue_data = data;
    }
    else
    {
        blue_data = 0;
    }
}

int read_analog_value(int channel)
{
    SetChanADC(channel);
    ConvertADC();
    while(BusyADC())
    {
        ;
    }
    return ReadADC();
}

int read_joystick(int channel)
{
    int ad_value;
    ad_value = read_analog_value(channel);
    
    if(307 <= ad_value && ad_value <= 716)
    {
        return 0;
    }
    else if(ad_value < 307)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void glcd_cursor(int location)
{
    switch(location)
    {
        case 0:
            /*Kの下のバー*/
            GLCD_Line(36,27,43,27,1);
            GLCD_Line(36,28,43,28,1);
            break;
        case 1:
            /*Aの下のバー*/
            GLCD_Line(53,27,60,27,1);
            GLCD_Line(53,28,60,28,1);
            break;
        case 2:
            /*Kの下のバー*/
            GLCD_Line(68,27,75,27,1);
            GLCD_Line(68,28,75,28,1);
            break;
        case 3:
            /*Iの下のバー*/
            GLCD_Line(84,27,91,27,1);
            GLCD_Line(84,28,91,28,1);
            break;
    }
    
}

void glcd_clear_cursor()
{
    GLCD_Line(0, 27, 127, 27, 0);
    GLCD_Line(0, 28, 127, 28, 0);    
}

/* -----------------------------------------------------------------------------
動作モード１
----------------------------------------------------------------------------- */
void Move_Mode1(void)
{
    static char str[] = "CARP";
    
    static int sw2_old = 0;
    static int joya_old = 0;
    static int joyb_old = 0;
    
    static int x;
    static int y;
    
    static int x_old;
    static int y_old;
    
    static int left_x = 16;
    static int right_x = 80;
    static int top_y = 44;
    static int bottom_y = 12;
    
    int i;
    
	/*他モードより移行時、初期状態より動作を開始させる*/
	if (mode != old_mode) 
	{
		/*初期化処理*/
		mode_init();
		/* move_mode1表示 */
		glcd_message(0, 1);
		glcd_write_num(0, 54,'1');
        
        sw2_old = SW2;
        joya_old = JOYA;
        joyb_old = JOYB;
        
        x = left_x;
        y = top_y;
        
        x_old = -1;
        y_old = -1;
        
	}
    
    if(cnt_10ms >= 5)
    {
        cnt_10ms = 0;
        
        if(SW2 != sw2_old)
        {
            if(SW2)
            {
                y = top_y;
            }
            else
            {
                y = bottom_y;
            }
        }
        
        if(JOYA == 0 && joya_old == 1)
        {
            x = right_x;
        }
        
        if(JOYB == 0 && joyb_old == 1)
        {
            x = left_x;
        }
        
        sw2_old = SW2;
        joya_old = JOYA;
        joyb_old = JOYB;
    }
    
    if(x != x_old || y != y_old)
    {
        for(i = 1; i < 8; i++)
        {
            glcd_bar(i, 128, 128);
        }
        GLCD_String_p(x, y, str);
        
        x_old = x;
        y_old = y;
    }
}

/* -----------------------------------------------------------------------------
動作モード２
----------------------------------------------------------------------------- */
void Move_Mode2(void)
{
 
    static int state;
    char send_char;
    int send_sw_state;
    
	/*他モードより移行時、初期状態より動作を開始させる*/
	if (mode != old_mode)
	{
		/*初期化処理*/
		mode_init();
		/*move_mode2表示*/
		glcd_message(0, 1);
		glcd_write_num(0, 54,'2');
        
        cnt_100ms = 0;
        
        state = 0;
        
        ROTARY_LED = 1;
	}
    
    if(cnt_100ms >= 2)
    {
        cnt_100ms = 0;
        switch(state)
        {
            case 0://SW2
                send_char = 'T';
                send_sw_state = SW2;
                break;
            case 1://SW3
                send_char = 'R';
                send_sw_state = !SW3;
                break;
            case 2://SWA
                send_char = 'A';
                send_sw_state = !JOYA;
                break;
            case 3://SWB
                send_char = 'B';
                send_sw_state = !JOYB;
                break;
            default:
                ;
        }
        state++;
        if(state > 3)
        {
            state = 0;
        }
        printf("S%c%dE", send_char, send_sw_state);
    }
    
}

/* -----------------------------------------------------------------------------
動作モード３
----------------------------------------------------------------------------- */
void Move_Mode3(void)
{
    static int color = 0;
    static int barled_num = 0;
    static int barled_data = 0;
    static int colors_i = 0;
    static int timer_init_flag = 0;
    static int state;
    static int sw2_old = 0;
    
    /*他モードより移行時、初期状態より動作を開始させる*/
	if (mode != old_mode)
	{
		mode_init();
		/* move_mode3表示 */
		glcd_message(0, 1);
		glcd_write_num(0, 54,'3');
        
        sw2_old = SW2;
        
        timer_init_flag = 1;
        
        state = 0;
	}
    
    if(timer_init_flag)
    {
        timer_init_flag = 0;
        barled_num = 10;
        colors_i = 0;
        color = RED;
        barled_data = (int)0x3ff << (10 - barled_num);
        state = 0;
        
        write_barled_data(barled_data, color);
    }
    
    switch(state)
    {
        case 0:
            if(SW2 && !sw2_old)
            {
                state++;
                cnt_100ms = 0;
            }
            break;
        case 1:
            if(cnt_100ms >= 1)
            {
                color = RED;
                cnt_100ms = 0;
                barled_num--;
                if(barled_num < 0)
                {
                    barled_num = 10;
                    state++;
                    cnt_100ms = 0;
                    break;
                }
                
                barled_data = (int)0x3ff << (10 - barled_num);
                write_barled_data(barled_data, color);
            }
            break;
        case 2:
            if(cnt_100ms >= 1)
            {
                color = GREEN;
                cnt_100ms = 0;
                barled_num--;
                if(barled_num < 0)
                {
                    barled_num = 10;
                    state++;
                    cnt_100ms = 0;
                    break;
                }
                
                barled_data = (int)0x3ff >> (10 - barled_num);
                write_barled_data(barled_data, color);
            }
            break;
        case 3:
            if(cnt_100ms >= 1)
            {
                color = BLUE;
                cnt_100ms = 0;
                barled_num--;
                if(barled_num < 0)
                {
                    barled_num = 10;
                    state++;
                    cnt_100ms = 0;
                    break;
                }
                
                barled_data = (int)0x3ff << (10 - barled_num);
                write_barled_data(barled_data, color);
            }
            break;
        case 4:
            if(cnt_100ms < 10)
            {
                BZ_CON = BZ_ON;
            }
            else
            {
                BZ_CON = BZ_OFF;
                state++;
            }
            break;
        case 5:
            BZ_CON = BZ_OFF;
            if(!SW2 && sw2_old)
            {
                state = 0;
                cnt_100ms = 0;
                timer_init_flag = 1;
            }
            break;
    }
    
    sw2_old = SW2;
   
}

/* -----------------------------------------------------------------------------
動作モード４
----------------------------------------------------------------------------- */
void Move_Mode4(void)
{
    //キーボードのキーと音階の対応
    static int key_note_map[13][2] = {{'a', DO}, {'w', DOs}, {'s', RE}, {'e', REs}, {'d', MI}, {'f', FA}, {'t', FAs}, {'g', SO}, {'y', SOs}, {'h', RA}, {'u', RAs}, {'j', SI}, {'k', DO_}};
    //音階と音の対応
    static int note_colors[13][2] = {{DO, RED}, {DOs, GREEN}, {RE, RED}, {REs, GREEN}, {MI, RED}, {FA, RED}, {FAs, GREEN}, {SO, RED}, {SOs, GREEN}, {RA, RED}, {RAs, GREEN}, {SI, RED}, {DO_, RED}};
    //音階とバーLEDの位置の対応
    static int note_position[13][2] = {{DO, 0}, {DOs, 0}, {RE, 1}, {REs, 1}, {MI, 2}, {FA, 3}, {FAs, 3}, {SO, 4}, {SOs, 4}, {RA, 5}, {RAs, 5}, {SI, 6}, {DO_, 7}};
    static char readed_char = 0;
    static int note = 0;
    static int note_old = 0;
    static int bz_ring_flag = 0;
    static int barled_rewrite_flag = 0;
    int color;
    int barled_num;
    int i;
    
	/*他モードより移行時、初期状態より動作を開始させる*/
	if (mode != old_mode)
	{
		/*初期化処理*/
		mode_init();
		/* move_mode4表示 */
		glcd_message(0, 1);
		glcd_write_num(0, 54,'4');
        
        note = -1;
        note_old = note;
        
        barled_rewrite_flag = 0;
        bz_ring_flag = 0;
	}
    
    if(DataRdyUSART())
    {
        readed_char = getcUSART();
        note_old = note;
        for(i = 0; i < 13; i++)
        {
            if(key_note_map[i][0] == readed_char)
            {
                note = key_note_map[i][1];
                break;
            }
        }
        if(note != note_old)
        {
            bz_ring_flag = 1;
            barled_rewrite_flag = 1;
            OpenPWM1(musical_scale[note]);
            cnt_10ms = 0;
            cnt_100ms = 0;
        }
        else
        {
            cnt_10ms = 0;
            cnt_100ms = 0;            
        }
    }
    
    if(barled_rewrite_flag)
    {
        barled_rewrite_flag = 0;
        for(i = 0; i < 13; i++)
        {
            if(note_colors[i][0] == note)
            {
                color = note_colors[i][1];
            }
            if(note_position[i][0] == note)
            {
                barled_num = note_position[i][1];
            }
        }
        write_barled_data(0x200 >> barled_num, color);
    }
    
    if(bz_ring_flag)
    {
        if(cnt_100ms * 10 + cnt_10ms <= 25) //200msごとにデータが送られてくるため余裕をもって350ms間音を鳴らす
        {
            BZ_CON = BZ_ON;
        }
        else
        {
            BZ_CON = BZ_OFF;
            bz_ring_flag = 0;
            note = -1;
        }
    }
    else
    {
        red_data = 0;
        green_data = 0;
        blue_data = 0;
    }
    
}

/* -----------------------------------------------------------------------------
動作モード５
----------------------------------------------------------------------------- */
void Move_Mode5(void)
{
    unsigned char i = 0;
    unsigned char data_K[8] = { 0x00, 0x7f, 0x10, 0x08, 0x14, 0x22, 0x41, 0x00 };	/*Kデータ*/
	unsigned char data_A[8] = { 0x60, 0x18, 0x16, 0x11, 0x16, 0x18, 0x60, 0x00 };	/*Aデータ*/
	unsigned char data_I[8] = { 0x00, 0x00, 0x41, 0x7f, 0x41, 0x00, 0x00, 0x00 };	/*Iデータ*/
    
    static int joystick_x = 0;
    static int cursor_location = 0;
    static int cursor_location_old = 0;
    
    static int angle[4] = {0, 0, 0, 0};
    
	/*他モードより移行時、初期状態より動作を開始させる*/
	if (mode != old_mode)
	{
		/*初期化処理*/
		mode_init();
		/* move_mode5表示 */
		glcd_message(0, 1);
		glcd_write_num(0, 54,'5');
        
        /*左画面の描画設定*/
		GLCD_SetDisp(1);					/*Disp指定*/
		GLCD_SetPage(3);					/*Page指定*/
		/*Kの初期描画処理*/
		GLCD_SetColumn(36);                 /*Column指定*/
		for ( i=0; i<8; i++ )
		{
			GLCD_WriteDisplayData( data_K[i] );
		}
		
		/*Aの初期描画処理*/
		GLCD_SetColumn(53);					/*Column指定*/
		for ( i=0; i<8; i++ )
		{
			GLCD_WriteDisplayData( data_A[i] );
		}
		/*右画面の描画設定*/
		GLCD_SetDisp(0);					/*Disp指定*/
		GLCD_SetPage(3);					/*Page指定*/
		/*Kの初期描画処理*/
		GLCD_SetColumn(4);					/*Column指定*/
		for ( i=0; i<8; i++ )
		{
			GLCD_WriteDisplayData( data_K[i] );
		}
		/*Iの初期描画処理*/
		GLCD_SetColumn(20);					/*Column指定*/
		for ( i=0; i<8; i++ )
		{
			GLCD_WriteDisplayData( data_I[i] );
		}
        
        for(i = 0; i < 4; i++)
        {
            angle[i] = 0;
        }
        
        cursor_location = 0;
        cursor_location_old = -1;
        
        ROTARY_LED = 1;
	}
    
    if(cnt_100ms >= 5)
    {   
        cnt_100ms = 0;
        joystick_x = read_joystick(ADC_CH1);
        if(joystick_x == 1)
        {
            cursor_location++;
            if(cursor_location > 3)
            {
                cursor_location = 0;
            }
        }
        else if(joystick_x == -1)
        {
            cursor_location--;
            if(cursor_location < 0)
            {
                cursor_location = 3;
            }
        }
    }
    
    if(cursor_location != cursor_location_old)
    {
        glcd_clear_cursor();
        glcd_cursor(cursor_location);
        cursor_location_old = cursor_location;
    }
    
//    if(flg.rotary_l_fix_flag)
//    {
//        flg.rotary_l_fix_flag = 0;
//        angle[cursor_location]--;
//        if(angle[cursor_location] < 0)
//        {
//            angle[cursor_location] = 3;
//        }
//    }
//
//    if(flg.rotary_r_fix_flag)
//    {
//        flg.rotary_r_fix_flag = 0;
//        angle[cursor_location]++;
//        if(angle[cursor_location] > 3)
//        {
//            angle[cursor_location] = 0;
//        }
//    }
    
}