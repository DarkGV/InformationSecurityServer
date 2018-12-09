# include "h_server.h"

/*
*   Sends an int to the user
*/

//This function is to transform an int to a string
static string itoa(int number){
    char* cInteger = (char*)malloc(4*sizeof(char));
    int i;
    for(i = 3; i > -1; i--){
        cInteger[i] = number & 0xFF;
        number >>= 8;
    }

    return cInteger;
}

int sendInt(int iSend, SOCKET sClient){
    string strMessage = itoa(iSend);    //  Transform int to a string.
    int iOffset, iCount;
    iOffset = 0;
    while(iOffset < 4){
        iCount = send(sClient, &strMessage[iOffset], 4-iOffset, 0); //  Write the string to the user
        if(iCount <= 0) return -1;
        iOffset += iCount;
    }

    return 0;
}

