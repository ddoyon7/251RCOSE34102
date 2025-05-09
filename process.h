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
#define MAX_IOBURSTTIME 5
#define IOREQUEST_NUMBER 3
#define MAX_IOREQUESTTIME 50

typedef struct process_info {
	int pid;
	int arrival_time;
	int cpu_burst_time;
	int io_burst_time;
	int io_request_time[IOREQUEST_NUMBER];
	int priority;
} process_info;

int CMP(const void*, const void*);

extern process_info process_list[PROCESS_NUMBER];
extern int pl_num;

void Create_Process();

void Show_Process();

int Get_Process_Idx(int);

#endif