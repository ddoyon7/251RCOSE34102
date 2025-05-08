#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROCESS_NUMBER 5
#define MAX_PID 32768
#define MAX_ARRIVALTIME 20
#define MAX_CPUBURSTTIME 20
#define MAX_PRIORITY 5
#define QUEUE_SIZE 100

typedef struct process_info {
	int pid;
	int arrival_time;
	int cpu_burst_time;
	//int io_burst_time;
	//int io_request_time;
	int priority;
} process_info;

void Config();

extern process_info process_list[PROCESS_NUMBER];
extern int pl_num;

int cmp(const void*, const void*);
void Create_Process();

void Show_Process();

extern process_info* ready_queue[QUEUE_SIZE];
extern int rq_front, rq_rear;

void Push_Ready_Queue(process_info*);

process_info* Pop_Ready_Queue();

int Is_Empty_QUEUE();

int Is_Full_QUEUE();

//process_info waiting_queue[QUEUE_SIZE];
//int wq_front, wq_rear;

#endif