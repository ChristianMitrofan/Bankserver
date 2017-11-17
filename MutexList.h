#ifndef MUTEXLIST_H
#define MUTEXLIST_H

#include <cstdio>
#include <string>
#include <iostream>
#include "MutexList.h"

class Mutex
{
	private:
		int mtxid;
		Mutex* next;

	public:
		Mutex(int id,Mutex* nxt);
		int GetId();
		Mutex* GetNext();
		void ChangeNext(Mutex* nxt);
};

class MutexList 
{
	private:
		Mutex * head;
		int size;

	public:
		MutexList();
		~MutexList();
		Mutex* GetStart();
		Mutex* Find_Mutex(int id);
		int GetSize();
		int Insert_Mutex(int id);
		int* Get_Mutexes();
		void Print_List();
};

#endif

