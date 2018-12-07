#include "h_server.h"

string IPCheck(SOCKET sClient){
    socketAddress saClient;
    socklen_t slClient = sizeof(socketAddress);
    string strLocalIP = (string) malloc(20*sizeof(char));

    if(!getpeername(sClient, (struct sockaddr*)&saClient, &slClient)){
        strcpy(strLocalIP, inet_ntoa(saClient.sin_addr));
        return strLocalIP;
    }

    return NULL;
}