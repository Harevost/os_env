#include "bootpack.h"
#include <stdio.h>

void sleep(struct TASK *task, struct semaphore *S);
void awake(struct semaphore *S);

/*bootpack.h,Makefile,bootpack.c*/
void wait (struct TASK *task, struct semaphore *S) {
    io_cli(); //close interrupt
    S->value--;
    if(S->value < 0) {
        //add to sleep list
        sleep(task,S);
    }else {
        io_sti();
    }   
}

void signal(struct TASK *task, struct semaphore *S) {
    io_cli(); //close interrupt
    S->value++;
    if(S->value <= 0) {
        awake(S);
    }else {
        io_sti();
    }
}

void sleep(struct TASK *task, struct semaphore *S) {
    if(S->first == -1 && S->last == -1){
        S->first++;
    }else if( (S->last+1)%MAX_TASKS == S->first){
        // sleep list has been full
        ;
    }
    S->last = (S->last+1)%MAX_TASKS;
    S->sleep_list[S->last] = task; //add the task to the sleep list
    task_sleep2(task,1); 
    //remove the task from running list, and switch to next task, and open interrupt
}

void awake(struct semaphore *S) {
    struct TASK* task;
    if(S->first == S->last)
        ;
    task = S->sleep_list[S->first];
    S->first = (S->first+1)%MAX_TASKS; //remove the task frm the sleep list
    task_run2(task,2,2,1); //add the task to the running list, and open interrupt;
}
