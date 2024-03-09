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