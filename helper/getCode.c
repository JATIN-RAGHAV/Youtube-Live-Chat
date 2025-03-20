#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headerFiles/https.h"

#define TRUE 1
#define FALSE 0

char* parseCode(request rs);
void* getCode(void* codePtr){
	request request = https();
	void** code = codePtr;
	*code = parseCode(request);
	return NULL;
}


char* parseCode(request rs){
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
	char url[]= "'https://accounts.google.com/o/oauth2/v2/auth?client_id=45593217983-4972c1kfhct6cp4cg5sir6p7ts2hcidd.apps.googleusercontent.com&redirect_uri=https://jatinraghav.duckdns.org&response_type=code&scope=openid%20profile%20email'";
	
	char* urlHeap = (char*)malloc(sizeof(url)+1);
	strcpy(urlHeap,url);
	return urlHeap;
}
