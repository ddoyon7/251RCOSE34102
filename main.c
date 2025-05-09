#include "scheduler.h"

int main() {
	//Config();
	Create_Process();
	Show_Process();
	FCFS();
	printf("\n\n");
	SJF();
	printf("\n\n");
	Priority();
	printf("\n\n");
	RR();
	printf("\n\n");
	Preemptive_SJF();
	printf("\n\n");
	Preemptive_Priority();
	return 0;
}


// 나중에 팝 푸시 할때 어라이벌 타임도 갱신해서 넣으면 , 동일한 우선순위를 가진 거에 대해서도 추가적인 정책도 생각해볼 수도