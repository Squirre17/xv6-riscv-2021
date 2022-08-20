#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXSIZE 1024
// find . b | xargs grep hello
int main(int argc, char *argv[]){
	if(argc < 2) {
		printf("Usage: xargs cmd\n");
		exit(1);
	}
	char *argvs[MAXARG];
	char tmp[MAXSIZE];
	char stdin_buf[MAXSIZE];
	char *ptr = tmp;
	int n, cnt = 0;
	for(int i = 1; i < argc; i++) {
		argvs[cnt++] = argv[i];
	}
	int t = 0;// extra argus count from stdin_buf truncated by ' ' until '\n'
	// pipe input from stdin
	while((n = read(0, stdin_buf, MAXSIZE)) > 0){
		for(int j = 0; j < n; j++){// traverse stdin
			if(stdin_buf[j] == '\n'){// pass final argu and fork-exec
				tmp[t] = '\x00';
				argvs[cnt++] = ptr;
				argvs[cnt] = 0;
				t = 0;// but there are elems left in stdin_buf
				ptr = tmp;
				cnt = argc - 1; // resume for next \n 
				if(fork() == 0){
					exec(argv[1], argvs);
				}
				// waiting for child process execute over
				wait(0);
			}else if(stdin_buf[j] == ' '){
				tmp[t++] = '\x00';
				argvs[cnt++] = ptr;
				ptr = &tmp[t];
			}else{
				tmp[t++] = stdin_buf[j];
			}
		}
	}
	exit(0);
}

