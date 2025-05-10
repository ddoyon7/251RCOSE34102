#include "scheduler.h"

int scheduling_time;
int next_process_idx;
process_info* running_process, * waiting_process;
process_info scheduling_process_list[PROCESS_NUMBER];
int gantt_chart[GANTT_SIZE];
log log_list[100];
int log_num;
double evaluation_list[SCHEDULE_TYPE_NUMBER][2];

/*
* 상황별 log를 문자열로 log_list에 저장한다.
*/
void Add_Log(int pid, int time, int type) {
	switch (type)
	{
	case ARRIVED:
		sprintf(log_list[log_num].text, "(Process %d Arrived) ", pid);
		break;
	case PREEMPTED:
		sprintf(log_list[log_num].text, "(Preempted by Process %d) ", pid);
		break;
	case IO_REQUESTED:
		sprintf(log_list[log_num].text, "(Process %d IO Requested) ", pid);
		break;
	case IO_FINISHED:
		sprintf(log_list[log_num].text, "(Process %d IO Finished) ", pid);
		break;
	}
	log_list[log_num].time = time;
	log_num++;
}

/*
* -----------------------------------------------------------------------------
* scheduling 구성 함수 ( init, is_finished, is_arrived, is_iorequested, locate )
* -----------------------------------------------------------------------------
*/

/*
* scheuling 하기 전 변수 값을 초기화하고, process_list에 저장된 process의 데이터를 scheduling_process_list에 copy한다.
*/
void Init() {
	Config();
	scheduling_time = 0;
	next_process_idx = 0;
	running_process = waiting_process = NULL;
	log_num = 0;
	for (int i = 0; i < GANTT_SIZE; i++) gantt_chart[i] = 0;
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		scheduling_process_list[i].pid = process_list[i].pid;
		scheduling_process_list[i].arrival_time = process_list[i].arrival_time;
		scheduling_process_list[i].cpu_burst_time = process_list[i].cpu_burst_time;
		scheduling_process_list[i].priority = process_list[i].priority;
		scheduling_process_list[i].io_burst_time = process_list[i].io_burst_time;
		for (int j = 0; j < IOREQUEST_NUMBER; j++) scheduling_process_list[i].io_request_time[j] = process_list[i].io_request_time[j];
	}
}

/*
* scheduling이 끝났는지를 확인하여 반환한다.
* scheduling_process_list에 저장된 모든 process의 cpu_burst_time이 0이 됐는지 확인한다.
*/
int Is_Finished() {
	for (int i = 0; i < PROCESS_NUMBER; i++) if (scheduling_process_list[i].cpu_burst_time) return 0;
	return 1;
}

/*
* process의 arrived 여부를 확인하여 반환한다.
* next_process의 arrival_time과 scheduling_time과 비교하여 반환한다.
*/
int Is_Arrived() {
	if (next_process_idx < PROCESS_NUMBER) return scheduling_process_list[next_process_idx].arrival_time == scheduling_time;
	return 0;
}

/*
* process의 iorequested 여부를 확인하여 반환한다.
* running_process의 io_request_time과 scheduling_time과 비교하여 반환한다.
*/
int Is_IoRequested() {
	for (int i = 0; i < IOREQUEST_NUMBER; i++) if (running_process->io_request_time[i] == scheduling_time) return 1;
	return 0;
}

/*
* running될 process를 선택한다.
*/
void Locate_Ready_To_Running(int type) {
	running_process = Pop_Ready_Queue(type);
}

/*
* running_process를 waiting_queue를 Push하고, 새로운 process를 선택한다.
*/
void Locate_Running_To_Wait(int type) {
	Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
	Push_Waiting_Queue(running_process);
	running_process = NULL;
}

/*
* waiting_process의 io_burst_time를 복구하고, waiting_process를 ready_queue로 Push한다.
*/
void Locate_Wait_To_Ready(int type) {
	waiting_process->io_burst_time = process_list[Get_Process_Idx(waiting_process->pid)].io_burst_time;
	Push_Ready_Queue(Pop_Waiting_Queue(), type);
	Add_Log(waiting_process->pid, scheduling_time + 1, IO_FINISHED);
}

/*
* -------------------------------------------------------------------------------------
* scheduling algorithm ( FCFS, SJF, Priority, RR, Preemptive_SJF, Preemptive_Priority )
* -------------------------------------------------------------------------------------
*/

/*
* FCFS()
*/
void FCFS() {
	printf("< FCFS() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) { // Arrived process를 ready_queue에 push한다. 
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_FCFS);
		}

		if (!Is_Empty_Ready_Queue() && running_process == NULL) Locate_Ready_To_Running(T_FCFS); // running process가 없다면 ready queue에서 process를 가져온다.

		while (running_process && Is_IoRequested()) { // running process의 io request가 발생하면, waiting queue로 옮긴다. running process의 io request가 없을 때까지 반복한다.
			Locate_Running_To_Wait(T_FCFS);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_FCFS);
		}

		if (running_process) { // running process의 남은 cpu_burst_time을 줄인다. 처리가 종료되면 running process를 null로 바꾼다.
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) { // waiting queue에서 IO 작업을 수행한다. waiting_process의 io_burst_time 줄인다. 작업이 끝나면 ready_queue로 보낸다.
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_FCFS);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< FCFS() end >\n\n");
	Compute_Time(scheduling_time, T_FCFS); // FCFS의 waiting time과 turnaround time을 계산한다.
}

/*
* SJF()
*/
void SJF() {
	printf("< SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_SJF);
		}

		if (!Is_Empty_Ready_Queue() && running_process == NULL) Locate_Ready_To_Running(T_SJF);

		while (running_process && Is_IoRequested()) {
			Locate_Running_To_Wait(T_SJF);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_SJF);
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) {
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_SJF);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< SJF() end >\n\n");
	Compute_Time(scheduling_time, T_SJF);
}

/*
* Priority
*/
void Priority() {
	printf("< Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_PR);
		}

		if (!Is_Empty_Ready_Queue() && running_process == NULL) Locate_Ready_To_Running(T_PR);

		while (running_process && Is_IoRequested()) {
			Locate_Running_To_Wait(T_PR);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_PR);
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) {
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_PR);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Priority() end >\n\n");
	Compute_Time(scheduling_time, T_PR);
}

/*
* RoundRobin
*/
void RR() {
	printf("< RR() with Time Quantum %d start >\n", TIME_QUANTUM);
	Init();
	int time_count = TIME_QUANTUM;
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_RR);
		}

		if (!Is_Empty_Ready_Queue() && running_process == NULL) {
			Locate_Ready_To_Running(T_RR);
			time_count = TIME_QUANTUM;
		}

		while (running_process && Is_IoRequested()) {
			Locate_Running_To_Wait(T_RR);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_RR);
			time_count = TIME_QUANTUM;
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			time_count--;
			if (time_count == 0 || running_process->cpu_burst_time == 0) { // TIME_QUANTUM 안에 작업이 끝나지 않으면 다시 ready_queue로 보낸다.
				if (running_process->cpu_burst_time != 0) Push_Ready_Queue(running_process, T_RR);
				running_process = NULL;
			}
		}

		if (!Is_Empty_Waiting_Queue()) {
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_RR);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< RR() end >\n\n");
	Compute_Time(scheduling_time, T_RR);
}

/*
* Preemptive SJF
*/
void Preemptive_SJF() {
	printf("< Preemptive_SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_PSJF);
		}

		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) Locate_Ready_To_Running(T_PSJF);
			else if (running_process != NULL && Get_Front_Ready_Queue()->cpu_burst_time < running_process->cpu_burst_time) { // ready_queue 첫 번째 원소의 cpu_burst_time가 더 작으면 preemption한다.
				Push_Ready_Queue(running_process, T_PSJF);
				Locate_Ready_To_Running(T_PSJF);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}

		while (running_process && Is_IoRequested()) {
			Locate_Running_To_Wait(T_PSJF);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_PSJF);
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) {
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_PSJF);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_SJF() end >\n\n");
	Compute_Time(scheduling_time, T_PSJF);
}

/*
* Preemptive Priority
*/
void Preemptive_Priority() {
	printf("< Preemptive_Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_PPR);
		}

		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) Locate_Ready_To_Running(T_PPR);
			else if (running_process != NULL && Get_Front_Ready_Queue()->priority < running_process->priority) { // ready_queue 첫 번째 원소의 priority가 더 작으면 preemption한다.
				Push_Ready_Queue(running_process, T_PPR);
				Locate_Ready_To_Running(T_PPR);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}

		while (running_process && Is_IoRequested()) {
			Locate_Running_To_Wait(T_PPR);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_PPR);
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) {
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_PPR);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_Priority() end >\n\n");
	Compute_Time(scheduling_time, T_PPR);
}

/*
* gantt_chart에 저장된 pid에 따라 gantt_chart를 출력한다.
* log_list에 저장된 log를 포함하여 출력한다.
*/
void Show_Gantt(int end_time) {
	if (end_time) {
		int bar[GANTT_SIZE];
		int log_ptr = 0;
		bar[0] = 1;
		for (int t = 1; t <= end_time; t++) {
			bar[t] = 0;
			if (gantt_chart[t] != gantt_chart[t - 1]) bar[t] = 1;
		}
		for (int t = 0; t <= end_time; t++) {
			if (bar[t]) {
				if (t != 0 && gantt_chart[t - 1]) printf("%6d\n", gantt_chart[t - 1]);
				printf("------ %d ", t);
				while (log_list[log_ptr].time == t) {
					printf("%s", log_list[log_ptr].text);
					log_ptr++;
				}
				printf("\n");
			}
			else if (log_list[log_ptr].time == t) {
				printf("       %d ", t);
				while (log_list[log_ptr].time == t) {
					printf("%s", log_list[log_ptr].text);
					log_ptr++;
				}
				printf("\n");
			}
		}
	}
}

/*
* ---------------------------------------------------
* evaluation_list 함수 ( compute, evaluation, clear )
* ---------------------------------------------------
*/

/*
* scheduling type별 end_time까지의 waiting time과 turnaround time을 계산한다.
* 계산 결과를 출력하고, 평균값을 evaluation_list에 저장한다.
*
* 프로세스별 waiting time 계산 : turnaroundtime - cpu_burst_time
* 프로세스별 turnaround time 계산 : completetime - arrival_time
*/
void Compute_Time(int end_time, int type) {
	if (end_time) {
		int waitingtime[PROCESS_NUMBER], turnaroundtime[PROCESS_NUMBER], completetime[PROCESS_NUMBER];
		for (int t = 0; t < end_time; t++) {
			if (gantt_chart[t] != gantt_chart[t + 1]) {
				if (gantt_chart[t]) {
					int idx = Get_Process_Idx(gantt_chart[t]);
					completetime[idx] = t + 1;
				}
			}
		}
		printf("< WaitingTime, TurnaroundTime >\n");
		double aw = 0.0, at = 0.0;
		for (int i = 0; i < PROCESS_NUMBER; i++) {
			turnaroundtime[i] = completetime[i] - process_list[i].arrival_time;
			waitingtime[i] = turnaroundtime[i] - process_list[i].cpu_burst_time;
			aw += waitingtime[i];
			at += turnaroundtime[i];
			printf("Process %5d | waitingtime : %3d, turnaroundtime : %3d\n", process_list[i].pid, waitingtime[i], turnaroundtime[i]);
		}
		printf("average waitingtime : %5.2lf\naverage turnaroundtime : %5.2lf\n", aw / PROCESS_NUMBER, at / PROCESS_NUMBER);
		evaluation_list[type][0] = aw / PROCESS_NUMBER;
		evaluation_list[type][1] = at / PROCESS_NUMBER;
	}
}

/*
* scheduling type별 average waiting time과 average turnaround time을 출력한다.
* Minimum average waiting time, Minimum average turnaround time, Maximum average waiting time, Maximum average turnaround time을 가진 scheduling을 출력한다.
*/
void Evaluation() {
	printf("< Evaluation >\n");
	double mwt = MAX_CPUBURSTTIME * PROCESS_NUMBER, Mwt = 0;
	double mtt = MAX_CPUBURSTTIME * PROCESS_NUMBER, Mtt = 0;
	char scheduling_name[SCHEDULE_TYPE_NUMBER][20] = { "FCFS","SJF","Priority","RoundRobin", "Preemptive SJF", "Preemptive Priority" };
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) {
		printf("%20s | ", scheduling_name[i]);
		if (evaluation_list[i][1]) {
			printf("average waitingtime : %5.2lf, average turnaroundtime : %5.2lf\n", evaluation_list[i][0], evaluation_list[i][1]);
			if (evaluation_list[i][0] < mwt) mwt = evaluation_list[i][0];
			if (evaluation_list[i][1] < mtt) mtt = evaluation_list[i][1];
			if (evaluation_list[i][0] > Mwt) Mwt = evaluation_list[i][0];
			if (evaluation_list[i][1] > Mtt) Mtt = evaluation_list[i][1];
		}
		else printf("Not scheduled\n");
	}
	printf("\n");

	printf("   Minimum average waiting time scheduling | ");
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) if (evaluation_list[i][0] == mwt) printf("(%s) ", scheduling_name[i]);
	printf("\n");
	printf("Minimum average turnaround time scheduling | ");
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) if (evaluation_list[i][1] == mtt) printf("(%s) ", scheduling_name[i]);
	printf("\n");
	printf("   Maximum average waiting time scheduling | ");
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) if (evaluation_list[i][0] == Mwt) printf("(%s) ", scheduling_name[i]);
	printf("\n");
	printf("Maximum average turnaround time scheduling | ");
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) if (evaluation_list[i][1] == Mtt) printf("(%s) ", scheduling_name[i]);
	printf("\n");
}

/*
* evaluation_list를 초기화한다.
* Create_Process()가 여러 번 호출됐을 때를 위하여 만들어졌다.
*/
void Clear_Evaluation() {
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) evaluation_list[i][0] = evaluation_list[i][1] = 0;
}