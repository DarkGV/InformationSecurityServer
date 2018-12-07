#ifndef _RSA_H
#define _RSA_H
#endif

//Include openssl used libraries for the server side

#include <stdio.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

typedef char* string;
typedef int boolean;
typedef unsigned char DIGEST[1024];   //  Signatures must come in SHA1, so we have 160 bits = 20 bytes
/* 
*   This key must have been generated before with openssl command.
*   openssl genrsa 1024(bit) (the key will be 1024 bits long)
*   User will also have the abillity to change his pk-sk.
*   
*   To run this code it is needed to have libssl installed.
*/


/*
*   Code written by Diogo Silva
*/

string digitalSignatureOfMsg(string);       //  Function to sign the message with SHA1

boolean checkSignature(EVP_PKEY*, string, string);   //  Function to check if message was from the right person (BIO)

string cipherMessage(EVP_PKEY*, string);    //  Wheneber I want to send a message to the user, must encrypt with his public key (BIO)

string decipherMessage(string);             // Whenever I want to decipher the message, it will need my pk