#include "scheduler.h"

int main() {
	//Config();
	Create_Process();
	Show_Process();
	FCFS();
	printf("\n\n");
	Show_Process();
	SJF();
	printf("\n\n");
	Show_Process();
	Priority();
	printf("\n\n");
	Show_Process();
	RR();
	printf("\n\n");
	Show_Process();
	Preemptive_SJF();
	printf("\n\n");
	Show_Process();
	Preemptive_Priority();
	printf("\n\n");
	Evaluation();
	return 0;
}


// ���߿� �� Ǫ�� �Ҷ� ����̹� Ÿ�ӵ� �����ؼ� ������ , ������ �켱������ ���� �ſ� ���ؼ��� �߰����� ��å�� �����غ� ����
// �˰��� �� �� �ʿ�