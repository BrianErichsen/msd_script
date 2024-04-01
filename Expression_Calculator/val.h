//Author: Brian Erichsen Fagundes
//MSD - UofU - Spring semester
//Val class that implements a value pointer for expressions results

#ifndef VAL_H
#define VAL_H

#include <string>
#include "pointer.h"

class Expr;// expr::interp can refer to val*

CLASS(Val) {
public:
    virtual PTR(Expr) to_expr() const = 0;
    virtual PTR(Val) add_to(const PTR(Val) rhs) const = 0;
    virtual PTR(Val) mult_with(const PTR(Val) rhs) const = 0;
    virtual std::string to_string() const = 0;
    virtual bool equals(PTR(Val) rhs) const = 0;
    virtual bool is_true() const = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) const = 0;

    virtual ~Val() {};
};//end of class Val bracket

class NumVal : public Val {
private:
    int val;

public:
    NumVal(int v);//public constructor that takes a val as input
    PTR(Expr) to_expr() const override;
    PTR(Val) add_to(const PTR(Val) rsh) const override;
    PTR(Val) mult_with(const PTR(Val) rhs) const override;
    std::string to_string() const override;
    bool equals(PTR(Val) rhs) const override;
    bool is_true() const override;
    PTR(Val) call(PTR(Val) actual_arg) const override;
    // ~NumVal();
};//end of class NumVal bracket

class BoolVal : public Val {
private:
    bool val;
    
public:
    BoolVal(bool v);
    PTR(Expr) to_expr() const override;
    PTR(Val) add_to(const PTR(Val) rsh) const override;
    PTR(Val) mult_with(const PTR(Val) rhs) const override;
    std::string to_string() const override;
    bool equals(PTR(Val) rhs) const override;
    bool is_true() const override;
    PTR(Val) call(PTR(Val) actual_arg) const override;
    // ~BoolVal();
};

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;

public:
    FunVal(std::string arg, PTR(Expr) body);
    PTR(Expr) to_expr() const override;
    PTR(Val) add_to(const PTR(Val) rsh) const override;
    PTR(Val) mult_with(const PTR(Val) rhs) const override;
    std::string to_string() const override;
    bool equals(PTR(Val) rhs) const override;
    bool is_true() const override;
    PTR(Val) call(PTR(Val) actual_arg) const override;
    // ~FunVal();
};

#endif // VAL_H