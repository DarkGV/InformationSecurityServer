#include "h_server.h"

/*
*	This is the most extense code.
*	This piece of code is to handle all clients.
*/

extern Users* usLoggedIn;
extern int iConnected;
extern pthread_mutex_t lock;
extern int iLocalCount;

//	The first client would be immediatly disconnected
//	SO this functions was defined to give it a try 3 more times with a puase of 2 seconds each.
int testUser(Users user){
	int iFlag = 0, iCont;
	iFlag = receiveIntFrom(user.sUserConnection);
	
	if(iFlag <= 0){
		iCont = 0;
		while(iCont < 3 && iFlag <= 0){
			sleep(1);
			iFlag = receiveIntFrom(user.sUserConnection);
			iCont++;
		}
	}

	return iFlag;
}

void* handleClients(){
	int iFlag = -1;
	string strMsg;
	int i, j, u;
	while(1){
		for(i = 0; i < iConnected ; i++){
			iFlag = testUser(usLoggedIn[i]);
			switch(iFlag){
				case SEND_MESSAGE:
					//	We received a SEND_MESSAGE. We need to stream the message to all users.
					strMsg = ReceiveStringFrom(usLoggedIn[i].sUserConnection); //	Get the string from that user.
					
					//	Is it was an error, disconnect the user (zero toleration)
					if(strMsg == NULL){
						pthread_mutex_lock(&lock);
							iConnected--;
							close(usLoggedIn[i].sUserConnection);
							for(j = i; j < iConnected; j++){
								usLoggedIn[j] = usLoggedIn[j+1];
							}
						pthread_mutex_unlock(&lock);
						continue;
					}

					//	Broadcast the message
					for(u = 0; u < iConnected; u++){
						if(u == i) continue;//	Except to the user who sent it.
						if(sendInt(SEND_MESSAGE, usLoggedIn[u].sUserConnection) < 0){//	Send the string size.

							pthread_mutex_lock(&lock);
								iConnected--;
								close(usLoggedIn[u].sUserConnection);
								for(j = u; j < iConnected; j++){
									usLoggedIn[j] = usLoggedIn[j+1];
								}
							pthread_mutex_unlock(&lock);
							continue;
						}

						//After sending the string size, just send the message.
						if(sendTo(usLoggedIn[u].sUserConnection, strMsg) < 0){
							pthread_mutex_lock(&lock);
								iConnected--;
								close(usLoggedIn[u].sUserConnection);
								for(j = u; j < iConnected; j++){
									usLoggedIn[j] = usLoggedIn[j+1];
								}
							pthread_mutex_unlock(&lock);
						}
					}
					free(strMsg);
					strMsg = NULL;
					break;

				case SOCKET_ALIVE:	//	SOCKET_ALIVE is a flag to tell each other they both are online.
					sendInt(SOCKET_ALIVE, usLoggedIn[i].sUserConnection);
					break;

				default:
					//	Nohting was received (a number wasn't received wither) so disconnect that user.
					pthread_mutex_lock(&lock);
					iConnected--;
					close(usLoggedIn[i].sUserConnection);
					for(j = i; j < iConnected; j++){
						usLoggedIn[j] = usLoggedIn[j+1];
					}
					pthread_mutex_unlock(&lock);

					break;
			}
		}
	}

	return NULL;
}
