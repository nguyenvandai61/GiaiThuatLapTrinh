#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Stack.cpp"
#define NSOP 3
#define KEY 101
const char* soperators[] = { "sqrt", "sin", "cos" };

enum soperator {
	SQRT = KEY,
	SIN,
	COS
};
///////////////////////////////////////
// Functions
bool isOperator(char c) {
	const char* operators = "+-*/%^\0";
	int length = strlen(operators);
	for (int i = 0; i < length; i++) {
		if (operators[i] == c) {
			return true;
		}
	}
	
	return false;
}

const char* token2text(char token) {
	if (token == SQRT) return soperators[SQRT-KEY];
	if (token == SIN) return soperators[SIN-KEY];
	if (token == COS) return soperators[COS - KEY];
	return "";
}

char text2token(const char* text) {
	if (strcmp(text, soperators[0]) == 0) return KEY;
	if (strcmp(text, soperators[1]) == 0) return KEY+1;
	if (strcmp(text, soperators[2]) == 0) return KEY + 2;
	return NULL;
}

char* revertStr(char* str) {
	char newstr[8];
	int n = strlen(str);
	for (int i = 0; i < n; i++) {
		newstr[i] = str[n - i - 1];
	}
	newstr[n] = '\0';
	return newstr;
}

char* revertStr(const char* str) {
	char newstr[8];
	int n = strlen(str);
	for (int i = 0; i < n; i++) {
		newstr[i] = str[n - i - 1];
	}
	newstr[n] = '\0';
	return newstr;
}

bool isSOperator(char* str) {
	for (int i = 0; i < NSOP; i++) {
		if (strcmp(str, soperators[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool isSOperator(char token) {
	switch (token) {
		case SQRT:
		case COS:
		case SIN: return true;
	}
	return false;
}

int getPrecedence(char c) {
	if (c == '^' || c == SQRT || c == SIN || c == COS) 
		return 4;
	if (c == '*' || c == '/' || c == '%')
		return 3;
	if (c == '+' || c == '-')
		return 2;
	return 0;
}


void passOperator2Stack(Stack<char>*& s, char*& postfix, int& j, char token, bool isPrefix = false) {
	
	if (!isPrefix) {
		while (!s->isEmpty() && getPrecedence(token) <= getPrecedence(s->topValue())) {
			if (!isSOperator(token)) {
				postfix[j++] = s->pop();
			}
			else {
				char c = s->pop();
				const char* strtxt = token2text(token);
				char str[8];
				strcpy(str, strtxt);
				if (isPrefix)
					strcpy(str,revertStr(str));
				
				for (int k = 0; k < strlen(str); k++) {
					postfix[j++] = str[k];
				}
			}
		}
	}
	else {

		while (!s->isEmpty() && getPrecedence(token) < getPrecedence(s->topValue())) {
			if (!isSOperator(s->topValue())) {
				postfix[j++] = s->pop();
			}
			else {
				char c = s->pop();
				const char* strtxt = token2text(c);

				
				int n = strlen(strtxt);
				for (int k = n - 1; k >= 0; k--) {
					postfix[j++] = strtxt[k];
				}
			}
		}
	}
	s->push(token);
}

///////////////////////////////////////
// Convert
void infix2Postfix(char infix[], char postfix[], bool isPrefix = false) {
	Stack<char>* s = new Stack<char>();
	s->init();
	// Duyet tat ca cac ky tu trong infix
	int i = 0;
	int j = 0;
	while (infix[i] != '\0') {
		char token = infix[i];
		// Neu la alpha hoac num
		if (isalnum(token)) {
			if (isalpha(token)) {
				// Truong hop alpha toan tu
				int k = i;
				char txt[8]="";
				while (isalpha(infix[k])) {
					txt[k-i]= infix[k];
					k++;
				}
				
				if (isPrefix) { 
					strcpy(txt, revertStr(txt)); 
				};

				if (isSOperator(txt)) {
					// Dua phep tinh SQRT vao stack
					token = text2token(txt);
					passOperator2Stack(s, postfix, j, token, isPrefix);
				}
				i = k - 1;

			}
			else {
				postfix[j++] = token;
				// Tach cac so voi nhau bang ' '
				if (isOperator(infix[i + 1]) && isdigit(infix[i])) {
					postfix[j++] = ' ';
				}
			}
			
		}
		else {
			// Neu la dau .
			if (token == '.') {
				postfix[j++] = token;
			}
			else
			// Neu la dau ngoac mo
			if (token == '(') {
				s->push('(');
			}

			else {
				// Neu la ngoac dong va data trong stack
				if (token == ')' && !s->isEmpty()) {
					char c;
					while ((c = s->pop()) != '(' && !s->isEmpty())
					{
						if (!isSOperator(c)) {
							postfix[j++] = c;
						}
						else {
							const char* strtxt = token2text(c);
							char str[8];
							strcpy(str, strtxt);
							for (int k = 0; k < strlen(str); k++) {
								postfix[j++] = str[k];
							}
						}
					}
				}
				else {
					// Neu la phep tinh
						// Neu la phep tinh co muc uu tien cao hoac bang
					passOperator2Stack(s, postfix, j, token, isPrefix);
				}
			}
		}
		i++;
	}
	while (!s->isEmpty())
	{
		char token = s->pop();
		if (!isSOperator(token)) {
			postfix[j++] = token;
		}
		else {
			const char* strtxt = token2text(token);
			char str[8];
			strcpy(str, strtxt);
			if (isPrefix)
				strcpy(str, revertStr(str));
			for (int k = 0; k < strlen(str); k++) {
				postfix[j++] = str[k];
			}
		}
	}

	// Ket thuc Postfix
	postfix[j] = '\0';
}

void infix2Prefix(char infix[], char prefix[]) {
	int infixLength = strlen(infix);
	// Dao chuoi
	char temp;

	for (int i = 0; i < infixLength / 2; i++) {
		temp = infix[i];
		infix[i] = infix[infixLength - 1 - i];
		infix[infixLength - 1 - i] = temp;
	}

	// Doi ngoac
	for (int i = 0; i < infixLength; i++)
		if (infix[i] == '(')
			infix[i] = ')';
		else if (infix[i] == ')')
			infix[i] = '(';
	// Chuyen sang hau to
	infix2Postfix(infix, prefix, true);

	// Dao nguoc lai
	int prefixLength = strlen(prefix);
	for (int i = 0; i < prefixLength / 2; i++) {
		temp = prefix[i];
		prefix[i] = prefix[prefixLength - 1 - i];
		prefix[prefixLength - 1 - i] = temp;
	}
}
//////////////////////////////
// Evaluate
double evaluatePostfix(char* postfix) {
	Stack<double>* s = new Stack<double>();
	s->init();
	char* p = &postfix[0];
	double a, b, res;

	while (*p != '\0') {
		while (*p == ' ') {
			p++;
		}
		if (isalnum(*p)) {
			// Xet truong hop so
			if (isdigit(*p))
			{
				double num = 0;
				while (isdigit(*p)) {
					num *= 10;
					num += (double)*p - '0';
					p++;
				}
				// Xet phan thap phan
				if (*p == '.') {
					p++;
					int n = 0;
					while (isdigit(*p)) {
						n++;
						num += ((double)*p - '0')*pow(10, -n);
						p++;
					}
				}
				p--;
				s->push(num);
			}
			// Xet la chu
			else
			{
				int i = 0;
				char txtOperator[10];
				txtOperator[i++] = *p;
				while (isalpha(*(p + 1))) {
					p++;
					txtOperator[i++] = *p;
				}

				txtOperator[i] = '\0';
				if (isSOperator(txtOperator)) {
					if (strcmp(txtOperator, "sqrt") == 0)
						res = sqrt(s->pop());
					if (strcmp(txtOperator, "sin") == 0)
						res = sin(s->pop());
					if (strcmp(txtOperator, "cos") == 0)
						res = cos(s->pop());
					s->push(res);
				} else {
					printf("Khong tinh duoc!!");
					return 0;
				}
			
			}
		}
		// xet la toan tu
		else
		{
			a = s->pop();
			b = s->pop();

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
			case '%':
				res = (int)b % (int)a;
				break;
			case '^':
				res = pow(b, a);
				break;
			default:
				printf("\nLoi ky tu");
				return 0;
			}
			s->push(res);
		}
		p++;
	}
	res = s->pop();
	return res;
}

double evaluatePrefix(char* prefix)
{
	Stack<double>* s = new Stack<double>();
	char* p;
	double a, b, res;
	int prefixLength = strlen(prefix);
	p = &prefix[prefixLength - 1];

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
				int i = 0;
				int product;
				double num = 0;
				// Kiem tra xem co phai so thap phan
				char* p1 = p;
				bool isReal = false;
				int nAfterDot = 0;
				while (!isOperator(*p1) && *p1 != ' ') {
					if (*p1 == '.') {
						isReal = true;
						break;
					}
					nAfterDot++;
					p1--;
				};

				// Phan thap phan
				if (isReal) {
					int n = nAfterDot;
					while (isdigit(*p)) {
						num += ((double)*p - '0') * pow(10, -n);
						n--;
						p--;
					}
					p--;
				}

				while (isdigit(*p)) {
					product = pow(10, i++);
					num += product * ((double)*p - '0');
					p--;
				}
				
				p++;
				s->push(num);
			}
			// Xet la chu
			else
			{
				int i = 0;
				char txtOperator[8];
				txtOperator[i++] = *p;
				while (p > &prefix[0] && isalpha(*(p - 1))) {
					p--;
					txtOperator[i++] = *p;
				}

				txtOperator[i] = '\0';
				strcpy(txtOperator, revertStr(txtOperator));

				if (isSOperator(txtOperator)) {
					if (strcmp(txtOperator, "sqrt") == 0)
						res = sqrt(s->pop());
					if (strcmp(txtOperator, "sin") == 0)
						res = sin(s->pop());
					if (strcmp(txtOperator, "cos") == 0)
						res = cos(s->pop());
					s->push(res);
				}
				else 
				{
					printf("Khong tinh duoc!!");
					return 0;
				}
			}
		}
		// xet la toan tu
		else
		{
			a = s->pop();
			b = s->pop();

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
				res = (double)a / b;
				break;
			case '%':
				res = (int)a % (int)b;
				break;
			case '^':
				res = pow(a, b);
				break;
			default:
				printf("\nLoi ky tu");
				return 0;
			}
			s->push(res);
		}
		p--;
	}
	res = s->pop();
	return res;
}

//////////////////
// MAIN	 ////////
////////////////
int main() {
	char A[MAX], B[MAX], C[MAX], D[MAX];

	FILE* file;
	char problem[MAX];

	file = fopen("Problems.txt", "r");
	while (!feof(file)) {
		fscanf(file, "%s", problem);
		printf("Bai toan: %s\n", problem);
		strcpy(A, problem);
		strcpy(C, problem);

		infix2Prefix(A, B);
		printf("Chuoi Prefix: %s\n", B);
		printf("Ket qua la %lf\n", evaluatePrefix(B));

		infix2Postfix(C, D);
		printf("Postfix: %s\n", D);
		printf("Ket qua la %lf\n", evaluatePostfix(D));
		printf("\n\n\n");
	}
	
	fclose(file);
}
