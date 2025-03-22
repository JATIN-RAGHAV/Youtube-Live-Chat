#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headerFiles/https.h"
#include "../headerFiles/read.h"
#include "../headerFiles/joinStrings.h"

#define TRUE 1
#define FALSE 0

char* parseCode(data_t rs);
void* getCode(void* codePtr){
	data_t* request = server(NULL);
	if(request == NULL){
		printf("Some error happened with the server.\n");
		return NULL;
	}
	void** code = codePtr;
	*code = parseCode(*request);
	return NULL;
}


char* parseCode(data_t rs){
	int isCode = FALSE;
	int index = 0;
	char *code = (char*)malloc(300);
	for(int i = 0;i<rs.length;i++){
		if(rs.request[i] == '=')
			isCode = TRUE;
		else if(isCode == TRUE){
			for(;rs.request[i] != '&' ;i++)
				code[index++] = rs.request[i];
			code[index] = 0;
			return code;
		}
	}
	return NULL;
}

char* getUrl(){
	char* before = "'https://accounts.google.com/o/oauth2/v2/auth?client_id=";
	char* after = "&redirect_uri=https://jatinraghav.duckdns.org&response_type=code&scope=openid%20profile%20email'";
	char* client_id = readFile("./.clientId");
	if(client_id == NULL){
		printf("Couln't create url, envFile not read.\n");
		return NULL;
	}
	char* strs[] = {before, client_id, after};
	char* url = join(strs,3);
	free(client_id);
	return url;
}
