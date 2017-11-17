#include <iostream>
#include "ThreadOps.h"

ConQueue* cons;
AccountTable* bank;

int * Lock_Mutexes(string names)               //Receives all the names that relate to the action
{
    istringstream iss(names);
    MutexList* mlist=new MutexList();
    string word;
    int pos,sum;
    char hashing[MAXNAMESIZE];
    while(iss>>word)
    {
        sum=0;                                  //We check which buckets must be locked
        strcpy(hashing,word.c_str());           //by hashing the names that are used in
        for (int i=0; i < word.length(); i++)   //the action.If two or more names are 
            sum += hashing[i];                  //hashed in the same bucket by default
        pos = sum % bank->GetSize();            //of the Insert_Mutex only one is inserted
        if(mlist->Find_Mutex(pos)==NULL)        //So at the end of the while in mlist we
            mlist->Insert_Mutex(pos);           //have all the unique buckets that have to be locked in an ordered list
    }                                           
    int* mids=mlist->Get_Mutexes();
    //mlist->Print_List();
    mids[mlist->GetSize()]=-1;
    for(int c=0; c<mlist->GetSize(); c++)
    {
        cout << "Locking Mutex: " << mids[c] <<endl;
        bank->Lock_Mutex(mids[c]);
    }
    delete mlist;
    return mids;
}

void Unlock_Mutexes(int * buckets)               
{
    int c=0;
    int bucksize=sizeof(buckets)/sizeof(int);
    cout << bucksize <<endl;
    while(buckets[c]!=-1)
    {
        cout << "Unlocking Mutex: " << buckets[c] <<endl;
        bank->Unlock_Mutex(buckets[c]);
        c++;
    }
}

void * Receive_Connections(void* argp)
{
    int newsock=*(int*)argp;
    cons->Push(newsock);
    cons->SignalEmpty(); 
    pthread_exit(0);
} 

void * Receive_Commands(void* argp) 
{
    int newsock;
    char cmd[MAXCMDSIZE+1];
    while(1)
    {
        newsock=cons->Pop();
        cons->SignalFull();
        while(read(newsock, cmd, MAXCMDSIZE+1) > 0)
        {
            string command(cmd);
            Execute_Commands(command,newsock);
        }
        close(newsock); 
    }      
}

void* Execute_Commands(string cmd,int sock)
{
    istringstream iss(cmd);
    int* mutexids;
    int flag=0; //0 if there is no delay
    int args=0;
    string word;
    string delay;
    iss >> word;
    string names[MAXARGSIZE];
    if(!word.compare("add_account"))
    {
        iss >> word;
        string amount=word;
        iss >> word;
        string name=word;
        mutexids=Lock_Mutexes(name);
        if(!iss.eof())      //Delay
        {
            iss >> word;
            delay=word;
            usleep(atoi(word.c_str()));
            flag=1;
        }
        if(!Add_Account(atoi(amount.c_str()),name,bank))      //Success
        {
            string response="Success. Account creation (" + name + ":" + amount ;
            if(flag==1)         //Also print the delay
                response=response + "[" + delay + "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        }
        else                                                //Failure
        {
            string response="Error. Account creation failed (" + name + ":" + amount;
             if(flag)         //Also print the delay
                response=response + "[" +delay+ "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        }
        Unlock_Mutexes(mutexids);
    }
    else if(!word.compare("add_transfer"))
    {
        string lnames;          //Names that are used in the action
        iss >> word;
        string amount=word;
        iss >> word;
        string src=word;
        iss >> word;
        string dst=word;
        lnames=src+" "+dst;
        mutexids=Lock_Mutexes(lnames);
        if(!iss.eof())          //Delay
        {
            iss >> word;
            int delay=atoi(word.c_str());
            usleep(delay);
        }
        if(!Add_Transfer(atoi(amount.c_str()),src,dst,bank))
        {
            string response="Success. Transfer addition (" + src + ":" +  dst + ":" + amount ;
            if(flag==1)         //Also print the delay
                response=response + "[" + delay + "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        }
        else
        {
            string response="Error. Transfer addition failed (" + src + ":" +  dst + ":" + amount ;
            if(flag==1)         //Also print the delay
                response=response + "[" + delay + "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        }
        Unlock_Mutexes(mutexids);
    }
    else if(!word.compare("add_multi_transfer"))
    {
        string lnames;
        iss >> word;
        string amount=word;
        iss >> word;
        string src=word;
        lnames=src;
        while(iss >> word)                      //Get all the dst_names in an array(Maybe delay too)
        {
            lnames=lnames+" "+word;
            names[args]=word;
            args++;
        }
        if(args<2)
        {
            string response = "Not enough arguments !";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        }
        char * p ;
        strtol(word.c_str(), &p, 10) ;   //Parse the last argument with strtol until it reaches a non-integer
        if(*p == 0)                             //If it reaches the end of the string then it means that it is an integer        
        {                                       //and probably the delay
            //cout << "There is a delay" << endl;
            lnames.clear();
            lnames=src;
            for(int c=0; c<args-1; c++)
            {
                lnames=lnames+" "+names[c];
            }
            mutexids=Lock_Mutexes(lnames);           //If there is a delay lock before the delay and ignore the last argument
            delay=word;
            usleep(atoi(delay.c_str()));
            flag=1;
        }
        else{mutexids=Lock_Mutexes(lnames);}         //Else do as usually       
        if(!Add_Multi_Transfer(atoi(amount.c_str()),src,names,args-flag,bank))  //If there is a delay then we have one less argument
        {
            string response="Success. Multi-Transfer addition (" + src + ":" + amount ;
            if(flag==1)         //Also print the delay
                response=response + "[" + delay + "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        } 
        else
        {
            string response="Error. Multi-Transfer addition failed (" + src + ":" + amount ;
            if(flag==1)         //Also print the delay
                response=response + "[" + delay + "]";
            response=response + ")";
            if (write(sock, response.c_str(), MAXCMDSIZE)<0)
                perror_exit("write");
        } 
        Unlock_Mutexes(mutexids);   
    }
    else if(!word.compare("print_balance"))
    {
        iss >> word;
        string name = word;
        mutexids=Lock_Mutexes(name);
        string response =Print_Balance(name,bank);
        Unlock_Mutexes(mutexids);
        if (write(sock, response.c_str(), MAXCMDSIZE)<0)
            perror_exit("write");
    }
    else if(!word.compare("print_multi_balance"))
    {
        string balances;
        while(iss >> word)                     
        {
            balances=balances + " " + word ;
        }
        mutexids=Lock_Mutexes(balances);
        string response =Print_Multi_Balance(balances,bank);
        Unlock_Mutexes(mutexids);
        if (write(sock, response.c_str(), MAXCMDSIZE)<0)
            perror_exit("write");

    }
    else 
    {
        string response="Error. Unknown command !";
        if (write(sock, response.c_str(), MAXCMDSIZE)<0)
            perror_exit("write");
    }
}

void perror_exit(const char *message) 
{
    perror(message);
    exit(EXIT_FAILURE);
}
