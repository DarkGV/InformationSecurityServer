#ifndef _RSA_H
#include "RSA.h"
#endif

string digitalSignatureOfMsg(unsigned char* dgst, int* size, string strMsg){

    EVP_PKEY *pkey;
    FILE* privateKeyFile = fopen("Security/sk", "r");
    RSA* privateKeyRSA;
    int ret;

    dgst = (unsigned char*) calloc(256, sizeof(char));

    pkey = PEM_read_PrivateKey(privateKeyFile, NULL, NULL, NULL);
    fclose(privateKeyFile);

    privateKeyRSA = EVP_PKEY_get1_RSA(pkey);

    ret = RSA_sign(NID_sha1WithRSA, strMsg, strlen(strMsg), dgst, size, privateKeyRSA);

    return dgst;
}

boolean checkSignature(RSA* pk, int size, unsigned char* dgst, string strMsg){
    return RSA_verify(NID_sha1WithRSA, strMsg, strlen(strMsg), dgst, size, pk);;
}

string cipherMessage(RSA* publicKey, string msg, string cipher, int* size){
    cipher = (string) calloc(128, sizeof(char)); //    working with a modulus of 1024 bits -> 128 bytes
    *size =RSA_public_encrypt(strlen(msg), msg, cipher, publicKey, RSA_PKCS1_OAEP_PADDING);
    return cipher;
}

string decipherMessage(string cipher, string message, int size){

    EVP_PKEY *pkey;
    FILE* privateKeyFile = fopen("Security/sk", "r");
    RSA* privateKeyRSA;
    int ret;

    pkey = PEM_read_PrivateKey(privateKeyFile, NULL, NULL, NULL);
    fclose(privateKeyFile);

    privateKeyRSA = EVP_PKEY_get1_RSA(pkey);

    message = (string)calloc(1024, sizeof(char));

    RSA_private_decrypt(size, cipher, message, privateKeyRSA, RSA_PKCS1_OAEP_PADDING);

    return message;
}