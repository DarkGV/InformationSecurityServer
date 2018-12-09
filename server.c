/*
*                    +---------------------+
*				     |	    Attention:     |
*				     +---------------------+
*		This program as some useful caracteristics in variables names to simplify in-Document search
*			variables are declared the following way:
*				type(([A-Z]+)([a-Z]+[0-1])*)
*				example:
*					int iNAme;
*					float fNAme;
*					    .
*					    .
*					    .
*
*			File created by K-team for Computer Security class
*			Code writeen by Diogo Silva
*
*/

#include "h_server.h"
#include <signal.h>

// These variables will be used by the userHandling Thread.
//SOCKET* sClient;		// To communicate with users.
int iConnected = 0;		// To know how many users are logged in
pthread_mutex_t lock;	// Change logged in users
Users* usLoggedIn;		// To know logged in users information.

// This global Variable is for local usage only.
// It will be set FALSE when a CTRL+C is sent to the server.
//static boolean serverUp;

// -------------------------- CTRL+C Handler --------------------

/*static void sigHandler(int signal){
	serverUp = FALSE;
}*/

// -------------------------------------------------------------

int main(){
	//------------------------------- VARIABLES -----------------------------------------------------
	SOCKET sServer;
	socklen_t slClilen = 0;
	socketAddress saClient;
	pthread_t thread;
	int iLocalCount;
	string strUserPasswd;
	sqlite3 *database;
	//---------------------------------- END --------------------------------------------------------

	//printf("PID = %d\nPARENT = %d\n", getpid(), getppid());
	//printf("%d", daemon(1, 1));

	//signal(SIGINT, sigHandler);

	// A socket will be returned (int) else -1 is returned.
	sServer = CreateSocket();

	iLocalCount = iConnected;
	//To accept the first client We must check if he is logged in
	//sClient = (SOCKET*) malloc(++(iConnected) * sizeof(SOCKET));
	usLoggedIn = (Users*) malloc(++(iLocalCount) * sizeof(Users));
	//iLocalCount = iConnected;

	// Prepare the database for further use.
	if(sqlite3_open("si_DataBase.db", &database) != SQLITE_OK){
		printf("Whoops, there was a problem opening the database. Please contact server administrator!");
		return -1;
	}

	// Start the thread for client handling
	pthread_create(&thread, NULL, handleClients, NULL);

	// and the mutex to avoid race condition
	pthread_mutex_init(&lock, NULL);

	/*	Set the serverUp (this variable is for ctrl+c input handling. 
	 	Whenever there's a ctrl+c, it will clean the server and exit.)*/
	//serverUp = TRUE;

	// Start the server (this must be a thread either. Need to change this later)
	while(1){
		if((usLoggedIn[iLocalCount-1].sUserConnection = accept(sServer,(struct sockaddr*)& saClient, &slClilen)) < 0){ // l.79
			printf("Oops, looks like I can't accept the client. Please contact server administrator");
			exit(-1);
		}

		/* We must get the user IP to know who is connecting.
			This IP will be used later for private chat usage */
		if((usLoggedIn[iLocalCount-1].ip = IPCheck(usLoggedIn[iLocalCount-1].sUserConnection)) == NULL){
			continue;
		}
		
		// Inform the server administrator about the user logged.
		printf("(%s)Peer Connected\n", usLoggedIn[iLocalCount-1].ip);

		if(receiveIntFrom(usLoggedIn[iLocalCount-1].sUserConnection) == USER_REGISTER){
			string strUsername = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);
			string strPasswd = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);
			string strEmail = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);
			string strNome = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);

			insertUser(database, strUsername, strPasswd, strEmail, strNome);

			close(usLoggedIn[iLocalCount-1].sUserConnection);
			continue;
		}

		// User must send login information.
		usLoggedIn[iLocalCount-1].id = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);
		
		strUserPasswd = ReceiveStringFrom(usLoggedIn[iLocalCount-1].sUserConnection);

		// Search for that user on the db. A pointer to the user information will be returned if found
		if(!checkLogin(database, &usLoggedIn[iLocalCount-1], strUserPasswd)){
			sendInt(USER_LOGIN_FAULT, usLoggedIn[iLocalCount-1].sUserConnection);
			close(usLoggedIn[iLocalCount-1].sUserConnection);
			continue;
		}

		// Inform the user he is logged in and can send SOCKET_ALIVE flag.
		sendInt(USER_LOGIN_OK, usLoggedIn[iLocalCount-1].sUserConnection);

		free(strUserPasswd);

		// Inform the UserHandling thread a new user logged in
		pthread_mutex_lock(&lock);
			//usLoggedIn = (Users*) realloc(usLoggedIn, ++iLocalCount * sizeof(SOCKET));
			//iLocalCount = iConnected;
			if(iLocalCount-1 > iConnected){
				usLoggedIn[iConnected] = usLoggedIn[iLocalCount-1];
				iLocalCount = iLocalCount - (iLocalCount -  iConnected) + 1;
				//iusLoggedIn = (Users*) realloc(usLoggedIn, ++iConnected * sizeof(Users));
			}
			
			iConnected = iLocalCount;
		pthread_mutex_unlock(&lock);
		usLoggedIn = (Users*) realloc(usLoggedIn, ++iLocalCount * sizeof(Users));
	}
	
	pthread_mutex_destroy(&lock);
	pthread_cancel(thread);

	return 0;
}
