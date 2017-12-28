#include "bootpack.h"
#include <stdio.h>
#include<string.h>
#define BUFFER_SIZE 100

static int t=0; 
static int counter = 0;
int LookupCounter();

int LookupCounter(){
	return counter;
}

void task_consumer(struct SHEET *sht_win_b)
{
	struct FIFO32 fifo;
	struct TIMER *timer_1s;
	int i, fifobuf[563];//547+16 = 563
	char s[40];
	int count = 90; 
	int num = 0; 

	fifo32_init(&fifo, 563, fifobuf, 0);
	timer_1s = timer_alloc();
	timer_init(timer_1s, &fifo, count);
	timer_settime(timer_1s, count);
	struct CONSOLE cons;
	cons.x0 = 168;
	cons.y0 = 56;
	cons.cur_x = 8;
	cons.cur_y = 28;
	cons.sht = sht_win_b;
	cons.xsize = 384;
	cons.ysize = 547;//600-53

	for (;;) {
	
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} 
		else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == count) {
			int result = LookupCounter();
			while(result<= 0){

			result = LookupCounter();
			}
			int temp = 0;
	
			num++;
			sprintf(s, "T%d:before consumer%d: %d", t,num,counter);//�������ӡ�����ߺ������ߵ�counter
			t++;
			cons_putstr1(&cons, s, 40);
			cons_newline(&cons);
			
			temp = counter;
			temp = temp - 1;
			int j = 0;
			for(;j<30000000;j++) //���������ռʱ��� 
				LookupCounter();
			counter = temp;
			sprintf(s, "T%d:after consumer%d: %d", t,num,counter);//�������ӡ�����ߺ������ߵ�counter 
			t++;
			cons_putstr1(&cons,s, 40);
			cons_newline(&cons);
			
			timer_settime(timer_1s, count);
			

		}
		}
	}
	
	
}

void task_producer(struct SHEET *sht_win_b)
{

	struct FIFO32 fifo;
	struct TIMER *timer_1s;
	int i, fifobuf[563];
	int count = 40;
	int num = 0;
	char s[40];

	fifo32_init(&fifo, 563, fifobuf, 0);
	timer_1s = timer_alloc();
	timer_init(timer_1s, &fifo, count);
	timer_settime(timer_1s, count);
	
	struct CONSOLE cons;
	cons.x0 = 600;
	cons.y0 = 56;
	cons.cur_x = 8;
	cons.cur_y = 28;
	cons.sht = sht_win_b;
	cons.xsize = 384;
	cons.ysize = 547;
	
	for (;;) {
		
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == count) {
			int result = LookupCounter();
			while(result>=BUFFER_SIZE){

			result = LookupCounter();
			}
			//else{
			num++;
			sprintf(s, "T%d:before producer%d: %d", t,num,counter);//�������ӡ�����ߺ������ߵ�counter
			cons_putstr1(&cons, s, 40);
			cons_newline(&cons);
			t++;
			int temp = 0;
			temp = counter;
			temp = temp + 1;
			int j = 0;
			for(;j<30000000;j++) //���������ռʱ��� 
				LookupCounter();
			counter = temp;
			sprintf(s, "T%d:after producer%d: %d", t,num,counter);//�������ӡ�����ߺ������ߵ�counter 
			cons_putstr1(&cons,s, 40);
			cons_newline(&cons);
			t++;
			timer_settime(timer_1s, count);
			}
			

		}
	}
}


/*void cons_newline(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	if (cons->cur_y < 28 + 544) { 
		cons->cur_y += 16; 
	} else {
	
		for (y = 28; y < 28 + 544; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
			}
		}
		for (y = 28 + 547; y < 28 + 560; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = COL8_C6C6C6;
			//	sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 563);
			}
		}
		sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 560);
	}
	cons->cur_x = 8;
	return;
}

*/

/*void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
	char s[2];
	s[0] = chr;
	s[1] = 0;
	if (s[0] == 0x09) {	
		for (;;) {
			putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_000000, COL8_C6C6C6, " ", 1);
			cons->cur_x += 8;
			if (cons->cur_x == cons->x0 + cons->xsize) {
				cons_newline(cons);
			}
			if (((cons->cur_x - 8) & 0x1f) == 0) {
				break;	
			}
		}
	} else if (s[0] == 0x0a) {	
		cons_newline(cons);
	} else if (s[0] == 0x0d) {
		
	} else {	
		putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y,COL8_000000, COL8_C6C6C6, s, 1);
		if (move != 0) {
			
			cons->cur_x += 8;
			if (cons->cur_x == cons->x0 + cons->xsize) {
				cons_newline(cons);
			}
		}
	}
	return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l)
{

	int i;
	for (i = 0; i < l; i++) {
		cons_putchar(cons, s[i], 1);
	}

	return;
}

*/