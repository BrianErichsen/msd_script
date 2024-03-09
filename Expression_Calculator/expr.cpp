#include <stdexcept>
#include "expr.h"
#include<string>
#include <sstream>
#include "val.h"


std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}
void Expr::pretty_print_at(std::ostream &os) {
    std::streampos startPos = os.tellp();
    this->pretty_print(os, prec_none, true, startPos);
}
//implements pretty print
std::string Expr::to_pretty_string() {
    std::stringstream st("");
    this->pretty_print_at(st);
    return st.str();
}
//--Beginning of Num class implementation
//Initialization list - member value with arg val
Num::Num(int val) : value(val) {}

bool Num::equals(const Expr* other) const {
    //dynamic cast converts the pointer from other to Num type
    //if successful meaning that class is Num or derived from it means
    //that otherNum will have a valid pointer hence if --then true
    if (const Num* otherNum = dynamic_cast<const Num*>(other)) {
        //if current instace of num val == other val then true
        return value == otherNum->value;
    }
    return false;
}
Val* Num::interp() const {
    //just return value from numVal class
    return new NumVal(value);
}

bool Num::has_variable() const {
    //numbers are always not variables
    return false;
}
Expr* Num::subst(std::string st, Expr* e) const {
    //Numbers are always it's own value - nothing more
    return new Num(value);
}
void Num::print(std::ostream& os) const {
    os << value;
}
void Num::pretty_print(std::ostream& os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    os << this->value;
}
//---- end of Num class implementation


// Beguinning of VarExpr class implementation
VarExpr::VarExpr(const std::string& name) : varName(name) {}

Val* VarExpr::interp() const {
    //a var has no fixed value
    throw std::runtime_error("no value for variable");
}

bool VarExpr::equals(const Expr* other) const {
    if (const VarExpr* otherVarExpr = dynamic_cast
    <const VarExpr*>(other)) {
        return varName == otherVarExpr->varName;
    }
    return false;
}
//helper method to access varName data - reference
const std::string& VarExpr::getVarName() const {
    return varName;
}
bool VarExpr::has_variable() const {
    return true;
}
Expr* VarExpr::subst(std::string st, Expr *e) const {
    //compares st variables with e variables
    if (this->varName == st) {
        //creates another object that is a clone of e and returns it
        return e;
    }
    return new VarExpr(varName);
}

void VarExpr::print(std::ostream& os) const {
    os << varName;
}
void VarExpr::pretty_print(std::ostream& os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    os << this->varName;
}
//-end of VarExpr class implementation
// Beginning of Add class
Add::Add(Expr* l, Expr* r) : left(l), right(r) {}

Val* Add::interp() const {
    // return left->interp() + right->interp();
    Val* leftVal = left->interp();
    Val* rightVal = right->interp();

    return leftVal->add_to(rightVal);
    
}
bool Add::equals(const Expr* other) const {
    //first check for a valid pointer to see if classes match
    if (const Add* otherAdd = dynamic_cast<const Add*>(other)) {
        //recursively compares other left with current left and same for right
        return left->equals(otherAdd->left) && right->equals(otherAdd->right);
    }
    return false;
}

bool Add::has_variable() const {
    //if lhs expr same is VarExpr or rhs // recursive -- approach
    return left->has_variable() || right->has_variable();
}

Expr* Add::subst(std::string st, Expr *e) const {
    //recursive finds nums and variables and assigns to l & r
    return new Add(left->subst(st, e), right->subst(st, e));
}

void Add::print(std::ostream& os) const {
    os << "(";
    left->print(os);
    os << "+";
    right->print(os);
    os << ")";
}

void Add::pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) {
    if (p > prec_add) {
        os << "(";
    }
    this->left->pretty_print(os, static_cast<precedence_t>(prec_add + 1), true, pos);
    os << " + ";
    //is it prec_add or prec none here ??
    this->right->pretty_print(os, prec_add, true, pos);
    if (p > prec_add) {
        os << ")";
    }
}
Add::~Add() {
    delete left;
    delete right;
}
//-----------------end of Add class implementation

//--------------Beginning of Multiplication class implementation
Mul::Mul(Expr* l, Expr* r) : left(l), right(r) {}

Val* Mul::interp() const {
    // return left->interp() * right->interp();
    Val* leftVal = left->interp();
    Val* rightVal = right->interp();

    return leftVal->mult_with(rightVal);
}

bool Mul::equals(const Expr* other) const {
    if (const Mul* otherMul = dynamic_cast<const Mul*>(other)) {
        return left->equals(otherMul->left) && right->equals
        (otherMul->right);
    }
    return false;
}

bool Mul::has_variable() const {
    //only returns instances of VarExpressions its lhs and rhs
    return left->has_variable() || right->has_variable();
}

Expr* Mul::subst(std::string st, Expr *e) const {
    //recursive finds nums and variables and assigns to l & r
    return new Mul(left->subst(st, e), right->subst(st, e));
}

void Mul::print(std::ostream& os) const {
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
    this->left->pretty_print(os, static_cast<precedence_t>(prec_mult + 1), true, pos);
    os << " * ";
    this->right->pretty_print(os, prec_mult, unparethesis && let_needs_parenthesesis, pos);
    if (p > prec_mult) {
        os << ")";
    }
}
Mul::~Mul() {
    delete left;
    delete right;
}
/// end of Mul class implementation ///

//Beginning of _let class implementation ////
Let::Let(std::string l, Expr* r, Expr* body) : left(l), right(r), body(body) {}

Val* Let::interp() const {
    //maybe not necessary
    // int rhs = right->interp();
    // //Expr* subsBody = body->subst(left, new Num(rhs));
    // return body->subst(left, new Num(rhs))->interp();

    Val* rhsVal = right->interp();
    Expr* rightExpr = rhsVal->to_expr();
    Val* subsBodyVal = body->subst(left, rightExpr)->interp();

    delete rhsVal;

    return subsBodyVal;
}

bool Let::equals(const Expr* other) const {
    //first check for a valid pointer to see if classes match
    if (const Let* other_let = dynamic_cast<const Let*>(other)) {
        //recursively compares other pointers and string left
        return other_let->left == left && right->equals(other_let->right)
        && body->equals(other_let->body);
    }
    return false;
}

bool Let::has_variable() const {
    //only returns instances of VarExpressions its rhs
    //
    return right->has_variable() || body->has_variable();
}

Expr* Let::subst(std::string st, Expr *e) const {
    // creates new expressions for right and body recursively
    Expr* rhs = this->right->subst(st, e);
    //if is equal not necessary
    Expr* subsBody = this->body->subst(st, e);
    //creates new expr with new input
    return new Let(this->left, rhs, subsBody);
}

void Let::print(std::ostream& os) const {
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
    //returns current pos of char in the os
    std::streampos letPos = os.tellp();
    //calculates the number of spaces needed for indentation
    int n = letPos - pos;
    //print _let following by variable and equal sign
    os << "_let " << this->left << " = ";
    //pretty print rhs of the expression
    right->pretty_print(os, p, false, letPos);
    //prints newline
    os << "\n";
    //get the position after the new line char
    std::streampos inPos = os.tellp();
    //adds spaces for identation
    while (n > 0) {
        os << " ";
        n--;
    }
    //prints _in by spaces for identation
    os << "_in  ";
    //pretty print the body
    body->pretty_print(os, prec_none, false, inPos);

    //checks if parentheses are needed
    if (p > prec_none && let_needs_parenthesesis) {
        os << ")";
    }
}
//destructor method for let class
Let::~Let() {
    delete right;
    delete body;
}
