#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#define CMDLEN 128
#define MAXPAR 16


int main()
{
    int nextcmd = 1;
    char command[CMDLEN];
    int p;
    char *cmdargs[MAXPAR];
    int i;


    while(nextcmd)
    {
        printf("Shell>");
        fgets(command, CMDLEN, stdin);
        command[strcspn(command,"\n\r")] = 0;


        // Separar cada una de las cadenas de command para ponerlas en el arreglo de apuntadores
        i=0;
        cmdargs[i] = strtok(command," ");  // El token separador es el espacio


        if(cmdargs[i]==0)
            continue;


        while(cmdargs[i]!=NULL)
        {
            i++;
            cmdargs[i] = strtok(NULL," ");
        }


        if(strcmp(command,"exit")!=0)
        {
            p = fork();
            if(p==0)
                execvp(cmdargs[0],cmdargs);
            wait(NULL);
        }
        else
            nextcmd = 0;
    }
}