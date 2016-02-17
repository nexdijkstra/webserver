#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

#ifndef QUEUE_H_
#define QUEUE_H_

#define TRUE 1
#define FALSE 0

struct node{
				char* message;
				struct node* next;
};

struct node* head;

int isEmpty();
void push(char* message);
char* pop();
void createQueue();
void* logging(void* data);

int count;
#endif
