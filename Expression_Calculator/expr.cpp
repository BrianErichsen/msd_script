/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#include <stdexcept>
#include "expr.h"
#include <string>
#include <sstream>
#include "val.h"
#include "pointer.h"
#include "env.h"



std::string Expr::to_string() {
    std::stringstream st("");
    print(st);
    return st.str();
}
void Expr::pretty_print_at(std::ostream &os) {
    std::streampos startPos = os.tellp();
    pretty_print(os, prec_none, false, startPos);
}
//implements pretty print
std::string Expr::to_pretty_string() {
    std::stringstream st("");
    pretty_print_at(st);
    return st.str();
}
//--Beginning of Num class implementation
//Initialization list - member value with arg val
Num::Num(int val) : value(val) {}

bool Num::equals( PTR(Expr) other)  {
    //dynamic cast converts the pointer from other to Num type
    //if successful meaning that class is Num or derived from it means
    //that otherNum will have a valid pointer hence if --then true
    // PTR(Num) otherNum = CAST( Num) (other)
    if ( PTR(Num) otherNum = CAST( Num) (other)) {
        //if current instace of num val == other val then true
        return value == otherNum->value;
    }
    return false;
}
PTR(Val) Num::interp(PTR(Env) env)  {
    //just return value from numVal class
    return NEW (NumVal)(value);
}

PTR(Expr) Num::subst(std::string st, PTR(Expr) e)  {
    //Numbers are always it's own value - nothing more
    return NEW(Num)(value);
}
void Num::print(std::ostream& os)  {
    os << value;
}
void Num::pretty_print(std::ostream& os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    os << value;
}
//---- end of Num class implementation


// Beguinning of VarExpr class implementation
VarExpr::VarExpr( std::string name) : varName(name) {}

PTR(Val) VarExpr::interp(PTR(Env) env)  {
    //a var has no fixed value
    // throw std::runtime_error("no value for variable");
    return env->lookup(varName);
}

bool VarExpr::equals( PTR(Expr) other)  {
    if ( PTR(VarExpr) otherVarExpr = CAST( VarExpr) (other)) {
        return varName == otherVarExpr->varName;
    }
    return false;
}
//helper method to access varName data - reference
 std::string& VarExpr::getVarName()  {
    return varName;
}

PTR(Expr) VarExpr::subst(std::string st, PTR(Expr) e)  {
    //compares st variables with e variables
    if (varName == st) {
        //creates another object that is a clone of e and returns it
        return e;
    }
    return NEW (VarExpr)(varName);
}

void VarExpr::print(std::ostream& os)  {
    os << varName;
}
void VarExpr::pretty_print(std::ostream& os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    os << varName;
}
//-end of VarExpr class implementation
// Beginning of Add class
Add::Add(PTR(Expr) l, PTR(Expr) r) : left(l), right(r) {}

PTR(Val) Add::interp(PTR(Env) env)  {
    // PTR(Val) leftVal = left->interp(PTR(Env) env);
    // PTR(Val) rightVal = right->interp(PTR(Env) env);

    // PTR(Val) result = leftVal->add_to(rightVal);
    
    //prevents memory leak
    // delete leftVal;
    // delete rightVal;

    // return leftVal->add_to(rightVal);
    // return result;
    return left->interp(env)->add_to(right->interp(env));
}
bool Add::equals( PTR(Expr) other)  {
    
    //first check for a valid pointer to see if classes match
    if ( PTR(Add) otherAdd = CAST( Add) (other)) {
        //recursively compares other left with current left and same for right
        return left->equals(otherAdd->left) && right->equals(otherAdd->right);
    }
    return false;
}

PTR(Expr) Add::subst(std::string st, PTR(Expr) e)  {
    //recursive finds nums and variables and assigns to l & r
    return NEW (Add)(left->subst(st, e), right->subst(st, e));
}

void Add::print(std::ostream& os)  {
    os << "(";
    left->print(os);
    os << "+";
    right->print(os);
    os << ")";
}

void Add::pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    bool print_parent = prec_add <= p;
    if (print_parent) {
        os << "(";
    }
    this->left->pretty_print(os, static_cast<precedence_t>(prec_add + 1), true, pos);
    os << " + ";
    //is it prec_add or prec none here ??
    this->right->pretty_print(os, prec_equal, let_needs_parenthesesis && !print_parent, pos);
    if (print_parent) {
        os << ")";
    }
}
//-----------------end of Add class implementation

//--------------Beginning of Multiplication class implementation
Mul::Mul(PTR(Expr) l, PTR(Expr) r) : left(l), right(r) {}

PTR(Val) Mul::interp(PTR(Env) env)  {
    // PTR(Val) leftVal = left->interp(PTR(Env) env);
    // PTR(Val) rightVal = right->interp(PTR(Env) env);

    // PTR(Val) result = leftVal->mult_with(rightVal);

    //prevents memory leak
    // delete leftVal;
    // delete rightVal;

    // return result;
    return left->interp(env)->mult_with(right->interp(env));
}

bool Mul::equals( PTR(Expr) other)  {
    
    if ( PTR(Mul) otherMul = CAST( Mul) (other)) {
        return left->equals(otherMul->left) && right->equals
        (otherMul->right);
    }
    return false;
}

PTR(Expr) Mul::subst(std::string st, PTR(Expr) e)  {
    //recursive finds nums and variables and assigns to l & r
    return NEW (Mul)(left->subst(st, e), right->subst(st, e));
}

void Mul::print(std::ostream& os)  {
    os << "(";
    left->print(os);
    os << "*";
    right->print(os);
    os << ")";
}

void Mul::pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    //if precendence is higher than prec of mult then we need '('
    bool unparethesis = p < prec_mult;

    if (p > prec_mult) {
        let_needs_parenthesesis = false;
        os << "(";
    }
    //left operand printed with higher precedence to ensure nested mult is
    //enclosed
    left->pretty_print(os, static_cast<precedence_t>(prec_mult + 1), true, pos);
    os << " * ";
    right->pretty_print(os, prec_mult, unparethesis && let_needs_parenthesesis, pos);
    if (p > prec_mult) {
        os << ")";
    }
}
/// end of Mul class implementation ///

//Beginning of _let class implementation ////
Let::Let(std::string l, PTR(Expr) r, PTR(Expr) body) : left(l), right(r), body(body) {}

PTR(Val) Let::interp(PTR(Env) env)  {
    // PTR(Val) subsBodyVal = body->subst(this->left, this->right)->interp(PTR(Env) env);
    
    // return subsBodyVal;
    PTR(Val) right_ = right->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(left, right_, env);

    return body->interp(new_env);
}

bool Let::equals( PTR(Expr) other)  {
    
    //first check for a valid pointer to see if classes match
    if ( PTR(Let) other_let = CAST( Let) (other)) {
        //recursively compares other pointers and string left
        return other_let->left == left && right->equals(other_let->right)
        && body->equals(other_let->body);
    }
    return false;
}

PTR(Expr) Let::subst(std::string st, PTR(Expr) e)  {
    //substitute the right first so val of higher val is passed into lower
    PTR(Expr) rhs = right->subst(st, e);
    //if same for st and left(string var) then update expression
    if (left == st) {
        return NEW (Let)(this->left, rhs, this->body);
    } else {
        PTR(Expr) subsBody = body->subst(st, e);
        //creates new expr with new input
        return NEW (Let)(left, rhs, body->subst(st, e));
    }
}

void Let::print(std::ostream& os)  {
    os << "(_let " << this->left << "=";
    right->print(os);
    os << " _in ";
    body->print(os);
    os << ")";
}
void Let::pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    //check if parentheses are needed
    if (p > prec_none && let_needs_parenthesesis) {
        os << "(";
    }
    int indentation = os.tellp() - pos;
    //returns current pos of char in the os
    //calculates the number of spaces needed for indentation

    //print _let following by variable and equal sign
    os << "_let " << left << " = ";
    //pretty print rhs of the expression
    right->pretty_print(os, prec_none, false, pos);
    //prints newline
    os << "\n";
    //get the position after the new line char
    pos = os.tellp();
    //prints _in by spaces for indentation
    os << std::string(indentation, ' ');
    os << "_in  ";
    //pretty print the body
    body->pretty_print(os, prec_none, false, pos);

    //checks if parentheses are needed
    if (p > prec_none && let_needs_parenthesesis) {
        os << ")";
    }
}
//end of Let class implementations

//Beguinning of Bool Expr methods implementation

//public ructor with param boolean v // sets member to that boolean
BoolExpr::BoolExpr(bool v) {
    val = v;
}

bool BoolExpr::equals( PTR(Expr) rhs)  {
    //compares pointer with class of rhs // if BoolExpr then
    if ( PTR(BoolExpr) other_bool = CAST( BoolExpr) (rhs)) {
        //returns boolean expression where both have equal val booleans
        return val == other_bool->val;
    }
    //if reached here //no valid pointer
    return false;
}

PTR(Val) BoolExpr::interp(PTR(Env) env)  {
    return NEW (BoolVal)(val);//returns a bool val expression with same val
}

PTR(Expr) BoolExpr::subst(std::string st, PTR(Expr) e)  {
    return NEW (BoolExpr)(val);//returns this // check with TA
}

void BoolExpr::print(std::ostream& os)  {
    //no parenthesis for true or false //
    //if val is true then prints true and so forth
    val ? os << "_true" : os << "_false";
}

void BoolExpr::pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) {
        //no parenthesis for true or false //
        //if val is true then prints true and so forth
        val ? os << "_true" : os << "_false";
}
//------------end of BoolExpr methods------------------//
//                                                     //
// --- Beginning of IfExpr methods implementation/methods

//public ructor for IfExpr
IfExpr::IfExpr(PTR(Expr) test, PTR(Expr) then, PTR(Expr) else_) {
    test_part = test;
    then_part = then;
    else_part = else_;
}

bool IfExpr::equals( PTR(Expr) other)  {
    //checks for pointer of valid IfExpr in type of class from other
    
    if ( PTR(IfExpr) other_if = CAST( IfExpr) (other)) {
        //returns true only if all 3 Expr are equal and from same class
        return test_part->equals(other_if->test_part) &&
        then_part->equals(other_if->then_part) && else_part->equals(
            other_if->else_part
        );
    }
    //if reached here returns false // other has different class
    return false;
}

PTR(Val) IfExpr::interp(PTR(Env) env)  {
    //follows the if (condition) then ___ else ___
    //if test part is true then returns then_part interp PTR(Env) envelse returns else_part
    if (test_part->interp(env)->is_true()) {
        return then_part->interp( env);
        //does it need delete for memory leaks?
    }
    return else_part->interp(env);
}
    
PTR(Expr) IfExpr::subst(std::string st, PTR(Expr) e)  {
    //recursively calls subst in all 3 expressions within IfExpr
    return (NEW (IfExpr)(test_part->subst(st, e), then_part->subst(st, e),
    else_part->subst(st, e)));
}

void IfExpr::print(std::ostream& os)  {
    //prints recursively itself around parentheses
    os << "(_if ";
    test_part->print(os);
    os << "_then ";
    then_part->print(os);
    os << "_else ";
    else_part->print(os);
    os << ")";
}//end of ifExpr print bracket

void IfExpr::pretty_print(std::ostream &os, precedence_t p,
bool let_needs_parenthesesis, std::streampos &pos) {
    //using similar context than let expr for either printing
    //parentheses
    if (p > prec_none && let_needs_parenthesesis) {
        os << "(";
    }
    //calculates proper indentation bsed on streampos
    int indentation = os.tellp() - pos;
    os << "_if ";//prints _if for ifExpr and recursively prints its test_part
    test_part->pretty_print(os, prec_none, false, pos);
    os << "\n";
    //recalculates proper stream position
    pos = os.tellp();
    //prints proper indentation of white spaces followed by _then
    os << std::string(indentation, ' ') << "_then ";
    //prints recursively the then_part
    then_part->pretty_print(os, prec_none, false, pos);
    os << "\n";
    pos = os.tellp();
    //prints proper indentation of white spaces followed by _else
    os << std::string(indentation, ' ') << "_else ";
    //prints recursively the else_part
    else_part->pretty_print(os, prec_none, false, pos);

    if (p > prec_none && let_needs_parenthesesis) {
        os << ")";
    }
}//end of IfExpr pretty print bracket
//--end of class IfExpr implementation
//
//---Beguinning of class EqExpr implementation

//public ructor that 2 expression as arguments
EqExpr::EqExpr(PTR(Expr) left, PTR(Expr) right) {
    this->right = right;
    this->left = left;
}

//takes int for lhs and int for rhs
EqExpr::EqExpr(int lhs, int rhs) {
    left = NEW (Num)(lhs);
    right = NEW (Num)(rhs);
}
//takes a string to create new var in the left and an int num expr for rhs
EqExpr::EqExpr(std::string lhs, int rhs) {
    left = NEW (VarExpr)(lhs);
    right = NEW (Num)(rhs);
}

bool EqExpr::equals( PTR(Expr) rhs)  {
    //checks for pointer of valid IfExpr in type of class from other
    if ( PTR(EqExpr) other = CAST( EqExpr) (rhs)) {
        //returns true only if all 3 Expr are equal and from same class
        return right->equals(other->right) && left->equals(other->left);
    }
    //if reached here returns false // other has different class
    return false;
}

PTR(Val) EqExpr::interp(PTR(Env) env)  {
    //check if needs to delete to prevent memory leak
    //returns a boolean comparison result from lhs and rhs
    return NEW (BoolVal)(left->interp(env)->equals(right->interp(env)));
}

PTR(Expr) EqExpr::subst(std::string st, PTR(Expr) e)  {
    //calls subst recursively from both lhs and rhs
    return NEW (EqExpr)(left->subst(st, e), right->subst(st, e));
    //check if needs to delete for memory leak
}

void EqExpr::print(std::ostream& os)  {
    //it simply recursively prints itself around parentheses
    os << "(";
    left->print(os);
    os << "==";
    right->print(os);
    os << ")";
}//end of print bracket for EqExpr method

void EqExpr::pretty_print(std::ostream &os, precedence_t p,
bool let_needs_parenthesesis, std::streampos &pos) {
    //only use parentheses if given precedence is > or equal than prec_equal
    bool print_parent = prec_equal <= p;
    if (print_parent) {
        os << "(";
    }
    //recursively calls pretty print
    left->pretty_print(os, prec_equal, true, pos);
    os << " == ";//prints the double == for comparison
    right->pretty_print(os, prec_none, !print_parent && let_needs_parenthesesis, pos);
    if (print_parent) {
        os << ")";
    }
}//end of pretty print bracket for EqExpr method

//-----end of EqExpr class methods implementation
//-----Beginning of FunExpr class implementation

//public ructor
FunExpr::FunExpr(std::string arg, PTR(Expr) expr) {
    formal_arg = arg;
    body = expr;
}

bool FunExpr::equals( PTR(Expr) rhs)  {
    //checks for pointer of valid FunExpr in type of class from other
    if ( PTR(FunExpr) other = CAST( FunExpr) (rhs)) {
        //returns true only equal strings and equal expressions
        return formal_arg == other->formal_arg && body->equals(
            other->body);
    }
    //if reached here returns false // other has different class
    return false;
}

PTR(Val) FunExpr::interp(PTR(Env) env)  {
    //this is kind of redunctant I think
    return NEW (FunVal)(formal_arg, body, env);
}

PTR(Expr) FunExpr::subst(std::string st, PTR(Expr) e)  {
    //if formal_arg is the same as string (var) then returns itself
    //with given expression
    if (formal_arg == st) {
        return NEW (FunExpr)(st, body);
    } //else forms a new funExpr where it's body recursively calls subst
    return NEW (FunExpr)(formal_arg, body->subst(st, e));
}

void FunExpr::print(std::ostream& os)  {
    //it simply recursively prints itself around parentheses
    os << "(_fun (" << formal_arg << ") ";
    body->print(os);
    os << ")";
}

void FunExpr::pretty_print(std::ostream &os, precedence_t p,
bool let_needs_parenthesesis, std::streampos &pos) {
    //using same context as let class for yes or no printing
    //parentheses around expression
    if (let_needs_parenthesesis) {
        os << "(";
    }
    //calculates the proper indentation to be added afer new line
    int indentation = os.tellp() - pos;
    //prints fun -> parenthesis -> expr -> parenthesis -> new line
    os << "_fun (" << formal_arg << ") \n";
    //updates position of stream position
    pos = os.tellp();
    //prints proper indentation + 2 of white spaces
    os << std::string(indentation + 2, ' ');
    //prints body's expr recursively
    body->pretty_print(os, prec_none, false, pos);
    if (let_needs_parenthesesis) {
        os << ")";
    }
}

//end of FunExpr methods implementation

//Beginning of CallExpr methods implementation
//public ructor
CallExpr::CallExpr(PTR(Expr) function, PTR(Expr) arg) {
    to_be_called = function;
    actual_arg = arg;
}
bool CallExpr::equals( PTR(Expr) rhs)  {
    //checks for pointer of valid CallExpr in type of class from other
    if ( PTR(CallExpr) other = CAST( CallExpr) (rhs)) {
        //returns true only equal strings and equal expressions
        return to_be_called->equals(other->to_be_called) &&
        actual_arg->equals(other->actual_arg);
    }
    //if reached here returns false // other has different class
    return false;
}

PTR(Val) CallExpr::interp(PTR(Env) env)  {
    //returns interp PTR(Env) envcalled recursively for both expressions
    //and calling call method ---
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

PTR(Expr) CallExpr::subst(std::string st, PTR(Expr) e)  {
    //returns new call expr where both member's expressions
    //recursively calls subst
    return NEW (CallExpr)(to_be_called->subst(st, e),
    actual_arg->subst(st, e));
}

void CallExpr::print(std::ostream& os)  {
    //it simply recursively prints itself around parentheses
    to_be_called->print(os);
    os << "(";
    actual_arg->print(os);
    os << ")";
}

void CallExpr::pretty_print(std::ostream &os, precedence_t p,
bool let_needs_parenthesesis, std::streampos &pos) {
    //it simply recursively prints itself around parentheses
    to_be_called->pretty_print(os, prec_none, true, pos);
    os << "(";
    actual_arg->pretty_print(os, prec_none, false, pos);
    os << ")";
}