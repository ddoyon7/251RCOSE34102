#include "process.h"

process_info process_list[PROCESS_NUMBER];
int pl_num;

void Create_Process() {
	srand(time(NULL));
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		process_list[pl_num].pid = rand() % MAX_PID + 1;
		process_list[pl_num].arrival_time = rand() % (MAX_ARRIVALTIME + 1);
		process_list[pl_num].cpu_burst_time = rand() % MAX_CPUBURSTTIME + 1;
		process_list[pl_num].priority = rand() % MAX_PRIORITY + 1;
		pl_num++;
	}
}

void Show_Process() {

	printf("----------------------------< created process >--------------------\n");
	for (int i = 0; i < pl_num; i++) printf("Process[%d] => ArrivalTime : %d, CpuBurstTime : %d, Priority : %d\n", process_list[i].pid, process_list[i].arrival_time, process_list[i].cpu_burst_time, process_list[i].priority);
	printf("-------------------------------------------------------------------\n");
}