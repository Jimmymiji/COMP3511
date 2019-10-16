#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


// Assume that each command line has at most 256 characters (including NULL)
#define MAX_CMDLINE_LEN 256

// Assume that we have at most 16 pipe segments
#define MAX_PIPE_SEGMENTS 16

// Assume that each segment has at most 256 characters (including NULL)
#define MAX_SEGMENT_LENGTH 256

/*
  Function  Prototypes
 */
void show_prompt();
int get_cmd_line(char *cmdline);
void process_cmd(char *cmdline);
void **tokenize(char **argv, char *line, int *numTokens, char *token);


/* The main function implementation */
int main()
{
    char cmdline[MAX_CMDLINE_LEN];
    printf("The shell program (pid=%d) starts\n", getpid());
    while (1)
    {
        show_prompt();
        if (get_cmd_line(cmdline) == -1)
            continue; /* empty line handling */

        process_cmd(cmdline);
    }
    return 0;
}

/* 
    Implementation of process_cmd
    TODO: Clearly explain how you implement process_cmd in point form. For example:
    Step 1: ....
    Step 2: ....
        Step 2.1: .....
        Step 2.2: .....
            Step 2.2.1: .....
    Step 3: ....
 */
void process_cmd(char *cmdline)
{
    //printf("Debug: %s\n", cmdline); // delete this line to start your work
    int n = strlen(cmdline);
    char argv[MAX_PIPE_SEGMENTS][MAX_SEGMENT_LENGTH];
    int  numOfTokens = 0;
    int  pfds[MAX_PIPE_SEGMENTS][2];
    const char delimeter[] = "|";
    int status;
    pid_t pid;
    char* ptr = strstr(cmdline,"exit");

    if( ptr != NULL){
        printf("The shell program(PID=%d) terminates \n",getpid());
        exit(0);
    }
    else{
        char** argvout = tokenize(argv,cmdline,&numOfTokens,delimeter);
        int numOfPipe = numOfTokens - 1;
        // if single or none command need to be excuted
        if(numOfTokens <= 1){
            pid = fork();
            if(pid == 0){
                char argv[10][20];
                int numofTokens = 0;
                const char space[] = " ";
                char** paramsOut = tokenize(argv,cmdline,&numofTokens,space);
                execlp(paramsOut[0],paramsOut[0],paramsOut[1],paramsOut[2], NULL);
            }
            else{
                wait(0);
            }
        }
        else{
            int i;
            for( i = 0; i<numOfPipe; i++){
                pipe(pfds[i]);
            }
            for( i = 0; i<numOfTokens;i++){
                pid = fork();
                if(pid == 0){
                    if(i != 0){
                        dup2(pfds[i-1][0],0);
                    }
                    if(i != numOfPipe){
                        dup2(pfds[i][1],1);
                    }
                    int j;
                    for(j = 0; j < numOfPipe;j++){
                        close(pfds[j][0]);
                        close(pfds[j][1]);
                    }
                    //printf("The shell program(PID=%d) process %s \n",getpid(),argvout[i]);
                    char argv[10][20];
                    int numofTokens = 0;
                    const char space[] = " ";
                    char** paramsOut = tokenize(argv,argvout[i],&numofTokens,space);
                    execlp(paramsOut[0],paramsOut[0],paramsOut[1],paramsOut[2], NULL);
                    exit(0);
                }
            }
            int j;
            for(j = 0; j < numOfPipe;j++){
                close(pfds[j][0]);
                close(pfds[j][1]);
            }
            wait(pid);
        }
    }
    return;
}


void show_prompt()
{
    printf("$> ");
}

int get_cmd_line(char *cmdline)
{
    int i;
    int n;
    if (!fgets(cmdline, MAX_CMDLINE_LEN, stdin))
        return -1;
    // Ignore the newline character
    n = strlen(cmdline);
    cmdline[--n] = '\0';
    i = 0;
    while (i < n && cmdline[i] == ' ') {
        ++i;
    }
    if (i == n) {
        // Empty command
        return -1;
    }
    return 0;
}

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