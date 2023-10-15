/* -----------------------------------------------------------------------------
|	��N�҂��̂Â��苣�Z���u�d�q��H�g���āv�E��
|	GLCD display Library for PIC18F4620  ==>  filename [ glcdlib_c18_v03.h ]
|	
|	�ύX����
|	(glcdlib_c18_v01)
|	    2017. 4. 19 : �V�K�쐬 lcdlib_c18_v01.h by D.A
|	(glcdlib_c18_v02)
|	    2018. 4.19 : GLCD_Pixel�֐��ǉ�			by D.A
|	    2018. 4.19 : GLCD_Line�֐��ǉ�			by D.A
|	(glcdlib_c18_v03)
|	    2019. 5.01 : GLCD_Char�֐��ǉ�			by D.A
|	    2019. 5.01 : GLCD_String�֐��ǉ�		by D.A
|	    2019. 5.01 : GLCD_String_p�֐��ǉ�		by D.A
	
----------------------------------------------------------------------------- */

#ifndef _GLCDLIB_C18_V01_H
#define _GLCDLIB_C18_V01_H

/*PIC��OSC�ɍ��킹�ĕύX����*/
#ifndef	CLOCK_FREQ
#define CLOCK_FREQ 40000000UL
#endif

#define GLCD_DB	    PORTD				//GLCD�f�[�^�o�X(DB7_DB0)
#define GLCD_DBDR   TRISD				//GLCD�f�[�^�o�X�����ݒ�
#define CS			LATEbits.LATE0		//GLCD��ʃZ���N�g 1:Left,0:Right
#define RS          LATEbits.LATE2		//GLCD�f�[�^/�R�}���h�؂�ւ�
#define EBL         LATEbits.LATE1		//GLCD�C�l�[�u��
#define R_W         LATAbits.LATA5		//GLCD���[�h/���C�g�؂�ւ�

#define XDOT_MAX	128					//GLCD_X���ő�s�N�Z����
#define XDOT_HALF	64					//GLCD_X����ʋ��E�s�N�Z����
#define YDOT_MAX	64					//GLCD_Y���ő�s�N�Z����
#define PAGE_MAX	8					//GLCD_�y�[�W�s�N�Z����

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
