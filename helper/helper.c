#include "string.h"
#include "stdlib.h"
#include <stdio.h>

void redirectUser(char *url){
    char cmd[] = "open ";
    char* command = (char*)malloc(sizeof(cmd) + sizeof(url)+2);
    if(command == NULL){
        printf("Couln't create the redirect command.Not enough space in heap\n");
        exit(1);
    }
    strcpy(command, cmd);
    strcpy(&command[sizeof(cmd)-1], url);
    system(command);
}
