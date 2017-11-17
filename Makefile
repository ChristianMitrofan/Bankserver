OBJSS 	= server.o Operations.o Account.o Transfer.o ConQueue.o ThreadOps.o MutexList.o
OBJSC   = client.o
SOURCES	= server.cpp client.cpp Operations.cpp Account.cpp Transfer.cpp ConQueue.cpp ThreadOps.cpp MutexList.cpp
HEADERS = Operations.h Account.h Transfer.h ConQueue.h ThreadOps.h MutexList.h
CC		= g++
FLAGS   = -g -c 

all: bankserver bankclient

server.o: server.cpp
	$(CC) $(FLAGS) server.cpp

client.o: client.cpp
	$(CC) $(FLAGS) client.cpp

Transfer.o: Transfer.cpp
	$(CC) $(FLAGS) Transfer.cpp
	
Account.o: Account.cpp
	$(CC) $(FLAGS) Account.cpp
	
Operations.o: Operations.cpp
	$(CC) $(FLAGS) Operations.cpp
	
ConQueue.o: ConQueue.cpp
	$(CC) $(FLAGS) ConQueue.cpp
	
MutexList.o: MutexList.cpp
	$(CC) $(FLAGS) MutexList.cpp
	
ThreadOps.o: ThreadOps.cpp
	$(CC) $(FLAGS) ThreadOps.cpp
	
bankserver: $(OBJSS)
	$(CC) -g $(OBJSS) -o bankserver -lpthread
	rm $(OBJSS)
	
bankclient: client.o
	$(CC) -g client.o -o bankclient
	rm client.o

clean:
	rm -f $(OBJSS) $(OBJSCS) 