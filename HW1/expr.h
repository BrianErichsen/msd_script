#ifndef EXPR_H
#define EXPR_H

#include <iostream>

class Expr {
    public:
    //pure virtual function meaning that any class inheriting must implement
    virtual int eval() const=0;
    //virtual function for equals
    virtual bool equals(const Expr* other) const=0;
    //checks if given expr input has a variable like x or y
    virtual bool has_variable() const = 0;
    //returns expr with variable in expression
    virtual Expr* subst(std::string st, Expr* e) const=0;
    virtual Expr* clone() const = 0;
    //virtual destructor in the base class of a hierarchy
    virtual ~Expr() {}

    static int interp(Expr* expr);
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
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    Expr* clone() const override;
};

class VarExpr : public Expr {
    private:
    std::string varName;

    public:
    VarExpr(const std::string& name);
    int eval() const override;
    bool equals(const Expr* other) const override;
    bool has_variable() const override;
    const std::string& getVarName() const;
    Expr* subst(std::string st, Expr *e) const override;
    Expr* clone() const override;
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
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    Expr* clone() const override;
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
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    Expr* clone() const override;
    ~Mul();
};

#endif // EXPR_H