#include <iostream>
#include "ThreadOps.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    int port, sock, newsock, thread_pool_size, queue_size;
    if(argc != 7)
    {
        cout << "Please give the required arguments!" << endl;
        return 1;
    }
    for(int c=1; c<argc; c=c+2)         //Checking the odd arguments
    {
        if(!strcmp(argv[c],"-p"))       //c+1 is port number
        {
            port=atoi(argv[c+1]);
        }
        else if(!strcmp(argv[c],"-s"))  
        {
            thread_pool_size=atoi(argv[c+1]);
        }
        else if(!strcmp(argv[c],"-q"))
        {
            queue_size=atoi(argv[c+1]);
        }
        else
        {
            cout << "Something is wrong with the arguments .." << endl;
        }
    }
    bank=new AccountTable(HTABLESIZE,MTABLESIZE);
    cons=new ConQueue(queue_size);
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(client);
    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;
    pthread_t  *mthread;
    pthread_t  *threads;
    if ((mthread = (pthread_t*)malloc(sizeof(pthread_t))) == NULL) 
    {
        perror("malloc"); 
        exit(1); 
    }
    if ((threads = (pthread_t*)malloc(thread_pool_size * sizeof(pthread_t))) == NULL) 
    {
        perror("malloc"); 
        exit(1); 
    }
    //Every worker thread gets a file descriptor from the queue and receives commands from the socket
    for (int i=0 ; i<thread_pool_size ; i++) 
    {
        if (pthread_create(threads+i, 0, Receive_Commands, 0)) 
        {
            cout << "Problem in thread creation " <<endl;
            exit(1); 
        } 
    }
    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");
    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind");
    /* Listen for connections */
    if (listen(sock, 5) < 0) 
        perror_exit("listen");
    printf("Listening for connections to port %d\n", port);
    while (1) 
    {
        /* accept connection */
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0) 
            perror_exit("accept");
        printf("Accepted connection\n");
        //Master Thread accepts connection and stores the file descriptor of the socket in a queue
        if (pthread_create(mthread, 0, Receive_Connections, (void*)&newsock)) 
        {
            cout << "Problem in thread creation" <<endl;
            exit(1); 
        }
    }
    delete bank;
    delete cons;
    return 0;
}