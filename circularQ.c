/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();
	element data;
	char command;

	printf("[----- [정준호]    [2020069046] -----]\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);


	/*** 명령어 입력에 2개 문자 이상 입력했을 때 오류 처리(다시 입력하도록 함) ***/
		if (getchar() != '\n') {
			printf("Sorry.. It was hard to understand...\n");
			command = '\0';
			while (getchar() != '\n');
			continue;
		}


		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));		// 큐 동적할당

	/* 전처리 */
	if (cQ == NULL) {
		printf("Error: Memory Allocation failed");
		exit(1);
	}

	cQ->front = 0;
	cQ->rear = 0;
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
 /* 큐가 비어있다면 front == rear */
int isEmpty(QueueType *cQ)
{
	if ( cQ->front == cQ->rear ) {
		printf("Current queue is EMPTY.. Insert an element(or more) and try again.\n");
		return 1;
	}
    return 0;
}

/* complete the function */
 /* 큐가 모두 차있을때, front와 rear의 위치가 변경되는 것을 막기위해 솔루션과 다르게 isFull이 작성되었습니다.
 	큐가 Full 일 때, front와 rear의 상황이 두 가지 경우가 있습니다.
	1. front = 0, cQ->rear = MAX_QUEUE_SIZE - 1 일때 (특수한 경우),
	2. front 와 rear 가 하나 차이날때 (일반적인 경우)
	두 경우 모두를 판별합니다.*/
int isFull(QueueType *cQ)
{
	if ( cQ->front == 0 ) {							// 특수한 경우
		if ( !((cQ->front + (MAX_QUEUE_SIZE - 1)) - cQ->rear) ) {	
			printf("Current queue is FULL.. Delete an element(or more) and try again.\n");
			return 1;
		}
	}
	else {											// 일반적인 경우
		if ( !((cQ->front - 1) - cQ->rear) ) {
			printf("Current queue is FULL.. Delete an element(or more) and try again.\n");
			return 1;
		}
	}
   return 0;
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{
	/* 2개 이상의 문자로 사용자가 잘못 입력했을 때, 맨 앞의 문자만 큐에 저장하고 나머지는 잃습니다. */
	if (getchar() != '\n') {
		printf("!You may lose some of your input data! Please try printing your queue and check.\n");
		while (getchar() != '\n');
	}

	/* 전처리 */
	if (isFull(cQ)) {
		return;
	}

	/* 아래와 같이 나머지 연산을 활용하면 rear 나 front 가 MAX_QUEUE_SIZE 를 넘어갈 때
	0으로 초기화하는 것과 같은 효과를 얻을 수 있습니다. */
	cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
	cQ->queue[cQ->rear] = item;

	// return 0;						
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
	/* 전처리 */
	if (isEmpty(cQ)) {
		return;
	}

	/* 아래와 같이 나머지 연산을 활용하면 rear 나 front 가 MAX_QUEUE_SIZE 를 넘어갈 때
	0으로 초기화하는 것과 같은 효과를 얻을 수 있습니다. */
	cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;
	*item = cQ->queue[cQ->front];
	printf("Popped item >>> %c\n", *item);

    // return 0;
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1) % MAX_QUEUE_SIZE;
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1) % MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}
