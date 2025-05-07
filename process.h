#ifndef  __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct process_info {
	pid_t pid;
	int arrival_time;
	int cpu_burst_time;
	int io_burst_time;
	int io_request_time;
	int priority;
} process_info;

void Create_Process(const process_info* info);

#endif