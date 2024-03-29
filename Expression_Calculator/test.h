/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#ifndef TEST_H
#define TEST_H

#include "expr.h"
#include "parse.h"
#include "catch.h"

Expr* parse_str(const std::string &str);

#endif //test.h