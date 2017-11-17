#include "Operations.h"

using namespace std;

int Add_Account(int init_amount,string name,AccountTable* bank)
{
	if(bank->Find_HAccount(name)!=NULL)
	{
		cout << "Account already exists!" << endl;
		return 1;
	}
	else
		return bank->Insert_HAccount(init_amount,name);
}

int Add_Transfer(int amount,string src_name,string dst_name,AccountTable* bank)		//To delay den 3erw an 8a to valw mesa sth synarthsh
{
	Account* src;
	Account* dst;
	if((src=bank->Find_HAccount(src_name))==NULL || (dst=bank->Find_HAccount(dst_name))==NULL)
	{
		cout << "One or more accounts of the transfer does not exist !" << endl;
		return 1;
	}
	if(src->GetAmount()<amount)
	{
		cout << "Not enough money in the source account for the transfer !" << endl;
		return 1;
	}
	src->ChangeAmount(src->GetAmount()-amount);
	dst->ChangeAmount(dst->GetAmount()+amount);
	return dst->GetTransfers()->Insert_Transfer(src_name,amount);
}

int Add_Multi_Transfer(int amount,string src_name,string* dst_names,int num,AccountTable* bank)
{
	Account* src;
	Account* dst[num];
	if((src=bank->Find_HAccount(src_name))==NULL)
	{
		cout << "Source of the transfer does not exist" << endl;
		return 1;
	}
	for(int c=0; c<num; c++)
	{
		if((dst[c]=bank->Find_HAccount(dst_names[c]))==NULL)
		{
			cout << "One or more accounts of the transfer does not exist !" << endl;
			return 1;
		}		
	}
	if(src->GetAmount()<num*amount)
	{
		cout << "Not enough money in the source account for the transfer !" << endl;
		return 1;
	}
	src->ChangeAmount(src->GetAmount()-num*amount);
	for(int c=0; c<num; c++)
	{
		dst[c]->ChangeAmount(dst[c]->GetAmount()+amount);
		dst[c]->GetTransfers()->Insert_Transfer(src_name,amount);
	}
	return 0;
}

string Print_Balance(string name,AccountTable* bank)
{
	Account* acc;
	string balance;
	ostringstream convert;   // stream used for the conversion
	if((acc=bank->Find_HAccount(name))==NULL)
	{
		cout << "The account does not exist" << endl;
		return "Error. Balance (" + name + ")";
	}
	convert << acc->GetAmount(); 
	balance=name + ":" + convert.str();
	return "Success. Balance (" + balance + ")";
}


string Print_Multi_Balance(string names,AccountTable* bank)
{
	Account* acc;
	string word;
	ostringstream convert;
	istringstream iss(names);
	cout << "Mphka Multi_balance" << endl;
	iss >> word;
	if((acc=bank->Find_HAccount(word))==NULL)
	{
		cout << "One or more accounts of the transfer does not exist !" << endl;
		return names;
	}
	convert << acc->GetAmount();
	string balance=word + "/" + convert.str();
	convert.str("");
	convert.clear();
	while(!iss.eof())
	{
		iss >> word;
		if((acc=bank->Find_HAccount(word))==NULL)
		{
			cout << "One or more accounts of the transfer does not exist !" << endl;
			return "Error. Multi-balance (" + names + ")";
		}
		convert << acc->GetAmount();
		balance= balance + ":" + word + "/" + convert.str();
		convert.str("");
		convert.clear();
	}
	return "Success. Multi-balance (" + balance + ")";
}