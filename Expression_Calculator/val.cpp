#include "val.h"
#include "expr.h"

//Implementation of NumVal class ----

//public constructor that takes a int as a parameter
NumVal::NumVal(int v) {
    val = v;//sets value to input
}

//returns a expression based on NumVal's value
Expr* NumVal::to_expr() const {
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

int NumVal::to_int() const {
    return val;
}

bool NumVal::is_true() const {
    throw std::runtime_error("A number cannot be a boolean!");
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

int BoolVal::to_int() const {
    //booleans are not integers
    throw std::runtime_error("A boolean cannot be a integer!");
}