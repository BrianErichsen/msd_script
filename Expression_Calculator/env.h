/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
//Spring 2024                                                */

#ifndef ENV_H
#define ENV_H

#include "pointer.h"
#include <string>

class Val;
class Expr;
//virtual class that is responsible for looking up variables bindings
//in the environment
CLASS(Env) {
public:
    static PTR(Env) empty;//point to instance of emptyEnv
    virtual PTR(Val) lookup(std::string find_name) = 0;
};//end of class env bracket

//represents an empty environmnet
class EmptyEnv : public Env {
public:
    PTR(Val) lookup(std::string find_name);
};//end of class emptyEnv bracket

//represents an environment with extended new variable binding
class ExtendedEnv : public Env {
private:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
public:
    ExtendedEnv(std::string name_, PTR(Val) val_, PTR(Env) rest_);
    PTR(Val) lookup(std::string find_name);
};//end of class extendedEnv bracket

#endif //ENV_H