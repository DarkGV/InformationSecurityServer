#include "h_server.h"

/*
*	This function will receive for the string containing the message
*/

string ReceiveStringFrom(SOCKET from){
	// Variables:
	int iMessageSize = 0;
	int iCount, iOffset;
	string strMessage;

	if((iMessageSize = receiveIntFrom(from)) < 0) return NULL;	//	First we need to get the message size

	strMessage = MakeString(iMessageSize+1);	//	And set a space for it.

	iOffset = iCount = 0;

	while(iOffset < iMessageSize){
		iCount = recv(from, &strMessage[iOffset], iMessageSize - iOffset, 0);	//	Read the message from the user socket.
		if(iCount == -1) return NULL;
		if(iCount == 0) return NULL;
		iOffset += iCount;
	}
	strMessage[iMessageSize] = '\0';
	return strMessage;
}