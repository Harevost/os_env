/* ƒ}ƒ‹ƒ`ƒ^ƒXƒNŠÖŒW */

#include "bootpack.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

struct TASK *task_now(void)
{
	struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
	return tl->tasks[tl->now];
}

void task_add(struct TASK *task)
{
	struct TASKLEVEL *tl = &taskctl->level[task->level];
	tl->tasks[tl->running] = task;
	tl->running++;
	task->flags = 2; /* “®ì’† */
	return;
}

void task_remove(struct TASK *task)
{
	int i;
	struct TASKLEVEL *tl = &taskctl->level[task->level];

	/* task‚ª‚Ç‚±‚É‚¢‚é‚©‚ğ’T‚· */
	for (i = 0; i < tl->running; i++) {
		if (tl->tasks[i] == task) {
			/* ‚±‚±‚É‚¢‚½ */
			break;
		}
	}

	tl->running--;
	if (i < tl->now) {
		tl->now--; /* ‚¸‚ê‚é‚Ì‚ÅA‚±‚ê‚à‚ ‚í‚¹‚Ä‚¨‚­ */
	}
	if (tl->now >= tl->running) {
		/* now‚ª‚¨‚©‚µ‚È’l‚É‚È‚Á‚Ä‚¢‚½‚çAC³‚·‚é */
		tl->now = 0;
	}
	task->flags = 1; /* ƒXƒŠ[ƒv’† */

	/* ‚¸‚ç‚µ */
	for (; i < tl->running; i++) {
		tl->tasks[i] = tl->tasks[i + 1];
	}

	return;
}

void task_switchsub(void)
{
	int i;
	/* ˆê”Ôã‚ÌƒŒƒxƒ‹‚ğ’T‚· */
	for (i = 0; i < MAX_TASKLEVELS; i++) {
		if (taskctl->level[i].running > 0) {
			break; /* Œ©‚Â‚©‚Á‚½ */
		}
	}
	taskctl->now_lv = i;
	taskctl->lv_change = 0;
	return;
}

void task_idle(void)
{
	for (;;) {
		io_hlt();
	}
}

struct TASK *task_init(struct MEMMAN *memman)
{
	int i;
	struct TASK *task, *idle;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;

	taskctl = (struct TASKCTL *) memman_alloc_4k(memman, sizeof (struct TASKCTL));
	for (i = 0; i < MAX_TASKS; i++) {
		taskctl->tasks0[i].flags = 0;
		taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
		set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
	}
	for (i = 0; i < MAX_TASKLEVELS; i++) {
		taskctl->level[i].running = 0;
		taskctl->level[i].now = 0;
	}

	task = task_alloc();
	task->flags = 2;	/* “®ì’†ƒ}[ƒN */
	task->priority = 2; /* 0.02•b */
	task->level = 0;	/* Å‚ƒŒƒxƒ‹ */
	task_add(task);
	task_switchsub();	/* ƒŒƒxƒ‹İ’è */
	load_tr(task->sel);
	task_timer = timer_alloc();
	timer_settime(task_timer, task->priority);

	idle = task_alloc();
	idle->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024;
	idle->tss.eip = (int) &task_idle;
	idle->tss.es = 1 * 8;
	idle->tss.cs = 2 * 8;
	idle->tss.ss = 1 * 8;
	idle->tss.ds = 1 * 8;
	idle->tss.fs = 1 * 8;
	idle->tss.gs = 1 * 8;
	task_run(idle, MAX_TASKLEVELS - 1, 1);

	return task;
}

struct TASK *task_alloc(void)
{
	int i;
	struct TASK *task;
	for (i = 0; i < MAX_TASKS; i++) {
		if (taskctl->tasks0[i].flags == 0) {
			task = &taskctl->tasks0[i];
			task->flags = 1; /* g—p’†ƒ}[ƒN */
			task->tss.eflags = 0x00000202; /* IF = 1; */
			task->tss.eax = 0; /* ‚Æ‚è‚ ‚¦‚¸0‚É‚µ‚Ä‚¨‚­‚±‚Æ‚É‚·‚é */
			task->tss.ecx = 0;
			task->tss.edx = 0;
			task->tss.ebx = 0;
			task->tss.ebp = 0;
			task->tss.esi = 0;
			task->tss.edi = 0;
			task->tss.es = 0;
			task->tss.ds = 0;
			task->tss.fs = 0;
			task->tss.gs = 0;
			task->tss.ldtr = 0;
			task->tss.iomap = 0x40000000;
			task->tss.ss0 = 0;
			return task;
		}
	}
	return 0; /* ‚à‚¤‘S•”g—p’† */
}

void task_run(struct TASK *task, int level, int priority)
{
	if (level < 0) {
		level = task->level; /* ƒŒƒxƒ‹‚ğ•ÏX‚µ‚È‚¢ */
	}
	if (priority > 0) {
		task->priority = priority;
	}

	if (task->flags == 2 && task->level != level) { /* “®ì’†‚ÌƒŒƒxƒ‹‚Ì•ÏX */
		task_remove(task); /* ‚±‚ê‚ğÀs‚·‚é‚Æflags‚Í1‚É‚È‚é‚Ì‚Å‰º‚Ìif‚àÀs‚³‚ê‚é */
	}
	if (task->flags != 2) {
		/* ƒXƒŠ[ƒv‚©‚ç‹N‚±‚³‚ê‚éê‡ */
		task->level = level;
		task_add(task);
	}

	taskctl->lv_change = 1; /* Ÿ‰ñƒ^ƒXƒNƒXƒCƒbƒ`‚Ì‚Æ‚«‚ÉƒŒƒxƒ‹‚ğŒ©’¼‚· */
	return;
}

void task_run2(struct TASK *task, int level, int priority, int openInt)
{
	if (level < 0) {
		level = task->level; /* ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ÏXï¿½ï¿½ï¿½È‚ï¿½ */
	}
	if (priority > 0) {
		task->priority = priority;
	}

	if (task->flags == 2 && task->level != level) { /* ï¿½ï¿½ï¿½ì’†ï¿½Ìƒï¿½ï¿½xï¿½ï¿½ï¿½Ì•ÏX */
		task_remove(task); /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½flagsï¿½ï¿½1ï¿½É‚È‚ï¿½Ì‚Å‰ï¿½ï¿½ï¿½ifï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ */
	}
	if (task->flags != 2) {
		/* ï¿½Xï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‡ */
		task->level = level;
		task_add(task);
	}

	taskctl->lv_change = 1; /* ï¿½ï¿½ï¿½ï¿½^ï¿½Xï¿½Nï¿½Xï¿½Cï¿½bï¿½`ï¿½Ì‚Æ‚ï¿½ï¿½Éƒï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */

	if(openInt == 1)
		io_sti();
	return;
}

void task_sleep(struct TASK *task)
{
	struct TASK *now_task;
	if (task->flags == 2) {
		/* “®ì’†‚¾‚Á‚½‚ç */
		now_task = task_now();
		task_remove(task); /* ‚±‚ê‚ğÀs‚·‚é‚Æflags‚Í1‚É‚È‚é */
		if (task == now_task) {
			/* ©•ª©g‚ÌƒXƒŠ[ƒv‚¾‚Á‚½‚Ì‚ÅAƒ^ƒXƒNƒXƒCƒbƒ`‚ª•K—v */
			task_switchsub();
			now_task = task_now(); /* İ’èŒã‚Å‚ÌAuŒ»İ‚Ìƒ^ƒXƒNv‚ğ‹³‚¦‚Ä‚à‚ç‚¤ */
			farjmp(0, now_task->sel);
		}
	}
	return;
}

void task_sleep2(struct TASK *task, int openInt)
{
	struct TASK *now_task;
	if (task->flags == 2) {
		/* ï¿½ï¿½ï¿½ì’†ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
		now_task = task_now();
		task_remove(task); /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½flagsï¿½ï¿½1ï¿½É‚È‚ï¿½ */
		if (task == now_task) {
			/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ÌƒXï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ÅAï¿½^ï¿½Xï¿½Nï¿½Xï¿½Cï¿½bï¿½`ï¿½ï¿½ï¿½Kï¿½v */
			task_switchsub();
			now_task = task_now(); /* ï¿½İ’ï¿½ï¿½Å‚ÌAï¿½uï¿½ï¿½ï¿½İ‚Ìƒ^ï¿½Xï¿½Nï¿½vï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ç‚¤ */
			farjmp(0, now_task->sel);
		}
	}
	if(openInt == 1) //kaizhongduan
		io_sti();
	return;
}

void task_switch(void)
{
	struct TASKLEVEL *tl = &taskctl->level[taskctl->now_lv];
	struct TASK *new_task, *now_task = tl->tasks[tl->now];
	tl->now++;
	if (tl->now == tl->running) {
		tl->now = 0;
	}
	if (taskctl->lv_change != 0) {
		task_switchsub();
		tl = &taskctl->level[taskctl->now_lv];
	}
	new_task = tl->tasks[tl->now];
	timer_settime(task_timer, new_task->priority);
	if (new_task != now_task) {
		farjmp(0, new_task->sel);
	}
	return;
}
