#include "Transfer.h"
#include <cstdio>
using namespace std;

/////////////////////////////////////////////////////////////
/////////////////  Transfer functions ////////////////////
/////////////////////////////////////////////////////////////

Transfer::Transfer(int s,Transfer* n,string sr): sum(s), next(n), source(sr)
{
	sum = s;
	next = n;
	source = sr;
}

int Transfer::GetSum()
{
	return sum;
}

int Transfer::AddSum(int nsum)
{
	sum=sum+nsum;
}

Transfer* Transfer::GetNext()
{
	return next;
}

int Transfer::ChangeNext(Transfer* n)
{
	next=n;
}

string Transfer::GetSource()
{
	return source;
}

/////////////////////////////////////////////////////////////
////////////// Transfer list functions ///////////////////
/////////////////////////////////////////////////////////////

TransferList::TransferList()
{
	head = NULL;	
}

TransferList::~TransferList()
{
	Transfer * temp;
	while(head!=NULL)
	{
		temp = head;
		delete head;
		head = temp->GetNext();
	}	
}

Transfer* TransferList::GetStart()
{
	return head;
}

Transfer* TransferList::Find_Transfer(string n)	
{
	Transfer* temp = head;
	while(temp != NULL)
	{	
		if(temp->GetSource() == n)
		{
			return temp;
		}
		temp = temp->GetNext();
	}
	return NULL;
}

int TransferList::Insert_Transfer(string sr,int s)		//We have the starting node and we need a pointer to the receiving one
{   
	if(head==NULL)	//First time a Transfer is inserted in this list
	{
		fflush(stdout);
		fflush(stdout);
		head = new Transfer(s, head, sr);	
	}
	else			//All the new Transfers are inserted at the beginning of the list
	{
		Transfer* temp = head;
		while(temp!=NULL)
		{
			if(temp->GetSource() == sr)		//If there is already a Transfer between those nodes
			{
				temp->AddSum(s);
				return 0;
			}
			temp=temp->GetNext();
		}								
		head = new Transfer(s,head,sr);					//Else if there is no such Transfer,create a new one
	}
	return 0;
}

int TransferList::Delete_Transfer(string n)
{
	Transfer* temp = NULL;
	Transfer* del =NULL;
	if(head->GetSource()==n)						//If its the first node of the list
	{
		del=head;
		head=del->GetNext();
		delete del;
		return 1;
	}
	temp = head;
	del =temp->GetNext();
	while(del != NULL)
	{	
		if( del->GetSource() == n)				//If its the one we want to delete then
		{
			temp->ChangeNext(del->GetNext());	//the next Transfer of the previous Transfer that will be deleted is changed 
			delete del;							//to the next Transfer that will be deleted
			return 0;
		}
		temp = temp->GetNext();
		del = del->GetNext();
	}
	return 1;
}

void TransferList::Print_List()
{
	int c=0;
	Transfer* temp = head;
	cout << "Printing list of Transfers : " << endl;
	while(temp!=NULL)
	{
		cout << "Sum : " << temp->GetSum() << endl;
		temp = temp->GetNext();
		c++;
	}
	cout << "There are " << c << " Transfers in this list!" << endl;
}