#include "string.h"
#include "stdlib.h"

void redirectUser(char *url){
    char cmd[] = "open ";
    char* command = (char*)malloc(sizeof(cmd) + sizeof(url)+2);
    strcpy(command, cmd);
    strcpy(&command[sizeof(cmd)-1], url);
    system(command);
}
