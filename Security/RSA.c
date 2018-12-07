#include "RSA.h"

string digitalSignatureOfMsg(string strMsg){

    EVP_MD_CTX  md_ctx;
    DIGEST dgst;
    string sha1Signed;
    unsigned int iBytesWritten = 0;

    //  How to sign a message:
    //  First we need to read the sk from the file:

    FILE* sk = fopen("pk", "r"); // Open the file read-only (chmod = 400)

    EVP_PKEY* pkeySK = PEM_read_PrivateKey(sk, NULL, NULL, NULL); //    Only want to get the private key fro the file;
    fclose(sk);

    //  We then have to init the signature of the message and tell that we want to sign with SHA1:
    EVP_SignInit(&md_ctx, EVP_sha1());

    //  After SIgnature is initialized, we have to set the message (strMsg)
    EVP_SignUpdate(&md_ctx, strMsg, strlen(strMsg));

    //  Now we sign the message:
    EVP_SignFinal(&md_ctx, dgst, &iBytesWritten, pkeySK);

    //  Clean up the sk handler
    EVP_PKEY_free(pkeySK);
    
    sha1Signed = (string) calloc(strlen(dgst), sizeof(char));

    strcpy(sha1Signed, dgst);

    return sha1Signed;
}

boolean checkSignature(EVP_PKEY* pk, string dgst, string strMsg){

    EVP_MD_CTX md_ctx;

    //  To check the signature we must get the pk and the dgst
    //  Since we have the pk (got as parameter), we just need to initiate the verification:

    EVP_VerifyInit(&md_ctx, EVP_sha1());

    //  After Verification is initiated, we need to to fill the ctx with the strMsg:
    EVP_VerifyUpdate(&md_ctx, strMsg, strlen(strMsg));

    //  Just return the value from the verify (1 -> OK, 0 -> FAILURE, -1 -> ERROR)
    int ret = EVP_VerifyFinal(&md_ctx, dgst, strlen(dgst), pk);

    printf("%d", ret);

    return ret;
}