/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#include "val.h"
#include "expr.h"
#include <string>

//Implementation of NumVal class ----

//public constructor that takes a int as a parameter
NumVal::NumVal(int v) {
    val = v;//sets value to input
}

//returns a expression based on NumVal's value
PTR(Expr) NumVal::to_expr() const {
    return new Num(val);
}

Val* NumVal::add_to(const Val* rhs) const {
    if (const NumVal* otherNum = dynamic_cast<const NumVal*>(rhs)) {
        return new NumVal((unsigned)val + otherNum->val);
    } else {
        throw std::runtime_error("Addition of non-numbers");
    }
}

Val* NumVal::mult_with(const Val* rhs) const {
    if (const NumVal* otherNum = dynamic_cast<const NumVal*>(rhs)) {
        return new NumVal((unsigned)val * otherNum->val);
    } else {
        throw std::runtime_error("Multiplication of non-numbers");
    }
}

std::string NumVal::to_string() const {
    return std::to_string(val);
}

bool NumVal::equals(Val* rhs) const {
    //checks for valid pointer in instance of class - if so
    if (const NumVal* otherNum = dynamic_cast<const NumVal*>(rhs)) {
        //returns this.val == other.val boolean expression
        return val == otherNum->val;
    } else {//false if different classes - non valid pointer
        return false;
    }
}

bool NumVal::is_true() const {
    throw std::runtime_error("A number cannot be a boolean!");
}

NumVal::~NumVal() {
    //NumVal class member is an int // no implementation needed
}

Val* NumVal::call(Val* actual_arg) const {
    throw std::runtime_error("No function to call!");
}
//end of NumVal class implementation

//Beginning of BoolVal class implementation

//public constructor that takes boolean as an argument
BoolVal::BoolVal(bool v) {
    val = v;
}

Expr* BoolVal::to_expr() const {
    //returns the equivalent of itself
    return new BoolExpr(val);
}

bool BoolVal::equals(Val* other) const {
    if (const BoolVal* otherBool = dynamic_cast<const BoolVal*>(other)) {
        //returns this.val == other.val boolean expression
        return val == otherBool->val;
    } //false if different classes - non valid pointer //if reached here
    return false;
}

Val* BoolVal::add_to(const Val* rhs) const {
    throw std::runtime_error("Addition of non numbers!");
}

Val* BoolVal::mult_with(const Val* rhs) const {
    throw std::runtime_error("Multiplication of non numbers!");
}

std::string BoolVal::to_string() const {
    //no parenthesis for true or false //
    //if val is true then prints true and so forth
    return val ? "_true" : "_false";
}

bool BoolVal::is_true() const {
    return val;//returns its own boolean result
}

BoolVal::~BoolVal() {
    //BoolVal class member is an boolean // no implementation needed
}

Val* BoolVal::call(Val* actual_arg) const {
    throw std::runtime_error("No function to call!");
}
//------------ end of BoolVal class implementation              //

//----------- Beginning of FunVal class implementation methods //
//public constructor
FunVal::FunVal(std::string arg, Expr* body) {
    this->formal_arg = arg;
    this->body = body;
}

Expr* FunVal::to_expr() const {
    //returns the same but in FunExpr
    return new FunExpr(formal_arg, body);
}

bool FunVal::equals(Val* rhs) const {
    //checks for pointer of valid CallExpr in type of class from other
    if (const FunVal* other = dynamic_cast<const FunVal*>(rhs)) {
        //returns true only equal strings and equal expressions
        return formal_arg == other->formal_arg &&
        body->equals(other->body);
    } //false if different classes - non valid pointer //if reached here
    return false;
}

Val* FunVal::add_to(const Val* rsh) const {
    throw std::runtime_error("Addition of non numbers!");
}

Val* FunVal::mult_with(const Val* rhs) const {
    throw std::runtime_error("Multiplication of non numbers!");
}

std::string FunVal::to_string() const {
    //simply returns expr to pretty string
    return to_expr()->to_pretty_string();
}

bool FunVal::is_true() const {
    throw std::runtime_error("Function value cannot be evaluated to a boolean!");
}

Val* FunVal::call(Val* actual_arg) const {

    // Substitution map where the formal argument is mapped to the actual argument
    // std::unordered_map<std::string, Expr*> substitution_map;
    // substitution_map[formal_arg] = actual_arg->to_expr();

    // Recursively substitute occurrences of the formal argument with the actual argument in the body of the function
    Expr* substituted_body = body->subst(formal_arg, actual_arg->to_expr());

    // Interpret the resulting expression after substitution
    return substituted_body->interp();
}
//public destructor
FunVal::~FunVal() {
    //deallocates body expr resource
    delete body;
}