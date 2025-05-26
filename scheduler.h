#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "queue.h"
#include <string.h>

#define LOG_LIST_SIZE 100

#define GANTT_SIZE 500

#define TIME_QUANTUM 5

/*
* log 구조체 ( time, text )
*/
typedef struct log {
	int time;
	char text[30];
}log;

/*
* log 타입
*/
enum log_type { ARRIVED, PREEMPTED, IO_REQUESTED, IO_FINISHED, PERIOD, DEADLINE_MISS };

/*
* log 리스트
*/
extern log log_list[LOG_LIST_SIZE];
extern int log_num;

/*
* log 추가 함수
*/
void Add_Log(int, int, int);

/*
* scheduling 구성 변수
*/
extern int scheduling_time;
extern int next_process_idx;
extern process_info* running_process, * waiting_process;
extern process_info scheduling_process_list[PROCESS_NUMBER];

/*
* scheduling 구성 함수 ( init, is_finished, is_arrived, is_iorequested, locate, is_deadline_missed )
*/
void Init();

int Is_Finished();

int Is_Arrived();

int Is_IoRequested();

void Locate_Ready_To_Running(int);

void Locate_Running_To_Wait(int);

void Locate_Wait_To_Ready(int);

int Is_Deadline_Missed(int);

/*
* scheduling algorithm ( FCFS, SJF, Priority, RR, Preemptive_SJF, Preemptive_Priority )
*/
void FCFS();
void SJF();
void Priority();
void RR();
void Preemptive_SJF();
void Preemptive_Priority();
void Rate_Monotonic();
void EDF();

/*
* gcd, lcm 함수
*/
int gcd(int, int);
int lcm(int, int);

/*
* gantt_chart 리스트 ( scheduling_time별 pid 저장 )
*/
extern int gantt_chart[GANTT_SIZE];

/*
* gantt_chart 출력 함수
*/
void Show_Gantt(int);

/*
* evaluation_list ( scheduling별 average waiting time, average turnarount time 저장 )
*/
extern double evaluation_list[SCHEDULE_TYPE_NUMBER][2];

/*
* evaluation_list 함수 ( compute, evaluation, clear )
*/
void Compute_Time(int, int);

void Evaluation();

void Clear_Evaluation();

#endif 