#ifndef _TOKEN_H_
#define _TOKEN_H_

typedef enum {
  LE = 256,
  GE,
  EQ,
  NE,
  AND,
  OR,
  INT_CONST,
  STR_CONST,
  IDENTIFIER,
  VOID,
  INT,
  WHILE,
  IF,
  ELSE,
  RETURN,
  BREAK,
  CONTINUE
} token_t;

static void print_token(int token) {
  static char* token_str[] = {
    "LE",        "GE",        "EQ",         "NE",    "AND",     "OR",
    "INT_CONST", "STR_CONST", "IDENTIFIER", "VOID",  "INT",     "WHILE",
    "IF",        "ELSE",      "RETURN",     "BREAK", "CONTINUE"};

  if (token < 256) printf("%-20c", token);
  else printf("%-20s", token_str[token - 256]);
}

#endif