#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define ifnot(x) if(!(x))
#define SIZE (35-2)
// 35 -> 2 3 5 7 11 13 17 19 23 29 31
/*
	2 3 4 5 6 7 8 9 10 11 -> 2 
	0 3 0 5 0 7 0 9 0  11 -> 3
	0 0 0 5 0 7 0 0 0  11 -> 5
	0 0 0 0 0 7 0 0 0  11 -> 7
	0 0 0 0 0 0 0 0 0  11 -> 11
	0 0 0 0 0 0 0 0 0  0
*/
int check_is_fini(int arr[]){
	int n = 0;
	for(int i = 0; i <= SIZE; i++)
		n |= arr[i];
	return !n;
}
void recursion(int p[2]){
	int pid, arr[SIZE], _p_[2], n ,pri = 0;// first num is prime
	pipe(_p_);

	// first and second elem discard
	for(int i = 0; i <= SIZE ; i++){
		read(p[0], &n, sizeof(n));
		if(n == 0) {
			arr[i] = 0;
			continue;
		}
		if(pri == 0) {// first prime assign
			pri = n;
			arr[i] = 0;
			printf("prime %d\n", (int)pri);
		}
		if(n % pri){
			arr[i] = n;
		}else{
			arr[i] = 0;
		}
	}// sieve vaild value into pipe

	if(check_is_fini(arr)) exit(0);

	// parent write arr to pipe, child pass pipe to next level recursion 
	if((pid = fork()) == 0){
		close(_p_[1]);
		recursion(_p_);
		exit(0);
	}else{
		close(_p_[0]);
		for(int idx = 0; idx <= SIZE ; idx++)
			write(_p_[1], &arr[idx], sizeof(n));
	}
	exit(0);
}

int
main(int argc, char *argv[]){
	int p[2];
	int x = 0;
	pipe(p);
	for(int i = 2; i <= SIZE + 2; i++ ){// initialize
		write(p[1], &i, sizeof(x));
	}
	recursion(p);
	exit(0);
}