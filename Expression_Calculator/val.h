//Author: Brian Erichsen Fagundes
//MSD - UofU - Spring semester
//Val class that implements a value pointer for expressions results

#ifndef VAL_H
#define VAL_H

#include <string>

class Expr;// expr::interp can refer to val*

class Val {
public:
    virtual Expr* to_expr() const = 0;
    virtual Val* add_to(const Val* rhs) const = 0;
    virtual Val* mult_with(const Val* rhs) const = 0;
    virtual std::string to_string() const = 0;
    virtual bool equals(Val* rhs) const = 0;
    virtual bool is_true() const = 0;
    virtual Val* call(Val* actual_arg) const = 0;

    virtual ~Val() {};
};//end of class Val bracket

class NumVal : public Val {
private:
    int val;

public:
    NumVal(int v);//public constructor that takes a val as input
    Expr* to_expr() const override;
    Val* add_to(const Val* rsh) const override;
    Val* mult_with(const Val* rhs) const override;
    std::string to_string() const override;
    bool equals(Val* rhs) const override;
    bool is_true() const override;
    Val* call(Val* actual_arg) const override;
    ~NumVal();
};//end of class NumVal bracket

class BoolVal : public Val {
private:
    bool val;
    
public:
    BoolVal(bool v);
    Expr* to_expr() const override;
    Val* add_to(const Val* rsh) const override;
    Val* mult_with(const Val* rhs) const override;
    std::string to_string() const override;
    bool equals(Val* rhs) const override;
    bool is_true() const override;
    Val* call(Val* actual_arg) const override;
    ~BoolVal();
};

class FunVal : public Val {
private:
    std::string formal_arg;
    Expr* body;

public:
    FunVal(std::string arg, Expr* body);
    Expr* to_expr() const override;
    Val* add_to(const Val* rsh) const override;
    Val* mult_with(const Val* rhs) const override;
    std::string to_string() const override;
    bool equals(Val* rhs) const override;
    bool is_true() const override;
    Val* call(Val* actual_arg) const override;
    ~FunVal();
};

#endif // VAL_H