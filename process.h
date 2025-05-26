#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
* process 내 데이터 범위
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
* process 구조체 ( pid, arrivaltime, cpu_burst_time, io_burst_time, io_request_time, priority )
*/
typedef struct process_info {
	int pid;
	int arrival_time;
	int cpu_burst_time;
	int io_burst_time;
	int io_request_time[IOREQUEST_NUMBER];
	int priority; // priority가 낮을수록, 우선순위가 높다.
	int period;
} process_info;

/*
* process 리스트
*/
extern process_info process_list[PROCESS_NUMBER];

/*
* qsort를 위한 CMP
*/
int CMP(const void*, const void*);

/*
* process 생성 함수
*/
void Create_Process();

/*
* process 출력 함수
*/
void Show_Process();

/*
* pid를 통한 프로세스 탐색 함수
*/
int Get_Process_Idx(int);

#endif