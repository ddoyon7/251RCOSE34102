#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "queue.h"

#define GANTT_SIZE 500
#define TIME_QUANTUM 5

extern int scheduling_time;
extern int scheduling_idx;
extern process_info* running_process;
extern process_info scheduling_process_list[PROCESS_NUMBER];
extern double evaluation_list[6][2];

extern int gantt_chart[GANTT_SIZE];

void Init();

int Is_Finished();

int Is_Arrived();

int Is_IoRequested();

void FCFS();

void SJF();

void Priority();

void RR();

void Preemptive_SJF();

void Preemptive_Priority();

void Show_Gantt(int);

void Compute_Time(int, int);

void Evaluation();
#endif 