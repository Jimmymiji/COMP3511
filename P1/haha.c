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

int exit_flag = 0;

#define ARG_SIZE 100

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

int main ()  
{
    char cmdline[] =  "ls | wc";
    process_cmd(cmdline);
    return 0;
}

// void process_cmd(char *cmdline)
// {
//     // printf("Debug: %s\n", cmdline); // delete this line to start your work
//     // printf("length of cmdline: %lu\n",strlen(cmdline));
//     int n = strlen(cmdline);
//     char argv[MAX_PIPE_SEGMENTS][MAX_SEGMENT_LENGTH];
//     int  numOfTokens = 0;
//     int  pfds[MAX_PIPE_SEGMENTS][2];
//     const char delimeter[] = "|";
//     int status;
//     char* ptr = strstr(cmdline,"exit");


//     if( ptr != NULL){
//         printf("The shell program(PID=%d) terminates ",getpid());
//         exit(0);
//     }
//     else{
//         char** argvout = tokenize(argv,cmdline,&numOfTokens,delimeter);
//         // if single or none command need to be excuted
//         if(numOfTokens <= 1){
//             pid_t pid = fork();
//             if(pid == 0){
//                 char argv[10][20];
//                 int numofTokens = 0;
//                 const char space[] = " ";
//                 char** paramsOut = tokenize(argv,cmdline,&numofTokens,space);
//                 execlp(paramsOut[0],paramsOut[0],paramsOut[1],paramsOut[2], NULL);
//             }
//             else{
//                 wait(0);
//             }
//         }
//         else{
//             for(int i = 0; i<numOfTokens-1; i++){
//                 pipe(pfds[i]);
//             }
//             for(int i = 0; i<numOfTokens;i++){
//                 pid_t pid = fork();
//                 if(pid == 0){
//                     if(i != 0){
//                         dup2(pfds[i-1][0],0);
//                     }
//                     if(i != numOfTokens - 1){
//                         dup2(pfds[i][1],1);
//                     }

//                     for(int j = 0; j < numOfTokens - 1;j++){
//                         // close(pfds[j][0]);
//                         // close(pfds[j][1]);
//                     }
//                     //printf("The shell program(PID=%d) process %s \n",getpid(),argvout[i]);
//                     char argv[10][20];
//                     int numofTokens = 0;
//                     const char space[] = " ";
//                     char** paramsOut = tokenize(argv,argvout[i],&numofTokens,space);
//                     execlp(paramsOut[0],paramsOut[0],paramsOut[1],paramsOut[2], NULL);
//                     exit(0);
//                 }
//                 else{
//                     for(int j = 0; j < numOfTokens -1;j++){
//                         // close(pfds[j][0]);
//                         // close(pfds[j][1]);
//                     }
//                     wait(pid);
//                     //waitpid(pid,&status,0);
//                 }
//             }
//         }
 
//     }
    
//     return;
// }
void process_cmd(char* cmdline){
    int size = 1000,i,j;
    int pipe_count = 0;
    int fd[100][2],cid1,cid2,length,status;
    char argv[100][100];
    int  numOfTokens = 0;
    const char delimeter[] = "|";
    char** string = tokenize(argv,cmdline,&numOfTokens,delimeter);
    pipe_count = numOfTokens - 1;

    if(pipe_count)
    {
        for(i = 0;i < pipe_count;i++)
        {
            pipe(fd[i]);
        }

        for(i = 0;i <= pipe_count;i++)
        {
            cid1 = fork();
            if(!cid1)
            {

                if(i!=0)
                {
                    dup2(fd[i-1][0],0);
                }


                if(i!=pipe_count)
                {
                    dup2(fd[i][1],1);
                }


                for(j = 0;j < pipe_count;j++)
                {   
                        close(fd[j][0]);
                        close(fd[j][1]);
                }

                char argv[10][20];
                int numofTokens = 0;
                const char space[] = " ";
                char** paramsOut = tokenize(argv,string[i],&numofTokens,space);
                execlp(paramsOut[0],paramsOut[0],paramsOut[1],paramsOut[2], NULL);
                //execlp(string[i], string[i], NULL);
                exit(0);
            }
        }
        for(i = 0;i < pipe_count;i++)
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        //waitpid(cid1,&status,0);
        wait(cid1);
    }
    else
    {
        execlp(string[0], string[0], NULL);
    }


    return 0;
}