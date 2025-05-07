#ifndef  __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROCESS_NUMBER 5
#define QUEUE_SIZE 100
#define MAX_PID 32768
#define MAX_ARRIVALTIME 10
#define MAX_CPUBURSTTIME 10
#define MAX_PRIORITY 5

typedef struct process_info {
	pid_t pid;
	int arrival_time;
	int cpu_burst_time;
	//int io_burst_time;
	//int io_request_time;
	int priority;
} process_info;

process_info process_list[PROCESS_NUMBER];
int pl_num;

void Create_Process(const process_info*);

process_info(*ready_queue)[QUEUE_SIZE];
int rq_front, rq_rear;

void Push_Ready_Queue(const process_info*);

//process_info waiting_queue[QUEUE_SIZE];
//int wq_front, wq_rear;

#endif