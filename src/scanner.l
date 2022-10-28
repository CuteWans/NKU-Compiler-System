%{
#include "token.h"
int cur_line_num = 1;
void init_scanner();
void lex_error(char* msg, int line);
%}

INTEGER             ([0-9]+)
UNTERM_STRING       (\"[^\"\n]*)
STRING              (\"[^\"\n]*\")
IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)
OPERATOR            ([+*-/%=,;!<>(){}])
SINGLE_COMMENT      ("//"[^\n]*)
PP_COMMENT          ("#"[^\n]*)
%%

[\n]                { ++cur_line_num;                                     }
[ \t\r\a]+          { /* ignore all spaces */                             }
{SINGLE_COMMENT}    { /* skip for single line comment */                  }
{PP_COMMENT}        { /* skip for preprocessor line comment */            }
{OPERATOR}          { return yytext[0];                                   }   

"<="                { return LE;                                          }
">="                { return GE;                                          }
"=="                { return EQ;                                          }
"!="                { return NE;                                          }
"&&"                { return AND;                                         }
"||"                { return OR;                                          }
"void"              { return VOID;                                        }
"int"               { return INT;                                         }
"while"             { return WHILE;                                       }
"if"                { return IF;                                          }
"else"              { return ELSE;                                        }
"return"            { return RETURN;                                      }
"break"             { return BREAK;                                       }
"continue"          { return CONTINUE;                                    }

{INTEGER}           { return INT_CONST;                                   }
{STRING}            { return STR_CONST;                                   }
{IDENTIFIER}        { return IDENTIFIER;                                  }

<<EOF>>             { return 0;                                           }

{UNTERM_STRING}     { lex_error("unterminated string ", cur_line_num);    }
.                   { lex_error("unrecognized character ", cur_line_num); }

%%

signed main(int argc, char* argv[]) {
  init_scanner();
  int token;
  while (token = yylex()) {
    print_token(token);
    puts(yytext);
  }
}

void init_scanner() {
  printf("%-20s%s\n", "TOKEN-TYPE", "TOKEN-VALUE");
  printf("-------------------------------------------------\n");
}

void lex_error(char* msg, int line) {
  printf("\nerror at line %-3d: %s\n\n", line, msg);
}

int yywrap(void) {
  return 1;
}