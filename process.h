#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
* process �� ������ ����
*/
#define PROCESS_NUMBER 5
#define MAX_PID 32768
#define MAX_ARRIVALTIME 20
#define MAX_CPUBURSTTIME 20
#define MAX_PRIORITY 5
#define MAX_IOBURSTTIME 5
#define IOREQUEST_NUMBER 3
#define MAX_IOREQUESTTIME 50
#define MAX_PERIOD 5
#define REALTIME_PROCESS_NUMBER 2

/*
* process ����ü ( pid, arrivaltime, cpu_burst_time, io_burst_time, io_request_time, priority )
*/
typedef struct process_info {
	int pid;
	int arrival_time;
	int cpu_burst_time;
	int io_burst_time;
	int io_request_time[IOREQUEST_NUMBER];
	int priority; // priority�� ��������, �켱������ ����.
	int period;
} process_info;

/*
* process ����Ʈ
*/
extern process_info process_list[PROCESS_NUMBER];

/*
* qsort�� ���� CMP
*/
int CMP(const void*, const void*);

/*
* process ���� �Լ�
*/
void Create_Process();

/*
* process ��� �Լ�
*/
void Show_Process();

/*
* pid�� ���� ���μ��� Ž�� �Լ�
*/
int Get_Process_Idx(int);

#endif