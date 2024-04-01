/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#ifndef parse_h
#include "expr.h"
#define parse_h

PTR(Expr) parse_num(std::istream &in);
static void consume(std::istream &in, int expect);
static void skip_whitespace(std::istream &in);
PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
static void consumeWord(std::istream &in, std::string word);
std::string parse_keyword(std::istream &in);

#endif
