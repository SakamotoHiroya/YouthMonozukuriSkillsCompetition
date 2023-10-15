/* -----------------------------------------------------------------------------
|	��N�҂��̂Â��苣�Z���u�d�q��H�g���āv�E��
|	GLCD display Library for PIC18F4620  ==>  filename [ glcdlib_c18_v03.c ]
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
|	
----------------------------------------------------------------------------- */

#include <delays.h>
#include "glcdlib_c18_v03.h"
#include "font.h"

//----------------------------------------------------------------------
// * Function	: void GLCD_Init(void)
// * Overview	: �f�k�b�c��ʂ̏����ݒ�
// * Argument	: �Ȃ�
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//----------------------------------------------------------------------

void GLCD_Init(void)
{
	Delay1KTCYx(5*(CLOCK_FREQ/10000000UL));			//20ms�҂� 20m/(4/40MHz)=20000cycle
	EBL = 1 ;
	GLCD_SetDisplayOnOff(0) ;						// ��ʕ\��:OFF
	GLCD_SetDisplayOnOff(1) ;						// ��ʕ\��:ON
	GLCD_ClearScreen() ;     		 				// ��ʃN���A
}

//----------------------------------------------------------------------
// * Function	: void GLCD_ClearScreen(void)
// * Overview	: ��ʂ��N���A����
// * Argument	: �Ȃ�
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//----------------------------------------------------------------------

void GLCD_ClearScreen(void)
{
	int x ;
	int y ;

	GLCD_SetDisp(1);								//�����
	GLCD_SetLine(0);								//Line�w��
	for (y = 0; y < 8; y++)
	{
		GLCD_SetPage(y);							//Page�w��
		GLCD_SetColumn(0);							//Column�w��
		for (x = 0; x < 64; x++)
		{
			GLCD_WriteDisplayData(0x00) ;			//1�񕪂�1�o�C�g�f�[�^
		}
	}

	GLCD_SetDisp(0);								//�E���
	GLCD_SetLine(0);								//Line�w��
	for (y = 0; y < 8; y++)
	{
		GLCD_SetPage(y);							//Page�w��
		GLCD_SetColumn(0);							//Column�w��
		for (x = 0; x < 64; x++)
		{
			GLCD_WriteDisplayData(0x00) ;			//1�񕪂�1�o�C�g�f�[�^
		}
	}
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetDisplayOnOff(unsigned char display)
// * Overview	: ��ʕ\���R�}���h
// * Argument	: �\���ݒ�Fdisplay��1�FOn�@display��0�FOff
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//----------------------------------------------------------------------

void GLCD_SetDisplayOnOff(unsigned char display)
{
	EBL = 0;
	RS  = 0;
	GLCD_SetDisp(1);								//�����
	GLCD_DB = 0x3E + display;
	EBL = 1;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
	
	GLCD_SetDisp(0);								//�E���
	GLCD_DB = 0x3E + display;
	EBL = 1;
	Delay10TCYx(CLOCK_FREQ/10000000UL);
	EBL = 0;
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetDisp(unsigned char disp)
// * Overview	: GLCD�̉�ʎw��R�}���h
// * Argument	: ��ʑI�� : disp : 1-Left,0-Right
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//----------------------------------------------------------------------

void GLCD_SetDisp(unsigned char disp)
{
	CS = disp;
}


//--------------------------------------------------------------------------------
// * Function	: void GLCD_SetLine(unsigned char line_number)
// * Overview	: GLCD�̕\���J�n���C���R�}���h
// * Argument	: ���C���ԍ��Fline_number : �\���J�n���C��(0_63)
// * Return		: �Ȃ�
// * Note		: ���C��0-7���y�[�W0
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
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetPage(unsigned char page_number)
// * Overview	: GLCD�̃y�[�W�ݒ�:�s�R�}���h(�c����)
// * Argument	: �y�[�W�ԍ��Fpage_number : Y�A�h���X(0-7)
// * Return		: �Ȃ�
// * Note		: �Ȃ�
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
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_SetColumn(unsigned char column_number)
// * Overview	: GLCD�̃J�����ݒ�:��R�}���h(������)
// * Argument	: �J�����ԍ��Fcolumn_number : X�A�h���X(0-63)
// * Return		: �Ȃ�
// * Note		: �Ȃ�
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
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: void GLCD_WriteDisplayData(unsigned char val)
// * Overview	: GLCD�̃f�[�^��������
// * Argument	: �����݃f�[�^�i8bit�j
// * Return		: �Ȃ�
// * Note		: �Ȃ�
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
	Delay10TCYx(CLOCK_FREQ/10000000UL);				//4us�҂� =40cycle(40MHz)
}

//----------------------------------------------------------------------
// * Function	: unsigned char GLCD_ReadDisplayData(void)
// * Overview	: GLCD����̃f�[�^�ǂݎ��֐�
// * Argument	: �Ȃ�
// * Return		: �Y���A�h���X�̃��W�X�^�l
// * Note		: dummy�f�[�^�ǂݏo���̂���2��ǂݏo��
//----------------------------------------------------------------------

unsigned char GLCD_ReadDisplayData(void)
{
	unsigned char value	= 0 ;						//�ǂݍ��݃f�[�^
	unsigned char i;								//���[�v�p
	
	/*dummy�f�[�^�̓ǂݏo���̂��߂�2�񃋁[�v*/
	for (i=0; i<2; i++){
		TRISD = 0xFF;								//�ꎞ�I��GLCD_DB���C������͐ݒ��
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
		TRISD = 0x00;								//�|�[�gD���o�͐ݒ�ɖ߂�
	}
	return value ;
}

//--------------------------------------------------------------------------------------------
// * Function	: void GLCD_Pixel(unsigned char column,unsigned char row,unsigned char light)
// * Overview	: 1Pixel�\���֐�
// * Argument	: column:X���W row�FY���W light�F�_���E�����i0�F�����C1�F�_���j
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//--------------------------------------------------------------------------------------------

void GLCD_Pixel(unsigned char column,unsigned char row,unsigned char light)
{
	unsigned char conversion_disp = 0;				//GLCD����pcss�i�[�p
	unsigned char conversion_page = 0;				//GLCD����ppage�i�[�p
	unsigned char conversion_column = 0;			//GLCD����pcolumn�i�[�p
	unsigned char conversion_row = 0;				//GLCD����prow�i�[�p
	unsigned char data = 0;							//�_���f�[�^�i�[�p

	if (column >= XDOT_MAX){						//X���̏���l����
		column = XDOT_MAX - 1;
	}
	if (row >= YDOT_MAX){							//Y���̏���l����
		row = YDOT_MAX - 1;
	}
	if (light >= 2){								//�_��/�����̏���l����
		light = 1;
	}

	conversion_disp = column / XDOT_HALF;			//Disp���f

	conversion_page = (PAGE_MAX - 1) - (row / PAGE_MAX);	//�y�[�W�̈ʒu���f
	if (row >= 8){									//�ォ��0�C1�C2�E�E�E7�܂ł̏���
		conversion_row = row % 8;
	}
	else{
		conversion_row = row;
	}

	if (column >= XDOT_HALF){						//column���f
		conversion_column = column - XDOT_HALF;
	}
	else{
		conversion_column = column;
	}

	switch (conversion_disp){						//��ʐݒ�
		case 0:
			GLCD_SetDisp(1);						//�����
			break;
		case 1:
			GLCD_SetDisp(0);						//�E���
			break;
		default:
													//�������Ȃ�
			break;
	}

	GLCD_SetPage(conversion_page);					//�����f�[�^�ւ̏㏑��
	GLCD_SetColumn(conversion_column);
	data = GLCD_ReadDisplayData();
	if (light != 0){
		data = data | (0x80 >> conversion_row);
	}
	else{
		data = data & ~(0x80 >> conversion_row);
	}

	GLCD_SetPage(conversion_page);					//�f�[�^�̏�����
	GLCD_SetColumn(conversion_column);
	GLCD_WriteDisplayData(data);	   
}

//------------------------------------------------------------------------------------------------------------------------
// * Function	: void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char light)
// * Overview	: �����`��֐�
// * Argument	: x1�FX1���W1 y1�FY1���W x2�FX2���W y2�FY2���W light�F�_���E�����i0�F�����C1�F�_���j
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//------------------------------------------------------------------------------------------------------------------------

void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char light)
{
	int time;										//���[�v�p
	int xerr = 0;									//x���ړ����f�p
	int yerr = 0;									//y���ړ����f�p
	int deltax, deltay, distance;					//�ړ��ʊi�[�p
	int incx, incy;									//�ړ������i�[
	unsigned char uRow;								//���݈ʒu�FY
	unsigned char uCol;								//���݈ʒu�FX

	deltax = (int)x2 - x1;							//�ŏ������X�e�b�v���f
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
												
	for (time = 0; time <= distance+1; time++){		//1�h�b�g���`�揈��

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
// * Overview	: �P�����\���֐��i�h�b�g���W�w��j
// * Argument	: x_pos:X���W(0-127)�@y_pos�FY���W(0-63)�@ascii�F�����R�[�h
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//------------------------------------------------------------------------------------------------------------------------

void GLCD_Char(unsigned char x_pos,unsigned char y_pos,unsigned char ascii)
{
	unsigned char conversion_disp = 0;		/*GLCD����pcss�i�[�p*/
			 char conversion_page = 0;		/*GLCD����ppage�i�[�p*/
	unsigned char conversion_column = 0;	/*GLCD����pcolumn�i�[�p*/
	unsigned char conversion_row = 0;		/*GLCD����prow�i�[�p*/
	unsigned char i;						/*���[�v�p*/
	unsigned char data_h,data_l;			/*�����f�[�^�㉺�I�t�Z�b�g�p*/
	unsigned char data;						/*���s�`��f�[�^�擾�p*/

	
	/*X���̏㉺���l�CY���̏㉺�����f����*/
	if ((x_pos < XDOT_MAX) && (y_pos < YDOT_MAX))
	{
		/*Pixel���W����ΏۂƂȂ��ʕ������擾����*/
		conversion_disp = x_pos / XDOT_HALF;

		/*Pixel���W����y�[�W�̈ʒu���擾����*/
		conversion_page = (PAGE_MAX - 1) - (y_pos / PAGE_MAX);

		/*Pixel���W����y�[�W���̏c�ʒu���擾����*/
		/*�ォ��0�C1�C2�E�E�E7�܂�*/
		if (y_pos >= 8){
			conversion_row = y_pos % 8;
		}
		else{
			conversion_row = y_pos;
		}

		/*Pixel���W����column���擾����*/
		if (x_pos >= XDOT_HALF)
		{
			conversion_column = x_pos - XDOT_HALF ;
		}
		else
		{
			conversion_column = x_pos;
		}

		/*�Ώۂ̉�ʂ��擾����*/
		switch (conversion_disp){
			case 0:
				GLCD_SetDisp(1);			/*�����*/
				break;
			case 1:
				GLCD_SetDisp(0);			/*�E���*/
				break;
			default:
				/*�������Ȃ�*/
				break;
		}

		/*ascii�l���當���f�[�^���擾���`��*/
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
				GLCD_SetDisp(0);			/*�E���*/
				conversion_column = 0;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------
// * Function	: void GLCD_String(unsigned char x_pos,unsigned char y_pos,const rom char *code)
// * Overview	: ������\���֐�(�萔�n��)
// * Argument	: x_pos:X���W(0-127) y_pos�FY���W(0-63) *code�F������|�C���^
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//------------------------------------------------------------------------------------------------

void GLCD_String(unsigned char x_pos,unsigned char y_pos,const rom char *code)
{
	unsigned char x_now,y_now;
	
	x_now = x_pos;
	y_now = y_pos;
	
	while(*code){
		switch(*code){
			case '\n':	y_now = y_pos - 8;			//CR+LF:���A���s
						x_now = x_pos;
						break;
			case '\r':	x_now = x_pos;				//CR:���A
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
// * Overview	: ������\���֐�(�ϐ��n��)
// * Argument	: x_pos:X���W(0-127) y_pos�FY���W(0-63) *code�F������|�C���^
// * Return		: �Ȃ�
// * Note		: �Ȃ�
//------------------------------------------------------------------------------------------------

void GLCD_String_p(unsigned char x_pos,unsigned char y_pos,unsigned char *code)
{
	unsigned char x_now,y_now;
	
	x_now = x_pos;
	y_now = y_pos;
	
	while(*code){
		switch(*code){
			case '\n':	y_now = y_pos - 8;			//CR+LF:���A���s
						x_now = x_pos;
						break;
			case '\r':	x_now = x_pos;				//CR:���A
						break;
			default  :	GLCD_Char(x_now,y_now,*code);
						break;
		}
		code++;
		x_now += 8;
	}
	return;
}

