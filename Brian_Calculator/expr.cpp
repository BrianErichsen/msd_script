#include <stdexcept>
#include "expr.h"
#include<string>
#include <sstream>

//first Expr method implementation
Expr* Expr::parseExpr(const std::vector<std::string>& tokens,
size_t& index) {
    //skip any white spaces
    //the purpose is to make tokens 1 + 2; from       1     +      2
    while (index < tokens.size() && tokens[index] == " ") {
        ++index;
    }
    std::string token = tokens[index + 1];
    // std::string token = tokens[index++];
    if (token == "+") {
        std::cout << "Hello from Add parser" << std::endl;
        return Add::parseExpr(tokens, index);
    } else if (token == "*") {
        std::cout << "Hello from Mult parser" << std::endl;
        return Mul::parseExpr(tokens, index);
    } else if (isdigit(token[0])) {
        std::cout << "Hello from Num parser" << std::endl;
        return new Num(std::stoi(token));
    } else {
        std::cerr << "Error: Invalid token '" << token << "'.\n";
        exit(1);
    }
    return nullptr;
}
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}
void Expr::pretty_print_at(std::ostream &os) {
    this->pretty_print(os, prec_none);
}
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
int Num::interp() const {
    //just return value
    return value;
}

Expr* Num::parseExpr(const std::vector<std::string>& tokens,
size_t& index) {
    if (index < tokens.size()) {
        std::string token = tokens[index++];
        if (isdigit(token[0])) {
            return new Num(std::stoi(token));
    } else {
        throw new std::runtime_error("Invalid expression from Num-Parser1");
    }
    } else {
        throw std::runtime_error("Invalid expression from Num-Parser2");
    }
}
bool Num::has_variable() const {
    //numbers are always not variables
    return false;
}
Expr* Num::subst(std::string st, Expr* e) const {
    //Numbers are always it's own value - nothing more
    return new Num(value);
}
Expr* Num::clone() const {
    return new Num(value);
}
void Num::print(std::ostream& os) const {
    os << value;
}
void Num::pretty_print(std::ostream& os, precedence_t p) {
    os << this->value;
}
//---- end of Num class implementation


// Beguinning of VarExpr class implementation
VarExpr::VarExpr(const std::string& name) : varName(name) {}

int VarExpr::interp() const {
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
        return e->clone();
    }
}
Expr* VarExpr::clone() const {
    return new VarExpr(varName);
}

void VarExpr::print(std::ostream& os) const {
    os << varName;
}
void VarExpr::pretty_print(std::ostream& os, precedence_t p) {
    os << this->varName;
}
//-end of VarExpr class implementation
// Beginning of Add class
Add::Add(Expr* l, Expr* r) : left(l), right(r) {}

int Add::interp() const {
    return left->interp() + right->interp();
}
bool Add::equals(const Expr* other) const {
    //first check for a valid pointer to see if classes match
    if (const Add* otherAdd = dynamic_cast<const Add*>(other)) {
        //recursively compares other left with current left and same for right
        return left->equals(otherAdd->left) && right->equals(otherAdd->right);
    }
    return false;
}
Expr* Add::parseExpr(const std::vector<std::string>& tokens,
    size_t& index) {
        Expr* left = new Num(stoi(tokens[0]));
        // Expr* left = parseExpr(tokens, ++index); - recursive approach
        Expr* right = new Num(stoi(tokens[2]));
        return new Add(left, right);
    }
bool Add::has_variable() const {
    //if lhs expr same is VarExpr or rhs // recursive -- approach
    return left->has_variable() || right->has_variable();
}
Expr* Add::subst(std::string st, Expr *e) const {
    //recursive finds nums and variables and assigns to l & r
    return new Add(left->subst(st, e), right->subst(st, e));
}
//recursively creates a new object from either var or num in both l & r
Expr* Add::clone() const {
    return new Add(left->clone(), right->clone());
}
void Add::print(std::ostream& os) const {
    os << "(";
    left->print(os);
    os << " + ";
    right->print(os);
    os << ")";
}
void Add::pretty_print(std::ostream &os, precedence_t p) {
    if (p > prec_add) {
        os << "(";
    }
    this->left->pretty_print(os, prec_add);
    os << " + ";
    this->right->pretty_print(os, prec_add);
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

int Mul::interp() const {
    return left->interp() * right->interp();
}

bool Mul::equals(const Expr* other) const {
    if (const Mul* otherMul = dynamic_cast<const Mul*>(other)) {
        return left->equals(otherMul->left) && right->equals
        (otherMul->right);
    }
    return false;
}

Expr* Mul::parseExpr(const std::vector<std::string>& tokens,
size_t& index) {
    Expr* left = new Num(stoi(tokens[0]));
    Expr* right = new Num(stoi(tokens[2]));
    return new Mul(left, right);
}
bool Mul::has_variable() const {
    //only returns instances of VarExpressions its lhs and rhs
    return left->has_variable() || right->has_variable();
}
Expr* Mul::subst(std::string st, Expr *e) const {
    //recursive finds nums and variables and assigns to l & r
    return new Mul(left->subst(st, e), right->subst(st, e));
}
Expr* Mul::clone() const {
    return new Mul(left->clone(), right->clone());
}
void Mul::print(std::ostream& os) const {
    os << "(";
    left->print(os);
    os << " * ";
    right->print(os);
    os << ")";
}
void Mul::pretty_print(std::ostream &os, precedence_t p) {
    //if precendence is higher than prec of mult then we need '('
    if (p > prec_mult) {
        os << "(";
    }
    //left operand printed with higher precedence to ensure nested mult is
    //enclosed
    this->left->pretty_print(os, static_cast<precedence_t>(prec_mult + 1));
    os << " * ";
    this->right->pretty_print(os, prec_mult);
    if (p > prec_mult) {
        os << ")";
    }
}
Mul::~Mul() {
    delete left;
    delete right;
}