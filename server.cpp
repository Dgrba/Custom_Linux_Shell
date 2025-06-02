#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BACKLOG 5

using namespace std;

struct sockaddr_un {
    sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[108];            /* Pathname */
};


int main(int argc, const char* argv[]){
    
    //-----create socket----------//
    struct sockaddr_un addr;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0); 

    //------------csv file testing-----------------//
    string file_name(argv[1]);
    ifstream fp(file_name);
    if (!fp) { 
		fprintf(stderr, "failed to open file for reading\n");
		return 1;
	}

    //------------csv file data reading begin-----------------//
    int idx = 0;
    getline(fp, line);
	
	stringstream ss(line);
	
	getline(ss, element, ',');
	size_t lines_size = atoi(element.c_str());
	getline(ss, element, ',');
	size_t items_size = atoi(element.c_str());

	vector <vector<double>> array(lines_size, vector<double>(items_size));
	vector<double> target;

	while (getline(fp, line)){	

		string element;
		stringstream ss(line);
		int j = 0;

		array[idx].resize(items_size);
		while (getline(ss, element, ',')){

			array[idx][j] = atof(element.c_str());
			j++;
		}
		idx++;
	}
	fp.close();

	int how_many = array.size();
    //------------csv file data reading end-----------------//



    //--------------socket testing begin---------------------//
    if(sfd == -1){ 
        cout << "Error!" << endl;
        return -1;
    }

    if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1) {
    
        cout << "Server socket path too long: " << SV_SOCK_PATH << endl;
        return -1;
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {

       cout << "error with remove" << endl;
       return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {

       cout << "error with bind" << endl;
       return -1;
    }

    if (listen(sfd, BACKLOG) == -1) {

        cout << "error with listen" << endl;
        return -1;
    }
    //--------------socket testing end---------------------//

    for (;;) {         

    // Accept a connection. The connection is returned on a NEW
    // socket, 'cfd'; the listening socket ('sfd') remains open
    // and can be used to accept further connections. */
    printf("Waiting to accept a connection...\n");
    // NOTE: blocks until a connection request arrives.
    int cfd = accept(sfd, NULL, NULL);
    printf("Accepted socket fd = %d\n", cfd);

    //
    // Transfer data from connected socket to stdout until EOF */
    //

    // Read at most BUF_SIZE bytes from the socket into buf.
    while ((numRead = read(cfd, buf, BUFSIZ)) > 0) {
        // Then, write those bytes from buf into STDOUT.
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
        fatal("partial/failed write");
        }
    }

    

   



    return 0;
}