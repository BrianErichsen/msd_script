#ifndef parse_h
#include "expr.h"
#define parse_h

Expr* parse_num(std::istream &in);
static void consume(std::istream &in, int expect);
static void skip_whitespace(std::istream &in);
static Expr* parse_expr(std::istream &in);
Expr* parse_addend(std::istream &in);
Expr* parse_multicand(std::istream &in);
Expr* parse(std::istream &in);
Expr* parse_var(std::istream &in);
Expr* parse_let(std::istream &in);
static void consumeWord(std::istream &in, std::string word);
bool isValid(std::istream& input);
bool isMatchingPairs(char open, char close);

#endif
