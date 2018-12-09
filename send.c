#include "h_server.h"


/*
*	SendTo function will just send a message to a user.
*/

int sendTo(SOCKET to, string strMessage){
	int iOffset, iCount;

	if(sendInt(strlen(strMessage), to) < 0) return -1;	//	First we need to send the length of the message to the user.

	iOffset = iCount = 0;

	//	After the user gets the size, just write the message to the socket.
	while(iOffset < strlen(strMessage)){
		iCount = send(to, &strMessage[iOffset], strlen(strMessage) - iOffset, 0);
		if(iCount <= 0){
			printf("Oops, looks like I couldn't send the message. Please contact server administrator.");
			return -1;
		}
		iOffset += iCount;
	}
	
    return 0;
}