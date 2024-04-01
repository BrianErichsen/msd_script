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
    return NEW(Num) (val);
}

PTR(Val) NumVal::add_to(const PTR(Val) rhs) const {
    if (const PTR(NumVal) otherNum = CAST(const NumVal) (rhs)) {
        return NEW(NumVal) ((unsigned) val + (unsigned) otherNum->val);
    } else {
        throw std::runtime_error("Addition of non-numbers");
    }
}

PTR(Val) NumVal::mult_with(const PTR(Val) rhs) const {
    if (const PTR(NumVal) otherNum = CAST(const NumVal) (rhs)) {
        return NEW(NumVal) ((unsigned)val * (unsigned)otherNum->val);
    } else {
        throw std::runtime_error("Multiplication of non-numbers");
    }
}

std::string NumVal::to_string() const {
    return std::to_string(val);
}

bool NumVal::equals(PTR(Val) rhs) const {
    //checks for valid pointer in instance of class - if so
    if (const PTR(NumVal) otherNum = CAST(const NumVal) (rhs)) {
        //returns this.val == other.val boolean expression
        return val == otherNum->val;
    } else {//false if different classes - non valid pointer
        return false;
    }
}

bool NumVal::is_true() const {
    throw std::runtime_error("A number cannot be a boolean!");
}

// NumVal::~NumVal() {
//     //NumVal class member is an int // no implementation needed
// }

PTR(Val) NumVal::call(PTR(Val) actual_arg) const {
    throw std::runtime_error("No function to call!");
}
//end of NumVal class implementation

//Beginning of BoolVal class implementation--------------------------
//-------------BOOLVAL

//public constructor that takes boolean as an argument
BoolVal::BoolVal(bool v) {
    val = v;
}

PTR(Expr) BoolVal::to_expr() const {
    //returns the equivalent of itself
    return NEW(BoolExpr) (val);
}

bool BoolVal::equals(PTR(Val) other) const {
    if (const PTR(BoolVal) otherBool = CAST(const BoolVal) (other)) {
        //returns this.val == other.val boolean expression
        return val == otherBool->val;
    } //false if different classes - non valid pointer //if reached here
    return false;
}

PTR(Val) BoolVal::add_to(const PTR(Val) rhs) const {
    throw std::runtime_error("Addition of non numbers!");
}

PTR(Val) BoolVal::mult_with(const PTR(Val) rhs) const {
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

// BoolVal::~BoolVal() {
//     //BoolVal class member is an boolean // no implementation needed
// }

PTR(Val) BoolVal::call(PTR(Val) actual_arg) const {
    throw std::runtime_error("No function to call!");
}
//------------ end of BoolVal class implementation              //

//----------- Beginning of FunVal class implementation methods //
//-------------------------FUNVAL------

//public constructor
FunVal::FunVal(std::string arg, PTR(Expr) body) {
    formal_arg = arg;
    this->body = body;
}

PTR(Expr) FunVal::to_expr() const {
    //returns the same but in FunExpr
    return NEW(FunExpr)(formal_arg, body);
}

bool FunVal::equals(PTR(Val) rhs) const {
    //checks for pointer of valid CallExpr in type of class from other
    if (const PTR(FunVal) other = CAST(const FunVal) (rhs)) {
        //returns true only equal strings and equal expressions
        return formal_arg == other->formal_arg &&
        body->equals(other->body);
    } //false if different classes - non valid pointer //if reached here
    return false;
}

PTR(Val) FunVal::add_to(const PTR(Val) rsh) const {
    throw std::runtime_error("Addition of non numbers!");
}

PTR(Val) FunVal::mult_with(const PTR(Val) rhs) const {
    throw std::runtime_error("Multiplication of non numbers!");
}

std::string FunVal::to_string() const {
    //simply returns expr to pretty string
    return to_expr()->to_pretty_string();
}

bool FunVal::is_true() const {
    throw std::runtime_error("Function value cannot be evaluated to a boolean!");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) const {
    // Recursively substitute occurrences of the formal argument with the actual argument in the body of the function
    PTR(Expr) substituted_body = body->subst(formal_arg, actual_arg->to_expr());

    // Interpret the resulting expression after substitution
    return substituted_body->interp();
}
//public destructor
// FunVal::~FunVal() {
//     //deallocates body expr resource
//     // delete body;
// }