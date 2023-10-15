/* -----------------------------------------------------------------------------
| �O���t�B�b�N�t�����䃉�C�u�����p�t�H���g�f�[�^
| �t�@�C���� [ font.c ] 
----------------------------------------------------------------------------- */

#include "font.h"

/* �����f�[�^ */
rom const char font_data[1024] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x00 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x01 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x02 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x03 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x04 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x05 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x06 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x07 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x08 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x09 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0a NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0b NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0c NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0d NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0e NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x0f NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x10 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x11 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x12 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x13 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x14 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x15 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x16 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x17 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x18 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x19 NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1a NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1b NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1c NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1d NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1e NULL
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// 0x1f NULL
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x20 '  '
0x00,0x00,0x00,0x5f,0x00,0x00,0x00,0x00,    // 0x21 '�I'
0x05,0x03,0x00,0x05,0x03,0x00,0x00,0x00,    // 0x22 '�h'
0x20,0x62,0x3e,0x63,0x3e,0x23,0x02,0x00,    // 0x23 '��'
0x00,0x24,0x2a,0x7a,0x2f,0x2a,0x12,0x00,    // 0x24 '��'
0x42,0x25,0x12,0x08,0x24,0x52,0x21,0x00,    // 0x25 '��'
0x20,0x56,0x49,0x55,0x22,0x58,0x40,0x00,    // 0x26 '��'
0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x27 '�f'
0x00,0x00,0x00,0x3e,0x22,0x41,0x41,0x00,    // 0x28 '�i'
0x41,0x41,0x22,0x3e,0x00,0x00,0x00,0x00,    // 0x29 '�j'
0x00,0x22,0x14,0x7f,0x14,0x22,0x00,0x00,    // 0x2a '��'
0x08,0x08,0x08,0x7f,0x08,0x08,0x08,0x00,    // 0x2b '�{'
0x50,0x30,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x2c '�C'
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,    // 0x2d '�|'
0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x2e '�D'
0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,    // 0x2f '�^'
0x00,0x3e,0x41,0x41,0x41,0x41,0x3e,0x00,    // 0x30 '�O'
0x00,0x00,0x42,0x7f,0x40,0x00,0x00,0x00,    // 0x31 '�P'
0x00,0x62,0x51,0x51,0x49,0x49,0x46,0x00,    // 0x32 '�Q'
0x00,0x22,0x41,0x49,0x49,0x49,0x36,0x00,    // 0x33 '�R'
0x00,0x30,0x28,0x24,0x22,0x7f,0x20,0x00,    // 0x34 '�S'
0x00,0x2f,0x45,0x45,0x45,0x45,0x39,0x00,    // 0x35 '�T'
0x00,0x3e,0x49,0x49,0x49,0x49,0x32,0x00,    // 0x36 '�U'
0x00,0x01,0x01,0x61,0x19,0x05,0x03,0x00,    // 0x37 '�V'
0x00,0x36,0x49,0x49,0x49,0x49,0x36,0x00,    // 0x38 '�W'
0x00,0x26,0x49,0x49,0x49,0x49,0x3e,0x00,    // 0x39 '�X'
0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,    // 0x3a '�F'
0x00,0x00,0x56,0x36,0x00,0x00,0x00,0x00,    // 0x3b '�G'
0x00,0x00,0x00,0x08,0x14,0x22,0x41,0x00,    // 0x3c '��'
0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x00,    // 0x3d '��'
0x41,0x22,0x14,0x08,0x00,0x00,0x00,0x00,    // 0x3e '��'
0x00,0x02,0x01,0x51,0x09,0x09,0x06,0x00,    // 0x3f '�H'
0x1c,0x22,0x59,0x55,0x4d,0x12,0x0c,0x00,    // 0x40 '��'
0x60,0x18,0x16,0x11,0x16,0x18,0x60,0x00,    // 0x41 '�`'
0x00,0x7f,0x49,0x49,0x49,0x49,0x36,0x00,    // 0x42 '�a'
0x00,0x1c,0x22,0x41,0x41,0x41,0x22,0x00,    // 0x43 '�b'
0x00,0x7f,0x41,0x41,0x41,0x22,0x1c,0x00,    // 0x44 '�c'
0x00,0x7f,0x49,0x49,0x49,0x49,0x41,0x00,    // 0x45 '�d'
0x00,0x7f,0x09,0x09,0x09,0x09,0x01,0x00,    // 0x46 '�e'
0x00,0x1c,0x22,0x41,0x49,0x49,0x3a,0x00,    // 0x47 '�f'
0x00,0x7f,0x08,0x08,0x08,0x08,0x7f,0x00,    // 0x48 '�g'
0x00,0x00,0x41,0x7f,0x41,0x00,0x00,0x00,    // 0x49 '�h'
0x00,0x20,0x40,0x40,0x40,0x40,0x3f,0x00,    // 0x4a '�i'
0x00,0x7f,0x10,0x08,0x14,0x22,0x41,0x00,    // 0x4b '�j'
0x00,0x7f,0x40,0x40,0x40,0x40,0x40,0x00,    // 0x4c '�k'
0x7f,0x02,0x0c,0x30,0x0c,0x02,0x7f,0x00,    // 0x4d '�l'  
0x00,0x7f,0x02,0x04,0x08,0x10,0x7f,0x00,    // 0x4e '�m'
0x00,0x1c,0x22,0x41,0x41,0x22,0x1c,0x00,    // 0x4f '�n'
0x00,0x7f,0x09,0x09,0x09,0x09,0x06,0x00,    // 0x50 '�o'
0x00,0x1c,0x22,0x41,0x51,0x22,0x5c,0x00,    // 0x51 '�p'
0x00,0x7f,0x09,0x09,0x19,0x29,0x46,0x00,    // 0x52 '�q'
0x00,0x26,0x49,0x49,0x49,0x49,0x32,0x00,    // 0x53 '�r'
0x01,0x01,0x01,0x7f,0x01,0x01,0x01,0x00,    // 0x54 '�s'
0x00,0x3f,0x40,0x40,0x40,0x40,0x3f,0x00,    // 0x55 '�t'  
0x03,0x0c,0x30,0x40,0x30,0x0c,0x03,0x00,    // 0x56 '�u'
0x1f,0x60,0x18,0x06,0x18,0x60,0x1f,0x00,    // 0x57 '�v'
0x41,0x22,0x14,0x08,0x14,0x22,0x41,0x00,    // 0x58 '�w'
0x01,0x02,0x04,0x78,0x04,0x02,0x01,0x00,    // 0x59 '�x'
0x00,0x41,0x61,0x51,0x49,0x45,0x43,0x00,    // 0x5a '�y'
0x00,0x00,0x00,0x00,0x7f,0x41,0x41,0x00,    // 0x5b '�m'
0x00,0x2b,0x2c,0x78,0x2c,0x2b,0x00,0x00,    // 0x5c '��'
0x41,0x41,0x7f,0x00,0x00,0x00,0x00,0x00,    // 0x5d '�n'
0x00,0x00,0x02,0x01,0x02,0x00,0x00,0x00,    // 0x5e '�O'
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,    // 0x5f '�Q'
0x05,0x03,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x60 '�M'
0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00,    // 0x61 '��'
0x00,0x7f,0x48,0x44,0x44,0x38,0x00,0x00,    // 0x62 '��'
0x00,0x38,0x44,0x44,0x44,0x28,0x00,0x00,    // 0x63 '��'
0x00,0x38,0x44,0x44,0x48,0x7f,0x00,0x00,    // 0x64 '��'
0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00,    // 0x65 '��'
0x00,0x00,0x04,0x7e,0x05,0x01,0x00,0x00,    // 0x66 '��'
0x00,0x08,0x54,0x54,0x54,0x3c,0x00,0x00,    // 0x67 '��'
0x00,0x7f,0x08,0x04,0x04,0x78,0x00,0x00,    // 0x68 '��'
0x00,0x00,0x00,0x7d,0x00,0x00,0x00,0x00,    // 0x69 '��'
0x00,0x20,0x40,0x40,0x3d,0x00,0x00,0x00,    // 0x6a '��'
0x00,0x00,0x7f,0x10,0x28,0x44,0x00,0x00,    // 0x6b '��'
0x00,0x00,0x01,0x7f,0x00,0x00,0x00,0x00,    // 0x6c '��'
0x00,0x7c,0x04,0x78,0x04,0x78,0x00,0x00,    // 0x6d '��'
0x00,0x7c,0x08,0x04,0x04,0x78,0x00,0x00,    // 0x6e '��'
0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00,    // 0x6f '��'
0x00,0x7c,0x14,0x14,0x14,0x08,0x00,0x00,    // 0x70 '��'
0x00,0x08,0x14,0x14,0x14,0x7c,0x00,0x00,    // 0x71 '��'
0x00,0x7c,0x08,0x04,0x04,0x08,0x00,0x00,    // 0x72 '��'
0x00,0x48,0x54,0x54,0x54,0x24,0x00,0x00,    // 0x73 '��'
0x00,0x04,0x3e,0x44,0x44,0x20,0x00,0x00,    // 0x74 '��'
0x00,0x3c,0x40,0x40,0x20,0x7c,0x00,0x00,    // 0x75 '��'
0x00,0x0c,0x30,0x40,0x30,0x0c,0x00,0x00,    // 0x76 '��'
0x00,0x1c,0x60,0x18,0x60,0x1c,0x00,0x00,    // 0x77 '��'
0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00,    // 0x78 '��'
0x00,0x44,0x58,0x20,0x18,0x04,0x00,0x00,    // 0x79 '��'
0x00,0x44,0x64,0x54,0x4c,0x44,0x00,0x00,    // 0x7a '��'
0x00,0x00,0x00,0x08,0x36,0x41,0x41,0x00,    // 0x7b '�o'
0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,    // 0x7c '�b'
0x41,0x41,0x36,0x08,0x00,0x00,0x00,0x00,    // 0x7d '�p'
0x08,0x04,0x04,0x08,0x10,0x10,0x08,0x00,    // 0x7e '�`'
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff		// 0x7f NULL
};

/*
�{�L�����N�^�f�[�^�́A
8�~8 �h�b�g���{��t�H���g�u����t�H���g�v
http://www.geocities.jp/littlimi/font.htm
��16�i���f�[�^�ɕϊ����Ďg�p���Ă��܂��B
*/