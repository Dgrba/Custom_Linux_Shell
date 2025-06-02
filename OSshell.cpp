#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100

using namespace std;

void StrTok(char *input, char **argv)
{
	char *token;
	token = strtok(input, " ");

	while(token != NULL)
	{
		*argv++ = token;
		token = strtok(NULL, " ");
	}

	*argv = NULL;
}

void execute(char **argv)
{
	int status;
	int childStatus;
	pid_t pid = fork();

	if(pid == 0)
	{
		childStatus = execvp(*argv, argv);
		if (childStatus < 0){
			cout<<"ERROR:Command line input issue";
		}
		exit(0);

	}
	else if(pid < 0){
		cout << "fork() failed" << endl;
	}
	else {   
        int status;
		wait(&pid);
	}
}

int main()
{
    char input[250];
	char *argv[SIZE];
    string commands[7] = {"clear", "pwd", "time", "date", "ls", "which", "\0"};

	while(true)
	{
		cout<< "OSshell$ ";
		cin.getline(input,250);
		StrTok(input, argv);  
		if (strcmp(input, "exit!") == 0)
			break;
		else if (input == NULL){
            cout<< endl << "OSshell$ ";
			continue;
		}
        else {
            bool match = false;
            for(int i = 0; i < 7; i++){
                if(strcmp(input, commands[i].c_str()) == 0)
                    match = true;
            }
            if(!match){
                cout << endl <<"ERROR:Unknown Command. The comands supported are: ";
                cout << endl << "clear" << endl << "pwd"<< endl << "time"
                << endl << "date" << endl << "ls" << endl << "which" << endl << endl;
                continue;
            }
            execute(argv);
        }
    }
		
	return 0;
}
