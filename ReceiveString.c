#include "h_server.h"

string ReceiveStringFrom(SOCKET from){
	// Variables:
	int iMessageSize = 0;
	int iCount, iOffset;
	string strMessage;

	if((iMessageSize = receiveIntFrom(from)) < 0) return NULL;

	strMessage = MakeString(iMessageSize+1);

	iOffset = iCount = 0;

	while(iOffset < iMessageSize){
		iCount = recv(from, &strMessage[iOffset], iMessageSize - iOffset, 0);
		if(iCount == -1) return NULL;
		if(iCount == 0) return NULL;
		iOffset += iCount;
	}
	strMessage[iMessageSize] = '\0';
	return strMessage;
}

string itoa(int number){
    char* cInteger = (char*)malloc(4*sizeof(char));
    int i;
    for(i = 3; i > -1; i--){
        cInteger[i] = number & 0xFF;
        number >>= 8;
    }

    return cInteger;
}