#ifndef EXPR_H
#define EXPR_H

#include <iostream>
typedef enum {
  prec_none,      // = 0
  prec_add,       // = 1
  prec_mult       // = 2
} precedence_t;

class Expr {
    public:
    //pure virtual function meaning that any class inheriting must implement
    virtual int interp() const=0;
    //virtual function for equals
    virtual bool equals(const Expr* other) const=0;
    //checks if given expr input has a variable like x or y
    virtual bool has_variable() const = 0;
    //returns expr with variable in expression
    virtual Expr* subst(std::string st, Expr* e) const=0;
    virtual void print(std::ostream& os) const = 0;
    std::string to_string();
    void pretty_print_at(std::ostream &os);
    virtual void pretty_print(std::ostream &os, precedence_t p) = 0;
    std::string to_pretty_string();
    //virtual destructor in the base class of a hierarchy
    virtual ~Expr() {}

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
    int interp() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p) override;
};

class VarExpr : public Expr {
    private:
    std::string varName;

    public:
    VarExpr(const std::string& name);
    int interp() const override;
    bool equals(const Expr* other) const override;
    bool has_variable() const override;
    const std::string& getVarName() const;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p) override;
};

class Add : public Expr {
    private:
    Expr* left;
    Expr* right;

    public:
    //constructor
    Add(Expr* l, Expr* r);
    int interp() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p) override;
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
    int interp() const override;
    bool equals(const Expr* other) const override;
    static Expr* parseExpr(const std::vector<std::string>& tokens,
    size_t& index);
    bool has_variable() const override;
    Expr* subst(std::string st, Expr *e) const override;
    void print(std::ostream& os) const override;
    void pretty_print(std::ostream &os, precedence_t p) override;
    ~Mul();
};

#endif // EXPR_H