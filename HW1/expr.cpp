#include "expr.h"
#include<string>
//first Expr method implementation
int Expr::interpret(Expr* expr) {
    return expr->eval();
}
Expr* Expr::parseExpr(const std::vector<std::string>& tokens,
size_t& index) {
    //skip any white spaces
    //the purpose is to make tokens 1 + 2; from       1     +      2
    while (index < tokens.size() && tokens[index] == " ") {
        ++index;
    }
    std::cout << tokens[0] << " " << tokens[1] << " " << tokens[2];
    std::string token = tokens[index];
    // std::string token = tokens[index++];
    if (token == "+") {
        return Add::parseExpr(tokens, index);
    } else if (token == "*") {
        return Mul::parseExpr(tokens, index);
    } else if (isdigit(token[0])) {
        return new Num(std::stoi(token));
    } else {
        std::cerr << "Error: Invalid token '" << token << "'.\n";
        exit(1);
    }
    return nullptr;
}
//--Num class implementation
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
//---- end of Num class implementation
// Beguinning of VarExpr class implementation
VarExpr::VarExpr(const std::string& name) : varName(name) {}

int VarExpr::eval() const {
    //for now returning 0
    return 0;
}

bool VarExpr::equals(const Expr* other) const {
    if (const VarExpr* otherVarExpr = dynamic_cast
    <const VarExpr*>(other)) {
        return varName == otherVarExpr->varName;
    }
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
        Expr* left = parseExpr(tokens, --index);
        Expr* right = parseExpr(tokens, ++index);
        return new Add(left, right);
    }
Add::~Add() {
    delete left;
    delete right;
}
//end

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
    if (index + 3 < tokens.size()) {
        Expr* left = parseExpr(tokens, --index);
        Expr* right = parseExpr(tokens, ++index);
        return new Mul(left, right);
    } else {
        throw new std::runtime_error("Invalid expression");
    }
}
Mul::~Mul() {
    delete left;
    delete right;
}