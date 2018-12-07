# include "h_server.h"

int sendInt(int iSend, SOCKET sClient){
    string strMessage = itoa(iSend);
    int iOffset, iCount;
    iOffset = 0;
    while(iOffset < 4){
        iCount = send(sClient, &strMessage[iOffset], 4-iOffset, 0);
        if(iCount <= 0) return -1;
        iOffset += iCount;
    }

    return 0;
}

