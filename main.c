#include "scheduler.h"

int main() {
	srand(time(NULL));
	int input;
	while (1) {
		printf("< CPU SCHEDULING SIMULATOR >\n");
		printf("0. Config() 1. Create_Process 2. Show_Process\n3. FirstComeFirstServed 4. ShortestJobFirst 5. Priority 6. RoundRobin 7. Preemptive SJF 8. Preemptive Priority\n9. Evaluation\n\nAny other key to exit\n\n");
		printf(">> ");
		scanf("%d", &input);
		printf("\n");
		switch (input)
		{
		case 0:
			break;
		case 1:
			Create_Process();
			break;
		case 2:
			Show_Process();
			break;
		case 3:
			FCFS();
			break;
		case 4:
			SJF();
			break;
		case 5:
			Priority();
			break;
		case 6:
			RR();
			break;
		case 7:
			Preemptive_SJF();
			break;
		case 8:
			Preemptive_Priority();
			break;
		case 9:
			Evaluation();
			break;
		default:
			return 0;
			break;
		}
		printf("\n\n");
	}
	return 0;
}


// 나중에 팝 푸시 할때 어라이벌 타임도 갱신해서 넣으면 , 동일한 우선순위를 가진 거에 대해서도 추가적인 정책도 생각해볼 수도