#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

#define REQUEST_METHOD "REQUEST_METHOD"
#define QUERY_STRING "QUERY_STRING"
#define CONTENT_LENGTH "CONTENT_LENGTH"
#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define MAX_PARAMETER 1024
#define SUCCES 1
#define FAIL 0

char* getParamString();
void printPage(char* param);

int main()
{
				char* param;

				param = getParamString();

				printPage(param);
				
				if(param == NULL)
				{
								perror("no input data\n");
								return 1;
				}
			


}
void printPage(char* param)
{
				printf("Content-Type: text/html\n\n\n");
				printf("<HTML>\n<HEAD></HEAD>\n<BODY>\n");
				printf("%s<br>\n", param);
				printf("</BODY>\n</HTML>\n");
}

char* getParamString()
{
				int l = 3;
				char* param;
				char* buf;
				int length, count = 0, total, offset;

				param = getenv(REQUEST_METHOD);

				if(param == NULL)
				{
								perror("REQUEST_METOHD env. variable doesn't exist\n");
								return NULL;
				}



				if(!strcmp(param, METHOD_GET))
				{
								buf = (char*)malloc(BUFSIZ);
								param = getenv(QUERY_STRING);

								if(param == NULL || buf == NULL)
								{
												perror("Memory allocation failed\n");
												return NULL;
								}

								strncpy(buf, param, BUFSIZ-1);
								buf[BUFSIZ] = '\0';

				}else if(!strcmp(param, METHOD_POST))
				{
								param = getenv(CONTENT_LENGTH);
								if(param == NULL)
								{
												perror("CONTENT_LENGTH env. variable doesn't exist\n");
												return NULL;
								}

								length = atoi(param);

								if(length <= 0)
								{
												perror("Content Length is negative\n");
												return NULL;
								}

								buf = (char*)malloc(length + 1);

								count = read(STDIN_FILENO, buf, length);
				}else{
								perror("Requested method is unrecognizable\n");
								return NULL;
				}
				return buf;
}
