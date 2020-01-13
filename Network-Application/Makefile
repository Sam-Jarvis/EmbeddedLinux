CC = gcc
T1 = TCPEchoClient
T2 = TCPEchoServer-Thread

$(T2): $(T2).o Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o
	$(CC) -o $(T2) $(T2).o Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o -lpthread -lusb-1.0

$(T1): $(T1).o CreateTCPClientSocket.o Auxiliary.o
	$(CC) -o $(T1) $(T1).o CreateTCPClientSocket.o Auxiliary.o

$(T2).o: $(T2).c Auxiliary.h AcceptTCPConnection.h CreateTCPServerSocket.h HandleTCPClient.h

$(T1).o: $(T1).c Auxiliary.h CreateTCPClientSocket.h
	$(CC) -c $(T1).c

Auxiliary.o: Auxiliary.c Auxiliary.h
	$(CC) -c Auxiliary.c

CreateTCPClientSocket.o: CreateTCPClientSocket.c CreateTCPClientSocket.h
	$(CC) -c CreateTCPClientSocket.c

AcceptTCPConnection.o: AcceptTCPConnection.c AcceptTCPConnection.h
	$(CC) -c AcceptTCPConnection.c

CreateTCPServerSocket.o: CreateTCPServerSocket.c CreateTCPServerSocket.h
	$(CC) -c CreateTCPServerSocket.c CreateTCPServerSocket.h

HandleTCPClient.o: HandleTCPClient.c HandleTCPClient.h
	$(CC) -c HandleTCPClient.c HandleTCPClient.h
