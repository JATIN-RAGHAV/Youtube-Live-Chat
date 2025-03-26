#include "stdio.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "./headerFiles/helper.h"
#include "./headerFiles/fetchCode.h"
#include "./headerFiles/fetchToken.h"
#include "./headerFiles/fetchAccessToken.h"
#include "./headerFiles/fetchLiveStream.h"
#include "./headerFiles/fetchMessages.h"
#include <pthread.h>

int main(){

	char* refresh_token = get_refresh_token();
	char* access_token = NULL;
	if(refresh_token == NULL || strlen(refresh_token) == 0){
		printf("Couln't load refresh token\n");
		printf("The user has to relogin\n");
		pthread_t codeThread;
		char*code = NULL;
		pthread_create(&codeThread, NULL,(void*) fetchCode,(void*)&code); 
		char* url = getUrl();
		if(url == NULL){
			printf("Didn't get URL.\n");
		}
		redirectUser(url);

		pthread_join(codeThread,NULL);
		if(code == NULL){
			printf("Didn't get code.\n");
			exit(1);
		}
		printf("Code: %s\n",code);
		char** tokens = fetchToken(code);

		if(save_tokens(tokens) == 0){
			printf("Couln't save tokens");
			exit(1);
		}
		access_token = tokens[0];
		refresh_token = tokens[1];
	}

	access_token = get_access_token();
	if(access_token == NULL){
		access_token = fetchAccessToken(refresh_token);
	}
	char* tokens[2] = {access_token,refresh_token};
	if(save_tokens(tokens) == 0){
		printf("Couln't save tokens");
		exit(1);
	}
	free(refresh_token);

	char* live_chat_id = fetchLiveStreamId(access_token);
	if(live_chat_id == NULL){
		access_token = fetchAccessToken(refresh_token);
		tokens[0] = access_token;
		if(save_tokens(tokens) == 0){
			printf("Couln't save tokens");
			exit(1);
		}
		live_chat_id = fetchLiveStreamId(access_token);
		if(live_chat_id == NULL){
			printf("Couldn't get live chat id");
			exit(1);
		}
	}
	fetchMessages(access_token, live_chat_id);
}
