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


// ���߿� �� Ǫ�� �Ҷ� ����̹� Ÿ�ӵ� �����ؼ� ������ , ������ �켱������ ���� �ſ� ���ؼ��� �߰����� ��å�� �����غ� ����