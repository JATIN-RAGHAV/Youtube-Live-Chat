// curl \
//  'https://youtube.googleapis.com/youtube/v3/liveBroadcasts?part=snippet&broadcastStatus=active&key=AIzaSyDGhMx8Y55cS_deD2B8kd1DhL10aM747FQ' \
//  --header 'Authorization: Bearer ya29.a0AeXRPp5PkGFLPdxngYUvo-rury310VwjOtPtb-In6JKxo3XF-iTOAx3FVghPD3meMajO1D_qHB1k0EsOPOq4YMnSrGUKIhYLHCvplMZ8fHEQHpwJF9HEQne2HNNLv8kI73cXNEQ5goAqp1TskZqb5Vqy2P8fLNg6ruO8Ac_0mwaCgYKAd4SARESFQHGX2MiTylANamt7wKF8cx6-cuSzQ0177' \
//  --header 'Accept: application/json' \
//  --compressed
//
//

#include "../headerFiles/helper.h"
#include "../headerFiles/joinStrings.h"
#include "../headerFiles/https.h"
#include "../headerFiles/read.h"
#include "../headerFiles/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

char* getLiveChatRequest(char* token);
char* parse_live_id(data_t* response);

static char* SERVER_DOMAIN = "https://youtube.googleapis.com";

char* fetchLiveStreamId(char* token){
    char* request = getLiveChatRequest(token);
    if(request == NULL){
        printf("Couldn't get live chat id request\n");
        return NULL;
    }

    data_t* response = client(request, SERVER_DOMAIN);
    if(response == NULL){
        printf("Couln't get response for live stream id\n");
        return NULL;
    }

    char* live_chat_id = parse_live_id(response);
    if(live_chat_id == NULL){
        printf("Coulnt' parse the live chat id\n");
        return NULL;
    }
    if(live_chat_id[0] == 'e' && live_chat_id[1] == 0){
        printf("No current Live Stream\n");
        return NULL;
    }

    return live_chat_id;
}

char* parse_live_id(data_t* response){
    cJSON* obj;
    int i = 0;
    while(response->request[i] != '{')
        i++;
    
    obj = cJSON_Parse(&response->request[i]);
    if(obj == NULL){
    }

    cJSON* items;
    cJSON_ArrayForEach(items, obj){
        if(strcmp(items->string,"items") == 0){
            int array_len = cJSON_GetArraySize(items);
            if(array_len == 0){
                char* empty = (char*) malloc(2);
                empty[0] = 'e';
                empty[1] = 0;
                return empty;
            }
            items = cJSON_GetArrayItem(items, 0);
            cJSON* snippet;
            cJSON_ArrayForEach(snippet,items){
                if(strcmp(snippet->string, "snippet") == 0){
                    cJSON* liveChatId;
                    cJSON_ArrayForEach(liveChatId, snippet){
                        if(strcmp(liveChatId->string, "liveChatId") == 0){
                            return liveChatId->valuestring;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

char* getLiveChatRequest(char* token){
    char* clientId = readFile("./.clientId");
    if(clientId == NULL){
        printf("Coulnt' get client id\n");
        return NULL;
    }

    char* requestLineList[] = {"GET /youtube/v3/liveBroadcasts?part=snippet&broadcastStatus=active&key=",clientId," HTTP/1.1\r\n"};
    char* requestLine = join(requestLineList,3);
    char* header_auth_less = "Host: youtube.googleapis.com\r\n"
                    "Accept: application/json\r\n"
                    "Content-Type: application/x-www-form-urlencoded\r\n"
                    "Authorization: Bearer ";
    char* header_list[] = {requestLine,header_auth_less,token,"\r\n\r\n"};
    char* header_auth_full = join(header_list, 4);
    if(header_auth_full == NULL){
        printf("Couln't create live id getter header\n");
        return NULL;
    }

    return header_auth_full;
}
