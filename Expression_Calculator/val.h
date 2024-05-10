/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
//Spring 2024                                                */

//Val class that implements a value pointer for expressions results

#ifndef VAL_H
#define VAL_H

#include <string>
#include "pointer.h"

class Expr;// expr::interp can refer to val*
class Env;

CLASS(Val) {
public:
    virtual PTR(Expr) to_expr()  = 0;
    virtual PTR(Val) add_to( PTR(Val) rhs)  = 0;
    virtual PTR(Val) mult_with( PTR(Val) rhs)  = 0;
    virtual PTR(Val) div_with(PTR(Val) rhs) = 0;
    virtual std::string to_string()  = 0;
    virtual bool equals(PTR(Val) rhs)  = 0;
    virtual bool is_true()  = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg)  = 0;

    virtual ~Val() {};
};//end of class Val bracket

class NumVal : public Val {
private:
    int val;

public:
    NumVal(int v);//public ructor that takes a val as input
    PTR(Expr) to_expr()  override;
    PTR(Val) add_to( PTR(Val) rsh)  override;
    PTR(Val) mult_with( PTR(Val) rhs)  override;
    PTR(Val) div_with(PTR(Val) rhs) override;
    std::string to_string()  override;
    bool equals(PTR(Val) rhs)  override;
    bool is_true()  override;
    PTR(Val) call(PTR(Val) actual_arg)  override;
};//end of class NumVal bracket

class BoolVal : public Val {
private:
    bool val;
    
public:
    BoolVal(bool v);
    PTR(Expr) to_expr()  override;
    PTR(Val) add_to( PTR(Val) rsh)  override;
    PTR(Val) mult_with( PTR(Val) rhs)  override;
    PTR(Val) div_with(PTR(Val) rhs) override;
    std::string to_string()  override;
    bool equals(PTR(Val) rhs)  override;
    bool is_true()  override;
    PTR(Val) call(PTR(Val) actual_arg)  override;
};

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

public:
    FunVal(std::string arg, PTR(Expr) body_, PTR(Env) env_);
    PTR(Expr) to_expr()  override;
    PTR(Val) add_to( PTR(Val) rsh)  override;
    PTR(Val) mult_with( PTR(Val) rhs)  override;
    PTR(Val) div_with(PTR(Val) rhs) override;
    std::string to_string()  override;
    bool equals(PTR(Val) rhs)  override;
    bool is_true()  override;
    PTR(Val) call(PTR(Val) actual_arg)  override;
};

#endif // VAL_H