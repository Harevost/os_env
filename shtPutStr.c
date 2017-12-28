#include "bootpack.h"
#include <stdio.h>

void sht_putchar(struct TASKSHT *sht,int chr, char move, int color)
{
	char s[2];
	struct SHEET *sheet = sht->sht;
	s[0] = chr;
	s[1] = 0;
	if (s[0] == 0x09) {	/* �^�u */
		for (;;) {
			putfonts8_asc_sht(sht, sht->cur_x, sht->cur_y, color, COL8_C6C6C6, " ", 1);
			sht->cur_x += 8;
			if (sht->cur_x == 8 + 312) {
				sht_newline(sht);
			}
			if (((sht->cur_x - 8) & 0x1f) == 0) {
				break;	/* 32�Ŋ���؂ꂽ��break */
			}
		}
	} else if (s[0] == 0x0a) {	/* ���s */
		sht_newline(sht);
	} else if (s[0] == 0x0d) {	/* ���A */
		/* �Ƃ肠�����Ȃɂ��Ȃ� */
	} else {	/* ���ʂ̕��� */
		putfonts8_asc_sht(sht->sht, sht->cur_x, sht->cur_y, color, COL8_C6C6C6, s, 1);
		if (move != 0) {
			/* move��0�̂Ƃ��̓J�[�\����i�߂Ȃ� */
			sht->cur_x += 8;
			if (sht->cur_x == 8 +312) {
				sht_newline(sht);
			}
		}
	}
	return;
}

void sht_newline(struct TASKSHT *sht)
{
	int x, y;
	struct SHEET *sheet = sht->sht;
	if (sht->cur_y < 28 + 208) {
		sht->cur_y += 16; /* ���̍s�� */
	} else {
		/* �X�N���[�� */
		for (y = 28; y < 28 + 208; y++) {
			for (x = 8; x < 8 + 312; x++) {
				sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
			}
		}
		for (y = 28 + 208; y < 28 + 224; y++) {
			for (x = 8; x < 8 + 312; x++) {
				sheet->buf[x + y * sheet->bxsize] = COL8_C6C6C6;
			}
		}
		sheet_refresh(sheet, 8, 28, 8 + 312, 28 + 224);
	}
	sht->cur_x = 8;
	return;
}

void sht_putstr0(struct TASKSHT *sht, char *s, int color)
{
	for (; *s != 0; s++) {
		sht_putchar(sht, *s, 1, color);
	}
	return;
}

void sht_putstr1(struct TASKSHT *sht, char *s, int l, int color)
{
	int i;
	for (i = 0; i < l; i++) {
		sht_putchar(sht, s[i], 1, color);
	}
	return;
}

void box_draw(struct TASKSHT *task_sht, char color) {
	struct SHEET *sheet = task_sht->sht;
	int x = task_sht->cur_x;
	int y = task_sht->cur_y;
	boxfill8(sheet->buf, sheet->bxsize, color, x, y, x + 2 * 8 - 1, y + 15);
	task_sht->cur_x += 8;
	if (task_sht->cur_x == 8 +312) {
		sht_newline(task_sht);
	}
}

/*void putfonts8_asc_sht2(struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
	sheet_refresh(sht, x, y, x + l * 8, y + 16);
	return;
}*/ 
