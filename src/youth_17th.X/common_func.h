/* -----------------------------------------------------------------------------
| ��17���N�҂��̂Â��苣�Z���u�d�q��H�g���āv�E��
| ���ʊ֐��w�b�_�t�@�C��
| �t�@�C���� [ common_func.h ] 
----------------------------------------------------------------------------- */

#ifndef COMMON_FUNC_H
#define	COMMON_FUNC_H

/* -------------------------------- �֐��錾 -------------------------------- */
void sw1_check(void);
void mode_init(void);
void barled_clear(void);
void glcd_bar(unsigned char page,unsigned char start,unsigned char stop);
void glcd_message(unsigned char page,unsigned char num);
void glcd_write_num(unsigned char page,unsigned char address,unsigned char ascii);

#endif	/* COMMON_FUNC_H */

