#include "h_server.h"

/*
*   The register will have the fucntions to search and add a user.
*/

static boolean bEqual;

static int callback_function(void* vData, int iNmrRows, string* strArguments, string* strColumnName){
    int i;
    for(i = 0; i < iNmrRows; i++){
        if(!strcmp("password", strColumnName[i]))
            if(!strcmp(strArguments[i], (string)vData)){
                bEqual = TRUE;
                //email = (string)realloc(email, strlen(strArguments[i-1])*sizeof(char));
                //strcpy(email, strArguments[i+1]);
                return 0;
            }   
    }
    return 0;
}

//  Check login is vulnerable to SQLInjection. Must not trust this fucntion.
boolean checkLogin(sqlite3* database, Users* usHandler, string strUserPasswd){
    bEqual = FALSE;
    
    if(usHandler->id == NULL){
        return bEqual;
    }

    string strSQLstmt = (string) calloc(strlen(usHandler->id) + 58, sizeof(char));
    strcat(strSQLstmt, "SELECT username, password FROM Users WHERE username = '");  //  Build this the query
    strcat(strSQLstmt, usHandler->id);  //  And fill itn with the name to search
    strcat(strSQLstmt, "';");
    strSQLstmt[strlen(strSQLstmt) -1] = '\0';

    string strErrorMsg = 0;

    sqlite3_exec(database, strSQLstmt, callback_function, (void*)strUserPasswd, &strErrorMsg);  //  Execute the query at the database

    free(strSQLstmt);
    free(strErrorMsg);

    return bEqual;
}
//  The same from the checkLogin was done in here.
void insertUser(sqlite3* database, string strUserNm, string strPasswd, string strEmail, string strNome){
    string strSQLstmt = (string) calloc(strlen(strUserNm) + strlen(strPasswd)+
    strlen(strEmail) + strlen(strNome) + 42, sizeof(char));
    strcat(strSQLstmt, "INSERT INTO Users VALUES('");
    strcat(strSQLstmt, strUserNm);
    strcat(strSQLstmt, "', '");
    strcat(strSQLstmt, strEmail);
    strcat(strSQLstmt, "', '");
    strcat(strSQLstmt, strPasswd);
    strcat(strSQLstmt, "', '");
    strcat(strSQLstmt, strNome);
    strcat(strSQLstmt, "');");
    strSQLstmt[strlen(strSQLstmt) -1] = '\0';

    sqlite3_exec(database, strSQLstmt, NULL, NULL, NULL);

}