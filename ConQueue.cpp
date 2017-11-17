#include "ConQueue.h"
#include <cstdio>

/////////////////////////////////////////////////////////////
/////////////////  Connection Queue functions //////////////////
/////////////////////////////////////////////////////////////

ConQueue::ConQueue(int s): size(s)
{
	connections=new int[CQUEUESIZE];
    start = 0;
    end = -1;
    count = 0;
    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);
}

ConQueue::~ConQueue()
{
    delete connections;
}

void ConQueue::Push(int con)
{
	pthread_mutex_lock(&mtx);
    while (count >= CQUEUESIZE) 
    {
        pthread_cond_wait(&cond_nonfull, &mtx);
    }
    end = (end + 1) % CQUEUESIZE;
    connections[end] = con;
    count++;
    pthread_mutex_unlock(&mtx);
}

int ConQueue::Pop() 
{
    int con = 0;
    pthread_mutex_lock(&mtx);
    while (count <= 0) 
    {
        pthread_cond_wait(&cond_nonempty, &mtx);
    }
    con = connections[start];
    start = (start + 1) % CQUEUESIZE;
    count--;
    pthread_mutex_unlock(&mtx);
    return con;
}

int ConQueue::GetSize()
{
	return size;
}

void ConQueue::SignalEmpty()
{
	pthread_cond_signal(&cond_nonempty);
}

void ConQueue::SignalFull()
{
	pthread_cond_signal(&cond_nonfull);
}