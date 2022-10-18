//-----------------------定义段（复制到头文件）-------------------------
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#ifndef YYSTYPE	
#define YYSTYPE char*
#endif
char id_str[50];	
char num_str[50];
int yylex();
extern int yyparse();		
FILE* yyin;					
void yyerror(const char* s);
%}

//---------------------------定义---------------------------

%token NUMBER 
%token ID
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

lines   :   lines expr ';' { printf("%s\n", $2); }
        |   lines ';'
        |
        ;

expr    :   expr ADD expr {
                            $$ = malloc(50);
                            strcpy($$, $1);
                            strcat($$, $3);
                            strcat($$, "+ ");
                          }
        |   expr SUB expr {
                            $$ = malloc(50);
                            strcpy($$, $1);
                            strcat($$, $3);
                            strcat($$, "- ");
                          }
        |   expr MUL expr {
                            $$ = malloc(50);
                            strcpy($$, $1);
                            strcat($$, $3);
                            strcat($$, "* ");
                          }
        |   LBR expr RBR  { $$ = $2; }
        |   expr DIV expr {
                            $$ = malloc(50);
                            strcpy($$, $1);
                            strcat($$, $3);
                            strcat($$, "/ ");
                          }
        |   SUB expr %prec UMINUS {
                                    $$ = malloc(50);
                                    strcpy($$, "-");
                                    strcat($$, $2);
                                  }
        |   NUMBER  {
                      $$ = malloc(50);
                      strcpy($$, $1);
                      strcat($$, " ");
                    }
        |   ID  {
                  $$ = malloc(50);
                  strcpy($$, $1);
                  strcat($$, " ");
                }
        ;
%%

int yylex() {
  int ch;
  while (1) {
    ch = getchar();
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
      ;
    else if ((ch >= '0' && ch <= '9')) {
      int op = 0;
      while ((ch >= '0' && ch <= '9')) {
        num_str[op] = ch;
        ch          = getchar();
        ++op;
      }
      num_str[op] = '\0';  //字符串结尾\0
      yylval      = num_str;
      ungetc(ch, stdin);
      return NUMBER;
    } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
               || (ch
                   == '_')) {  //标识符由字母/下划线开头，字母/下划线/数字组成
      int op = 0;
      while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_')
             || (ch >= '0' && ch <= '9')) {
        id_str[op] = ch;
        ++op;
        ch = getchar();
      }
      id_str[op] = '\0';
      yylval     = id_str;
      ungetc(ch, stdin);
      return ID;
    } else if (ch == '+') return ADD;
    else if (ch == '-') return SUB;
    else if (ch == '*') return MUL;
    else if (ch == '/') return DIV;
    else if (ch == '(') return LBR;
    else if (ch == ')') return RBR;
    else return ch;
  }
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error:%s\n", s);
  exit(1);
}

signed main(void) {
  yyin = stdin;
  do {
    yyparse();  //语法分析器
  } while (!feof(yyin));
}