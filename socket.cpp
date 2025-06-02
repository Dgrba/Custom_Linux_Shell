#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>


void myfun(int signum){
    return;
}

int main(){

    int shmid;
    shmid = shmget(IPC_PRIVATE, 128, IPC_CREAT | 0777);

    if(shmid < 0){
        cout << "Failed to create shared memory." << endl;
        return -1;
    }

    char* p;

    int pid = fork();

    if(pid ==0){
        p = (char*) shmat(shmid, NULL, 0);
        if(p == NULL){
            cout << "Child process: shmat function failure." << endl;
            return -1;
        }

        char str[] = "Hello World!";
        memcpy(p, str, strlen(str));

        kill(getppid(), SIGUSR2);
    }
    else{

        signal(SIGUSR2, myfun);

        p = (char*) shmat(shmid, NULL, 0);
        if(p == NULL){
            cout << "Child process: shmat function failure." << endl;
            return -1;
        }

        pause();

        cout << "shared memory data: " << p << endl;

        waitpid(pid, NULL, 0);
    }
    
    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
    return 0
}