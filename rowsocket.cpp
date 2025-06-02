#include "lib/error_functions.h"
#include "sockets/unix_socket.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

using namespace std;

struct sockaddr_un {
        sa_family_t sun_family;               /* AF_UNIX */
        char        sun_path[108];            /* Pathname */
};

int main(int argc, const char* argv[]){
    struct sockaddr_un addr;

    int sv[2];
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(sfd == -1){
        cout << "Error!" << endl;
        return -1;
    }

    if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1) {
    
        cout << "Server socket path too long: " << SV_SOCK_PATH << endl;
        return -1;
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {

        errExit("remove-%s", SV_SOCK_PATH);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {

        errExit("bind");
    }

    if (listen(sfd, BACKLOG) == -1) {

        errExit("listen");
    }

    char* str;

    if(pid == 0){

        char buff[100];

        read(sv[1], &buff, sizeof(buff));

        cout << "Child: receive " << buff << endl;


    }
    else{

        str = "Hello World!";

        write(sv[0], str, strlen(str));

        cout << "Parent: sent " << str << endl;

        wait(NULL);

    }



    return 0;
}