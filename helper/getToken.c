#include "stdio.h"

int getToken(char* code, char* clientId){
    int client_fd;
    char* header = "POST /token HTTP/1.1\r\n"
        "Host: oauth2.googleapis.com\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n\r\n";

    char* body = "";
    return 1;
}
char* header = "POST /token HTTP/1.1\r\n"
"Host: oauth2.googleapis.com\r\n"
"Content-Type: application/x-www-form-urlencoded\r\n\r\n"
"code=4/P7q7W91a-oMsCeLvIaQm6bTrgtp7&\r\n"
"client_id=your_client_id&"
"client_secret=your_client_secret&"
"redirect_uri=https%3A//oauth2.example.com/code&"
"grant_type=authorization_code";
