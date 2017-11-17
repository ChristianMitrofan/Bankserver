#ifndef THREADOPS_H
#define THREADOPS_H

#include "Operations.h"
#include "ConQueue.h"
#include "MutexList.h"

#define HTABLESIZE 1000      	//Hash table size
#define MTABLESIZE 1000       	//Mutex table size
#define MAXCMDSIZE 10000      	//Maximum size of a command
#define MAXARGSIZE 1000       	//Maximum number of arguments of a command

extern ConQueue* cons;
extern AccountTable* bank;

int * Lock_Mutexes(string names);

void Unlock_Mutexes(int * buckets) ;

void * Receive_Connections(void* argp);

void * Receive_Commands(void* argp);

void* Execute_Commands(string cmd,int sock);

void perror_exit(const char *message);

#endif