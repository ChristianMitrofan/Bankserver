This program simulates the functions of a bank.It keeps information about the name and
the amount of the accounts.Every account maintains the current amount and a list of the 
money(in total) that it has received from other accounts.The makefile creates two executables:

1)bankserver which can be executed with the command:
./bankserver -p < port > -s < thread pool size > -q < queue size > 		
e.g. ./bankserver -p 2222 -s 10 -q 10

The bankserver receives commands from bankclients via connections using sockets.
Every client connects once but can input multiple commands.In general the bankserver:
	*Creates the data structure (hash table) where all the information about the clients is saved
	*Receives commands from multiple bankclients
	*Executes these commands and returns the result (success,failure) to the corresponding client
	
Specifically, after creating the data structure, the bankserver waits for external 
connection on the port that is given as a command line argument.Every connection 
received will be put in an execution queue.The bankserver has a thread pool
of workers the size of which is also given as a command line argument.The workers 
undertake a connection from the queue each and executes the commands that it receives 
and sends appropriate responses.When a client finishes it's job the corresponding 
worker thread will become available to the server once again.At this point said 
thread will return to the thread pool to undertake a new connection from the
queue or wait until a new one appears.

2)bankclient which can be executed with the command:
./bankclient -h < server_host > -p < server_port > -i < command_file >	
e.g. ./bankclient -h localhost -p 2222 -i input1.txt

The bankclient connects to the bankserver using the IP address and port that are 
passed as command line arguments.It can either receive commands from a file or manually 
from stdin.The command that it can receive (with their arguments) can be seen below:

	1. add account <init amount> <name> [delay]
	Adds an account with an initial amount
	2. add transfer <amount> <src name> <dst name> [delay]
	Transfers the amount from src name to dst name
	3. add multi transfer <amount> <src name> <dst name1> <dst name2> ... [delay]
	Transfers the amount from src name to multiple dst names
	4. print balance <name>
	Prints the balance of the account
	5. print multi balance <name1> <name2> ...
	Prints the balances of multiple account
	6. sleep <time>
	Delays the next command
	7. exit
	Terminates the bankclient 

