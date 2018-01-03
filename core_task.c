#include "bootpack.h"
#include <stdio.h>

void buf_print(struct proAndCon *pac, struct TASKSHT *task_sht);

void task_prodsr_main0(struct SHEET *sht_win_b, struct proAndCon *pac, int protime)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[128];
	char s[40];
	struct TASK *task;
	struct TASKSHT task_sht;
	struct semaphore *empty, *full, *mutex;

	task = task_now();
	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;
	empty = pac->empty;
	full = pac->full;
	mutex = pac->mutex;

	fifo32_init(&fifo, 128, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, protime);
	timer_settime(timer, protime);
 
	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == protime) {
				/*add newItem to buf*/
				pac->buf[pac->in] = NEWITEM;
				sprintf(s, "Producer has put newItem[%d]\n", pac->in);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->in = (pac->in+1)%pac->size;
			
				timer_settime(timer, protime);
			}
		}
	} 
}

void task_consmr_main0(struct SHEET *sht_win_b, struct proAndCon *pac, int contime)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[128], count = 0;
	char s[40];
	struct TASK *task;
	struct TASKSHT task_sht;
	struct semaphore *empty, *full, *mutex;

	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;
	task = task_now();
	empty = pac->empty;
	full = pac->full;
	mutex = pac->mutex;

	fifo32_init(&fifo, 128, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, contime);
	timer_settime(timer, contime);

	int newItem;

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == contime) {
				/*get an item from buf*/
				newItem = pac->buf[pac->out];
				pac->buf[pac->out] = NULL;
				sprintf(s, "Consumer has get newItem[%d]\n", pac->out);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->out = (pac->out + 1) % pac->size;

				timer_settime(timer, contime);
			}
		}
	}
}

void task_prodsr_main(struct SHEET *sht_win_b, struct proAndCon *pac, int protime) 
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[128];
	struct TASK *task;
	struct TASKSHT task_sht;
	struct semaphore *empty, *full, *mutex;

	task = task_now();
	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;
	empty = pac->empty;
	full = pac->full;
	mutex = pac->mutex;

	fifo32_init(&fifo, 128, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, protime);
	timer_settime(timer, protime);
 
	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == protime) {
				/*produce an item*/

				/*get semaphore*/
				wait(task,empty);
				sht_putstr0(&task_sht, "Producer get semaphore:empty\n", COL8_000000);
				wait(task,mutex);
				sht_putstr0(&task_sht, "Producer get semaphore:mutex\n", COL8_000000);			

				/*add newItem to buf*/
				char s[40];
				pac->buf[pac->in] = NEWITEM;
				sprintf(s, "Producer has put newItem[%d]\n", pac->in);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->in = (pac->in + 1) % pac->size;
		
				/*release semaphore*/
				signal(task,mutex);
				sht_putstr0(&task_sht, "Producer release semaphore:mutex\n", COL8_000000);	
				signal(task,full);
				sht_putstr0(&task_sht, "Producer release semaphore:full\n", COL8_000000);

				timer_settime(timer, protime);
			}
		}
	} 
}

void task_consmr_main(struct SHEET *sht_win_b, struct proAndCon *pac, int contime)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[128], count = 0;
	struct TASK *task;
	struct TASKSHT task_sht;
	struct semaphore *empty, *full, *mutex;

	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;
	task = task_now();
	empty = pac->empty;
	full = pac->full;
	mutex = pac->mutex;

	fifo32_init(&fifo, 128, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, contime);
	timer_settime(timer, contime);

	int newItem;

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == contime) {
				/*get semaphore*/
				sht_putstr0(&task_sht, "wait for semaphore:full\n", COL8_000000);
				wait(task,full);
				sht_putstr0(&task_sht, "get S: full\n", COL8_000000);
		
				sht_putstr0(&task_sht, "wait for semaphore:mutex\n", COL8_000000);
				wait(task,mutex);
				sht_putstr0(&task_sht, "get S: mutex\n", COL8_000000);			

				/*get an item from buf*/
				char s[40];
				newItem = pac->buf[pac->out];
				pac->buf[pac->out] = NULL;
				sprintf(s, "Consumer has get newItem[%d]\n", pac->out);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->out = (pac->out + 1) % pac->size;
		
				/*release semaphore*/
				signal(task,mutex);
				sht_putstr0(&task_sht, "release semaphore:mutex\n", COL8_000000);	
				signal(task,empty);
				sht_putstr0(&task_sht, "release semaphore:empty\n", COL8_000000);

				timer_settime(timer, contime);
			}
		}
	}
}

void task_pacSta_main(struct SHEET *sht_win_b, struct proAndCon *pac)
{
	struct FIFO32 fifo;
	struct TIMER *timer_1s;
	int i, fifobuf[128], count = 0; //count0 = 0;
	//char s[12];
	int in, out;// buf[BUF_SIZE];
	struct TASKSHT task_sht;
	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;
	in = pac->in;
	out = pac->out;

	fifo32_init(&fifo, 128, fifobuf, 0);
	timer_1s = timer_alloc();
	timer_init(timer_1s, &fifo, 100);
	timer_settime(timer_1s, 100);
	
	sht_putstr0(&task_sht,"Producer:", COL8_000000);
	sht_putstr0(&task_sht,"$",COL8_FF0000);
	sht_putstr0(&task_sht,"	Consumer:", COL8_000000);
	sht_putstr0(&task_sht,"#",COL8_0000FF);
	sht_newline(&task_sht);

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == 100) {
				buf_print(pac, &task_sht);
				timer_settime(timer_1s, 100);
			}
		}
	}
}

void buf_print(struct proAndCon *pac, struct TASKSHT *task_sht) {
	/*Producer:$ COL8_FF0000 Consumer:# COL8_0000FF*/
	int n;
	int *buf = pac->buf;
	int cur_x = task_sht->cur_x, cur_y = task_sht->cur_y;
	for(n = 0; n < BUF_SIZE; n++) {
		if( n % 20 == 0)
			sht_newline(task_sht);
		if(buf[n] == NEWITEM) {
			sht_putstr0(task_sht,"$ ", COL8_FF0000);
		} else if(buf[n] == NULL) {
			sht_putstr0(task_sht,"# ", COL8_0000FF);
		} else {
			sht_putstr0(task_sht,"_ ", COL8_000000);
		}
	}
	task_sht->cur_x = cur_x;
	task_sht->cur_y = cur_y;
}
