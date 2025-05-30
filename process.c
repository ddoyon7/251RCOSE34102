#include "process.h"

process_info process_list[PROCESS_NUMBER];

/*
* process의 arrival_time을 기준으로 비교하여 1, -1, 0을 반환한다.
*/
int CMP(const void* p1, const void* p2) {
	if (((process_info*)p1)->arrival_time > ((process_info*)p2)->arrival_time) return 1;
	else if (((process_info*)p1)->arrival_time < ((process_info*)p2)->arrival_time) return -1;
	return 0;
}

/*
* PORCESS_NUMBER만큼의 process를 생성하고, 각 데이터를 난수로 설정한다.
* arrival_time을 기준으로 오름차순 정렬하여 저장한다.
*/
void Create_Process() {
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		process_list[i].pid = rand() % MAX_PID + 1;
		process_list[i].arrival_time = rand() % (MAX_ARRIVALTIME + 1);
		process_list[i].cpu_burst_time = rand() % MAX_CPUBURSTTIME + 1;
		process_list[i].priority = rand() % MAX_PRIORITY + 1;
		process_list[i].io_burst_time = rand() % MAX_IOBURSTTIME + 1;
		process_list[i].period = MAX_CPUBURSTTIME + 5 * (rand() % MAX_PERIOD);
		for (int j = 0; j < IOREQUEST_NUMBER; j++) process_list[i].io_request_time[j] = rand() % (MAX_IOREQUESTTIME + 1);
	}
	/* debug code*/
	/*process_info temp_list[PROCESS_NUMBER] = {
		{1,3,25,5,{31,19,4},1, 50},
		{2,7,35,4,{33,37,5},5 ,80},
		{21805,8,30,2,{31,47,29}, 3, 100},
		{30538,18,20,3,{4,35,44},2,30},
		{18573,20,10,2,{8,32,30},2,30}
	};
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		process_list[i].pid = temp_list[i].pid;
		process_list[i].arrival_time = temp_list[i].arrival_time;
		process_list[i].cpu_burst_time = temp_list[i].cpu_burst_time;
		process_list[i].priority = temp_list[i].priority;
		process_list[i].io_burst_time = temp_list[i].io_burst_time;
		process_list[i].period = temp_list[i].period;
		for (int j = 0; j < IOREQUEST_NUMBER; j++) process_list[i].io_request_time[j] = temp_list[i].io_request_time[j];
	}*/
	qsort(process_list, PROCESS_NUMBER, sizeof(process_info), CMP);
	printf("5 Processes Created.");
}

/*
* 생성된 process의 정보를 출력한다.
*/
void Show_Process() {
	printf("--------------------------------------------------------< Created Processes >------------------------------------------------\n");
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		printf("Process %5d | ArrivalTime : %2d, CpuBurstTime : %2d, Priority : %2d, IoBurstTime : %2d, ", process_list[i].pid, process_list[i].arrival_time, process_list[i].cpu_burst_time, process_list[i].priority, process_list[i].io_burst_time);
		printf("IoRequestTime : ");
		for (int j = 0; j < IOREQUEST_NUMBER; j++) printf("%2d, ", process_list[i].io_request_time[j]);
		printf("Period : %2d\n", process_list[i].period);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

/*
* process의 pid를 통해 process 리스트의 인덱스를 반환한다.
* pid가 리스트 내 존재하지 않다면 0을 반환한다.
*/
int Get_Process_Idx(int pid) {
	for (int i = 0; i < PROCESS_NUMBER; i++) if (process_list[i].pid == pid) return i;
	return 0;
}