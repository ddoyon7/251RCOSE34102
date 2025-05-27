#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "queue.h"
#include <string.h>

#define LOG_LIST_SIZE 100

#define GANTT_SIZE 500

#define TIME_QUANTUM 5

#define MAX_READY_TIME 10

#define BUFFER_SIZE 30

/*
* log ����ü ( time, text )
*/
typedef struct log {
	int time;
	char text[BUFFER_SIZE];
}log;

/*
* log Ÿ��
*/
enum log_type { ARRIVED, PREEMPTED, IO_REQUESTED, IO_FINISHED, PERIOD, DEADLINE_MISS, AGED };

/*
* log ����Ʈ
*/
extern log log_list[LOG_LIST_SIZE];
extern int log_num;

/*
* log �߰� �Լ�
*/
void Add_Log(int, int, int);

/*
* scheduling ���� ����
*/
extern int scheduling_time;
extern int next_process_idx;
extern process_info* running_process, * waiting_process;
extern process_info scheduling_process_list[PROCESS_NUMBER];

/*
* scheduling ���� �Լ� ( init, is_finished, is_arrived, is_iorequested, locate, is_deadline_missed )
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

/*
* realtime scheduling algorithm ( Rate_Monotonic, EDF )
*/
void Rate_Monotonic();
void EDF();

/*
* gcd, lcm �Լ�
*/
int gcd(int, int);
int lcm(int, int);

/*
* gantt_chart ����Ʈ ( scheduling_time�� pid ���� )
*/
extern int gantt_chart[GANTT_SIZE];

/*
* gantt_chart ��� �Լ�
*/
void Show_Gantt(int);

/*
* evaluation_list ( scheduling�� average waiting time, average turnarount time ���� )
*/
extern double evaluation_list[SCHEDULE_TYPE_NUMBER][2];

/*
* evaluation_list �Լ� ( compute, evaluation, clear )
*/
void Compute_Time(int, int);

void Evaluation();

void Clear_Evaluation();

#endif 