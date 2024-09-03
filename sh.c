#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int num_children = 6;
int processes[6]; // Array to store PIDs of child processes
#define EXIT_SHUTDOWN 99 // Define a special exit code for shutdown

// Function to create getty process in xterm
void create_getty(int index) {
    int pid = fork();
    if (pid == 0) {
        // Create a new getty process
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
        sleep(1);
    }

    // Monitor the children to ensure they are always running
    while (1) {
        int status;
        int terminated_pid = wait(&status); // Capture the exit status of the child process
        printf("Status received from getty in init: %d (exit code: %d)\n", status, WEXITSTATUS(status)); // Debugging print

        // Check if the exit status indicates a shutdown
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SHUTDOWN) {
            printf("Shutdown requested. Terminating all processes...\n");

            // Terminate all getty processes
            for (int i = 0; i < num_children; i++) {
                if (processes[i] > 0) { // Ensure the process exists before attempting to kill
                    printf("Killing process %d\n", processes[i]); // Debug statement
                    kill(processes[i], SIGKILL);
                }
            }
            break; // Exit the loop and end the init process
        }

        // Restart the terminated process if it's not a shutdown
        for (int i = 0; i < num_children; i++) {
            if (terminated_pid == processes[i]) {
                printf("Process %d terminated. Restarting...\n", terminated_pid); // Debug statement
                create_getty(i); // Restart the terminated process
                break;
            }
        }
    }

    printf("All processes have been terminated. Exiting init.\n");
    return 0; // Gracefully exit after shutdown
}
