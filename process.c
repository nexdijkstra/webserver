#include"process.h"

int mainProcess(int clientFileDiscripter)
{
				int n = 0;
				int fileDescriptor;
				/*각 메시지를 담을 변수들*/
				char buf[BUFSIZE+1] = {'\0', };
				char message[BUFSIZE+1] = {'\0', };
				char method[BUFSIZE+1] = {'\0', };

				memset(headerStruct.method, '\0', 50);
				memset(headerStruct.contentType, '\0', 50);
				headerStruct.split = 0;

				n = recv(clientFileDiscripter, buf, BUFSIZE-1, 0);


				buf[BUFSIZE] = '\0';

				strcpy(message, buf);

				printf("%s\n", message);
				//메시지의 포맷이 맞는지 판단
				if(isAvailableFormat(buf) == FALSE)
				{
								//bad request 반환
								char header[BUFSIZE] = {'\0', };
								strcpy(header, createHeader(400, "Bad Request"));
								push(header);
								response(clientFileDiscripter, buf, header);
				}else //포맷이 맞으면
				{
								char* p = NULL;

								p = strtok(buf, " ");

								/*GET인지 POST인지 판단*/
								if(strcmp(p, "GET") == 0)
								{

												p = strtok(NULL, " ");
												

												/*cgi요청인지 아닌지 판단*/
												if(isCgi(p) == TRUE)
												{
																int fileDescriptor;
																int i = 0;
																char temp[BUFSIZE] = {'\0', };
																FILE* pipe;

																strcpy(temp, p);

																for(i = 0; i < BUFSIZE; i++)
																{
																				if(temp[i] == '/')
																				{
																								sprintf(p, "%s", temp+(i+1));
																				}
																}
																

																//cgi에 필요한 변수를 얻어온다.
																createRequestCgi(message);

																if((fileDescriptor = open(p, O_RDONLY)) < 0)
																{
																			cgiProcess(clientFileDiscripter, p);
																}else
																{
																				char header[BUFSIZE] = {'\0', };
																				strcpy(header, createHeader(400, "Bad Reqeust"));
																				push(header);
																				response(clientFileDiscripter, p, header);
																}
												}else
												{
																printf("ttttt\n");

																int i = 0;
																char temp[BUFSIZE] = {'\0', };

																strcpy(temp, p);

																/*uri파싱*/
																for(i = 0; i < BUFSIZE; i++)
																{
																				if(temp[i] == '/')
																				{
																								sprintf(p, "%s", temp+(i+1));
																				}
																}
																/*url가 존재하는지 판단*/
																if(isExistFile(p) == TRUE)
																{
																				char header[BUFSIZE] = {'\0', };
																				strcpy(header, createHeader(200, "OK"));
																				push(header);
																				response(clientFileDiscripter, p, header);//00헤더를 보내고 response안에서 mime헤더와 메시지를 보냄
																}else
																{
																				// error
																				char header[BUFSIZE] = {'\0', };
																				strcpy(header, createHeader(404, "File Not Found"));
																				push(header);
																				response(clientFileDiscripter, p, header);//404에러 표출

																}
												}
								}else if(strcmp(p, "POST") == 0)
								{
												p = strtok(NULL, " ");
												/*cgi인지 아닌지 판단*/
												if(isCgi(p) == TRUE)
												{
																int fileDescriptor;
																int i = 0;
																char temp[BUFSIZE] = {'\0', }, header[BUFSIZE] = {'\0', };
																FILE* pipe;

																strcpy(temp, p);

															
																for(i = 0; i < BUFSIZE; i++)
																{
																				if(temp[i] == '/')
																				{
																								sprintf(p, "%s", temp+(i+1));
																				}
																}
												

																/*200헤더를 보냄 - 이미 iscgi에서 존재하는 파일인지 아닌지 확인 했으므로*/

																strcpy(header, createHeader(200, "OK"));
																send(clientFileDiscripter, header, BUFSIZE, 0);

																createRequestCgi(message);
															
																/*cgi 데이터 전송을 위한 프로세스 실행*/
																cgiProcess(clientFileDiscripter, p);

																close(clientFileDiscripter);
												}else
												{
																int i = 0;
																char temp[BUFSIZE] = {'\0', };

																strcpy(temp, p);

																for(i = 0; i< BUFSIZE; i++)
																{
																				if(temp[i] == '/')
																				{
																								sprintf(p, "%s", temp+(i+1));
																				}
																}

																/*URI가 존재하는지 않아는지*/
																if(isExistFile(p) == TRUE)
																{
																				char header[BUFSIZE] = {'\0', };
																				strcpy(header, createHeader(200, "OK"));
																				push(header);
																				response(clientFileDiscripter, p, header);//200헤더와 MIME헤더, 메시지를 보냄
																}else
																{
																				// 에러 메시지
																				char header[BUFSIZE] = {'\0', };
																				strcpy(header, createHeader(404, "File Not Found"));
																				push(header);
																				response(clientFileDiscripter, p, header);
																}
												}

								}else
								{
												/*포맷이 잘못되었으면 에러*/
												char header[BUFSIZE] = {'\0', };
												strcpy(header, createHeader(400, "Bad Request"));
												push(header);
												response(clientFileDiscripter, p, header);
								}
				}
}

int isAvailableFormat(char *tempMessage)
{
				char buf[BUFSIZE] = {'\0', };
				char* p = NULL;

				strcpy(buf, tempMessage);

				p = strtok(buf, " ");
				if(p != NULL) //빈 메시지면
				{
								p = strtok(NULL, " ");
								if(p != NULL)//메소드만 존재하면
								{
											p = strtok(NULL, " ");
											
											if(p != NULL)//메소드와 uri만 존재
											{
															return TRUE;
											}else
											{
															return FALSE;
											}

								}else
								{
												return FALSE;
								}

				}else
				{
								return FALSE;
				}



}

int isCgi(char* URL)
{
				int i = 0;
				char buf[BUFSIZE] = {'\0', };
				char* p;
				struct {
								char* ext;
								char* filetype;
				}cgiExtensions[] = {
								{"cgi", "text/html"},
								{0, 0} };

				strcpy(buf, URL);

				p = strtok(buf, ".");
				p = strtok(NULL, " ");

				/* 확장자가 있는 파일일 때 cgi인지 판별*/
				if(p != NULL)
				{

								for(i = 0; cgiExtensions[i].ext != 0; i++)
								{
												if(strcmp(p, cgiExtensions[i].ext) == 0)
												{
																return TRUE;
												}

								}
				}

				return FALSE;
}

int isExistFile(char* URL)
{
				int fileDiscripter = 0;

				/*파일이 존재하는지 않하는지를 판별*/
				if((fileDiscripter = open(URL, O_RDONLY)) < 0)
				{
								close(fileDiscripter);
								return FALSE;
				}else
				{
								close(fileDiscripter);
								return TRUE;
				}
}

char* createHeader(int code, char* phrase)
{
				/*매개 변수를 받아 헤더 생성*/
				char* buf = (char*)malloc(sizeof(char) * BUFSIZE);

				memset(buf,'\0', BUFSIZE);
				
				sprintf(buf, "HTTP/1.1 %d %s\r\n", code, phrase);

				return buf;
}

void response(int clientFileDescripter, char* URL, char* header)
{
				int n = 0, i = 0;
				int fileDescripter = 0;
				char buf[BUFSIZE] = {'\0', };
				char* p;

				struct {
								char* ext;
								char* filetype;
				}extensions[] = {
								{"gif", "image/gif"},
								{"jpg", "image/jpeg"},
								{"bmp", "/image/bmp"},
								{"jpeg", "image/jpeg"},
								{"png", "image/png"},
								{"zip", "image/zip"},
								{"gz", "image/gz"},
								{"tar", "image/tar"},
								{"html", "text/html"},
								{"htm", "text/html"},
								{0, 0} };

				//헤더 전송
				n = send(clientFileDescripter, header, BUFSIZE, 0);

				strcpy(buf, URL);

				p = strtok(buf, ".");
				p = strtok(NULL, " ");

				printf("%s\n", buf);
				/*MIME헤더 생성 후 전송, 메시지 바디 전송*/
				for(i = 0; extensions[i].ext != 0; i++)
				{
								if(strcmp(p, extensions[i].ext) == 0)
								{
												memset(buf, '\0', BUFSIZE);
												
												sprintf(buf, "Content-type: %s; charset=ko-KR\r\n\r\n\0", extensions[i].filetype);

												n = send(clientFileDescripter, buf, strlen(buf), 0);

												memset(buf, '\0', BUFSIZE);
												//printf("%s\n", buf);
												//strcpy(buf, "\r\n");

												//n = send(clientFileDescripter, buf, strlen(buf), 0);

												//printf("%s\n", buf);
												if((fileDescripter = open(URL, O_RDONLY)) >= 0)
												{
																while((n = read(fileDescripter, buf, BUFSIZE)) > 0)
																{
																				send(clientFileDescripter, buf, n, 0);
																				printf("%s\n", buf);
																				memset(buf, '\0', BUFSIZE);
																}
																break;
												}
								}
				}
}

void createRequestCgi(char* message)
{
				char buf[BUFSIZE] = {'\0', };
				char* p;
				headerStruct.body = (char*)malloc(sizeof(char) * BUFSIZE);

				strcpy(buf, message);

				p = strtok(buf, " ");

				strcpy(headerStruct.method, p);

				strcpy(buf, message);


				/*content-type 파싱*/
				p = strstr(buf, "Content-Type:");
				strcpy(buf, p);
				p = NULL;
				p = strtok(buf, " ");
				p = strtok(NULL, " ");
				strcpy(headerStruct.contentType, p);



				strcpy(buf, message);
				
				/*content-Length 파싱*/
				p = strstr(buf, "Content-Length:");
				strcpy(buf, p);
				p = NULL;
				p = strtok(buf, " ");
				p = strtok(NULL, " \r\n");
				headerStruct.contentLength = atoi(p);



				strcpy(buf, message);

				if((p = strstr(buf, "\r\n\r\n")) != NULL)
				{

								headerStruct.split = 1;
								strcpy(headerStruct.body, p+strlen("\r\n\r\n"));
				}
}
void cgiProcess(int fileDescriptor, char* URL)
{
				int fd[2], fd1[2];
				int n = 0;
				char buf[BUFSIZE] = {'\0', };
				pid_t pid;

				fflush(stdin);
				fflush(stdout);

				if(strcmp(headerStruct.method, "GET") == 0)
				{
								//GET일 때 URI에서 QUERSTRING 파싱
								char* p;
								char lengthBuf[BUFSIZE] = {'\0', };
								sprintf(lengthBuf, "%d", headerStruct.contentLength);

								p = strtok(URL, "?");
								strcpy(buf, p);
								strcpy(URL, p);

								p = strtok(NULL, " ");

								setenv("QUERY_STRING", p, 1);
								setenv("REQUEST_METHOD", headerStruct.method, 1);
								setenv("CONTENT_LENGTH", lengthBuf, 1);
				}else if(strcmp(headerStruct.method, "POST") == 0)
				{
								char lengthBuf[BUFSIZE] = {'\0', };
								//if(headerStruct.contentLength > BUFSIZE)	
								//			headerStruct.contentLength -= strlen(headerStruct.body);
								sprintf(lengthBuf, "%d", headerStruct.contentLength);
								setenv("REQUEST_METHOD", headerStruct.method, 1);
								setenv("CONTENT_LENGTH", lengthBuf, 1);

			
				}

				pipe(fd);
				pipe(fd1);

				pid = fork();


				if(pid == 0)
				{
								//파이프를 출력과 입력용 2개를 열고 CGI프로그램 실행
								//파이프의 출력과 입력을 표준 입출력으로 바꿈

								close(fd[1]);
								dup2(fd[0], 0);

								close(fd1[0]);
								dup2(fd1[1], 1);

								if(execl(URL, NULL) == -1)
								{
												exit(1);
								}

				}else
				{
								//파이프를 출력과 입력용 2개를 열고 CGI 필요한 정보를 넘겨줌 필요한 정보를 넘겨줌
								//파이프의 출려과 입력을 표준 입출력으로 바꿈
								//
								close(fd[0]);
								dup2(fd[1], 1);

								close(fd1[1]);
								dup2(fd1[0], 0);

								/*컨텐츠의 길이에 따라 분할로 보낼지 한번에 보낼지 정한다.*/
								if(headerStruct.contentLength < BUFSIZE)
								{
												printf("%s\n", headerStruct.body);
								}else{

										
												printf("%s\n", headerStruct.body);
												while((n = recv(fileDescriptor, buf, 1024, 0)) > 0)
																
												{
																printf("%s\n", buf);
																memset(buf, '\0', 1024);
												}
								}


								//cgi에서 읽은 메시지를 클라이언트에게 전송
								while((n = read(0, buf, 1024)) > 0)
								{
												send(fileDescriptor, buf, n, 0);
								}
				}	
}
