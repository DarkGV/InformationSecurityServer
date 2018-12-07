CC=gcc
FLAGS=-c -Wall
RES = server.o CreateSocket.o send.o ReceiveString.o ReceiveInt.o SendInt.o HandleClient.o IPCheck.o Register.o RSA.o

all:out

RSA.o: Security/RSA.h Security/RSA.c
					$(CC) $(FLAGS) Security/RSA.c

Register.o: h_server.h Register.c
					$(CC) $(FLAGS) Register.c

HandleClient.o: h_server.h HandleClient.c
					$(CC) $(FLAGS) HandleClient.c

SendInt.o: h_server.h SendInt.c
					$(CC) $(FLAGS) SendInt.c

ReceiveInt.o: h_server.h ReceiveInt.c
					$(CC) $(FLAGS) ReceiveInt.c

CreateSocket.o: h_server.h CreateSocket.c
					$(CC) $(FLAGS) CreateSocket.c

ReceiveString.o: h_server.h ReceiveString.c
					$(CC) $(FLAGS) ReceiveString.c

send.o: h_server.h send.c
					$(CC) $(FLAGS) send.c

server.o: h_server.h server.c
					$(CC) $(FLAGS) server.c
IPCheck.o: h_server.h IPCheck.c
					$(CC) $(FLAGS) IPCheck.c

out: $(RES)
					$(CC) -o server $(RES) -lpthread -lsqlite3

clean:
	rm $(RES) server

debug: $(RES)
					$(CC) -g -o server $(RES)
	