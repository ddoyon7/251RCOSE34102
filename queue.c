#include "queue.h"

process_info* ready_queue[QUEUE_SIZE];
int rq_rear;
process_info* waiting_queue[QUEUE_SIZE];
int wq_front, wq_rear;

/*
* queue �ʱ� ����
*/
void Config() {
	rq_rear = 0;
	wq_front = -1, wq_rear = -1;
}

/*
* -----------------------------------------------------------------------
* ready_queue �Լ� ( push, pop, is_empty, is_full, heapify, swap, front )
* -----------------------------------------------------------------------
*/

/*
* process ���� pi�� scheduling Ÿ�� type�� ���� �ٸ��� pi�� Push�Ѵ�.
* FCFS, RR�� ��� ���� ť�� ����� rq_rear�� �����Ѵ�.
* SJF, PR, PSJF, PPR�� ��� �� ������ ť�� ����� rq_rear�� ������ �� , Heapify_Up�� �Ѵ�.
*/
void Push_Ready_Queue(process_info* pi, int type, int time) {
	if (Is_Full_Ready_Queue()) printf("ERR_QUEUE IS FULL\n");
	else {
		ready_queue[rq_rear] = pi;
		ready_queue[rq_rear]->ready_time = time;
		if (type == T_SJF || type == T_PR || type == T_PSJF || type == T_PPR || type == T_RM || type == T_EDF) Heapify_Up(rq_rear, type);
		rq_rear++;
	}
}

/*
* process ���� pi�� scheduling Ÿ�� type�� ���� �ٸ��� pi�� Push�Ѵ�.
* FCFS, RR�� ��� �� ��° ���Һ��� rq_rear�� ���ұ��� �մ�� ���� ť�� �����ϰ� ù ���Ҹ� ��ȯ�Ѵ�.
* SJF, PR, PSJF, PPR�� ��� Heapify_Down�� ���� �� ������ �����ϰ�, ù ���Ҹ� ��ȯ�Ѵ�.
*/
process_info* Pop_Ready_Queue(int type) {
	if (Is_Empty_Ready_Queue()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	else {
		process_info* res = ready_queue[0];
		rq_rear--;
		if (type == T_SJF || type == T_PR || type == T_PSJF || type == T_PPR || type == T_RM || type == T_EDF) {
			ready_queue[0] = ready_queue[rq_rear];
			Heapify_Down(0, type);
		}
		else if (type == T_FCFS || type == T_RR) for (int i = 1; i <= rq_rear; i++) ready_queue[i - 1] = ready_queue[i];
		return res;
	}
}

/*
* ready_queue�� ��� �ִ����� Ȯ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_Empty_Ready_Queue() {
	return rq_rear == 0;
}

/*
* ready_queue�� ���� á������ Ȯ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_Full_Ready_Queue() {
	return rq_rear == QUEUE_SIZE;
}

/*
* ready_queue�� ù ���Ҹ� ��ȯ�Ѵ�.
*/
process_info* Get_Front_Ready_Queue() {
	return ready_queue[0];
}

/*
* SJF, PSJF, PR, PPR�� ��� �������� �����ϱ� ���Ͽ� ����� Heapify�� �����Ѵ�.
* parent���� �񱳴� scheduling type�� ���� �޸� �Ѵ�.
*/
void Heapify_Up(int idx, int type) {
	if (idx == 0) return;
	int parent = (rq_rear - 1) / 2;

	switch (type) {
	case T_SJF:
	case T_PSJF:
		if (ready_queue[parent]->cpu_burst_time > ready_queue[idx]->cpu_burst_time) {
			Swap_ReadyQueue(parent, idx);
			Heapify_Up(parent, type);
		}
		break;

	case T_PR:
	case T_PPR:
		if (ready_queue[parent]->priority > ready_queue[idx]->priority) {
			Swap_ReadyQueue(parent, idx);
			Heapify_Up(parent, type);
		}
		break;

	case T_RM:
	case T_EDF:
		if (ready_queue[parent]->period > ready_queue[idx]->period) {
			Swap_ReadyQueue(parent, idx);
			Heapify_Up(parent, type);
		}
		break;

	default:
		break;
	}
}

/*
* SJF, PSJF, PR, PPR�� ��� �������� �����ϱ� ���Ͽ� ����� Heapify�� �����Ѵ�.
* left, right���� �񱳴� scheduling type�� ���� �޸� �Ѵ�.
*/
void Heapify_Down(int idx, int type) {
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	int target = idx;

	switch (type) {

	case T_SJF:
	case T_PSJF:
		if (left < rq_rear && ready_queue[left]->cpu_burst_time < ready_queue[target]->cpu_burst_time)
			target = left;
		if (left < rq_rear && ready_queue[right]->cpu_burst_time < ready_queue[target]->cpu_burst_time)
			target = right;
		break;

	case T_PR:
	case T_PPR:
		if (left < rq_rear && ready_queue[left]->priority < ready_queue[target]->priority)
			target = left;
		if (left < rq_rear && ready_queue[right]->priority < ready_queue[target]->priority)
			target = right;
		break;

	default:
		break;
	}

	if (target != idx) {
		Swap_ReadyQueue(target, idx);
		Heapify_Down(target, type);
	}
}

/*
* index�� �������� swap�Ѵ�.
*/
void Swap_ReadyQueue(int idx1, int idx2) {
	process_info* temp = ready_queue[idx1];
	ready_queue[idx1] = ready_queue[idx2];
	ready_queue[idx2] = temp;
}

/*
* ready_quue�� ����� process ��, max_time�� �ʰ��Ͽ� waiting�� process�� aging�Ѵ�.
* aging�� process�� pid�� ��ȯ�Ѵ�.
*/
int Set_ReadyQueue_Aging(int time, int max_time, int type) {
	for (int i = 0; i < rq_rear; i++) {
		if (time - ready_queue[i]->ready_time >= max_time) {
			int pid = ready_queue[i]->pid;
			ready_queue[i]->priority--;
			ready_queue[i]->ready_time = time;
			Heapify_Up(i, type);
			return pid;
		}
	}
	return 0;
}

/*
* ----------------------------------------------------------
* waiting_queue �Լ� ( push, pop, is_empty, is_full, front )
* ----------------------------------------------------------
*/

/*
* waiting_queue�� pi�� Push�Ѵ�.
*/
void Push_Waiting_Queue(process_info* pi) {
	if (Is_Full_Waiting_Queue()) printf("ERR_QUEUE IS FULL\n");
	else waiting_queue[++wq_rear] = pi;
}

/*
* waiting_queue�� ù ���Ҹ� Pop�Ѵ�.
*/
process_info* Pop_Waiting_Queue() {
	if (Is_Empty_Waiting_Queue()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	return waiting_queue[++wq_front];
}

/*
* waiting_queue�� ��� �ִ����� Ȯ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_Empty_Waiting_Queue() {
	return wq_front == wq_rear;
}

/*
* waiting_queue�� ���� á������ Ȯ���Ͽ� ��ȯ�Ѵ�.
*/
int Is_Full_Waiting_Queue() {
	return wq_rear == QUEUE_SIZE - 1;
}

/*
* waiting_queue�� ù ���Ҹ� ��ȯ�Ѵ�.
*/
process_info* Get_Front_Waiting_Queue() {
	return waiting_queue[wq_front + 1];
}