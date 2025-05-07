#include "process.h"
#include <time.h>

void Config() {
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

int main() {

	Config();

	for (int i = 0; i <= pl_num; i++) {
		if (fork() == 0) {
			Create_Process(&process_list[i]);
			exit(0);
		}
	}
	while (wait(NULL) > 0);
	return 0;
}