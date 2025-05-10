#include "scheduler.h"

int scheduling_time;
int scheduling_idx;
process_info* running_process;
process_info scheduling_process_list[PROCESS_NUMBER];
int gantt_chart[GANTT_SIZE];
log log_list[100];
int log_num;

void Init() {
	scheduling_time = 0;
	scheduling_idx = 0;
	running_process = NULL;
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
		sprintf(log_list[log_num].text, "(Process %d Io Requested) ", pid);
		break;
	case IO_FINISHED:
		sprintf(log_list[log_num].text, "(Process %d Io finished) ", pid);
		break;
	}
	log_list[log_num].time = time;
	log_num++;
}

log* Get_Front_Log() {
	return &log_list[0];
}

int Is_Finished() {
	for (int i = 0; i < PROCESS_NUMBER; i++) if (scheduling_process_list[i].cpu_burst_time) return 0;
	return 1;
}

int Is_Arrived() {
	return scheduling_process_list[scheduling_idx].arrival_time == scheduling_time;
}

int Is_IoRequested() {
	for (int i = 0; i < IOREQUEST_NUMBER; i++) if (running_process->io_request_time[i] == scheduling_time) return 1;
	return 0;
}

void FCFS() {
	printf("< FCFS() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_FCFS);
		}
		if (!Is_Empty_Ready_Queue() && running_process == NULL) running_process = Pop_Ready_Queue(T_FCFS);
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_FCFS);
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_FCFS);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< FCFS() end >\n\n");
	Compute_Time(scheduling_time, T_FCFS);
}

void SJF() {
	printf("< SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_SJF);
		}
		if (!Is_Empty_Ready_Queue() && running_process == NULL) running_process = Pop_Ready_Queue(T_SJF);
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_SJF);

		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_SJF);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< SJF() end >\n\n");
	Compute_Time(scheduling_time, T_SJF);
}

void Priority() {
	printf("< Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_PR);
		}
		if (!Is_Empty_Ready_Queue() && running_process == NULL) running_process = Pop_Ready_Queue(T_PR);
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_PR);
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_PR);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Priority() end >\n\n");
	Compute_Time(scheduling_time, T_PR);
}

void RR() {
	printf("< RR() start >\n");
	Init();
	int time_count = TIME_QUANTUM;
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_RR);
		}
		if (!Is_Empty_Ready_Queue() && running_process == NULL) {
			running_process = Pop_Ready_Queue(T_RR);
			time_count = TIME_QUANTUM;
		}
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_RR);
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
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_RR);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< RR() end >\n\n");
	Compute_Time(scheduling_time, T_RR);
}

void Preemptive_SJF() {
	printf("< Preemptive_SJF() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_PSJF);
		}
		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) running_process = Pop_Ready_Queue(T_PSJF);
			else if (running_process != NULL && Get_Front_Ready_Queue()->cpu_burst_time < running_process->cpu_burst_time) {
				Push_Ready_Queue(running_process, T_PSJF);
				running_process = Pop_Ready_Queue(T_PSJF);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_PSJF);
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_PSJF);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_SJF() end >\n\n");
	Compute_Time(scheduling_time, T_PSJF);
}

void Preemptive_Priority() {
	printf("< Preemptive_Priority() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) {
			Add_Log(scheduling_process_list[scheduling_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[scheduling_idx++], T_PPR);
		}
		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) running_process = Pop_Ready_Queue(T_PPR);
			else if (running_process != NULL && Get_Front_Ready_Queue()->priority < running_process->priority) {
				Push_Ready_Queue(running_process, T_PPR);
				running_process = Pop_Ready_Queue(T_PPR);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}
		while (running_process && Is_IoRequested()) { // 아이오 리퀘스트가 연쇄적으로 나올 수 있음 스왑 되자마자 바로 아이오리퀘스트가 나올수있음
			Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
			Push_Waiting_Queue(running_process);
			running_process = NULL;
			if (!Is_Empty_Ready_Queue()) running_process = Pop_Ready_Queue(T_PPR);
		}
		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}
		if (!Is_Empty_Waiting_Queue()) { // 아이오 끝난 애랑 어라이벌은 겹치면 아이오 끝난 애부터 들어감
			process_info* pptr = Get_Front_Waiting_Queue();
			pptr->io_burst_time--;
			if (pptr->io_burst_time == 0) {
				pptr->io_burst_time = process_list[Get_Process_Idx(pptr->pid)].io_burst_time;
				Push_Ready_Queue(Pop_Waiting_Queue(), T_PPR);
				Add_Log(pptr->pid, scheduling_time + 1, IO_FINISHED);
			}
		}
		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Preemptive_Priority() end >\n\n");
	Compute_Time(scheduling_time, T_PPR);
}

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

void Compute_Time(int end_time, int type) {
	if (end_time) {
		int waitingtime[PROCESS_NUMBER], turnaroundtime[PROCESS_NUMBER], completetime[PROCESS_NUMBER];
		for (int i = 0; i < PROCESS_NUMBER; i++) waitingtime[i] = turnaroundtime[i] = -process_list[i].arrival_time;
		for (int t = 0; t < end_time; t++) {
			if (gantt_chart[t] != gantt_chart[t + 1]) {
				if (gantt_chart[t]) {
					int idx = Get_Process_Idx(gantt_chart[t]);
					waitingtime[idx] -= t + 1;
					completetime[idx] = t + 1;
				}
				if (gantt_chart[t + 1]) {
					int idx = Get_Process_Idx(gantt_chart[t + 1]);
					waitingtime[idx] += t + 1;
				}
			}
		}
		printf("< WaitingTime, TurnaroundTime >\n");
		double aw = 0.0, at = 0.0;
		for (int i = 0; i < PROCESS_NUMBER; i++) {
			waitingtime[i] += completetime[i];
			turnaroundtime[i] += completetime[i];
			aw += waitingtime[i];
			at += turnaroundtime[i];
			printf("Process %5d | waitingtime : %3d, turnaroundtime : %3d\n", process_list[i].pid, waitingtime[i], turnaroundtime[i]);
		}
		printf("average waitingtime : %5.2lf\naverage turnaroundtime : %5.2lf\n", aw / PROCESS_NUMBER, at / PROCESS_NUMBER);
		evaluation_list[type][0] = aw / PROCESS_NUMBER;
		evaluation_list[type][1] = at / PROCESS_NUMBER;
	}
}

void Evaluation() {
	printf("< Evaluation >\n");
	char scheduling_name[6][20] = { "FCFS","SJF","Priority","RoundRobin", "Preemptive SJF", "Preemptive Priority" };
	for (int i = 0; i < 6; i++) {
		printf("%20s | ", scheduling_name[i]);
		if (evaluation_list[i][1] != 0) printf("average waitingtime : %5.2lf, average turnaroundtime : %5.2lf\n", evaluation_list[i][0], evaluation_list[i][1]);
		else printf("Not scheduled\n");
	}
}