#include "scheduler.h"

int main() {
	//Config();
	Create_Process();
	Show_Process();
	FCFS();
	printf("\n\n");
	SJF();
	printf("\n\n");
	Priority();
	printf("\n\n");
	RR();
	return 0;
}