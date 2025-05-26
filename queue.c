#include "queue.h"

process_info* ready_queue[QUEUE_SIZE];
int rq_rear;
process_info* waiting_queue[QUEUE_SIZE];
int wq_front, wq_rear;

/*
* queue 초기 설정
*/
void Config() {
	rq_rear = 0;
	wq_front = -1, wq_rear = -1;
}

/*
* -----------------------------------------------------------------------
* ready_queue 함수 ( push, pop, is_empty, is_full, heapify, swap, front )
* -----------------------------------------------------------------------
*/

/*
* process 정보 pi와 scheduling 타입 type에 따라 다르게 pi를 Push한다.
* FCFS, RR의 경우 선형 큐를 만들어 rq_rear에 삽입한다.
* SJF, PR, PSJF, PPR의 경우 힙 구조의 큐를 만들어 rq_rear에 삽입한 후 , Heapify_Up을 한다.
*/
void Push_Ready_Queue(process_info* pi, int type) {
	if (Is_Full_Ready_Queue()) printf("ERR_QUEUE IS FULL\n");
	else {
		ready_queue[rq_rear] = pi;
		if (type == T_SJF || type == T_PR || type == T_PSJF || type == T_PPR || type == T_RM || type == T_EDF) Heapify_Up(rq_rear, type);
		rq_rear++;
	}
}

/*
* process 정보 pi와 scheduling 타입 type에 따라 다르게 pi를 Push한다.
* FCFS, RR의 경우 두 번째 원소부터 rq_rear의 원소까지 앞당겨 선형 큐를 유지하고 첫 원소를 반환한다.
* SJF, PR, PSJF, PPR의 경우 Heapify_Down을 통해 힙 구조를 유지하고, 첫 원소를 반환한다.
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
* ready_queue가 비어 있는지를 확인하여 반환한다.
*/
int Is_Empty_Ready_Queue() {
	return rq_rear == 0;
}

/*
* ready_queue가 가득 찼는지를 확인하여 반환한다.
*/
int Is_Full_Ready_Queue() {
	return rq_rear == QUEUE_SIZE;
}

/*
* ready_queue의 첫 원소를 반환한다.
*/
process_info* Get_Front_Ready_Queue() {
	return ready_queue[0];
}

/*
* SJF, PSJF, PR, PPR의 경우 힙구조를 유지하기 위하여 상향식 Heapify를 수행한다.
* parent와의 비교는 scheduling type에 따라 달리 한다.
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
* SJF, PSJF, PR, PPR의 경우 힙구조를 유지하기 위하여 하향식 Heapify를 수행한다.
* left, right와의 비교는 scheduling type에 따라 달리 한다.
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
* Index를 기준으로 swap한다.
*/
void Swap_ReadyQueue(int idx1, int idx2) {
	process_info* temp = ready_queue[idx1];
	ready_queue[idx1] = ready_queue[idx2];
	ready_queue[idx2] = temp;
}

/*
* ----------------------------------------------------------
* waiting_queue 함수 ( push, pop, is_empty, is_full, front )
* ----------------------------------------------------------
*/

/*
* waiting_queue에 pi를 Push한다.
*/
void Push_Waiting_Queue(process_info* pi) {
	if (Is_Full_Waiting_Queue()) printf("ERR_QUEUE IS FULL\n");
	else waiting_queue[++wq_rear] = pi;
}

/*
* waiting_queue의 첫원소를 Pop한다.
*/
process_info* Pop_Waiting_Queue() {
	if (Is_Empty_Waiting_Queue()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	return waiting_queue[++wq_front];
}

/*
* waiting_queue가 비어 있는지를 확인하여 반환한다.
*/
int Is_Empty_Waiting_Queue() {
	return wq_front == wq_rear;
}

/*
* waiting_queue가 가득 찼는지를 확인하여 반환한다.
*/
int Is_Full_Waiting_Queue() {
	return wq_rear == QUEUE_SIZE - 1;
}

/*
* waiting_queue의 첫 원소를 반환한다.
*/
process_info* Get_Front_Waiting_Queue() {
	return waiting_queue[wq_front + 1];
}