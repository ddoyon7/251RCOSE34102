#include "process.h"

int main() {

	Config();

	for (int i = 0; i < pl_num; i++) {
		if (fork() == 0) {
			Create_Process(&process_list[i]);
			exit(0);
		}
	}
	while (wait(NULL) > 0);
	return 0;
}