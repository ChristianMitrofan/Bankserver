#ifndef CONQUEUE_H
#define CONQUEUE_H

#include <iostream>
#include <cstdlib>
#include <string>

#define CQUEUESIZE 10

using namespace std;

class ConQueue 
{
    private:
        int* connections;          
        int start;
        int end;
        int count;
        int size;
        pthread_mutex_t mtx;
        pthread_cond_t cond_nonempty;
        pthread_cond_t cond_nonfull;
    public:
        ConQueue(int s);
        ~ConQueue();
        void Push(int con);
        int Pop();
        int GetSize();
        void SignalEmpty();
        void SignalFull();
};

#endif