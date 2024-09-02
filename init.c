#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int num_children = 6;

void create_getty(){

    
    
}

int main(){
        
    for(int i =0; i<6; i++){
        if(fork()==0){
            execlp("xterm","xterm","-e"," ./getty",(char *)NULL);

        }
        
    }
}