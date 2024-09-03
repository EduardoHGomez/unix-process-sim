#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define NUM_TERMINALS 6

int child_pids[NUM_TERMINALS];

void handle_shutdown(int sig) {
    printf("init: Initiating shutdown...\n");
    fflush(stdout);
    for (int i = 0; i < NUM_TERMINALS; i++) {
        if (child_pids[i] > 0) {
            printf("init: Sending SIGTERM to child PID %d\n", child_pids[i]);
            fflush(stdout);
            kill(child_pids[i], SIGTERM);
        }
    }
    
    printf("init: Waiting for all children to terminate...\n");
    fflush(stdout);
    while (wait(NULL) > 0);

    printf("init: System shutdown complete.\n");
    fflush(stdout);
    exit(0);
}

void spawn_terminal(int index) {
    int pid = fork();
    if (pid == 0) {
        char ppid_str[20];
        snprintf(ppid_str, sizeof(ppid_str), "%d", getppid());
        printf("init: Spawning getty with PID: %d\n", getpid());
        fflush(stdout);
        execlp("xterm", "xterm", "-e", "./getty", ppid_str, (char *)NULL);
        perror("init: execlp failed");
        exit(1);
    } else if (pid > 0) {
        child_pids[index] = pid;
        printf("init: Spawned getty with PID: %d\n", pid);
        fflush(stdout);
    } else {
        perror("init: fork failed");
        exit(1);
    }
}

int main() {
    printf("init: Starting with PID: %d\n", getpid());
    fflush(stdout);

    if (signal(SIGUSR1, handle_shutdown) == SIG_ERR) {
        perror("init: Failed to set SIGUSR1 handler");
        exit(1);
    }

    for (int i = 0; i < NUM_TERMINALS; i++) {
        child_pids[i] = 0;
    }

    for (int i = 0; i < NUM_TERMINALS; i++) {
        spawn_terminal(i);
    }

    while (1) {
        int status;
        int terminated_pid = wait(&status);
        if (terminated_pid > 0) {
            printf("init: Child process %d terminated with status %d\n", terminated_pid, WEXITSTATUS(status));
            fflush(stdout);
            for (int i = 0; i < NUM_TERMINALS; i++) {
                if (child_pids[i] == terminated_pid) {
                    printf("init: Respawning terminal %d\n", i);
                    fflush(stdout);
                    spawn_terminal(i);
                    break;
                }
            }
        }
    }

    return 0;
}
