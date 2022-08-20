#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


#define MAX_BUF_SIZE 1024
#define PADDING 16
void find(const char *path, const char *fn){
	int fd;
	struct stat st;
	struct dirent de;
	char *p;
	char buf[MAX_BUF_SIZE + PADDING];
	if((fd = open(path, 0)) < 0){
		fprintf(2, "ls: cannot open %s\n", path);
		exit(1);
	}
	if(fstat(fd, &st) < 0){// TODO: fstat  stat
		fprintf(2, "ls: cannot stat %s\n", path);
		exit(1);
	}
	switch(st.type)
	{
		case T_FILE:
			printf("path is a file\n");
			exit(1);
		case T_DIR:
			if(strlen(path) > MAX_BUF_SIZE){
				printf("path too long\n");
				exit(1);
			}
			strcpy(buf, path);// "/path"
			p = buf + strlen(buf);
			*p++ = '/';

			while(read(fd, &de ,sizeof(de)) == sizeof(de)){
				if(de.inum == 0) continue;

				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = '\x00';
				if(stat(buf, &st) < 0){
					fprintf(2, "ls: cannot stat %s\n", path);
					continue;
				}
				if(st.type == T_DIR && strcmp(p, ".") && strcmp(p, "..")){
					find(buf, fn);
					continue;
				}
				if(!strcmp(p ,fn)){
					printf("%s\n", buf);
				}
			}
			break;
	}
	close(fd);
}


int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Usage: find /path/to/dir filename");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}