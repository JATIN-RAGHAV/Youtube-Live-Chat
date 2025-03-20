#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../headerFiles/cJSON.h"

#define PORT 443  // Use 443 for production
#define CERT_FILE "/Users/jatinraghav/.acme.sh/jatinraghav.duckdns.org_ecc/fullchain.cer"
#define KEY_FILE "/Users/jatinraghav/.acme.sh/jatinraghav.duckdns.org_ecc/jatinraghav.duckdns.org.key"
#define BUFFERSIZE 1024
#define TRUE 1
#define FALSE 0

typedef struct{
	char *request;
	int length;
}request;


request SSL_dynamic_read(SSL *ssl);
char isCompleteRequest(char *request, int length);
request handleSSL(int *client_fd, SSL_CTX*);
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
void init_openssl();
char *getResponse();

request https(){
	init_openssl();
	SSL_CTX *ctx = create_context();
	configure_context(ctx);
	int server_fd, client_fd;

	struct sockaddr_in6 server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);

	server_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if(server_fd == -1){
		perror("Server socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin6_family= AF_INET6;
	server_addr.sin6_addr = in6addr_any;
	server_addr.sin6_port = htons(PORT);

	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("Bind Failed\n");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd, 10) <0){
		perror("Listen Failed\n");
		exit(EXIT_FAILURE);
	}

	client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0) {
		perror("Accept failed");
	}
	request request =  handleSSL(&client_fd, ctx);
	close(server_fd);
	SSL_CTX_free(ctx);
	EVP_cleanup();
	return request;
}


SSL_CTX *create_context() {
	SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
	if (!ctx) {
		perror("Unable to create SSL context");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	return ctx;
}

void configure_context(SSL_CTX *ctx) {
	if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

request handle_client(SSL *ssl) {
	request request = SSL_dynamic_read(ssl);
	printf("\a");
	char* response = getResponse();
	if(SSL_write(ssl, response,sizeof(response)) < 0){
		perror("Couln't sent response\n");
	}
	SSL_shutdown(ssl);
	SSL_free(ssl);
	return request;
}

char *getResponse(){
	cJSON* body = cJSON_CreateObject();
	cJSON_AddStringToObject(body,"Have you read about Gilgemish??", "No, you Dick I invented that shit.");
	cJSON_AddStringToObject(body,"Have you read about Hari Sheldon?", "Who hasn't?");
	cJSON_AddStringToObject(body,"You must be aware of the principle of least Action, which governs the working of the whole universe.", "Yup, I can find the equations for anything with the appropriate Lagrange.");
	cJSON_AddStringToObject(body,"Have you seen the monkey.", "uhun");
	cJSON_AddStringToObject(body,"Gott ist", "ToT.");
	char *strBody = cJSON_PrintUnformatted(body);
	const char *header= 
		"HTTP/1.1 200 YOOOO\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: ";  // The length of the body ({"})
	int bodyLen = (strlen(strBody));
	int contentLen = (int)(((ceil(log10(bodyLen))))*sizeof(char));
	char contentLength[contentLen];
	sprintf(contentLength, "%d", bodyLen);
	char *buffer = "\r\n\r\n";
	int headerLen = strlen(header);
	int bufferLen = 4;

	char *response = (char*)malloc(headerLen + contentLen + bufferLen + bodyLen);
	int index = 0;
	memcpy(&response[index], header, headerLen);
	index += headerLen;
	memcpy(&response[index], contentLength, contentLen);
	index += contentLen;
	memcpy(&response[index], buffer, bufferLen);
	index += bufferLen;
	memcpy(&response[index], strBody, bodyLen);
	index += bodyLen;
	free(strBody);
	cJSON_Delete(body);
	return response;
}

request handleSSL(int *client_fd, SSL_CTX* ctx){
	int clientD = *client_fd;
	SSL *ssl = SSL_new(ctx);
	SSL_set_fd(ssl, clientD);
        request request;
	if(SSL_accept(ssl) <= 0) {
		ERR_print_errors_fp(stderr);
	} else {
		request = handle_client(ssl);
	}

	close(clientD);
	return request;

}

request SSL_dynamic_read(SSL *ssl) {
	char *data = NULL;
	char buffer[BUFFERSIZE];
	size_t totalSize = 0;
	int receivedBytes;

	while ((receivedBytes = SSL_read(ssl, buffer, BUFFERSIZE)) > 0) {
		char *newData = realloc(data, totalSize + receivedBytes + 1);
		if (!newData) {
			free(data);
			perror("Memory allocation failed");
			request err = {NULL, 0};
			return err;
		}

		data = newData;
		memcpy(data + totalSize, buffer, receivedBytes);
		totalSize += receivedBytes;
		data[totalSize] = 0; 
		int isComplete = isCompleteRequest(data, totalSize);
		if(isComplete == TRUE)
			break;
	}
	request req = {data, totalSize};
	return req;
}

char isCompleteRequest(char *request, int length){
	int contentCompareLength = 15;
	for(int i = 0;i<length; i++){
		if(request[i] == '\n'){
			if(i+contentCompareLength < length){
				int isContent = strncmp(&request[i+1],"Content-Length:",contentCompareLength );
				if(isContent == 0){
					i += contentCompareLength + 2;
					char *intBase = &request[i];
					while(request[i] != '\n')
						i++;

					request[i] = 0;
					int contentLength = atoi(intBase);
					if(contentLength == 0)
						return TRUE;
					request[i] = '\n';
					for(;request[i]== '\n' && i<length;i++){
						if (strncmp(&request[i], "\n\r\n", 3) == 0){
							if(i+contentLength+3 == length)
								return TRUE;
							else
								return FALSE;
						}
					}
				}
			}
			if(strncmp(&request[i], "\n\r\n",3) == 0)
				return TRUE;
		}
	}
	return FALSE;

}

void init_openssl() {
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}
