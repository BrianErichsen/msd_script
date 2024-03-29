/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#ifndef EXPR_H
#define EXPR_H

#include <iostream>
#include "pointer.h"
/**
 * \brief Enumeration to represent different precedences for pretty printing.
 */
typedef enum {
  prec_none = 0,
  prec_equal = 1,
  prec_add = 2,
  prec_mult = 3,
} precedence_t;
/**
 * \brief Base class for representing expressions.
 */

class Val;// expr::interp can refer to val*

CLASS(Expr) {
    public:
    /**
     * \brief Evaluates the expression.
     * \return Result of the expression.
     */
    virtual PTR(Val) interp() const=0;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    virtual bool equals(const PTR(Expr) other) const=0;
    /**
     * \brief Substitutes a variable in the expression with another expression.
     * \param st Variable name to be substituted.
     * \param e Expression to substitute for the variable.
     * \return New expression after substitution.
     */
    virtual PTR(Expr) subst(std::string st, PTR(Expr) e) const=0;
    /**
     * \brief Prints the expression to the output stream.
     * \param os Output stream to print the expression.
     */
    virtual void print(std::ostream& os) const = 0;
    /**
     * \brief Converts the expression to a string.
     * \return String representation of the expression.
     */
    std::string to_string();
    void pretty_print_at(std::ostream &os);
    /**
     * \brief Pretty prints the expression to the output stream.
     * \param os Output stream to pretty print the expression.
     * \param p Precedence level for pretty printing.
     */
    virtual void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) = 0;
    /**
     * \brief Converts the expression to a pretty string.
     * \return Pretty string representation of the expression.
     */
    std::string to_pretty_string();
    //virtual destructor in the base class of a hierarchy
    virtual ~Expr() {}
};//end of Expr class bracket
/**
 * \brief Represents a numeric constant expression.
 */
class Num : public Expr {
    private:
    int value;///< Numeric value of the expression.


    public:
    //constructor
    Num(int val);
    /**
     * \brief Evaluates the numeric constant expression.
     * \return Numeric value of the expression.
     */
    PTR(Val) interp() const override;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    bool equals(const PTR(Expr) other) const override;
    /**
     * \brief Substitutes a variable in the expression with another expression.
     * \param st Variable name to be substituted.
     * \param e Expression to substitute for the variable.
     * \return New expression after substitution (which is always the same as the original Num).
     */
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    /**
     * \brief Prints the numeric constant expression to the output stream.
     * \param os Output stream to print the expression.
     */
    void print(std::ostream& os) const override;
    /**
     * \brief Pretty prints the numeric constant expression to the output stream.
     * \param os Output stream to pretty print the expression.
     * \param p Precedence level for pretty printing.
     */
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
};
/**
 * \brief Represents a variable expression.
 */
class VarExpr : public Expr {
    private:
    std::string varName;///< Name of the variable.

    public:
    VarExpr(const std::string& name);
    /**
     * \brief Evaluates the variable expression (throws an error since variables have no fixed value).
     * \return None since variables have no fixed value.
     */
    PTR(Val) interp() const override;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    bool equals(const PTR(Expr) other) const override;
    /**
     * \brief Gets the name of the variable.
     * \return Name of the variable.
     */
    const std::string& getVarName() const;
    /**
     * \brief Substitutes a variable in the expression with another expression.
     * \param st Variable name to be substituted.
     * \param e Expression to substitute for the variable.
     * \return New expression after substitution.
     */
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    /**
     * \brief Prints the variable expression to the output stream.
     * \param os Output stream to print the expression.
     */
    void print(std::ostream& os) const override;
    /**
     * \brief Pretty prints the variable expression to the output stream.
     * \param os Output stream to pretty print the expression.
     * \param p Precedence level for pretty printing.
     */
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
};
/**
 * \brief Represents an addition expression.
 */
class Add : public Expr {
    private:
    PTR(Expr) left;///< Left operand of the addition.
    PTR(Expr) right;///< Right operand of the addition.

    public:
    //constructor
    Add(PTR(Expr) l, PTR(Expr) r);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
    //destructor making sure that the sub expr (left and right)
    //are properly deleted
    ~Add();
};
/**
 * \brief Represents an multiplication expression.
 */
class Mul : public Expr {
    private:
    PTR(Expr) left;///< Left operand of the multiplication.
    PTR(Expr) right;///< Right operand of the addition.

    public:
    Mul(PTR(Expr) l, PTR(Expr) r);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
    ~Mul();
};
//
class Let : public Expr {
    private:
    std::string left; //variable name
    PTR(Expr) right;    //binding Expression
    PTR(Expr) body;     //body Expression
    
    public:
    //default constructor
    Let(std::string left, Expr* right, Expr* body);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
    ~Let();
};//end of let class bracket

/**
 * \brief sub class for representing Boolean expressions.
 */
class BoolExpr : public Expr {
private:
    bool val;

public:
    BoolExpr(bool v);//public constructor
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    Expr* subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) override;
    ~BoolExpr();
};//end of BoolExpr bracket

class IfExpr : public Expr {
    private:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    public:
    IfExpr(Expr* test, Expr* then, Expr* else_);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    Expr* subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) override;
    ~IfExpr();
};//end of class IfExpr bracket

class EqExpr : public Expr {
private:
    PTR(Expr) left;
    PTR(Expr) right;

public:
    EqExpr(PTR(Expr) left, PTR(Expr) right);
    EqExpr(int lhs, int rhs);
    EqExpr(std::string lhs, int rhs);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) override;
    ~EqExpr();
};//end of EqExpr bracket

class FunExpr : public Expr {
private:
    std::string formal_arg;
    PTR(Expr) body;

public:
    FunExpr(std::string arg, PTR(Expr) expr);
    PTR(Val) interp() const override;
    bool equals(const PTR(Expr) other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) override;
    ~FunExpr();
};

class CallExpr : public Expr {
private:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

public:
    CallExpr(PTR(Expr) function, PTR(Expr) arg);
    PTR(Val) interp() const override;
    bool equals(const Expr* other) const override;
    PTR(Expr) subst(std::string st, PTR(Expr) e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p,
    bool let_needs_parenthesesis, std::streampos &pos) override;
    ~CallExpr();
};

#endif // EXPR_H