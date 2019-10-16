#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


void **tokenize(char **argv, char *line, int *numTokens, char *delimiter)
{
    int argc = 0;
    char *token = strtok(line, delimiter);
    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, delimiter);
    }
    argv[argc++] = NULL;
    *numTokens = argc - 1;
    return argv;
}

int main() {
    printf("*********");
    char argv[10][20];
    int numofTokens = 0;
    char line[] = "ps aux | grep root | sort | less";
    const char delimiter[] = "|";
    printf("******");
    char** out = tokenize(argv,line,&numofTokens,delimiter);
    printf("num of tokens: %d \n",numofTokens);
    for(int i = 0; i<numofTokens; i++){
        printf("*%s*\n",out[i]);
    }
    //execlp(out[0],out[0],out[1],out[2],NULL);
    return 0;
}