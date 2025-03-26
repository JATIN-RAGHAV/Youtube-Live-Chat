#include "../headerFiles/read.h"
#include "../headerFiles/joinStrings.h"
#include "../headerFiles/https.h"
#include <stdio.h>
#include "../headerFiles/cJSON.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    char* msg;
}message;

typedef struct {
    message* chats;
    int len;
}chats;

static char* SERVER_DOMAIN = "https://youtube.googleapis.com";
chats* parse_get_messages(char* response);

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

    chats* chats = parse_get_messages(response->request);
    
    if(chats == NULL){
        printf("Couln't get chats\n");
    }
    else if(chats->len == 0){
        printf("No chats yet\n");
    }
    for(int i = 0;i<chats->len;i++){
        printf("%s:  %s\n",chats->chats[i].name, chats->chats[i].msg);
    }

}

chats* parse_get_messages(char* response){
    cJSON* obj;
    int i = 0;
    while(response[i] != '{')
        i++;

    obj = cJSON_Parse(&response[i]);
    if(obj == NULL){
        printf("Coulnt' parse response\n");
        return NULL;
    }



    cJSON* items;
    int msgIndex = 0;
    message* messages;
    chats* ans = (chats*)malloc(sizeof(chats));
    cJSON_ArrayForEach(items, obj){
        if(strcmp(items->string, "items") == 0){
            int messages_count = cJSON_GetArraySize(items);
            if(messages_count == 0){
                ans->len = 0;
                ans->chats = NULL;
                return ans;
            }
            else{
                messages = (message*)malloc(sizeof(message)*messages_count);
                memset(messages, 0, sizeof(message)*messages_count);
                ans->chats = messages;
                ans->len = messages_count;
            }
            cJSON* item;
            cJSON_ArrayForEach(item, items){
                cJSON* msg;
                cJSON_ArrayForEach(msg, item){
                    if(strcmp(msg->string,"snippet") == 0){
                        cJSON* textMsg;
                        cJSON_ArrayForEach(textMsg, msg){
                            if(strcmp(textMsg->string, "displayMessage") == 0){
                                    messages[msgIndex].msg = textMsg->valuestring;
                            }
                        }
                    }
                    else if(strcmp(msg->string,"authorDetails") == 0){
                        cJSON* displayName;
                        cJSON_ArrayForEach(displayName, msg){
                            if(strcmp(displayName->string, "displayName") == 0){
                                messages[msgIndex].name = displayName->valuestring;
                                msgIndex++;
                            }
                        }
                    }
                }
            }
        }
    }
    return ans;
}

char* getMessageRequest(char* token,char* live_chat_id ){
    char* clientId = readFile("./.clientId");
    if(clientId == NULL){
        printf("Couln't read client id");
        return NULL;
    }
    char* request_line_array[] = {"GET /youtube/v3/liveChat/messages?part=snippet,authorDetails&fields=items(snippet(displayMessage),authorDetails(displayName))&liveChatId=",live_chat_id,"&key=",clientId," HTTP/1.1\r\n"};
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
