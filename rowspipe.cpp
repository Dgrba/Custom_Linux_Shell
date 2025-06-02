#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main(){

    int fd[2];

    if(pipe(fd) < 0){

        cout << "Failed to create a pipe." << endl;
        return -1;
    }

    int pid = fork();

    if(pid == 0){
        
        close(fd[0]);
        char str[] = "Hello World";
        write(fd[1], str, strlen(str));
        close(fd[1]);
    }
    else{

        close(fd[1]);
        char readbuffer[100];
        read(fd[0], readbuffer, sizeof(readbuffer));
        close(fd[0]);

        cout << "The parent got message: " << readbuffer << endl;   

        waitpid(pid, NULL, 0);
    }



    return 0;
}