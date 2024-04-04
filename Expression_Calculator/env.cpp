/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#include "env.h"


PTR(Env) Env::empty = NEW(EmptyEnv)();

//throws an exception when attempting to look a variable in an
//emtpy environment
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("Free variable: " + find_name);
};
//constructor sets name, val and rest for environment
ExtendedEnv::ExtendedEnv(std::string name_, PTR(Val) val_, PTR(Env) rest_) {
    name = name_;
    val = val_;
    rest = rest;
}

//looks for a variable in the current environmnet - if found returns its val
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name) {
        return val;
    } else {//else delegates lookup to the rest of environmnet
        return rest->lookup(find_name);
    }
};
