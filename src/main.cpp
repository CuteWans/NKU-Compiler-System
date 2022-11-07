#include <string.h>
#include <unistd.h>

#include <iostream>

#include "Ast.h"
#include "SymbolTable.h"
#include "Type.h"
using namespace std;

Ast          ast;
extern FILE* yyin;
extern FILE* yyout;

int yyparse();

char outfile[256] = "a.out";
bool dump_tokens;
bool dump_ast;

int main(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "ato:")) != -1) {
    switch (opt) {
      case 'o': strcpy(outfile, optarg); break;
      case 'a': dump_ast = true; break;
      case 't': dump_tokens = true; break;
      default:
        fprintf(stderr, "Usage: %s [-o outfile] infile\n", argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }
  if (optind >= argc) {
    fprintf(stderr, "no input file\n");
    exit(EXIT_FAILURE);
  }
  if (!(yyin = fopen(argv[optind], "r"))) {
    fprintf(stderr, "%s: No such file or directory\nno input file\n",
            argv[optind]);
    exit(EXIT_FAILURE);
  }
  if (!(yyout = fopen(outfile, "w"))) {
    fprintf(stderr, "%s: fail to open output file\n", outfile);
    exit(EXIT_FAILURE);
  }
  SymbolEntry* selib1 = new IdentifierSymbolEntry(TypeSystem::intType, "getint",
                                                  identifiers->getLevel());
  identifiers->install("getint", selib1);
  SymbolEntry* selib2 = new IdentifierSymbolEntry(TypeSystem::intType, "getch",
                                                  identifiers->getLevel());
  identifiers->install("getch", selib2);
  SymbolEntry* selib3 = new IdentifierSymbolEntry(
    TypeSystem::floatType, "getfloat", identifiers->getLevel());
  identifiers->install("getfloat", selib3);
  SymbolEntry* selib4 = new IdentifierSymbolEntry(
    TypeSystem::intType, "getarray", identifiers->getLevel());
  identifiers->install("getarray", selib4);
  SymbolEntry* selib5 = new IdentifierSymbolEntry(
    TypeSystem::intType, "getfarray", identifiers->getLevel());
  identifiers->install("getfarray", selib5);

  SymbolEntry* selib6 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "putint", identifiers->getLevel());
  identifiers->install("putint", selib6);
  SymbolEntry* selib7 = new IdentifierSymbolEntry(TypeSystem::voidType, "putch",
                                                  identifiers->getLevel());
  identifiers->install("putch", selib7);
  SymbolEntry* selib8 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "putarray", identifiers->getLevel());
  identifiers->install("putarray", selib8);
  SymbolEntry* selib9 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "putfloat", identifiers->getLevel());
  identifiers->install("putfloat", selib9);
  SymbolEntry* selib10 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "putfarray", identifiers->getLevel());
  identifiers->install("putfarray", selib10);

  SymbolEntry* selib11 = new IdentifierSymbolEntry(TypeSystem::voidType, "putf",
                                                   identifiers->getLevel());
  identifiers->install("putf", selib11);

  SymbolEntry* selib12 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "before_main", identifiers->getLevel());
  identifiers->install("before_main", selib12);
  SymbolEntry* selib13 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "after_main", identifiers->getLevel());
  identifiers->install("after_main", selib13);
  SymbolEntry* selib14 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "_sysy_starttime", identifiers->getLevel());
  identifiers->install("_sysy_starttime", selib14);
  SymbolEntry* selib15 = new IdentifierSymbolEntry(
    TypeSystem::voidType, "_sysy_stoptime", identifiers->getLevel());
  identifiers->install("_sysy_stoptime", selib15);
  yyparse();
  if (dump_ast) ast.output();
  return 0;
}
