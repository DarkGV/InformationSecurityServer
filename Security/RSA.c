#include "RSA.h"

string digitalSignatureOfMsg(string strMsg){

    EVP_MD_CTX  md_ctx;
    DIGEST dgst;
    int iBytesWritten = 0;

    //  How to sign a message:
    //  First we need to read the sk from the file:

    FILE* sk = fopen("pk", "r"); // Open the file read-only (chmod = 400)

    EVP_PKEY* pkeySK = PEM_read_PrivateKey(sk, NULL, NULL, NULL); //    Only want to get the private key fro the file;
    fclose(sk);

    //  We then have to init the signature of the message and tell that we want to sign with SHA1:
    EVP_SignInit(&md_ctx, EVP_sha1());

    //  After SIgnature is initialized, we have to set the message (strMsg)
    EVP_SignUpdate(&md_ctx, strMsg, strlen(strMsg));

    //Now we sign the message:
    EVP_SignFinal(&md_ctx, dgst, iBytesWritten, pkeySK);

    EVP_PKEY_free(pkeySK);

    return dgst;
}