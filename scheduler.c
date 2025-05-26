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
* ��Ȳ�� log�� ���ڿ��� log_list�� �����Ѵ�.
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
	case PERIOD:
		sprintf(log_list[log_num].text, "(Process %d Period) ", pid);
		break;
	case DEADLINE_MISS:
		sprintf(log_list[log_num].text, "(Process %d Deadline Miss) ", pid);
		break;
	}
	log_list[log_num].time = time;
	log_num++;
}

/*
* -----------------------------------------------------------------------------
* scheduling ���� �Լ� ( init, is_finished, is_arrived, is_iorequested, locate )
* -----------------------------------------------------------------------------
*/

/*
* scheuling �ϱ� �� ���� ���� �ʱ�ȭ�ϰ�, process_list�� ����� process�� �����͸� scheduling_process_list�� copy�Ѵ�.
*/
void Init() {
	Config();
	scheduling_time = 0;
	next_process_idx = 0;
	running_process = waiting_process = NULL;
	log_num = 0;
	for (int i = 0; i < LOG_LIST_SIZE; i++) {
		log_list[i].time = 0;
		strcpy(log_list[i].text, "\0");
	}
	for (int i = 0; i < GANTT_SIZE; i++) gantt_chart[i] = 0;
	for (int i = 0; i < PROCESS_NUMBER; i++) {
		scheduling_process_list[i].pid = process_list[i].pid;
		scheduling_process_list[i].arrival_time = process_list[i].arrival_time;
		scheduling_process_list[i].cpu_burst_time = process_list[i].cpu_burst_time;
		scheduling_process_list[i].priority = process_list[i].priority;
		scheduling_process_list[i].io_burst_time = process_list[i].io_burst_time;
		scheduling_process_list[i].period = process_list[i].period;
		for (int j = 0; j < IOREQUEST_NUMBER; j++) scheduling_process_list[i].io_request_time[j] = process_list[i].io_request_time[j];
	}
}

/*
* scheduling�� ���������� Ȯ���Ͽ� ��ȯ�Ѵ�.
* scheduling_process_list�� ����� ��� process�� cpu_burst_time�� 0�� �ƴ��� Ȯ���Ѵ�.
*/
int Is_Finished() {
	for (int i = 0; i < PROCESS_NUMBER; i++) if (scheduling_process_list[i].cpu_burst_time) return 0;
	return 1;
}

/*
* process�� arrived ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�.
* next_process�� arrival_time�� scheduling_time�� ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_Arrived() {
	if (next_process_idx < PROCESS_NUMBER) return scheduling_process_list[next_process_idx].arrival_time == scheduling_time;
	return 0;
}

/*
* process�� iorequested ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�.
* running_process�� io_request_time�� scheduling_time�� ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_IoRequested() {
	for (int i = 0; i < IOREQUEST_NUMBER; i++) if (running_process->io_request_time[i] == scheduling_time) return 1;
	return 0;
}

/*
* deadline�� miss ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�.
* deadline �� running ���� process�� ������, ready_queue�� front�� �ִ��� Ȯ���Ѵ�.
*/
int Is_Deadline_Missed(int idx) {
	return (running_process != NULL && running_process->pid == scheduling_process_list[idx].pid) || (!Is_Empty_Ready_Queue() && Get_Front_Ready_Queue()->pid == scheduling_process_list[idx].pid);
}

/*
* running�� process�� �����Ѵ�.
*/
void Locate_Ready_To_Running(int type) {
	running_process = Pop_Ready_Queue(type);
}

/*
* running_process�� waiting_queue�� Push�ϰ�, ���ο� process�� �����Ѵ�.
*/
void Locate_Running_To_Wait(int type) {
	Add_Log(running_process->pid, scheduling_time, IO_REQUESTED);
	Push_Waiting_Queue(running_process);
	running_process = NULL;
}

/*
* waiting_process�� io_burst_time�� �����ϰ�, waiting_process�� ready_queue�� Push�Ѵ�.
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
* First Come First Served
*/
void FCFS() {
	printf("< FCFS() start >\n");
	Init();
	while (!Is_Finished()) {
		while (Is_Arrived()) { // Arrived process�� ready_queue�� push�Ѵ�. 
			Add_Log(scheduling_process_list[next_process_idx].pid, scheduling_time, ARRIVED);
			Push_Ready_Queue(&scheduling_process_list[next_process_idx++], T_FCFS);
		}

		if (!Is_Empty_Ready_Queue() && running_process == NULL) Locate_Ready_To_Running(T_FCFS); // running process�� ���ٸ� ready queue���� process�� �����´�.

		while (running_process && Is_IoRequested()) { // running process�� io request�� �߻��ϸ�, waiting queue�� �ű��. running process�� io request�� ���� ������ �ݺ��Ѵ�.
			Locate_Running_To_Wait(T_FCFS);
			if (!Is_Empty_Ready_Queue()) Locate_Ready_To_Running(T_FCFS);
		}

		if (running_process) { // running process�� ���� cpu_burst_time�� ���δ�. ó���� ����Ǹ� running process�� null�� �ٲ۴�.
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) running_process = NULL;
		}

		if (!Is_Empty_Waiting_Queue()) { // waiting queue���� IO �۾��� �����Ѵ�. waiting_process�� io_burst_time ���δ�. �۾��� ������ ready_queue�� ������.
			waiting_process = Get_Front_Waiting_Queue();
			waiting_process->io_burst_time--;
			if (waiting_process->io_burst_time == 0) Locate_Wait_To_Ready(T_FCFS);
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< FCFS() end >\n\n");
	Compute_Time(scheduling_time, T_FCFS); // FCFS�� waiting time�� turnaround time�� ����Ѵ�.
}

/*
* Shortest Job First
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
			if (time_count == 0 || running_process->cpu_burst_time == 0) { // TIME_QUANTUM �ȿ� �۾��� ������ ������ �ٽ� ready_queue�� ������.
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
			else if (running_process != NULL && Get_Front_Ready_Queue()->cpu_burst_time < running_process->cpu_burst_time) { // ready_queue ù ��° ������ cpu_burst_time�� �� ������ preemption�Ѵ�.
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
			else if (running_process != NULL && Get_Front_Ready_Queue()->priority < running_process->priority) { // ready_queue ù ��° ������ priority�� �� ������ preemption�Ѵ�.
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
* Rate Monotonic
*/
void Rate_Monotonic() {
	printf("< Rate_Monotonic() start with two processes >\n");
	Init();
	int scheduling_res = 1;
	int finish_time = lcm(scheduling_process_list[0].period, scheduling_process_list[1].period);
	while (scheduling_time <= finish_time) {
		for (int i = 0; i < REALTIME_PROCESS_NUMBER; i++) {
			if (scheduling_time % scheduling_process_list[i].period == 0) {
				if (scheduling_time == 0) Add_Log(scheduling_process_list[i].pid, scheduling_time, ARRIVED);
				else {
					Add_Log(scheduling_process_list[i].pid, scheduling_time, PERIOD);
					if (Is_Deadline_Missed(i)) {
						Add_Log(scheduling_process_list[i].pid, scheduling_time, DEADLINE_MISS);
						scheduling_res = 0;
					}
				}
				Push_Ready_Queue(&scheduling_process_list[i], T_RM);
			}
		}

		if (scheduling_time == finish_time || scheduling_res == 0) break;

		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) Locate_Ready_To_Running(T_RM);
			else if (running_process != NULL && Get_Front_Ready_Queue()->period < running_process->period) {
				Push_Ready_Queue(running_process, T_RM);
				Locate_Ready_To_Running(T_RM);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) {
				running_process->cpu_burst_time = process_list[Get_Process_Idx(running_process->pid)].cpu_burst_time;
				running_process = NULL;
			}
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< Rate_Monotonic() end with two processes >\n\n");
	if (scheduling_res) printf("Scheduling succeeded.\n");
	else printf("Scheduling failed.\n");
}

/*
* Earliest Deadline First
*/
void EDF() {
	printf("< EDF() start with two processes >\n");
	Init();
	int scheduling_res = 1;
	int finish_time = lcm(scheduling_process_list[0].period, scheduling_process_list[1].period);
	while (scheduling_time <= finish_time) {
		for (int i = 0; i < REALTIME_PROCESS_NUMBER; i++) {
			if (scheduling_time % scheduling_process_list[i].period == 0) {
				if (scheduling_time == 0) Add_Log(scheduling_process_list[i].pid, scheduling_time, ARRIVED);
				else {
					Add_Log(scheduling_process_list[i].pid, scheduling_time, PERIOD);
					scheduling_process_list[i].period += process_list[i].period;
					if (Is_Deadline_Missed(i)) {
						Add_Log(scheduling_process_list[i].pid, scheduling_time, DEADLINE_MISS);
						scheduling_res = 0;
					}
				}
				Push_Ready_Queue(&scheduling_process_list[i], T_EDF);
			}
		}

		if (scheduling_time == finish_time || scheduling_res == 0) break;

		if (!Is_Empty_Ready_Queue()) {
			if (running_process == NULL) Locate_Ready_To_Running(T_EDF);
			else if (running_process != NULL && Get_Front_Ready_Queue()->period < running_process->period) {
				Push_Ready_Queue(running_process, T_EDF);
				Locate_Ready_To_Running(T_EDF);
				Add_Log(running_process->pid, scheduling_time, PREEMPTED);
			}
		}

		if (running_process) {
			gantt_chart[scheduling_time] = running_process->pid;
			(running_process->cpu_burst_time)--;
			if (running_process->cpu_burst_time == 0) {
				running_process->cpu_burst_time = process_list[Get_Process_Idx(running_process->pid)].cpu_burst_time;
				running_process = NULL;
			}
		}

		scheduling_time++;
	}
	Show_Gantt(scheduling_time);
	printf("< EDF() end with two processes >\n\n");
	if (scheduling_res) printf("Scheduling succeeded.\n");
	else printf("Scheduling failed.\n");
}

/*
* �� ���� �ִ������� �ּҰ������ ���Ѵ�.
*/
int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

int lcm(int a, int b) {
	return (a * b) / gcd(a, b);
}

/*
* gantt_chart�� ����� pid�� ���� gantt_chart�� ����Ѵ�.
* log_list�� ����� log�� �����Ͽ� ����Ѵ�.
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
* evaluation_list �Լ� ( compute, evaluation, clear )
* ---------------------------------------------------
*/

/*
* scheduling type�� end_time������ waiting time�� turnaround time�� ����Ѵ�.
* ��� ����� ����ϰ�, ��հ��� evaluation_list�� �����Ѵ�.
*
* ���μ����� waiting time ��� : turnaroundtime - cpu_burst_time
* ���μ����� turnaround time ��� : completetime - arrival_time
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
* scheduling type�� average waiting time�� average turnaround time�� ����Ѵ�.
* Minimum average waiting time, Minimum average turnaround time, Maximum average waiting time, Maximum average turnaround time�� ���� scheduling�� ����Ѵ�.
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
* evaluation_list�� �ʱ�ȭ�Ѵ�.
* Create_Process()�� ���� �� ȣ����� ���� ���Ͽ� ���������.
*/
void Clear_Evaluation() {
	for (int i = 0; i < SCHEDULE_TYPE_NUMBER; i++) evaluation_list[i][0] = evaluation_list[i][1] = 0;
}