#include "h_server.h"

// +-----------------------------------------+
// |		AN INTEGER IN JAVA	             |
// |	4 bytes -> WATING 4 bytes	         |
// +-----------------------------------------+

int receiveIntFrom(SOCKET sClient){
    int iOffset, iCount, iFlag;
    char cByte = 0;

    iFlag ^= iFlag;

    iOffset = 0;
    while(iOffset < 4){
        iCount = recv(sClient, &cByte, 1, 0);   //  We receive byte per byte the integer.
        if(iCount <= 0) return -1;
        iFlag = (int)cByte | (iFlag << 8);  //  And OR it to the ourput variable. (byte per byte)
        iOffset += iCount;
    }

    return iFlag;
}