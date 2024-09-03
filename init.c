#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int num_children = 6;
int processes[6];

// Function to create getty process in xterm
void create_getty(int index) {
    int pid = fork();
    if (pid == 0) {
        processes[index] = pid;
        execlp("xterm", "xterm", "-e", "./getty", (char *)NULL);
        perror("Failed to exec getty");
        exit(1);
    }
}

int main() {
    // Create 6 getty processes
    for (int i = 0; i < num_children; i++) {
        create_getty(i);
    }


    return 0;
}
