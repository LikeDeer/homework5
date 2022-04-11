/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	printf("[----- [정준호]    [2020069046] -----]\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		if (getPriority(*exp) == operand)			// 피연산자면 배열에 그대로 전달
		{
			x = *exp;
			charCat(&x);
		}
		else if (getPriority(*exp) == lparen)		// '('면 무조건 스택에 저장
		{
			postfixPush(*exp);
		}
		else if (getPriority(*exp) == rparen)		// ')'면 '(' 전 까지 스택에서 Pop 그리고 배열에 저장
		{
			while ((x = postfixPop()) != '(') {
				charCat(&x);
			}
		}
		/* 이후엔 괄호를 제외한 연산자가 등장하면 */
		else if (getPriority(*exp) > getPriority(postfixStack[postfixStackTop]))		// 연산자가 이전 연산자보다 우선순위가 높다면 임시 스택에 Push
		{
			postfixPush(*exp);
		}
		else																			// 그렇지 못하다면 배열에 기록
		{
			x = *exp;
			charCat(&x);
		}
		exp++;				// infix의 다음 문자로 넘어가기 위해 exp++;
	}

	/* infixExp 가 소진되었다면, 스택에 남은 연산자들을 postfixExp 에 기록 */
	while(postfixStackTop != -1)
	{
		x = postfixPop();
		charCat(&x);
	}

	printf("Check postfix => %s\n", postfixExp);		// 후위 표기식 확인
}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	int oprnd1, oprnd2;
	int i;
	int length = strlen(postfixExp);
	char symbol;

	/* 수식을 왼쪽에서 오른쪽으로 스캔 */
	for(i = 0; i < length; i++) {
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) {	   	// 연산자를 만날 때까지 피연산자를 스택에 저장
			evalPush(symbol - '0');				  /* 정수는 문자 숫자에서 48을 뺀 값 */
		}
		else {								   	// 연산자를 만나면 연산에 필요한 만큼(2개)의 피연산자를 스택에서 pop
			oprnd2 = evalPop();					  /* oprnd2 -> oprnd1 순서로 pop 해야 본래 맞는 순서로 연산됨 */
			oprnd1 = evalPop();

			switch(getToken(symbol)) {			// 연산
			case plus:
				evalPush(oprnd1 + oprnd2);		// 연산 결과를 다시 스택에 저장
				break;
			case minus:
				evalPush(oprnd1 - oprnd2);
				break;
			case times:
				evalPush(oprnd1 * oprnd2);
				break;
			case divide:
				evalPush(oprnd1 / oprnd2);
				break;
			default:
				break;
			}
		}	// 식의 끝에 도달할 때까지 위의 과정 반복
	}
	evalResult = evalPop();		// 스택의 톱에서 해답을 가져옴

	printf("Evaluation result => %d\n", evalResult);
}
