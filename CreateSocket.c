#include "h_server.h"

SOCKET CreateSocket(){
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, 0);
	socketAddress saServer;
	
	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(1234);
	saServer.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sServer, (struct sockaddr*)&saServer, sizeof(saServer)) < 0){
		printf("Oops, looks like I can't bind socket. Please contact server administrator");
		return -1;
	}
	
	if(listen(sServer, MAXLEN) < 0){
		printf("Oops, looks like I can't listen on this socket. Please contact server administrator");
		return -1;
	}
	
	return sServer;
}

string MakeString ( int size ) {
	return (string) malloc(size* sizeof(char));
}