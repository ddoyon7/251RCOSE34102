#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "process.h"

#define QUEUE_SIZE 100

#define SCHEDULE_TYPE_NUMBER 6

/*
* ready_queue ( ���� ť / �켱���� ť ), waiting_queue ( ���� ť )
*/
extern process_info* ready_queue[QUEUE_SIZE];
extern int rq_rear;

extern process_info* waiting_queue[QUEUE_SIZE];
extern int wq_front, wq_rear;

/*
* scheduling algorithm Ÿ��
*/
enum SCHEDULE_TYPE { T_FCFS, T_SJF, T_PR, T_RR, T_PSJF, T_PPR };

/*
* queue �ʱ� ����
*/
void Config();

/*
* ready_queue �Լ� ( push, pop, is_empty, is_full, heapify, swap, front )
*/
void Push_Ready_Queue(process_info*, int);

process_info* Pop_Ready_Queue(int);

int Is_Empty_Ready_Queue();

int Is_Full_Ready_Queue();

process_info* Get_Front_Ready_Queue();

void Heapify_Up(int, int);

void Heapify_Down(int, int);

void Swap_ReadyQueue(int, int);

/*
* waiting_queue �Լ� ( push, pop, is_empty, is_full, front )
*/
int Is_Empty_Waiting_Queue();

int Is_Full_Waiting_Queue();

void Push_Waiting_Queue(process_info*);

process_info* Pop_Waiting_Queue();

process_info* Get_Front_Waiting_Queue();

#endif