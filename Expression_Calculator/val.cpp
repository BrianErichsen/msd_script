/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
//Spring 2024                                                */

#include "val.h"
#include "expr.h"
#include <string>
#include "env.h"


//Implementation of NumVal class ----

//public ructor that takes a int as a parameter
NumVal::NumVal(int v) {
    val = v;//sets value to input
}

//returns a expression based on NumVal's value
PTR(Expr) NumVal::to_expr()  {
    return NEW(Num) (val);
}

PTR(Val) NumVal::add_to( PTR(Val) rhs)  {
    if ( PTR(NumVal) otherNum = CAST( NumVal) (rhs)) {
        return NEW(NumVal) ((unsigned) val + (unsigned) otherNum->val);
    } else {
        throw std::runtime_error("Addition of non-numbers");
    }
}

PTR(Val) NumVal::mult_with( PTR(Val) rhs)  {
    if ( PTR(NumVal) otherNum = CAST( NumVal) (rhs)) {
        return NEW(NumVal) ((unsigned)val * (unsigned)otherNum->val);
    } else {
        throw std::runtime_error("Multiplication of non-numbers!");
    }
}

PTR(Val) NumVal::div_with(PTR(Val) rhs) {
    if (PTR(NumVal) otherNum = CAST(NumVal) (rhs)) {
        return NEW(NumVal) ((unsigned)val / (unsigned)otherNum->val);
    } else {
        throw std::runtime_error("Division of non-numbers!");
    }
}

std::string NumVal::to_string()  {
    return std::to_string(val);
}

bool NumVal::equals(PTR(Val) rhs)  {
    //checks for valid pointer in instance of class - if so
    if ( PTR(NumVal) otherNum = CAST( NumVal) (rhs)) {
        //returns this.val == other.val boolean expression
        return val == otherNum->val;
    } else {//false if different classes - non valid pointer
        return false;
    }
}

bool NumVal::is_true()  {
    throw std::runtime_error("A number cannot be a boolean!");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg)  {
    throw std::runtime_error("No function to call!");
}
//end of NumVal class implementation

//Beginning of BoolVal class implementation--------------------------
//-------------BOOLVAL

//public ructor that takes boolean as an argument
BoolVal::BoolVal(bool v) {
    val = v;
}

PTR(Expr) BoolVal::to_expr()  {
    //returns the equivalent of itself
    return NEW(BoolExpr) (val);
}

bool BoolVal::equals(PTR(Val) other)  {
    if ( PTR(BoolVal) otherBool = CAST( BoolVal) (other)) {
        //returns this.val == other.val boolean expression
        return val == otherBool->val;
    } //false if different classes - non valid pointer //if reached here
    return false;
}

PTR(Val) BoolVal::add_to( PTR(Val) rhs)  {
    throw std::runtime_error("Addition of non numbers!");
}

PTR(Val) BoolVal::mult_with( PTR(Val) rhs)  {
    throw std::runtime_error("Multiplication of non numbers!");
}

PTR(Val) BoolVal::div_with(PTR(Val) rhs) {
    throw std::runtime_error("Division of non numbers!");
}

std::string BoolVal::to_string()  {
    //no parenthesis for true or false //
    //if val is true then prints true and so forth
    return val ? "_true" : "_false";
}

bool BoolVal::is_true()  {
    return val;//returns its own boolean result
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg)  {
    throw std::runtime_error("No function to call!");
}
//------------ end of BoolVal class implementation              //

//----------- Beginning of FunVal class implementation methods //
//-------------------------FUNVAL------

//public ructor
FunVal::FunVal(std::string arg, PTR(Expr) body_, PTR(Env) env_) {
    formal_arg = arg;
    this->body = body_;
    env = env_;
}

PTR(Expr) FunVal::to_expr()  {
    //returns the same but in FunExpr
    return NEW(FunExpr)(formal_arg, body);
}

bool FunVal::equals(PTR(Val) rhs)  {
    //checks for pointer of valid CallExpr in type of class from other
    if ( PTR(FunVal) other = CAST( FunVal) (rhs)) {
        //returns true only equal strings and equal expressions
        return formal_arg == other->formal_arg &&
        body->equals(other->body);
    } //false if different classes - non valid pointer //if reached here
    return false;
}

PTR(Val) FunVal::add_to( PTR(Val) rsh)  {
    throw std::runtime_error("Addition of non numbers!");
}

PTR(Val) FunVal::mult_with( PTR(Val) rhs)  {
    throw std::runtime_error("Multiplication of non numbers!");
}

PTR(Val) FunVal::div_with(PTR(Val) rhs) {
    throw std::runtime_error("Division of non numbers!");
}

std::string FunVal::to_string()  {
    //simply returns expr to pretty string
    return to_expr()->to_pretty_string();
}

bool FunVal::is_true()  {
    throw std::runtime_error("Function value cannot be evaluated to a boolean!");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg)  {
    // Recursively substitute occurrences of the formal argument with the actual argument in the body of the function
    // PTR(Expr) substituted_body = body->subst(formal_arg, actual_arg->to_expr());

    // Interpret the resulting expression after substitution
    // return substituted_body->interp();
    return body->interp(NEW (ExtendedEnv)(formal_arg, actual_arg, env));
}