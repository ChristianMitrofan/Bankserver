#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Account.h"

int Add_Account(int init_amount,string name,AccountTable* bank);

int Add_Transfer(int amount,string src_name,string dst_name,AccountTable* bank);

int Add_Multi_Transfer(int amount,string src_name,string* dst_names,int num,AccountTable* bank);

string Print_Balance(string name,AccountTable* bank);

string Print_Multi_Balance(string names,AccountTable* bank);

#endif