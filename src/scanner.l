%{
#include <stdlib.h>

#include "symbol_table.h"
#include "token.h"
int cur_line_num = 1;
void scanner_init();
void scanner_cleanup();
void lex_error(char* msg, int line);
symbol_t* symbol_table;
long long cur_token_attr;
%}

INT                 ([+-]?([0-9]+))
FLOAT               ([+-]?(([0-9]*\.[0-9]+)|([0-9]+[eE][+-]?[0-9]+)))
UNTERM_STRING       (\"[^\"\n]*)
STRING              (\"[^\"\n]*\")
IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)
SINGLE_OPERATOR     ([&+*-/%=,;!<>()\[\]{}])
LINE_COMMENT        ("//"[^\n]*)
PP_COMMENT          ("#"[^\n]*)
NEWLINE             (\n)
WHITESPACE          ([ \t\r\a]+)
%%

{NEWLINE}           { ++cur_line_num;                                     }
{WHITESPACE}        { /* ignore all spaces */                             }
{LINE_COMMENT}      { /* skip for single line comment */                  }
{PP_COMMENT}        { /* skip for preprocessor comment */                 }
{SINGLE_OPERATOR}   { return yytext[0];                                   }   

"<="                { return LE;                                          }
">="                { return GE;                                          }
"=="                { return EQ;                                          }
"!="                { return NE;                                          }
"&&"                { return AND;                                         }
"||"                { return OR;                                          }
"void"              { return VOID;                                        }
"int"               { return INT;                                         }
"float"             { return FLOAT;                                       }
"do"                { return DO;                                          }
"while"             { return WHILE;                                       }
"for"               { return FOR;                                         }
"if"                { return IF;                                          }
"else"              { return ELSE;                                        }
"return"            { return RETURN;                                      }
"break"             { return BREAK;                                       }
"continue"          { return CONTINUE;                                    }

{INT}               {
                      cur_token_attr = atoi(yytext);
                      return INT_CONST;
                    }
{FLOAT}             {
                      double temp = atof(yytext);
                      cur_token_attr = *((long long*) &temp);
                      return FLOAT_CONST;
                    }
{STRING}            { return STR_CONST;                                   }
{IDENTIFIER}        {
                      symbol_t* symb = symbol_get(symbol_table, yytext);
                      if (symb == NULL) {
                        symb = malloc(sizeof(symbol_t));
                        symb->type = 0;
                        strncpy(symb->name, yytext, MX_ID_LENGTH);
                        symb->name[MX_ID_LENGTH] = '\0';
                        symbol_insert(symbol_table, symb);
                      }
                      cur_token_attr = symb;
                      return IDENTIFIER;
                    }

<<EOF>>             { return 0;                                           }

{UNTERM_STRING}     { lex_error("unterminated string ", cur_line_num);    }
.                   { lex_error("unrecognized character ", cur_line_num); }

%%

signed main(int argc, char* argv[]) {
  scanner_init();
  int token;
  while (token = yylex()) {
    print_token(token);
    printf("%-40s", yytext);
    if (token == IDENTIFIER || token == INT_CONST || token == FLOAT_CONST)
      printf("%-20lld\n", cur_token_attr);
    else puts("");
  }
  scanner_cleanup();
}

void scanner_init() {
  symbol_table = symbol_init();
  printf("%-20s%-40s%-20s\n", "TOKEN-TYPE", "TOKEN-VALUE", "TOKEN_ATTR");
  puts("--------------------------------------------------------------------------------");
}

void scanner_cleanup() {
  symbol_destory(symbol_table);
}

void lex_error(char* msg, int line) {
  printf("\nerror at line %-3d: %s\n\n", line, msg);
}

int yywrap(void) {
  return 1;
}