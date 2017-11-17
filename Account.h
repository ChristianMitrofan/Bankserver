#ifndef Account_H
#define Account_H

#include <cstdio>
#include <cstdlib>           /* exit */
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */	
#include "Transfer.h"

#define MAXNAMESIZE 1000

using namespace std;

/*		Account is the class that implements the nodes in the Account list :			/
/		name -> is the identification of every node 								/
/		next -> is the next Account node 											/
/		node -> is the unique graph node that the Account node points to			   */

class Account
{
	private:
		int amount;
		string name;
		Account* next;
		TransferList* start;

		
	public:
		Account(int am,string n,Account* nxt);
		~Account();
		string GetName();
		int GetAmount();
		int ChangeAmount(int am);
		Account* GetNext();
		int ChangeNext(Account* nxt);
		TransferList* GetTransfers();
};

/*	List with all the Accounts that start from an element of the AccountTable	/
/	head -> start of the list                              					   */

class AccountList
{
	private:
		Account* head;
		
	public:
		AccountList();
		~AccountList();
		Account* GetStart();
		int Insert_LAccount(int am,string n);			// name is the identification of the Account
		Account* Find_LAccount(string n);			
		//int Delete_LAccount(int i);
		void Print_List();
};

/*	Account table that has pointers to Account Lists as elements 	/
/	size -> size of the Account table 							/
/	table -> the table itself							   */

class AccountTable
{
	private:
		int size;
		AccountList** table;
		pthread_mutex_t* mutexes;

	public:
		AccountTable(int s,int ms);
		~AccountTable();
		int GetSize();
		AccountList* GetAccountList(int pos);
		int Insert_HAccount(int am,string n);
		Account* Find_HAccount(string n);
		void Lock_Mutex(int pos);
		void Unlock_Mutex(int pos);
		void Print_Table();
};

#endif