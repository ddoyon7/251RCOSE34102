#include "process.h"

void Create_Process(const process_info* pi) {
	sleep(pi->arrival_time);
	printf("[%d] : Arrived at %ds\n", pi->pid, pi->arrival_time);
	sleep(pi->cpu_burst_time);
	printf("[%d] : CPU burst fin %ds\n", pi->pid, pi->arrival_time + pi->cpu_burst_time);
}