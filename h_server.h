#ifndef H_SERVER_H
#define H_SERVER_H
#endif


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <openssl/rsa.h>

#define MAXLEN 50
#define TRUE 1
#define FALSE 0

typedef struct sockaddr_in socketAddress;	// This type of variable will be called saName
typedef int SOCKET;				            // This type of variable will be called sName
typedef char* string;				        // Easier to understand variables named strName
typedef int boolean;                        // This type of variable will be called bName

// ---------------------------------- STRUCTURES -------------------------------------

typedef struct handleUsers {
    string id;
    RSA* pk;
    string ip;
    SOCKET sUserConnection;
} Users;

// ---------------------------------------- END --------------------------------------


// ----------------------------------- FLAGS ------------------------------------------
#define USER_LOGIN 0x00
#define USER_LOGIN_OK 0x01
#define USER_LOGIN_FAULT 0x02
#define USER_REGISTER 0x03
#define USER_DISCONNECT 0x03
#define SOCKET_ALIVE 0x0A
#define SEND_MESSAGE 0x0B
#define SERVER_UPDATE_USERS 0x0C
// ----------------------------------- END --------------------------------------------


// ------------------------------ STRING MANIPULATION ----------------------------------

/**
* MakeString returns a pointer to the memory containing the first byte of the string
* The input is the size of the memory to alloc
*/
string MakeString ( int );

/**
 * Returns the respective int value to a string
 * receives the int value to transform into ascii 
 */
string itoa(int);
// --------------------------------------- END --------------------------------------------

// ------------------------------------- SOCKET MANIPULATION ------------------------------
int sendTo(SOCKET, string);
string ReceiveStringFrom(SOCKET);
SOCKET CreateSocket();
int receiveIntFrom(SOCKET);
int sendInt(int, SOCKET);
void* handleClients();
// -------------------------------------------- END ---------------------------------------

// ---------------------------------- LOGIN INFORMATIONS ---------------------------------
string IPCheck (SOCKET);
// ------------------------------------------- END ---------------------------------------

// ------------------------------------ HANDLE DATABASE ----------------------------------
boolean checkLogin(sqlite3*, Users*, string);
void insertUser(sqlite3*, string, string, string, string);
// -------------------------------------------- END --------------------------------------
