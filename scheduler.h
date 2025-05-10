#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "queue.h"
#include <string.h>
#define GANTT_SIZE 500
#define TIME_QUANTUM 5
#define LOG_LIST_SIZE 100

typedef struct log {
	int time;
	char text[30];
}log;

enum log_type { ARRIVED, PREEMPTED, IO_REQUESTED, IO_FINISHED };

extern int scheduling_time;
extern int scheduling_idx;
extern process_info* running_process;
extern process_info scheduling_process_list[PROCESS_NUMBER];
extern log log_list[100];
extern int log_num;
extern int gantt_chart[GANTT_SIZE];

void Init();

void Add_Log(int, int, int);

log* Get_Front_Log();

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