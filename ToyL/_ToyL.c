#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "expr_val.h"

char ch;  //����ȫ���ַ�����
FILE *fp;  //����ȫ���ļ�ָ�����
Position P;  //����ȫ��tokenָ��
Position P_old; //�����﷨���������еĵ��ʻ���

Token init_token() {
	Token T;
	T = malloc(sizeof(struct Token_Node));
	T->next = NULL;
	T->last = NULL;
	P = T;
	return T;
}

//�������͵Ķ��ַ����������˸�ʽ��
void next_Char() {
	ch = fgetc(fp);
}
void next_avail_char() {
	next_Char();
	while (is_layout(ch)) {
		next_Char();
	}
}

void back() {
	fseek(fp, -1, 1); //���ļ�ָ��ӵ�ǰλ����ǰ�ƶ�һ���ֽ�
}

//�������������������ַ������뵽����
void recognize_number() {
	int N = 0;
	while (is_digit(ch)) {
		N = N * 10 + (ch)-'0';
		next_Char();
	}
	Position Tmp;
	Tmp = malloc(sizeof(struct Token_Node));
	Tmp->_class = NUMB;
	Tmp->seman = malloc(sizeof(char) + 1);
	Tmp->seman[0] = N;  //������������ֱ�Ӵ����ַ������У��ַ�����ĵ�һ��Ԫ�ؼ�ΪASCII��ΪN��Ӧ���ַ�
	Tmp->seman[1] = '\0';
	P->next = Tmp;
	Tmp->last = P;
	P = Tmp;
	back();
}

//������ʶ��
void recognize_name() {
	char name[10];
	int np = 0;
	name[np++] = ch;
	next_Char();
	while (is_digit(ch) || is_letter(ch)) {
		name[np++] = ch;
		next_Char();
	}
	name[np] = '\0';
	if (strcmp(name, "begin") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = BEGIN;
		Tmp->seman = "begin";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "end") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = END;
		Tmp->seman = "end";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		return; //end�����ֲ���ʹ��back()������Ӧ��ֱ�ӷ��أ�
	}
	else if (strcmp(name, "read") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = READ;
		Tmp->seman = "read";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else if (strcmp(name, "write") == 0) {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = WRITE;
		Tmp->seman = "write";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	else {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = IDEN;
		Tmp->seman = malloc(sizeof(char) * strlen(name));
		strcpy(Tmp->seman, name);
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
	}
	back();
}

//����һ�����ʵĴʷ�������
void next_token(void) {

	next_avail_char();
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': recognize_number(); break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
	case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
	case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
	case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z': recognize_name(); break;

	case '+': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = PLUS;
		Tmp->seman = "+";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case '*': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = MULT;
		Tmp->seman = "*";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ':': {
		next_Char();
		if (ch != '=') Error("Error: invalid identifier!\n");
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = ASS;
		Tmp->seman = ":=";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ';': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = SEMI;
		Tmp->seman = ";";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case '(': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = OPEN;
		Tmp->seman = "(";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case ')': {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = CLOSE;
		Tmp->seman = ")";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	case  EOF: {
		Position Tmp;
		Tmp = malloc(sizeof(struct Token_Node));
		Tmp->_class = EOF;
		Tmp->seman = "eof";
		P->next = Tmp;
		Tmp->last = P;
		P = Tmp;
		Tmp->next = NULL;
		break;
	}
	default: break;
	}
}

//�﷨����������
void token(int _class) {
	if (P->_class != _class) {
		Error("Syntax Error!\n");
	}
	P = P->next;
}

void expr_parser() {
loop:	if (P->_class != IDEN && P->_class != NUMB) {
	Error("Syntax Error!\n");
}
		P = P->next;
		if (P->_class == PLUS || P->_class == MULT) {
			P = P->next;
			goto loop;
		}
		return;
}

void prog_parser() {
	token(BEGIN);
	while (P->_class != END) {
		switch (P->_class) {
		case READ: P = P->next; token(OPEN); token(IDEN); token(CLOSE); token(SEMI); break;
		case WRITE: P = P->next; token(OPEN); expr_parser(); token(CLOSE); token(SEMI); break;
		case IDEN: P = P->next; token(ASS); expr_parser(); token(SEMI); break;
		default: Error("Syntax Error!\n"); break;
		}
	}
	P = P->next;
	token(EOF);

	return;
}

//�������ģ��

//����ʽ��ֵ


/*
//�������
void in_out() {

}
*/

void interpreter() {
	token(BEGIN);
	Position Tmp;
	while (P->_class != END) {
		switch (P->_class) {
		case READ:
			P = P->next;
			token(OPEN);
			updata(P->seman, read());
			P = P->next;
			token(CLOSE);
			token(SEMI);
			break;
		case WRITE:
			P = P->next;
			token(OPEN);
			printf("%d\n", fetch(P->seman));
			P = P->next;
			token(CLOSE);
			token(SEMI);
			break;
		case IDEN:
			Tmp = P;
			P = P->next;
			token(ASS);
			updata(Tmp->seman, expr_val(P));
			P = P->next;
			if (mp == 2) {
				P = P->next;
				P = P->next;
			}
			token(SEMI);
			break;
		default: Error("Syntax Error444!\n");
		}
	}
}

int main() {
	/*Token T;
	T = init_token();
	char c[100];
	printf("Input directory of file:");
	gets(c);
	fp = fopen(c, "r");

	//�ʷ�����
	while (P->_class != EOF) {
		next_token();
	}
	P = T->next;

	//�﷨����
	prog_parser();

	//�������
	P = T->next;
	interpreter();

	fclose(fp);
	

	int _ = 1;
	printf("%d", _);
	*/
	#define D 3
	enum A {
		A, B, C
	};
	printf("%d", D);
	int a;

	return 0;
}