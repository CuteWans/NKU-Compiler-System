%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
int yylex();//获得token
extern int yyparse();//语法分析器
FILE *yyin;//IO
void yyerror(const char *s);
%}

//定义
%token NUMBER
%token ADD
%token SUB
%token MUL
%token DIV
%token LBR
%token RBR
%left ADD SUB
%left MUL DIV
%right UMINUS

%%
//规则
lines	:	lines expr ';'	{ printf("%f\n", $2); }
		|	lines ';'
		|
		;

expr	:	expr ADD expr	{ $$ = $1 + $3; }
		|	expr SUB expr	{ $$ = $1 - $3; }
		|	expr MUL expr	{ $$ = $1 * $3; }
		|	expr DIV expr	{ $$ = $1 / $3; }
		|	LBR expr RBR	{ $$ = $2; }
		|	SUB expr %prec UMINUS	{ $$ = -$2; }
		|	NUMBER { $$ = $1; }
		;
		
%%
//子程序段
int yylex() {
	char ch;
	while(1) {
		ch = getchar();
		if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {

		} else if(isdigit(ch)) {
			yylval = 0;//yacc全局变量，用yylval的值给number赋值
			while(isdigit(ch)) {
				yylval = yylval * 10 + ch - '0';
				ch = getchar();
			}
			ungetc(ch, stdin);
			return NUMBER;
		} else if(ch == '+')	return ADD;
		else if(ch == '-')	return SUB;
		else if(ch == '*')	return MUL;
		else if(ch == '/')	return DIV;
		else if(ch == '(')	return LBR;
		else if(ch == ')')	return RBR;
		else return ch;//调用yyerror
	}
}

void yyerror(const char *s) {
	fprintf(stderr, "Parse error:%s\n", s);
	exit(1);
}

int main(void)
{
	yyin = stdin;
	do {
		yyparse();//语法分析器
	} while(!feof(yyin));
	return 0;
}
