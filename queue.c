#include"queue.h"

void createQueue()
{
				head = (struct node*)malloc(sizeof(struct node)); //큐 생성
				head->next = NULL;
				head->message = NULL;
}

int isEmpty()
{
				if(head->next == NULL)
				{
								return TRUE;
				}else
				{
								return FALSE;
				}
}

void push(char* message)
{
				struct node* temp = NULL;
				struct node* newNode = (struct node*)malloc(sizeof(struct node));

				newNode->message = (char*)malloc(sizeof(char) * BUFSIZ); //message = 로그남길 메시지
				strcpy(newNode->message, message);
				newNode->next = NULL;

				count++;

				//큐의 끝을 찾는다.
				for(temp = head; temp->next != NULL; temp = temp->next)
				{
				}

				temp->next = newNode;
}

char* pop()
{
				struct node* popNode = NULL;
				char* buf = (char*)malloc(sizeof(char) * BUFSIZ);

				popNode = head->next;

				head->next = popNode->next;

				strcpy(buf, popNode->message);

				free(popNode);

				return buf;//기록할 메시지 반환
}

void *logging(void* data)
{
				FILE* logFile;

				count = 0;

				createQueue();

				if((logFile = fopen("log.txt", "w")) < 0)
				{
								printf("log file error\n");
				}

				while(1)
				{
							//기록할 메시지가 있으면 등록	
								if(count>0)
								{
												fprintf(logFile, pop());
												//count--;
								}else
								{
												continue;
								}
				}
}
