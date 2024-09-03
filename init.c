#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CMDLEN 128
#define MAXPAR 16
#define EXIT_SHUTDOWN 99 // Define the shutdown exit code

int main() {
    int nextcmd = 1;
    char command[CMDLEN];
    int p;
    char *cmdargs[MAXPAR];
    int i;

    while (nextcmd) {
        printf("Shell> ");
        fgets(command, CMDLEN, stdin);
        command[strcspn(command, "\n\r")] = 0;

        // Split the command into arguments
        i = 0;
        cmdargs[i] = strtok(command, " ");

        if (cmdargs[i] == 0)
            continue;

        while (cmdargs[i] != NULL) {
            i++;
            cmdargs[i] = strtok(NULL, " ");
        }

        // Check if the shutdown command is issued
        if (strcmp(command, "shutdown") == 0) {
            printf("Shutting down... Exiting with code %d\n", EXIT_SHUTDOWN); // Debug: confirm exit code
            exit(EXIT_SHUTDOWN); // Exit with the correct shutdown code (99)
        }

        // Execute other commands if not shutdown
        if (strcmp(command, "exit") != 0) {
            p = fork();
            if (p == 0)
                execvp(cmdargs[0], cmdargs);
            wait(NULL);
        } else {
            nextcmd = 0;
        }
    }

    printf("Exiting shell normally.\n");
    return 0; // Normal exit if no shutdown
}
