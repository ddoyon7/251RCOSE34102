#include "scheduler.h"

int scheduling_time;
int scheduling_idx;
process_info* running_process;
process_info scheduling_process_list[PROCESS_NUMBER];
int gantt_chart[GANTT_SIZE];

void Init() {
	scheduling_time = 0;
	scheduling_idx = 0;
	running_process = NULL;
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		scheduling_process_list[i].pid = process_list[i].pid;
		scheduling_process_list[i].arrival_time = process_list[i].arrival_time;
		scheduling_process_list[i].cpu_burst_time = process_list[i].cpu_burst_time;
		scheduling_process_list[i].priority = process_list[i].priority;
	}
}

int Is_Finished() {
	for (int i = 0; i < PROCESS_NUMBER; i++) if (scheduling_process_list[i].cpu_burst_time) return 0;
	return 1;
}

int Is_Arrived() {
	return scheduling_process_list[scheduling_idx].arrival_time == scheduling_time;
}

void FCFS() {
	printf("< FCFS() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++]);
		if (!Is_Empty_QUEUE() && running_process == NULL) running_process = Pop_Ready_Queue();
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< FCFS() end >\n");
}

void Show_Gantt(int end_time) {
	int bar[GANTT_SIZE];
	bar[0] = 1;
	for (int i = 1; i <= end_time; i++) {
		bar[i] = 0;
		if (gantt_chart[i] != gantt_chart[i - 1]) bar[i] = 1;
	}
	printf("------ 0\n");
	for (int i = 1; i <= end_time; i++) {
		if (bar[i]) {
			if (i != 0 && gantt_chart[i - 1]) printf("%6d\n", gantt_chart[i - 1]);
			printf("------ %d\n", i);
		}
	}
}