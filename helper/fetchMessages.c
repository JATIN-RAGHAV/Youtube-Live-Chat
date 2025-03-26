// curl \
//   'https://youtube.googleapis.com/youtube/v3/liveChat/messages?part=snippet,authorDetails&liveChatId=KicKGFVDN1ZzRFFRVTJBaEdWZlNQdXIxVVRaZxILMEI2OEF4c3RYdmc&key=AIzaSyDGhMx8Y55cS_deD2B8kd1DhL10aM747FQ' \
//   --header 'Authorization: Bearer ya29.a0AeXRPp5PkGFLPdxngYUvo-rury310VwjOtPtb-In6JKxo3XF-iTOAx3FVghPD3meMajO1D_qHB1k0EsOPOq4YMnSrGUKIhYLHCvplMZ8fHEQHpwJF9HEQne2HNNLv8kI73cXNEQ5goAqp1TskZqb5Vqy2P8fLNg6ruO8Ac_0mwaCgYKAd4SARESFQHGX2MiTylANamt7wKF8cx6-cuSzQ0177' \
//   --header 'Accept: application/json' \
//   --compressed
//
//

#include "../headerFiles/read.h"
#include "../headerFiles/joinStrings.h"
#include "../headerFiles/https.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* name;
    char* msg;
}message;

static char* SERVER_DOMAIN = "https://youtube.googleapis.com";

char* getMessageRequest(char* token,char* live_chat_id );

void fetchMessages(char* token, char* live_chat_id){
    char* request = getMessageRequest(token, live_chat_id);

    if(request == NULL){
        printf("Couldn't generate request to get messages\n");
    }

    data_t* response = client(request, SERVER_DOMAIN);
    if(response == NULL){
        printf("Couln't get response from server to fetch messages\n");
    }

    for(int i = 0;i<response->length;i++)
        putchar(response->request[i]);
    putchar(10);
}

char* getMessageRequest(char* token,char* live_chat_id ){
    char* clientId = readFile("./.clientId");
    if(clientId == NULL){
        printf("Couln't read client id");
        return NULL;
    }
    char* request_line_array[] = {"GET /youtube/v3/liveChat/messages?part=snippet,authorDetails&liveChatId=",live_chat_id,"&key=",clientId," HTTP/1.1\r\n"};
    char* request_line = join(request_line_array,5);

    char* header_auth_less = "Host: youtube.googleapis.com\r\n"
                    "Accept: application/json\r\n"
                    "Content-Type: application/x-www-form-urlencoded\r\n"
                    "Authorization: Bearer ";
    char* header_list[] = {request_line,header_auth_less,token,"\r\n\r\n"};
    char* header_auth_full = join(header_list, 4);
    if(header_auth_full == NULL){
        printf("Couln't create live id getter header\n");
        return NULL;
    }

    return header_auth_full;
}
