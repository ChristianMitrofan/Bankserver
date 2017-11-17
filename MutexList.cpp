#include "MutexList.h"

using namespace std;

Mutex::Mutex(int id,Mutex* nxt):mtxid(id) ,next(nxt) {}

int Mutex::GetId()
{
	return mtxid;
}

Mutex* Mutex::GetNext()
{
	return next;
}

void Mutex::ChangeNext(Mutex* nxt)
{
	next=nxt;
}

MutexList::MutexList():head(NULL), size(0){};

MutexList::~MutexList()
{
	Mutex* temp=head;
	Mutex* todel=temp;
	while(temp->GetNext()!=NULL)
	{
		temp=temp->GetNext();
		delete todel;
		todel=temp;
	}
}

Mutex* MutexList::GetStart()
{
	return head;
}

Mutex* MutexList::Find_Mutex(int id)
{
	Mutex* temp=head;
	while(temp!=NULL)
	{
		if(temp->GetId()==id)
			return temp;
		temp=temp->GetNext();
	}
	return NULL;
}

int MutexList::GetSize()
{
	return size;
}

int MutexList::Insert_Mutex(int id)
{
	Mutex* mtx;
	Mutex* temp;
	if(head==NULL )				//If the list is empty o
	{
		mtx=new Mutex(id,NULL);
		head=mtx;
		size++;
	}
	else if(id<head->GetId()) //The new id is the smallest so insert at the beginning
	{
		mtx=new Mutex(id,head);
		head=mtx;
		size++;
	}
	else
	{
		temp=head;
		while(temp->GetNext()!=NULL)
		{
			if(temp->GetNext()->GetId()>id)			//If the next id is bigger than the new id 
			{
				mtx=new Mutex(id,temp->GetNext());	//Then insert before it
				temp->ChangeNext(mtx);				//and after temp
				size++;
				return 0;
			}
			temp=temp->GetNext();
		}
		mtx=new Mutex(id,NULL);
		temp->ChangeNext(mtx);
		size++;
	}
	return 0;
}

int* MutexList::Get_Mutexes()
{
	int* ids=new int[size];
	Mutex* temp=head;
	for(int c=0; c<size; c++)
	{
		ids[c]=temp->GetId();
		temp=temp->GetNext();
	}
	return ids;
}

void MutexList::Print_List()
{
	Mutex* temp=head;
	while(temp->GetNext()!=NULL)
	{
		cout << "Id :" << temp->GetId() << endl;
		temp=temp->GetNext();
	}
	cout << "Id :" << temp->GetId() << endl;
}