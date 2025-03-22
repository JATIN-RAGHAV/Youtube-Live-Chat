#include "stdio.h"
#include "../headerFiles/joinStrings.h"
#include "../headerFiles/read.h"
#include "stdlib.h"
#include <arpa/inet.h>
#include <sys/socket.h>

char* get_request(char* code);

char* TOKEN_ENDPOINT = "https://oauth2.googleapis.com/token";

int getToken(char* code){
    char* request = get_request(code);
    return 1;
}

char* get_request(char* code){
    char* header = "POST /token HTTP/1.1\r\n"
        "Host: oauth2.googleapis.com\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n\r\n";

    char* client_id = readFile("../.clientId");
    char* client_secret = readFile("../.clientSecret");
    char* strs[] = {header,
        "code=",code,"\r\n",
        "client_id=",client_id,"\r\n",
        "client_secret=",client_secret,"\r\n",
        "redirect_uri=https://jatinraghav.duckdns.org\r\ngrant_type=authorization_code"};
    char* response = join(strs,11);
    free(client_id);
    free(client_secret);
    return response;
}

