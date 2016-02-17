#include"server.h"
#include"process.h"
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>

void zombieHandler();  //좀비 프로세서 죽이는 함수

int main(int argc, char* argv[])
{
				int clientFileDescriptor;
				int serverSocket;
				int state;
				struct sockaddr_in clientAddress;
				struct sigaction act;
				pid_t forkId;
				pthread_t logThread;
				socklen_t sin_len = sizeof(clientAddress);

				setSocket(&serverSocket, getPort(argc, argv));

				act.sa_handler = zombieHandler;
				sigemptyset(&act.sa_mask);
				act.sa_flags = 0;

				state = sigaction(SIGCHLD, &act, 0);


				if(pthread_create(&logThread, NULL, logging, NULL) < 0) //로그 쓰레드는 로그를 계쏙 체크
				{
								printf("log file open error\n");
				}

				while(1)
				{
								if((clientFileDescriptor = accept(serverSocket, (struct sockaddr*)&clientAddress, &sin_len)) == -1)
								{
												continue;
								}

								if((forkId =fork()) == 0)
								{
												mainProcess(clientFileDescriptor);//메인 프로세스 실행

												close(clientFileDescriptor);

								}else
								{
												close(clientFileDescriptor);
								}
				}



}


int getPort(int argc, char* argv[])
{

				if(argc < 2)
				{
								printf("Enter a port number\n");
								return 0;
				}

				return atoi(argv[1]);//포트 반환
}

void setSocket(int* serverSocket, int port)
{

				int one = 1;
				struct sockaddr_in serverAddress;

				/*서버 개통을 위한 설정*/
				serverAddress.sin_family = AF_INET;
				serverAddress.sin_addr.s_addr = INADDR_ANY;
				serverAddress.sin_port = htons(port);

				if(((*serverSocket) = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				{
								printf("Can't open socket\n");
								exit(1);
				}

				setsockopt((*serverSocket), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

				if(bind((*serverSocket), (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
				{
								close((*serverSocket));
								printf("Can't bind\n");
								exit(1);
				}

				listen((*serverSocket), 10);
				
}

void zombieHandler()
{
				pid_t child;
				int state;

				child = waitpid(-1, &state, WNOHANG);
}
