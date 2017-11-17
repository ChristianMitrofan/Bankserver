#include "Account.h"

using namespace std;

/////////////////////////////////////////////////////////////
//////////////////	Hash node functions	/////////////////////
/////////////////////////////////////////////////////////////

Account::Account(int am,string n,Account* nxt): amount(am), name(n), next(nxt)
{
	start=new TransferList();
}

Account::~Account()
{
	delete start;
}

string Account::GetName()
{
	return name;
}

int Account::GetAmount()
{
	return amount;
}

int Account::ChangeAmount(int am)
{
	amount=am;
}

Account* Account::GetNext()
{
	return next;
}

int Account::ChangeNext(Account* nxt)
{
	next=nxt;
}

TransferList* Account::GetTransfers()
{
	return start;
}

/////////////////////////////////////////////////////////////
//////////////////	Hash list functions	/////////////////////
/////////////////////////////////////////////////////////////

AccountList::AccountList(): head(NULL)
{
}

AccountList::~AccountList()
{
	Account * temp;
	while(head!=NULL)
	{
		temp = head;
		delete head;
		head = temp->GetNext();
	}
}

Account* AccountList::GetStart()
{
	return head;
}

int AccountList::Insert_LAccount(int am, string n)
{
	head = new Account(am,n,head);		//Now we can create the hash node 
	return 0; 
}

Account* AccountList::Find_LAccount(string n)
{
	Account* temp = head;
	while(temp != NULL)
	{	
		if(temp->GetName() == n)
		{
			return temp;
		}
		temp = temp->GetNext();
	}
	//cout << "The Account with name = " << n << " was not found!" << endl ;
	return NULL;
}

void AccountList::Print_List()
{
	int c=0;
	Account* temp = head;
	cout << "Printing list of nodes : " << endl;
	while(temp!=NULL)
	{
		cout << "Name : " << temp->GetName() << endl;
		temp = temp->GetNext();
		c++;
	}
	cout << "There are " << c << " Accounts in this list!" << endl;
}

/////////////////////////////////////////////////////////////
/////////////////  Hash table functions /////////////////////
/////////////////////////////////////////////////////////////

AccountTable::AccountTable(int s,int ms)	//Size of the hash table and size of mutex table
{
	size=s;
	table=new AccountList*[s];
	mutexes=new pthread_mutex_t[ms];
	for (int i=0; i<ms; i++)
	{
		pthread_mutex_init(&mutexes[i], 0);
	}
	for(int c=0; c<size; c++)
	{
		table[c]=NULL;
	}

}

AccountTable::~AccountTable()
{
	for(int c=0; c<size; c++)
	{
		delete table[c];
	}
	delete table;
}

int AccountTable::GetSize()
{
	return size;
}

AccountList* AccountTable::GetAccountList(int pos)
{
	return table[pos];
}

int AccountTable::Insert_HAccount(int am,string n)
{
	int sum = 0;
	char hashing[MAXNAMESIZE];
   	strcpy(hashing,n.c_str());
   	for (int i=0; i < n.length(); i++)
     	sum += hashing[i];
    int pos = sum % size ;
	if(table[pos] == NULL)	//First time inserting in that position of the hash table
	{
		table[pos] =new AccountList();				//Creating the list 
		return table[pos]->Insert_LAccount(am,n);	//So by inserting a graph in an empty(NULL) position of the table,we create a list,
													//insert its first hash node after we already have created the graph node that it points to
	}
	else
	{
		return table[pos]->Insert_LAccount(am,n);
	}
}

Account* AccountTable::Find_HAccount(string n)
{
	int sum = 0;
	char hashing[MAXNAMESIZE];
   	strcpy(hashing,n.c_str());
   	for (int i=0; i < n.length(); i++)
     	sum += hashing[i];
    int pos = sum % size ;
    if(table[pos]==NULL)
    	return NULL;
	Account* temp=table[pos]->Find_LAccount(n);	
	if(temp == NULL)
	{
		cout << "The Account with name = " << n << " was not found!" << endl ;
		return NULL;
	}
	return temp;
}

void AccountTable::Lock_Mutex(int pos)
{
	pthread_mutex_lock(&mutexes[pos]);
}

void AccountTable::Unlock_Mutex(int pos)
{
	pthread_mutex_unlock(&mutexes[pos]);
}

void AccountTable::Print_Table()
{
	Account* tempc;
	Transfer* tempt;
	if(table==NULL)
		cout << "The table is empty!" << endl;
	for(int c=0; c<size; c++)
	{
		if(table[c]!=NULL)
		{
			tempc=table[c]->GetStart();
			while(tempc!=NULL)							//For all vertices
			{
				cout << "Account(" << tempc->GetName() << ":" << tempc->GetAmount() << ") : " ;
				if(tempc->GetTransfers()!=NULL)
				{
					tempt=tempc->GetTransfers()->GetStart();
					while(tempt!=NULL)					//For every transaction(out) of each vertice
					{
						cout << "(" << tempt->GetSource() << "," << tempt->GetSum() << ") ";
						tempt=tempt->GetNext();
					}
					cout << endl;
				}
				tempc=tempc->GetNext();
			}
		}
	}
}
