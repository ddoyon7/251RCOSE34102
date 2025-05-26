#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "process.h"

#define QUEUE_SIZE 100

#define SCHEDULE_TYPE_NUMBER 6
#define REAL_TIME_SCHEDULE_TYPE_NUMBER 2
/*
* ready_queue ( 선형 큐 / 우선순위 큐 ), waiting_queue ( 선형 큐 )
*/
extern process_info* ready_queue[QUEUE_SIZE];
extern int rq_rear;

extern process_info* waiting_queue[QUEUE_SIZE];
extern int wq_front, wq_rear;

/*
* scheduling algorithm 타입
*/
enum SCHEDULE_TYPE { T_FCFS, T_SJF, T_PR, T_RR, T_PSJF, T_PPR, T_RM, T_EDF };

/*
* queue 초기 설정
*/
void Config();

/*
* ready_queue 함수 ( push, pop, is_empty, is_full, heapify, swap, front )
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
* waiting_queue 함수 ( push, pop, is_empty, is_full, front )
*/
int Is_Empty_Waiting_Queue();

int Is_Full_Waiting_Queue();

void Push_Waiting_Queue(process_info*);

process_info* Pop_Waiting_Queue();

process_info* Get_Front_Waiting_Queue();

#endif