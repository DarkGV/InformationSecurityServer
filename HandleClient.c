#include "h_server.h"

//extern SOCKET* sClient;
extern Users* usLoggedIn;
extern int iConnected;
extern pthread_mutex_t lock;
extern int iLocalCount;

int testUser(Users user){
	int iFlag = 0, iCont;;
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

					strMsg = ReceiveStringFrom(usLoggedIn[i].sUserConnection);

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

					for(u = 0; u < iConnected; u++){
						if(u == i) continue;
						if(sendInt(SEND_MESSAGE, usLoggedIn[u].sUserConnection) < 0){

							pthread_mutex_lock(&lock);
								iConnected--;
								close(usLoggedIn[u].sUserConnection);
								for(j = u; j < iConnected; j++){
									usLoggedIn[j] = usLoggedIn[j+1];
								}
							pthread_mutex_unlock(&lock);
							continue;
						}

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

				case SOCKET_ALIVE:
					sendInt(SOCKET_ALIVE, usLoggedIn[i].sUserConnection);
					break;

				default:
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

	//printf("Nao quero saber mais sobre isto!");
	return NULL;
}
