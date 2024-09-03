#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXIT_SHUTDOWN 99 // Define the same shutdown exit code

// Function to handle user login
void log_user() {
    FILE *file = fopen("passwd.txt", "r");
    if (file == NULL) {
        printf("File can't be opened \n");
        exit(1);
    }

    char user[25];
    char password[25];
    int found = 0;

    while (1) {
        printf("User: ");
        scanf("%s", user);

        printf("Password: ");
        scanf("%s", password);

        rewind(file);
        char buffer[256]; // Buffer for each line

        while (fgets(buffer, sizeof(buffer), file)) {
            buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

            char *file_user = strtok(buffer, ":");
            char *file_password = strtok(NULL, ":");

            if (strcmp(user, file_user) == 0 && strcmp(password, file_password) == 0) {
                found = 1;
                break; // Match found
            }
        }

        if (found) {
            printf("Login successful!\n");
            break;
        } else {
            printf("Invalid username or password. Please try again.\n");
        }
    }

    fclose(file);
}

int main() {
    log_user();

    // Execute the shell and capture its exit status
    int status;
    if (fork() == 0) {
        execlp("./sh", "./sh", (char *)NULL);
        perror("Failed to start shell");
        exit(1);
    }

    wait(&status); // Wait for the shell to exit
    printf("Status received from shell in getty: %d (exit code: %d)\n", status, WEXITSTATUS(status)); // Debugging print

    // Propagate the shutdown signal upwards if received
    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SHUTDOWN) {
        printf("getty: Propagating shutdown signal upwards with exit code %d\n", EXIT_SHUTDOWN); // Debugging print
        exit(EXIT_SHUTDOWN); // Exit with the shutdown code
    }

    printf("getty: Exiting normally.\n");
    return 0; // Normal exit if no shutdown was requested
}
