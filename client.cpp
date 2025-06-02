#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <errno.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

typedef struct {
	vector<vector<double>> v_tmp;
    vector <double> full_rows;
    int thold;
} MY_ARGS;

struct sockaddr_un {
    sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[108];            /* Pathname */
};
	
int row_full(vector<double> tmp_row, int thold) {
	int row_size = tmp_row.size();
	int greater_count = 0;
	for (int i = 0; i < row_size; i++) {
		if (tmp_row[i] > thold) {
			greater_count++;
		}
	}
	if (greater_count == row_size){
		return 1;
	}

	return 0;
}

void * find_rows(void* args) {
	MY_ARGS* my_args = (MY_ARGS*)args;
	vector<vector<double>> v_tmp = my_args->v_tmp;
	int thold = my_args->thold;

	int i = first;
	int counter = 0;
	for (; i < last; i++) {
		vector<double> tmp_row = v_tmp[i];
		if (row_full(tmp_row, thold)) {
			my_args->row_count++;
		}
	}

	return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    ssize_t numRead;
    char buf[BUFSIZ];

    int thold = stoi(argv[1]);

    // Create a new client socket with domain: AF_UNIX, type: SOCK_STREAM, protocol: 0
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Client socket fd = %d\n", sfd);

    // Make sure socket's file descriptor is legit.
    if (sfd == -1) {
      cout << "error with socket descriptor" << endl;
      return -1;
    }

    //
    // Construct server address, and make the connection.
    //
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Connects the active socket referred to be sfd to the listening socket
    // whose address is specified by addr.
    if (connect(sfd, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1) {
      cout << "error with connection" << endl;
      return -1;
    }

    //
    // Copy stdin to socket.
    //

    // Read at most BUF_SIZE bytes from STDIN into buf.
    while ((numRead = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
      // Then, write those bytes from buf into the socket.
      if (write(sfd, buf, numRead) != numRead) {
        cout << "error with write" << endl;
        return -1;
      }
    }

    if (numRead == -1) {
        cout << "error with read" << endl;
        return -1;
    }

    // Closes our socket; server sees EOF.
    exit(EXIT_SUCCESS);
}