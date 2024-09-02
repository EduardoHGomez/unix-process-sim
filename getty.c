#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void log_user()
{
    // Todo lo relacionado con leer el archivo
    FILE *file;
    file = fopen("passwd.txt", "r");

    if (file == NULL) {
        printf("File can't be opened \n");
    }

    // User y Password
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
        char buffer[256]; // Buffer de cada linea

        while (fgets(buffer, sizeof(buffer), file)) 
        {
            // Remove newline character if present
            buffer[strcspn(buffer, "\n")] = 0;

            // Split the line into username and password
            char *file_user = strtok(buffer, ":");
            char *file_password = strtok(NULL, ":");

            // Check if both username and password match
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



int main () 
{

        log_user();
        printf("Saliendo\n");


    return 0;
}