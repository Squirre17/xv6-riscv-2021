#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PIPE_RW_SIZE 256
int
main(int argc, char *argv[]){
	if(argc != 1){
		printf("pingpong don't have argument\n");
		exit(1);
	}
	// if bidirective communication ,need two pipe ,one p write to c ,other revserse
	int ptoc[2];
	int ctop[2];
	if(((pipe(ptoc)) < 0) || (pipe(ctop) < 0)) {// return two file descriptor p[0] is read, p[1] is write
		printf("pipe error\n");
		exit(1);
	}
	int pid = fork();
	// child will inherit parent's file descriptor
	char buf[MAX_PIPE_RW_SIZE];
	if(pid < 0){
		printf("Error: fork failed\n");
		exit(1);
	}else if(pid == 0){// child process
		close(ptoc[1]);// parent to child ,child only need read
		close(ctop[0]);

		read(ptoc[0], buf ,MAX_PIPE_RW_SIZE);
		write(ctop[1], buf, MAX_PIPE_RW_SIZE);
		printf("%d: received ping\n", getpid());
		exit(0);
	}else{// parent process
		close(ctop[1]);
		close(ptoc[0]);
		
		write(ptoc[1], "mother in here", MAX_PIPE_RW_SIZE);
		read(ctop[0], buf ,MAX_PIPE_RW_SIZE);
		printf("%d: received pong\n", getpid());
		exit(0);
	}
}