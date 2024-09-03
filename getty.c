#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void handle_sigterm(int sig) {
    exit(0);
}

int log_user()
{
    FILE *file;
    file = fopen("passwd.txt", "r");
    if (file == NULL) {
        printf("File can't be opened \n");
        return 0;
    }

    char user[25];
    char password[25];
    int found = 0;

    while (1)
    {
        printf("User: ");
        scanf("%s", user);
        printf("Password: ");
        scanf("%s", password);

        rewind(file);
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) 
        {
            buffer[strcspn(buffer, "\n")] = 0;
            char *file_user = strtok(buffer, ":");
            char *file_password = strtok(NULL, ":");
            if (strcmp(user, file_user) == 0 && strcmp(password, file_password) == 0) {
                found = 1;
                break;
            }
        }

        if (found) {
            printf("Login successful!\n");
            fclose(file);
            return 1;
        } else {
            printf("Invalid username or password. Please try again.\n");
        }        
    }
}

int main(int argc, char *argv[]) 
{
    signal(SIGTERM, handle_sigterm);

    while (1) {
        int n = log_user();
        if (n == 1) {
            int pid = fork();
            if (pid == 0) {
                char *ppid_str = argv[1];
                execl("./sh", "sh", ppid_str,(char *)NULL);
                perror("execl failed");
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork failed");
                exit(1);
            }
        }
    }

    return 0;
}