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

	printf("REFRESH TOKEN: %s\n",refresh_token);
	if(access_token == NULL){
		access_token = fetchAccessToken(refresh_token);
	}
	printf("ACCESS TOKEN: %s\n",access_token);
	free(refresh_token);

	putchar(10);
	char* live_chat_id = fetchLiveStreamId(access_token);
	if(live_chat_id == NULL){
		printf("Couldn't get live chat id\n");
		exit(1);
	}
	printf("LIVE CHAT ID: %s\n",live_chat_id);


}
