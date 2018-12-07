#include "h_server.h"

int sendTo(SOCKET to, string strMessage){
	int iOffset, iCount;

	if(sendInt(strlen(strMessage), to) < 0) return -1;

	iOffset = iCount = 0;

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