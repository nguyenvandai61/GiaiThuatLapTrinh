#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_STACK 100
#define MAX 100
//////////////////////////////
// Setup stack

struct Stack{
	float data[MAX_STACK];
	int top;
};

void init(Stack *s) {
	s->top = -1; 
}

int isEmpty(Stack *s) {
	return (s->top == -1)?1:0;
}

int topValue(Stack *s) {
	return s->data[s->top];
}

void push(Stack *s, int data) {
	if (s->top == MAX_STACK - 1) {
		printf("Stack is full");
		return;
	}
	
	s->top++;
	s->data[s->top] = data; 
}

int pop(Stack *s) {
	int ret = -1;
	if (s->top == -1) {
		printf("Stack is empty");
	} else {
		ret = s->data[s->top--];
	}
	return ret;
}

///////////////////////////////////////
// Functions
///////////////
// Convert
int getPrecedence(char c) {
	if (c == '*' || c == '/')
		return 2;
	if (c == '+' || c == '-')
		return 1;
	return 0;
}

void infix2Postfix(char infix[], char postfix[], bool isPrefix=false) {
	Stack s;
	init(&s);
	// Duyet tat ca cac ky tu trong infix
	int i = 0;
	int j = 0;
	while (infix[i] != '\0') {
		int token = infix[i];
		// Neu la alpha hoac num
		if (isalnum(token)) {
			postfix[j++] = token;
			// Tach cac so voi nhau bang ' '
			if (!isdigit(infix[i+1]) && isdigit(infix[i])) {
				postfix[j++] = ' ';
			}
		}
		else {
			// Neu la dau ngoac mo
			if (token == '(')
				push(&s, '(');
			else {
				// Neu la ngoac dong va data trong stack
				if (token == ')' && !isEmpty(&s)) {
					char c;
					while (c = pop(&s)!='(' && !isEmpty(&s)) {
						postfix[j++] = c; 
					}
				}
				else {
				// Neu la phep tinh
					// Neu la phep tinh co muc uu tien cao hoac bang
					if (!isPrefix) {
						while(getPrecedence(token) <= getPrecedence(topValue(&s))) {
							postfix[j++] = pop(&s);
						}
					} else {
						while(getPrecedence(token) < getPrecedence(topValue(&s))) {
							postfix[j++] = pop(&s);
						}
					}
					
					push(&s, token);
				}
			}
		}
		i++;
	}
	
	int x;
	while (!isEmpty(&s))
	{
		x = pop(&s);
		postfix[j++] = x;
	}

	// Ket thuc Postfix
	postfix[j] = '\0'; 
}

void infix2Prefix(char infix[], char prefix[]) {
	int infixLength = strlen(infix);
	// Dao chuoi
	char temp;
	
	for(int i = 0; i <infixLength/2; i++) {
		temp = infix[i];
		infix[i] = infix[infixLength-1-i];
		infix[infixLength-1-i] = temp;
	}
	
	// Doi ngoac
	for(int i = 0; i < infixLength; i++)
		if (infix[i] == '(')
			infix[i] = ')'; 
		else if (infix[i] == ')')
				infix[i] = '(';
	// Chuyen sang hau to
	infix2Postfix(infix, prefix, true);
	printf("%s\n", prefix);
	
	// Dao nguoc lai
	int prefixLength = strlen(prefix);
	for(int i = 0; i <prefixLength/2; i++) {
		temp = prefix[i];
		prefix[i] = prefix[prefixLength-1-i];
		prefix[prefixLength-1-i] = temp;
	}
}
//////////////////////////////
// Evaluate
float evaluatePostfix(char postfix[]) {
	Stack s;
	init(&s);
	char *p = &postfix[0];
	float a, b, res;
	
	while(*p != '\0') {
		while (*p == ' ') {
			p++;
		}
		if (isalnum(*p)) {
			// Xet truong hop so
			if (isdigit(*p))
			{
				int num = 0;
				while(isdigit(*p)) {
					num*= 10;
					num+= *p - '0';
					p++;
				}
				p--;		
				push(&s, num);
			}
			// Xet la chu
			else
			{
				printf("Can not evaluate!!");
				return 0;
			}
		}
		// xet la toan tu
		else
		{
			a = pop(&s);
			b = pop(&s);
			
			switch (*p)
			{
			case '+':
				res = b + a;
				break;
			case '-':
				res = b - a;
				break;
			case '*':
				res = b * a;
				break;
			case '/':
				res = b / a;
				break;
			default:
				printf("\nInvalid Operator");
				return 0;
			}
			push(&s, res);
		}
		p++;
	}
	res = pop(&s);
	return res;

}


float evaluatePrefix(char *prefix)
{
	struct Stack s;
	char *p;
	int a, b, res;
	int prefixLength = strlen(prefix);
	s.top = -1; 
	p = &prefix[prefixLength-1];
	
	while (p >= &prefix[0])
	{

		// Xet truong hop dau " "
		while (*p == ' ' || *p == '\t')
		{
			p--;
		}
		
		if (isalnum(*p)) {
			// Xet truong hop so
			if (isdigit(*p))
			{
				int num = *p - 48;
//				while(isdigit(*p)) {
//					num*= 10;
//					num+= *p - 48;
//					p--;
//				}
//				p++;		
				push(&s, num);
			}
			// Xet la chu
			else
			{
				printf("Can not evaluate!");
				return 0;
			}
		}
		// xet la toan tu
		else
		{
			a = pop(&s);
			b = pop(&s);
			
			switch (*p)
			{
			case '+':
				res = a + b;
				break;
			case '-':
				res = a - b;
				break;
			case '*':
				res = a * b;
				break;
			case '/':
				res = a / b;
				break;
			default:
				printf("\nInvalid Operator");
				return 0;
			}
			push(&s, res);
		}
		p--;
	}
	res = pop(&s);
	return res;
}

////////////////
// MAIN 
int main() {
	char A[MAX], B[MAX], C[MAX], D[MAX];
	printf("Infix : ");
//	const char* word = "2+6*9-7+5";

//	const char* word = "4+5*3-2";
//	const char* word = "2-3*5+4"; 
//	const char* word = "2+3*5+4";
//	const char* word = "5*7-4*2*3";
//	const char* word = "4+3*2-5";
//	const char* word = "5+3*2-4+5"; 

	const char* word = "7-2*3+5*8-4/2";
	printf("%s\n", word);
	// Chuyen const char ve char array
    strncpy(A,word, MAX);
//    
//    
	infix2Prefix(A, B);
	printf("Chuoi Prefix: %s\n", B);
	printf("Equals is %f\n", evaluatePrefix(&B[0]));
	
	strncpy(C,word, MAX);
	infix2Postfix(C, D);
	printf("Postfix: %s\n", D);
	printf("Equals is %f\n", evaluatePostfix(&D[0]));
}
