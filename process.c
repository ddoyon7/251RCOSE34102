#include "process.h"

process_info process_list[PROCESS_NUMBER];
int pl_num;

process_info(*ready_queue)[QUEUE_SIZE];
int rq_front, rq_rear;

void Config() {
	pl_num = 0;
	srand(time(NULL));
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		process_list[pl_num].pid = rand() % MAX_PID + 1;
		process_list[pl_num].arrival_time = rand() % (MAX_ARRIVALTIME + 1);
		process_list[pl_num].cpu_burst_time = rand() % MAX_CPUBURSTTIME + 1;
		process_list[pl_num].priority = rand() % MAX_PRIORITY + 1;
		pl_num++;
	}

	rq_front = rq_rear = -1;
}


void Create_Process(const process_info* pi) {
	sleep(pi->arrival_time);
	printf("[%d] : Arrived at %ds\n", pi->pid, pi->arrival_time);
	sleep(pi->cpu_burst_time);
	printf("[%d] : CPU burst fin %ds while %ds\n", pi->pid, pi->arrival_time + pi->cpu_burst_time, pi->cpu_burst_time);
}

void Push_Ready_Queue(const process_info* pi) {

}