#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    void* start = sbrk(0);
    void* p = sbrk(1032);
    void* end  = sbrk(0);
    printf("start: %p\n",start);
    printf("end: %p\n",end);
    printf("now: %p\n",sbrk(0));
    return 0;
}