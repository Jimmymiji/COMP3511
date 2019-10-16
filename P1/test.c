#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pfds[2];
    pipe(pfds);
    pid_t pid = fork(); /* 0 (child), non-zero (parent) */
    if (pid == 0)
    {                   /* The child process*/
        /* don't need this */
        execlp("cat", "cat","/home/jimmy/COMP3511/P1/test.c", NULL);
    }
    wait(0);
    return 0;
}