#ifndef TEST_H
#define TEST_H

#include "expr.h"
#include "parse.h"
#include "catch.h"

Expr* parse_str(const std::string &str);

#endif //test.h