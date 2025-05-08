#include "queue.h"

process_info* ready_queue[QUEUE_SIZE];
int rq_num;

void Push_Ready_Queue(process_info* pi, int type) {
	if (Is_Full_QUEUE()) printf("ERR_QUEUE IS FULL\n");
	else {
		ready_queue[rq_num] = pi;
		Heapify_Up(rq_num, type);
		rq_num++;
	}
}

process_info* Pop_Ready_Queue(int type) {
	if (Is_Empty_QUEUE()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	else {
		process_info* res = ready_queue[0];
		ready_queue[0] = ready_queue[--rq_num];
		Heapify_Down(0, type);
		return res;
	}
}

int Is_Empty_QUEUE() {
	return rq_num == 0;
}

int Is_Full_QUEUE() {
	return rq_num == QUEUE_SIZE;
}

void Heapify_Up(int idx, int type) {
	if (idx == 0) return;
	int parent = (rq_num - 1) / 2;

	switch (type) {
	case T_SJF:
		if (ready_queue[parent]->cpu_burst_time > ready_queue[idx]->cpu_burst_time) {
			Swap_ReadyQueue(parent, idx);
			Heapify_Up(parent, type);
		}
		break;

	case T_PR:
		if (ready_queue[parent]->priority > ready_queue[idx]->priority) { // 낮을수록 우선순위가 높다.
			Swap_ReadyQueue(parent, idx);
			Heapify_Up(parent, type);
		}
		break;

	default:
		break;
	}
}

void Heapify_Down(int idx, int type) {
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	int target = idx;

	switch (type) {
	case T_FCFS:
		if (left < rq_num && ready_queue[left]->arrival_time < ready_queue[target]->arrival_time)
			target = left;
		if (left < rq_num && ready_queue[right]->arrival_time < ready_queue[target]->arrival_time)
			target = right;
		break;

	case T_SJF:
		if (left < rq_num && ready_queue[left]->cpu_burst_time < ready_queue[target]->cpu_burst_time)
			target = left;
		if (left < rq_num && ready_queue[right]->cpu_burst_time < ready_queue[target]->cpu_burst_time)
			target = right;
		break;

	case T_PR:
		if (left < rq_num && ready_queue[left]->priority < ready_queue[target]->priority)
			target = left;
		if (left < rq_num && ready_queue[right]->priority < ready_queue[target]->priority)
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

void Swap_ReadyQueue(int idx1, int idx2) {
	process_info* temp = ready_queue[idx1];
	ready_queue[idx1] = ready_queue[idx2];
	ready_queue[idx2] = temp;
}