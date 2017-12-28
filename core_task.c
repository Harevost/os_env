#include "bootpack.h"
#include <stdio.h>

#define PRO0_TIMER 40
#define CON0_TIMER 90
#define BUFFER_SIZE 100

static int t = 0;
static int counter = 0;
void buf_print(struct proAndCon *pac, struct TASKSHT *task_sht);
int lookup_counter(void);

void task_prodsr0_main(struct SHEET *sht_win_b)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[563];
	char s[40];
	int num = 0;
	struct TASK *task;
	struct TASKSHT task_sht;

	task = task_now();
	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;

	fifo32_init(&fifo, 563, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, PRO0_TIMER);
	timer_settime(timer, PRO0_TIMER);

	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == PRO0_TIMER) {
				int result = lookup_counter();
				while (result >= BUFFER_SIZE) {
					result = lookup_counter();
				}

				num++;
				sprintf(s, "T%d: before Producer %d: %d", t, num, counter);
				sht_putstr0(&task_sht, s, COL8_000000);
				sht_newline(&task_sht);
				t++;

				int temp = 0;
				temp = counter;
				temp += 1;
				int j = 0;
				/*for ( ; j < 30000000; j++) {
					lookup_counter();
				}*/
				counter = temp;
				sprintf(s, "T%d: after Producer %d: %d", t, num, counter);
				t++;
				sht_putstr0(&task_sht, s, COL8_000000);
				sht_newline(&task_sht);

				timer_settime(timer, PRO0_TIMER);
			}
		}
	}
}

void task_consmr0_main(struct SHEET *sht_win_b)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[563];
	int num = 0;
	char s[40];
	struct TASK *task;
	struct TASKSHT task_sht;

	task = task_now();
	task_sht.sht = sht_win_b;
	task_sht.cur_x =  8;
	task_sht.cur_y = 28;
	task_sht.cur_c = -1;

	fifo32_init(&fifo, 563, fifobuf, 0);
	timer = timer_alloc();
	timer_init(timer, &fifo, CON0_TIMER);
	timer_settime(timer, CON0_TIMER);

	for (;;) {

		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti(); 
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == CON0_TIMER) {
				int result = lookup_counter();
				while (result <= 0) {
					result = lookup_counter();
				}
			
				int temp = 0;
				num++;
				sprintf(s, "T%d: before Consumer %d: %d", t, num, counter);
				t++;
				sht_putstr0(&task_sht, s, COL8_000000);
				sht_newline(&task_sht);
			
				temp = counter;
				temp -= 1;
				int j = 0;
				for ( ; j < 30000000; j++) {
					lookup_counter();
				}
				
				//counter = temp;
				//sprintf(s, "T%d: after Consumer %d: %d", t, num, counter);
				//t++;
				//sht_putstr0(&task_sht, s, COL8_000000);
				//sht_newline(&task_sht);

				timer_settime(timer, CON0_TIMER); 
			}
		} 
	}
}

void task_prodsr_main(struct SHEET *sht_win_b, struct proAndCon *pac)
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
	timer_init(timer, &fifo, PRO_TIMER);
	timer_settime(timer, PRO_TIMER);
 
	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == PRO_TIMER) {
				/*produce an item*/
			//	sprintf(s,"rand=",ran);
			//	sht_putstr0(&task_sht, s, COL8_000000);
				//sht_putstr0(&task_sht, "newItem", COL8_000000);
				//sht_newline(&task_sht);

				/*get semaphore*/
				/*wait(task,empty);
				sht_putstr0(&task_sht, "Producer get semaphore:empty", COL8_000000);
				sht_newline(&task_sht);	
				wait(task,mutex);
				sht_putstr0(&task_sht, "Producer get semaphore:mutex", COL8_000000);
				sht_newline(&task_sht);				
				*/
				/*add newItem to buf*/
				pac->buf[pac->in] = NEWITEM;
				sprintf(s, "Producer has put newItem[%d]\n", pac->in);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->in = (pac->in+1)%pac->size;
				
		
				/*release semaphore*/
				/*signal(task,mutex);
				sht_putstr0(&task_sht, "Producer relase semaphore:mutex", COL8_000000);
				sht_newline(&task_sht);	
				signal(task,full);
				sht_putstr0(&task_sht, "Producer release semaphore:full", COL8_000000);
				sht_newline(&task_sht);	
				*/
				timer_settime(timer, PRO_TIMER);
			}
		}
	} 
}

void task_consmr_main(struct SHEET *sht_win_b, struct proAndCon *pac)
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
	timer_init(timer, &fifo, CON_TIMER);
	timer_settime(timer, CON_TIMER);

	int newItem;

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == CON_TIMER) {
				/*get semaphore*/
				/*sht_putstr0(&task_sht, "wait for semaphore:full\n", COL8_000000);
				wait(task,full);
				sht_putstr0(&task_sht, "get!\n", COL8_000000);
		
				sht_putstr0(&task_sht, "wait for semaphore:mutex\n", COL8_000000);
				wait(task,mutex);
				sht_putstr0(&task_sht, "get!\n", COL8_000000);*/			

				/*get an item from buf*/
				newItem = pac->buf[pac->out];
				pac->buf[pac->out] = NULL;
				sprintf(s, "Consumer has get newItem[%d]\n", pac->out);
				sht_putstr0(&task_sht, s, COL8_000000);
				pac->out = (pac->out+1)%pac->size;
				
				//sht_putstr0(&task_sht, "Consumer has get a newItem from the buf\n", COL8_000000);
				
				/*release semaphore*/
				/*signal(task,mutex);
				sht_putstr0(&task_sht, "relase semaphore:mutex\n", COL8_000000);	
				signal(task,empty);
				sht_putstr0(&task_sht, "release semaphore:empty\n", COL8_000000);
				*/
				timer_settime(timer, CON_TIMER);
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
				 //sprintf(s, "pac->in=%d, ", pac->in);
				 //sht_putstr0(&task_sht,s, COL8_FF0000);
				 //sprintf(s, "pac->out=%d", pac->out);
				 //sht_putstr0(&task_sht,s, COL8_FF0000);				
				 //sht_newline(&task_sht);
				 //count0 = count;
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

int lookup_counter(void)
{
	return counter;
}
