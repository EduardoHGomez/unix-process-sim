#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#define CMDLEN 128
#define MAXPAR 16

void sighandler(int signum) {
    printf("sh: Received signal %d\n", signum);
    fflush(stdout);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <init_pid>\n", argv[0]);
        exit(1);
    }

    int init_pid = (int)atoi(argv[1]);
    printf("sh: Started with init PID: %d\n", init_pid);
    fflush(stdout);

    signal(SIGTERM, sighandler);

    int nextcmd = 1;
    char command[CMDLEN];
    char *cmdargs[MAXPAR];
    int status;

    while(nextcmd)
    {
        printf("Shell> ");
        fflush(stdout);
        fgets(command, CMDLEN, stdin);
        command[strcspn(command,"\n\r")] = 0;

        int i=0;
        cmdargs[i] = strtok(command," ");
        if(cmdargs[i]==NULL) continue;
        while(cmdargs[i]!=NULL)
        {
            i++;
            cmdargs[i] = strtok(NULL," ");
        }

        if(strcmp(cmdargs[0],"exit")==0)
        {
            nextcmd = 0;
        }
        else if(strcmp(cmdargs[0],"shutdown")==0)
        {
            printf("sh: Shutdown command received. Sending SIGUSR1 to init (PID: %d)\n", init_pid);
            fflush(stdout);
            if (kill(init_pid, SIGUSR1) == -1) {
                perror("sh: Failed to send SIGUSR1");
            } else {
                printf("sh: SIGUSR1 sent successfully\n");
            }
            fflush(stdout);
            exit(0);
        }
        else
        {
            int p = fork();
            if(p==0)
            {
                execvp(cmdargs[0],cmdargs);
                perror("sh: execvp failed");
                exit(1);
            }
            wait(&status);
        }
    }
    return 0;
}
