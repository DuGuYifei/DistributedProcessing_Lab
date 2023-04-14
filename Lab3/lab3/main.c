#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void fork_process(int n, int children_num){
	if(n == 0){
		sleep(5);
		exit(0);
	}
	for(int i = 0; i < children_num; i++){
		int child_pid = fork();
		// child prcess return 0
		if(child_pid == 0){
			printf("Child process: pid-%d | ppid-%d\n", getpid(), getppid());
			fork_process(n - 1, children_num);
			sleep(5);
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		printf("Wrong number of arguments. Plz input one/two positive number\n");
		return 0;
	}
	int n = atoi(argv[1]);
	if (n <= 0)
	{
		printf("Wrong arguments. Plz input one/two positive number\n");
		return 0;
	}
	int children_num = 2;
	if(argc == 3)
	{
		children_num = atoi(argv[2]);
		if(children_num < 2)
		{
			printf("Wrong children num. Use default: 2\n");
			return 0;
		}
	}
	printf("Number of levels: %d\n", n);
	printf("Number of children: %d\n", children_num);

	int root = getpid();
	printf("Root: %d\n", root);
	fork_process(n - 1, children_num);
	if(root == getpid()){
		// tree view of user process
		execlp("ps", "-u yifei", "--forest", NULL);
	}

	return 1;
}