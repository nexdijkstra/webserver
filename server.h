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

#ifndef _CONST_H_
#define _CONST_H_

#define TRUE 1
#define FALSE 0


int getPort(int argc, char* argv[]);
void setSocket(int* serverSocket, int port);

#endif
