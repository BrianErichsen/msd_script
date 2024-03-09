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
    //
    virtual int to_int() const = 0;

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
    int to_int() const override;
};//end of class NumVal bracket

#endif // VAL_H