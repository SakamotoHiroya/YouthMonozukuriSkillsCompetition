/* -----------------------------------------------------------------------------
| 第17回若年者ものづくり競技大会「電子回路組立て」職種
| 共通関数ヘッダファイル
| ファイル名 [ common_func.h ] 
----------------------------------------------------------------------------- */

#ifndef COMMON_FUNC_H
#define	COMMON_FUNC_H

/* -------------------------------- 関数宣言 -------------------------------- */
void sw1_check(void);
void mode_init(void);
void barled_clear(void);
void glcd_bar(unsigned char page,unsigned char start,unsigned char stop);
void glcd_message(unsigned char page,unsigned char num);
void glcd_write_num(unsigned char page,unsigned char address,unsigned char ascii);

#endif	/* COMMON_FUNC_H */

