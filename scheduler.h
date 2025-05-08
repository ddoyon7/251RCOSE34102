#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "process.h"
#define GANTT_SIZE 500

enum SCHEDULE_TYPE { T_FCFS, T_SJF, T_PR, T_RR, T_PSJF, T_PPR };

extern int scheduling_time;
extern int scheduling_idx;
extern process_info* running_process;
extern process_info scheduling_process_list[PROCESS_NUMBER];

extern int gantt_chart[GANTT_SIZE];

extern int (*cmp_list[3])(const void*, const void*);

int Cmp_FCFS(const void*, const void*);

int Cmp_SJF(const void*, const void*);

int Cmp_PR(const void*, const void*);

void Init(int);

int Is_Finished();

int Is_Arrived();

void FCFS();

void SJF();

void Priority();

void RR();

void Preemptive_SJF();

void Preemptive_Priority();

void Show_Gantt(int);
#endif 