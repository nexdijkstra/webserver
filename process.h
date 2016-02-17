#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<err.h>
#include"queue.h"


#ifndef PROCESS_H_
#define PROCESS_H_

#define TRUE 1
#define FALSE 0
#define BUFSIZE	1024
int mainProcess(int clientSocket);
int isAvailableFormat(char* message);
int isCgi(char* URL);
void createRequestCgi(char* message);
int isExistFile(char* URL);
char* createHeader(int code, char* phrase);
void response(int clientFileDiscripter, char* URL,  char* header);
void cgiProcess(int fileDescripter, char* URL);
void headerInitialize();

struct{
				int split;
				char method[50];
				char contentType[50];
				int contentLength;
				char* body;
}headerStruct;

#endif
