#include "stdio.h"
#include "./headerFiles/getCode.h"
#include <string.h> 
#include <stdlib.h>
#include "./headerFiles/helper.h"
#include <pthread.h>

void redirectUser(char *url);

int main(){
    pthread_t codeThread;
    char*code;
    pthread_create(&codeThread, NULL,(void*) getCode,(void*)&code); 
    char* url = getUrl();
    redirectUser(url);

    pthread_join(codeThread,NULL);
    printf("Code: %s\n",(char*)code);
}

