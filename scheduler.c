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
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_FCFS);
		if (!Is_Empty_QUEUE() && running_process == NULL) running_process = Pop_Ready_Queue(T_FCFS);
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

void SJF() {
	printf("< SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_SJF);
		if (!Is_Empty_QUEUE() && running_process == NULL) running_process = Pop_Ready_Queue(T_SJF);
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< SJF() end >\n");
}

void Priority() {
	printf("< Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_PR);
		if (!Is_Empty_QUEUE() && running_process == NULL) running_process = Pop_Ready_Queue(T_PR);
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Priority() end >\n");
}

void RR() {
	printf("< RR() start >\n");
	Init();
	int time_count = TIME_QUANTUM;
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_RR);
		if (!Is_Empty_QUEUE() && running_process == NULL) {
			running_process = Pop_Ready_Queue(T_RR);
			time_count = TIME_QUANTUM;
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			time_count--;
			if (time_count == 0 || running_process->cpu_burst_time == 0) {
				if (running_process->cpu_burst_time != 0) Push_Ready_Queue(running_process, T_RR);
				running_process = NULL;
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< RR() end >\n");
}

void Preemptive_SJF() {
	printf("< Preemptive_SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_SJF);
		if (!Is_Empty_QUEUE()) {
			if (running_process == NULL) running_process = Pop_Ready_Queue(T_SJF);
			else if (running_process != NULL && Get_Front_Ready_Queue()->cpu_burst_time < running_process->cpu_burst_time) {
				Push_Ready_Queue(running_process, T_SJF);
				running_process = Pop_Ready_Queue(T_SJF);
			}
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_SJF() end >\n");
}

void Preemptive_Priority() {
	printf("< Preemptive_Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_PR);
		if (!Is_Empty_QUEUE()) {
			if (running_process == NULL) running_process = Pop_Ready_Queue(T_PR);
			else if (running_process != NULL && Get_Front_Ready_Queue()->cpu_burst_time < running_process->cpu_burst_time) {
				Push_Ready_Queue(running_process, T_PR);
				running_process = Pop_Ready_Queue(T_PR);
			}
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_Priority() end >\n");
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