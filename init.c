#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int num_children = 6;
int processes[6]; // Array to store PIDs of child processes

// Function to create getty process in xterm
void create_getty(int index) {
    int pid = fork();
    if (pid == 0) {
        // Child process: Replace its image with getty using xterm
        execlp("xterm", "xterm", "-e", "./getty", (char *)NULL);
        perror("Failed to exec getty");
        exit(1);
    } else if (pid > 0) {
        // Parent process: Store the child's PID in the array
        processes[index] = pid;
    } else {
        perror("Fork failed");
    }
}

int main() {
    // Create 6 getty processes
    for (int i = 0; i < num_children; i++) {
        create_getty(i);
    }

    // Monitor the children to ensure they are always running
    while (1) {
        // Wait for any child to terminate
        int terminated_pid = wait(NULL);

        // Find which process ended and restart it
        for (int i = 0; i < num_children; i++) {
            if (terminated_pid == processes[i]) {
                printf("Process %d terminated. Restarting...\n", terminated_pid);
                create_getty(i); // Restart the terminated process
                break;
            }
        }
    }

    return 0;
}
