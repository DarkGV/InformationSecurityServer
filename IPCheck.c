#include "h_server.h"

/* 
*   This piece of code is to get the user ip address. might be usefull for private chat.
*/

string IPCheck(SOCKET sClient){
    socketAddress saClient; //  Prepare a address struct to receive the IP
    socklen_t slClient = sizeof(socketAddress);
    string strLocalIP = (string) malloc(20*sizeof(char));   //And the string containing the IP
    //Return the information about the user.
    if(!getpeername(sClient, (struct sockaddr*)&saClient, &slClient)){
        strcpy(strLocalIP, inet_ntoa(saClient.sin_addr));// And fill the IP
        return strLocalIP;
    }

    return NULL;
}