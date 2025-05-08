#include "process.h"

process_info process_list[PROCESS_NUMBER];
int pl_num;

process_info* ready_queue[QUEUE_SIZE];
int rq_front, rq_rear;

void Config() {
	pl_num = 0;
	rq_front = rq_rear = -1;
}

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

void Push_Ready_Queue(process_info* pi) {
	if (Is_Full_QUEUE()) printf("ERR_QUEUE IS FULL\n");
	else ready_queue[++rq_rear] = pi;
}

process_info* Pop_Ready_Queue() {
	if (Is_Empty_QUEUE()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	return ready_queue[++rq_front];
}

int Is_Empty_QUEUE() {
	return rq_front == rq_rear;
}

int Is_Full_QUEUE() {
	return rq_rear == QUEUE_SIZE - 1;
}