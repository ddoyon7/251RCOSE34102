#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "process.h"

enum SCHEDULE_TYPE { T_FCFS, T_SJF, T_PR, T_RR, T_PSJF, T_PPR };
#define QUEUE_SIZE 100

extern process_info* ready_queue[QUEUE_SIZE];
extern int rq_num;

//void Config();

void Push_Ready_Queue(process_info*, int);

process_info* Pop_Ready_Queue(int);

int Is_Empty_QUEUE();

int Is_Full_QUEUE();

void Heapify_Up(int, int);

void Heapify_Down(int, int);

void Swap_ReadyQueue(int, int);
//process_info waiting_queue[QUEUE_SIZE];
//int wq_front, wq_rear;
#endif