#ifndef TRANSFER_H
#define TRANSFER_H

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

/*		Transfer is the class that implements the edges of the graph :		/
/		sum -> is the value of every node 									/
/		next -> is a pointer to the next Transfer   						/
/		Source -> is the pointer to the receiving node of the Transfer   */

class Transfer
{
	private:
		int sum;
		Transfer* next;
		string source;

	public:
		Transfer(int s,Transfer* n,string sr);
		//~Transfer();
		int AddSum(int nsum);
		int GetSum();
		Transfer* GetNext();
		int ChangeNext(Transfer* n);
		string GetSource();
};

/*	List with all the Transfers that start from a node 	/
/	head -> start of the list                              */

class TransferList
{
	private:
		Transfer* head;
		
	public:
		TransferList();
		~TransferList();
		Transfer* GetStart();
		int Insert_Transfer(string sr,int s);		// sr is the name of the source	
		Transfer* Find_Transfer(string n);			// n(ame) is used as identification
		int Delete_Transfer(string n);
		void Print_List();
};

#endif