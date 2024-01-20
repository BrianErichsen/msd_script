#ifndef EXPR_H
#define EXPR_H

#include <iostream>

class Expr {
    public:
    //pure virtual function meaning that any class inheriting must implement
    virtual int eval() const=0;
    //virtual function for equals
    virtual bool equals(const Expr* other) const=0;
    //virtual destructor in the base class of a hierarchy
    virtual ~Expr() {}

    static int interpret(Expr* expr);
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
};
//child class from expr
class Num : public Expr {
    private:
    int value;

    public:
    //constructor
    Num(int val);
    int eval() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
};

class VarExpr : public Expr {
    private:
    std::string varName;

    public:
    VarExpr(const std::string& name);
    int eval() const override;
    bool equals(const Expr* other) const override;
};

class Add : public Expr {
    private:
    Expr* left;
    Expr* right;

    public:
    //constructor
    Add(Expr* l, Expr* r);
    int eval() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    //destructor making sure that the sub expr (left and right)
    //are properly deleted
    ~Add();
};
//Mul from multiply - using the first 3 chars
class Mul : public Expr {
    private:
    Expr* left;
    Expr* right;

    public:
    Mul(Expr* l, Expr* r);
    int eval() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    ~Mul();
};

#endif // EXPR_H