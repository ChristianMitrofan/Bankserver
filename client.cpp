#include <iostream>
#include <cstdio>
#include <fstream>
#include <pthread.h>         /* threads */
#include <cstdlib>           /* exit */
#include <cstring>           /* strlen */
#include <string>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */

#define MAXCMDSIZE 10000

using namespace std;

void perror_exit(const char* message);

int main(int argc, char *argv[]) 
{
    int server_port, sock;
    string server_host,command_file;
    string command;     //The command that is given from the user
    string check;       //Check for sleep or exit
    char response[MAXCMDSIZE];
    if(argc != 7)
    {
        cout << "Please give the required arguments!" << endl;
        return 1;
    }
    for(int c=1; c<argc; c=c+2)         //Checking the odd arguments
    {
        if(!strcmp(argv[c],"-h"))       //c+1 is port number
        {
            server_host=argv[c+1];
        }
        else if(!strcmp(argv[c],"-p"))  
        {
            server_port=atoi(argv[c+1]);
        }
        else if(!strcmp(argv[c],"-i"))
        {
            command_file=argv[c+1];
        }
        else
        {
            cout << "Something is wrong with the arguments .." << endl;
        }
    }
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;
	/* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	perror_exit("socket");
	/* Find server address */
    if ((rem = gethostbyname(server_host.c_str())) == NULL) {	
	   herror("gethostbyname"); exit(1);
    }
    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(server_port);         /* Server port */
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
	   perror_exit("connect");
    printf("Connecting to %s port %d\n", server_host.c_str(), server_port);
    ifstream operationfile(command_file.c_str());
    cout << "Reading from file" << endl;
    if (operationfile.is_open())
    {
        while (getline(operationfile,command))
        {
            check=command.substr(0,command.find_first_of(" \t",0));                         //First word of the command
            if(!check.compare("sleep"))                                         
            {
                check=command.substr(command.find_first_of(" \t",0)+1,command.length());    //Second word of the command
                usleep(atoi(check.c_str()));
            }
            else if(!check.compare("exit"))
            {
                return 0;   /* Finish on "end" */
            }
            else    //For the rest of the commands send the command to the server for execution
            {
                if (write(sock, command.c_str(), MAXCMDSIZE)<0)
                    perror_exit("write");
                if (read(sock, response, MAXCMDSIZE) < 0)
                    perror_exit("read");
                cout << response << endl;
            }
        }
        operationfile.close();
    }
    else
    {
        cout << "Could not open file!" << endl;
    }
    do 
    {
    	cout << "Please input a command:" << endl;
    	getline(cin, command);
        check=command.substr(0,command.find_first_of(" \t",0));                         //First word of the command
        if(!check.compare("sleep"))                                         
        {
            check=command.substr(command.find_first_of(" \t",0)+1,command.length());    //Second word of the command
            usleep(atoi(check.c_str()));
        }
        else if(!check.compare("exit"))
        {
            return 0;   /* Finish on "end" */
        }
        else    //For the rest of the commands send the command to the server for execution
        {
            if (write(sock, command.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
            if (read(sock, response, MAXCMDSIZE) < 0)
                perror_exit("read");
            cout << response << endl;
        }
    } while (1); 
    close(sock);        /* Close socket and exit */
    return 0;
}			     

void perror_exit(const char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
