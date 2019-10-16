#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *fib(void *arg);

int main(int argc, char **argv){
	int n = atoi(argv[1]);

	printf("%d\n", (int)fib(n));
}

void *fib(void *arg){
	int n;
	pthread_t thread1;
	pthread_t thread2;
	void *a;
	void *b;
	int c;

	n = (int)arg;
	if (n <= 0) return 0;
	if (n == 1) return 1;
	pthread_create(&thread1, NULL, fib, n-1);
	pthread_create(&thread2, NULL, fib, n-2);

	pthread_join(thread1, &a);
	pthread_join(thread2, &b);

	c = (int)a + (int)b;
	return (c);
}
