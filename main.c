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
	return 0;
}