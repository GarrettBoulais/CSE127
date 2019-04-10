#include<stdio.h>

int f(int num) {
	if(num == 0 || num == 1){
		return num;
	}
	else {
		return f(num-1) + f(num-2);
	}
}

int main(void) {
	int limit = 10, i, n;

	printf("Here are the first %d numbers in the Fibonacci sequence:\n", limit);
	for(i = 0; i < limit; i++) {
		n = f(i);
		printf("%d\n", n);
	}

	return 0;
}
