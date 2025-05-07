#include "process.h"

int main() {
	process_info process_stack[10] = { {1,2,3,3,3,3},{2,3,4,4,4,3},{3,4,5,5,5,3} };
	int top = 2;
	for (int i = 0; i <= top; i++) {
		if (fork() == 0) {
			Create_Process(&process_stack[i]);
			exit(0);
		}
	}
	while (wait(NULL) > 0);
	return 0;
}