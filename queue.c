#include "queue.h"

process_info* ready_queue[QUEUE_SIZE];
int rq_front, rq_rear;

void Config() {
	rq_front = rq_rear = -1;
}

//priority나 그냥 을 둘다 쓸 수있는걸로 만들어야되네 

void Push_Ready_Queue(process_info* pi, int type) {
	if (Is_Full_QUEUE()) printf("ERR_QUEUE IS FULL\n");
	else ready_queue[++rq_rear] = pi;
}

process_info* Pop_Ready_Queue(int type) {
	if (Is_Empty_QUEUE()) {
		printf("ERR_QUEUE IS EMPTY\n");
		return NULL;
	}
	return ready_queue[++rq_front];
}

int Is_Empty_QUEUE() {
	return rq_front == rq_rear;
}

int Is_Full_QUEUE() {
	return rq_rear == QUEUE_SIZE - 1;
}

void Heapify_Up(int type) {
	if (rq_rear == 0) return;
	int parent = (rq_rear - 1) / 2;


	if (heap->data[parent] > heap->data[index]) {
		swap(&heap->data[parent], &heap->data[index]);
		heapify_up(heap, parent);
	}
}

void Heapify_Down(int type) {

}

void Swap_ReadyQueue(int idx1, int idx2) {
	process_info* temp = ready_queue[idx1];
	ready_queue[idx1] = ready_queue[idx2];
	ready_queue[idx2] = temp;
}