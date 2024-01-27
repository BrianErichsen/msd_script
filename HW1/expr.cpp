#include <stdexcept>
#include "expr.h"
#include<string>
//first Expr method implementation
int Expr::interp(Expr* expr) {
    //goes into if eval returns a non 0 value
    if (expr->eval()) {
        return expr->eval();
    } else {
        std::runtime_error("Expression has to have a value to be interpreted!");
    }
}
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
int Num::eval() const {
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
//---- end of Num class implementation
// Beguinning of VarExpr class implementation
VarExpr::VarExpr(const std::string& name) : varName(name) {}

int VarExpr::eval() const {
    //for now / returning that string digits to int
    if (isdigit(this->varName[0])) {
        return stoi(varName);
    } else
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
    //iterate through it's expression length
    for (int i = 0; i < varName.size(); i++) {
        //checks from l to r is any char is any character
        if (isalpha(varName[i])) {
            return true;
        }
    }
    //if reached here - no chars encountered in the expression
    return false;
}

//-end
// Beginning of Add class
Add::Add(Expr* l, Expr* r) : left(l), right(r) {}

int Add::eval() const {
    return left->eval() + right->eval();
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
    //if lhs expr same is VarExpr or rhs
    return dynamic_cast<const VarExpr*>(left) ||
    dynamic_cast<const VarExpr*>(right);
}
Add::~Add() {
    delete left;
    delete right;
}
//end of Add class implementation

//Beginning of Multiplication class implementation
Mul::Mul(Expr* l, Expr* r) : left(l), right(r) {}

int Mul::eval() const {
    return left->eval() * right->eval();
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
    return dynamic_cast<const VarExpr*>(left) ||
    dynamic_cast<const VarExpr*>(right);
}
Mul::~Mul() {
    delete left;
    delete right;
}