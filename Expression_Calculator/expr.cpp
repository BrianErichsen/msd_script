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
    Val* leftVal = left->interp();
    Val* rightVal = right->interp();

    Val* result = leftVal->add_to(rightVal);
    
    //prevents memory leak
    delete leftVal;
    delete rightVal;

    return result;
}
bool Add::equals(const Expr* other) const {
    //first check for a valid pointer to see if classes match
    if (const Add* otherAdd = dynamic_cast<const Add*>(other)) {
        //recursively compares other left with current left and same for right
        return left->equals(otherAdd->left) && right->equals(otherAdd->right);
    }
    return false;
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
Add::~Add() {
    delete left;
    delete right;
}
//-----------------end of Add class implementation

//--------------Beginning of Multiplication class implementation
Mul::Mul(Expr* l, Expr* r) : left(l), right(r) {}

Val* Mul::interp() const {
    Val* leftVal = left->interp();
    Val* rightVal = right->interp();

    Val* result = leftVal->mult_with(rightVal);

    //prevents memory leak
    delete leftVal;
    delete rightVal;

    return result;
}

bool Mul::equals(const Expr* other) const {
    if (const Mul* otherMul = dynamic_cast<const Mul*>(other)) {
        return left->equals(otherMul->left) && right->equals
        (otherMul->right);
    }
    return false;
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
    Val* rhsVal = right->interp();
    Expr* rightExpr = rhsVal->to_expr();
    Val* subsBodyVal = body->subst(this->left, rightExpr)->interp();
    
    //prevents memory leak
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

Expr* Let::subst(std::string st, Expr *e) const {
    //substitute the right first so val of higher val is passed into lower
    Expr* rhs = this->right->subst(st, e);
    //if same for st and left(string var) then update expression
    if (this->left == st) {
        return new Let(this->left, rhs, this->body);
    } else {
        Expr* subsBody = this->body->subst(st, e);
        //creates new expr with new input
        return new Let(left, rhs, body->subst(st, e));
    }
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
    int indentation = os.tellp() - pos;
    //returns current pos of char in the os
    //calculates the number of spaces needed for indentation

    //print _let following by variable and equal sign
    os << "_let " << this->left << " = ";
    //pretty print rhs of the expression
    this->right->pretty_print(os, prec_none, false, pos);
    //prints newline
    os << "\n";
    //get the position after the new line char
    pos = os.tellp();
    //prints _in by spaces for indentation
    os << std::string(indentation, ' ');
    os << "_in  ";
    //pretty print the body
    this->body->pretty_print(os, prec_none, false, pos);

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
//end of Let class implementations

//Beguinning of Bool Expr methods implementation

//public constructor with param boolean v // sets member to that boolean
BoolExpr::BoolExpr(bool v) {
    val = v;
}

bool BoolExpr::equals(const Expr* rhs) const {
    //compares pointer with class of rhs // if BoolExpr then
    if (const BoolExpr* other_bool = dynamic_cast<const BoolExpr*>(rhs)) {
        //returns boolean expression where both have equal val booleans
        return val == other_bool->val;
    }
    //if reached here //no valid pointer
    return false;
}

Val* BoolExpr::interp() const {
    return new BoolVal(val);//returns a bool val expression with same val
}

Expr* BoolExpr::subst(std::string st, Expr *e) const {
    return new BoolExpr(val);//returns this // check with TA
}

void BoolExpr::print(std::ostream& os) const {
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

//public destructor
BoolExpr::~BoolExpr() {
    //BoolExpr only has a boolean as a member
}
//------------end of BoolExpr methods------------------//
//                                                     //
// --- Beginning of IfExpr methods implementation/methods

//public constructor for IfExpr
IfExpr::IfExpr(Expr* test, Expr* then, Expr* else_) {
    test_part = test;
    then_part = then;
    else_part = else_;
}

bool IfExpr::equals(const Expr* other) const {
    //checks for pointer of valid IfExpr in type of class from other
    if (const IfExpr* other_if = dynamic_cast<const IfExpr*>(other)) {
        //returns true only if all 3 Expr are equal and from same class
        return test_part->equals(other_if->test_part) &&
        then_part->equals(other_if->then_part) && else_part->equals(
            other_if->else_part
        );
    }
    //if reached here returns false // other has different class
    return false;
}

Val* IfExpr::interp() const {
    //follows the if (condition) then ___ else ___
    //if test part is true then returns then_part interp else returns else_part
    if (test_part->interp()->is_true()) {
        return then_part->interp();
        //does it need delete for memory leaks?
    }
    return else_part->interp();
}
    
Expr* IfExpr::subst(std::string st, Expr *e) const {
    //recursively calls subst in all 3 expressions within IfExpr
    return (new IfExpr(test_part->subst(st, e), then_part->subst(st, e),
    else_part->subst(st, e)));
}

void IfExpr::print(std::ostream& os) const {
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

//public destructor for IfExpr class
IfExpr::~IfExpr() {
    delete test_part;
    delete then_part;
    delete else_part;
}
//--end of class IfExpr implementation
//
//---Beguinning of class EqExpr implementation

//public constructor that 2 expression as arguments
EqExpr::EqExpr(Expr* left, Expr* right) {
    this->right = right;
    this->left = left;
}

//takes int for lhs and int for rhs
EqExpr::EqExpr(int lhs, int rhs) {
    left = new Num(lhs);
    right = new Num(rhs);
}
//takes a string to create new var in the left and an int num expr for rhs
EqExpr::EqExpr(std::string lhs, int rhs) {
    left = new VarExpr(lhs);
    right = new Num(rhs);
}

bool EqExpr::equals(const Expr* rhs) const {
    //checks for pointer of valid IfExpr in type of class from other
    if (const EqExpr* other = dynamic_cast<const EqExpr*>(rhs)) {
        //returns true only if all 3 Expr are equal and from same class
        return right->equals(other->right) && left->equals(other->left);
    }
    //if reached here returns false // other has different class
    return false;
}

Val* EqExpr::interp() const {
    //check if needs to delete to prevent memory leak
    //returns a boolean comparison result from lhs and rhs
    return new BoolVal(left->interp()->equals(right->interp()));
}

Expr* EqExpr::subst(std::string st, Expr *e) const {
    //calls subst recursively from both lhs and rhs
    return new EqExpr(left->subst(st, e), right->subst(st, e));
    //check if needs to delete for memory leak
}

void EqExpr::print(std::ostream& os) const {
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
//public destructor for EqExpr class
EqExpr::~EqExpr() {
    delete left;
    delete right;
}
//-----end of EqExpr class methods implementation
//-----Beginning of FunExpr class implementation

//public constructor
FunExpr::FunExpr(std::string arg, Expr* expr) {
    this->formal_arg = arg;
    this->body = expr;
}

bool FunExpr::equals(const Expr* rhs) const {
    //checks for pointer of valid FunExpr in type of class from other
    if (const FunExpr* other = dynamic_cast<const FunExpr*>(rhs)) {
        //returns true only equal strings and equal expressions
        return formal_arg == other->formal_arg && body->equals(
            other->body);
    }
    //if reached here returns false // other has different class
    return false;
}

Val* FunExpr::interp() const {
    //this is kind of redunctant I think
    return new FunVal(formal_arg, body);
}

Expr* FunExpr::subst(std::string st, Expr *e) const {
    //if formal_arg is the same as string (var) then returns itself
    //with given expression
    if (formal_arg == st) {
        return new FunExpr(st, e);
    } //else forms a new funExpr where it's body recursively calls subst
    return new FunExpr(formal_arg, body->subst(st, e));
}

void FunExpr::print(std::ostream& os) const {
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

FunExpr::~FunExpr() {
    delete body;
}

//end of FunExpr methods implementation

//Beginning of CallExpr methods implementation
//public constructor
CallExpr::CallExpr(Expr* function, Expr* arg) {
    to_be_called = function;
    actual_arg = arg;
}
bool CallExpr::equals(const Expr* rhs) const {
    //checks for pointer of valid CallExpr in type of class from other
    if (const CallExpr* other = dynamic_cast<const CallExpr*>(rhs)) {
        //returns true only equal strings and equal expressions
        return to_be_called->equals(other->to_be_called) &&
        actual_arg->equals(other->actual_arg);
    }
    //if reached here returns false // other has different class
    return false;
}

Val* CallExpr::interp() const {
    //returns interp called recursively for both expressions
    //and calling call method ---
    return to_be_called->interp()->call(actual_arg->interp());
}

Expr* CallExpr::subst(std::string st, Expr *e) const {
    //returns new call expr where both member's expressions
    //recursively calls subst
    return new CallExpr(to_be_called->subst(st, e),
    actual_arg->subst(st, e));
}

void CallExpr::print(std::ostream& os) const {
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

//public deconstructor
CallExpr::~CallExpr() {
    delete to_be_called;
    delete actual_arg;
}