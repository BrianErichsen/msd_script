/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
//Spring 2024                                                */

#ifndef TEST_H
#define TEST_H

#include "expr.h"
#include "parse.h"
#include "catch.h"

PTR(Expr) parse_str(const std::string &str);

#endif //test.h