#include<stdio.h>
#include<stdlib.h>

int fin(int n);

int fib(int arg){
    int n = arg;
    if (n<=0){
        return 0;
    }
    if (n == 1){
        return 1;
    }
    int ans = fib(n-1) + fib(n-2);
    return ans;
}

int main(int argc,char** argv){

    int n = atoi(argv[1]);
    printf("%d\n",fib(n));
}