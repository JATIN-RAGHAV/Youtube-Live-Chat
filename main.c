#include "stdio.h"
#include "./headerFiles/getCode.h"
#include <string.h> 
#include <stdlib.h>
#include "./headerFiles/helper.h"
#include <pthread.h>

int main(){
    pthread_t codeThread;
    char*code = "code";
    pthread_create(&codeThread, NULL,(void*) getCode,(void*)&code); 
    char* url = getUrl();
    if(url == NULL){
	printf("Didn't get URL.\n");
    }
    redirectUser(url);

    pthread_join(codeThread,NULL);
    printf("Code: %s\n",(char*)code);
}
