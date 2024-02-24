#ifndef EXPR_H
#define EXPR_H

#include <iostream>
/**
 * \brief Enumeration to represent different precedences for pretty printing.
 */
typedef enum {
  prec_none = 0,
  prec_add = 1,
  prec_mult = 2,
} precedence_t;
/**
 * \brief Base class for representing expressions.
 */
class Expr {
    public:
    /**
     * \brief Evaluates the expression.
     * \return Result of the expression.
     */
    virtual int interp() const=0;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    virtual bool equals(const Expr* other) const=0;
    /**
     * \brief Checks if the expression has a variable.
     * \return True if the expression contains a variable, false otherwise.
     */
    virtual bool has_variable() const = 0;
    /**
     * \brief Substitutes a variable in the expression with another expression.
     * \param st Variable name to be substituted.
     * \param e Expression to substitute for the variable.
     * \return New expression after substitution.
     */
    virtual Expr* subst(std::string st, Expr* e) const=0;
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
};
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
    int interp() const override;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    /**
     * \brief Checks if the expression has a variable.
     * \return False since numeric constants do not have variables.
     */
    bool has_variable() const override;
    /**
     * \brief Substitutes a variable in the expression with another expression.
     * \param st Variable name to be substituted.
     * \param e Expression to substitute for the variable.
     * \return New expression after substitution (which is always the same as the original Num).
     */
    Expr* subst(std::string st, Expr *e) const override;
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
    int interp() const override;
    /**
     * \brief Checks if the expression is equal to another expression.
     * \param other Another expression for comparison.
     * \return True if expressions are equal, false otherwise.
     */
    bool equals(const Expr* other) const override;
    /**
     * \brief Checks if the expression has a variable.
     * \return True since VarExpr is a variable expression.
     */
    bool has_variable() const override;
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
    Expr* subst(std::string st, Expr *e) const override;
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
    Expr* left;///< Left operand of the addition.
    Expr* right;///< Right operand of the addition.

    public:
    //constructor
    Add(Expr* l, Expr* r);
    int interp() const override;
    bool equals(const Expr* other) const override;
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
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
    Expr* left;///< Left operand of the multiplication.
    Expr* right;///< Right operand of the addition.

    public:
    Mul(Expr* l, Expr* r);
    int interp() const override;
    bool equals(const Expr* other) const override;
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
    ~Mul();
};
//
class Let : public Expr {
    private:
    std::string left; //variable name
    Expr* right;    //binding Expression
    Expr* body;     //body Expression
    
    public:
    //default constructor
    Let(std::string left, Expr* right, Expr* body);
    int interp() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p, bool let_needs_parenthesesis, std::streampos &pos) override;
    ~Let();
};

#endif // EXPR_H