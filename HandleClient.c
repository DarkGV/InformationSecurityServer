#include "h_server.h"

//extern SOCKET* sClient;
extern Users* usLoggedIn;
extern int iConnected;
extern pthread_mutex_t lock;
extern int iLocalCount;

void* handleClients(){
	string strMsg;
	int i, j, u;
	while(1){
		for(i = 0; i < iConnected ; i++){
			//printf("A ler o %d USER\n", i);
			switch(receiveIntFrom(usLoggedIn[i].sUserConnection)){
				case SEND_MESSAGE:

					strMsg = ReceiveStringFrom(usLoggedIn[i].sUserConnection);

					if(strMsg == NULL){
						//printf("Recebi NULL\n");
						//if(i == 0) continue;
						pthread_mutex_lock(&lock);
						iConnected--;
						close(usLoggedIn[i].sUserConnection);
						for(j = i; j < iConnected-1; j++)
							usLoggedIn[j] = usLoggedIn[j+1];
						pthread_mutex_unlock(&lock);
						continue;
					}

					//printf("%s\n", strMsg);

					for(u = 0; u < iConnected; u++){
						if(u == i) continue;
						if(sendInt(SEND_MESSAGE, usLoggedIn[u].sUserConnection) < 0){
							//printf("ENVIEI NADA\n");
							//if(i == 0) continue;
							pthread_mutex_lock(&lock);
							iConnected--;
							close(usLoggedIn[u].sUserConnection);
							for(j = u; j < iConnected-1; j++)
								usLoggedIn[j] = usLoggedIn[j+1];
                        	/*sClient = (SOCKET*)realloc(sClient, iConnected * sizeof(SOCKET));
                        	iLocalCount = iConnected;*/
							pthread_mutex_unlock(&lock);
							continue;
						}

						if(sendTo(usLoggedIn[u].sUserConnection, strMsg) < 0){
							//if(i == 0) continue;
							pthread_mutex_lock(&lock);

							iConnected--;
							close(usLoggedIn[u].sUserConnection);
							for(j = u; j < iConnected; j++)
								usLoggedIn[j] = usLoggedIn[j+1];
                    	    /*sClient = (SOCKET*)realloc(sClient, iConnected * sizeof(SOCKET));
                    	    iLocalCount = iConnected;*/
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
					//if(i == 0) continue;

					pthread_mutex_lock(&lock);
					iConnected--;
					close(usLoggedIn[i].sUserConnection);
					for(j = i; j < iConnected; j++)
						usLoggedIn[j] = usLoggedIn[j+1];
					pthread_mutex_unlock(&lock);

					break;
			}
		}
	}

	printf("Nao quero saber mais sobre isto!");
	return NULL;
}