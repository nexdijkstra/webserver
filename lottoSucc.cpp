#include"lotto.h"

#define REQUEST_METHOD "REQUEST_METHOD"
#define QUERY_STRING "QUERY_STRING"
#define CONTENT_LENGTH "CONTENT_LENGTH"
#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define SUCCESS 1
#define ERROR -1

char* getParamString(lottomachine& Lottomachine);

void printPage(int status, lottomachine& Lottomachine);
int main() {
				char* param;
				lottomachine Lottomachine;
			
				srand((int) time(NULL));

				//파라미터를 받아들이는 함수
				param = getParamString(Lottomachine);

				//파라미터가 없다면
				if(param == NULL)
				{
								printPage(ERROR, Lottomachine);
								return 0;
				}
				
				printPage(SUCCESS, Lottomachine);

				return 0;
}

char* getParamString(lottomachine& Lottomachine)
{
				char* param;
				char* buf;
				char* p;
				int length, count = 0, total, offset;
				char boundary[BUFSIZ] = {'\0', };
				
				param = getenv(REQUEST_METHOD);
				if(param == NULL)
				{
								perror("REQUEST_METHOD env. variable doesn't exist\n");
								return NULL;
				}

				if(!strcmp(param, METHOD_GET))
				{
								/*메시지가 GET일 경우 쿼리 스트링을 메시지바디로 받는다.*/
								buf = (char*)malloc(BUFSIZ * sizeof(char));
								param = getenv(QUERY_STRING);

								if(param == NULL || buf == NULL)
								{
												perror("Memory allocation failed\n");
												return NULL;
								}

								strncpy(buf, param,BUFSIZ-1);
								buf[BUFSIZ] = '\0';
				}else if(!strcmp(param, METHOD_POST))
				{
								/*환경 변수 메시지의 길이를 받아 들인다.*/
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

								buf = (char*)malloc((length + 1) * sizeof(char));

								Lottomachine.initializeBuf(length);
								/*메시지 바디의 길이보다 현재 받아들인 길이가 작으면 계속 반복한다.*/
							  int checkLength = 0;
								char temp[BUFSIZ] = {'\0', };

								while(checkLength <= length)
								{
											count = read(STDIN_FILENO, buf, 512);
											checkLength += count;
											Lottomachine.getNumber(buf);
											memset(buf, '\0', 512);
											fflush(stdin);
											fflush(stdout);
								}
								Lottomachine.splitNumber();
				}else
				{
								perror("Requested method is unrecognizalbe\n");
								return NULL;
				}

				return buf;
}


void printPage(int status, lottomachine& Lottomachine)
{

				/*로또의 결과를 웹 서버에 표준 입출력으로 전송*/			
				printf("Content-Type: text/html\n\n\n");
				printf("<HTML>\n<HEAD></HEAD>\n<BODY>\n");


								switch(status)
				{
								case SUCCESS:
												for (int i = 0; i < 10; i++) 
												{
																Lottomachine.pickNumber();
																cout << endl;
																cout << "<br>";
												}
												break;
				}
				printf("</BODY>\n</HTML>\n");
}
