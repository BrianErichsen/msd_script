/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
//Spring 2024                                                */

#include <iostream>
#include "parse.h"
#include <stack>
#include <sstream>


PTR(Expr) parse_expr(std::istream &in) {
    //creates an expression to represent lhs
    PTR(Expr) lhs = parse_comparg(in);
    skip_whitespace(in);// skip white spaces
    int next = in.peek();// peeks next char from input
    //if + sign then
    if (next == '=') {
        consume(in, '=');// consume the sign
        if (next != '=') {
            std::cerr << "Error: Expected '==' for equality check!";
        }
        consume(in, '=');
        PTR(Expr) rhs = parse_expr(in);//then parse the rhs recursively
        //return new Add expr from both parsed data (left and right)
        return NEW (EqExpr)(lhs, rhs);
    } else {
        return lhs;
    }
}

PTR(Expr) parse(std::istream &in) {
    skip_whitespace(in);// skip white spaces
    //attempts to parse
    PTR(Expr) expr = parse_expr(in);
    skip_whitespace(in);// skip white spaces
    // if (in.peek() != EOF) {
    //     throw std::runtime_error("bad input");
    // }
    //returns the ptr to the parsed expression
    return expr;
}

//parses a numeric expression
PTR(Expr) parse_num(std::istream &in) {
    int n = 0;
    bool isNegative = false;
    bool isDigitSeen = false;

    //checks for negative sign
    if (in.peek() == '-') {
        isNegative = true;// changes flag to true
        consume(in, '-');// consumes the negative sign
    }
    //loops until all numbers are parsed
    while (true) {
        int next = in.peek();
        //if is digit (numbers) then consume the digit
        if (isdigit(next)) {
            consume(in, next);
            //builds the number
            n = n * 10 + (next - '0');
            isDigitSeen = true;// changes flag to true
        } else
        break;// exit loop if no more digits are found
    }//end of true loop bracket

    //throws error if negative sign is not followed by a digit
    if (isNegative & !isDigitSeen) {
        throw std::runtime_error("Invalid input");
    }
    //changes the sign of the number if given negative sign
    if (isNegative) {
        n = -n;
    }

    //builds a new number based on built number and returns it
    return NEW (Num)(n);
}

//parses a variable expression
PTR(Expr) parse_var(std::istream &in) {
    //creates string to be built on
    std::string var;
    //enters loop and breaks when no more variables are found
    while(true) {
        int next = in.peek();
        if (isalpha(next)) {// if is a char input
            consume(in, next);// consumes the char
            //builds variable
            var += static_cast<char>(next);
        } else {
            break;//no more chars found // break loop
        }
    }//end of while true loop bracket
    //returns new build var expression
    return NEW (VarExpr)(var);
}

//Parses a Let expression
PTR(Expr) parse_let(std::istream &in) {
    //consumes white spaces
    skip_whitespace(in);
    //defines _let string to be consumed
    std::string _let = "_let";
    consumeWord(in, _let);//consumes the _let portion of it

    //consumes white spaces
    skip_whitespace(in);
    //parses the variable portion of it
    PTR(Expr) lhs = parse_var(in);
    skip_whitespace(in); // skip white spaces
    consume(in, '=');// consume the equal sign

    skip_whitespace(in);// skip white spaces
    //creates rhs by parsing it
    PTR(Expr) rhs = parse_expr(in);

    //defines string to be consumed
    std::string _in = "_in";
    //consumes _in from input
    consumeWord(in, _in);

    skip_whitespace(in);// skip white spaces
    //parses the body (last piece of it)
    PTR(Expr) body = parse(in);

    //creates new Let from parsed data and returns it
    return NEW (Let)(lhs->to_string(), rhs, body);
    //return new class Let(lhs->to_string(), rhs, body);
    
}

PTR(Expr) parse_fun(std::istream &in) {
    skip_whitespace(in); //skip spaces
    consume(in, '('); //takes opening parenthesis out of input stream
    //creates new var expre from parsing the input stream
    PTR(Expr) varExpr = parse_var(in);
    consume(in, ')');//takes the closing parenthesis out of input stream
    //creates new expr from parsing the input stream
    PTR(Expr) body = parse_expr(in);
    //returns new FunExpr from parsed input using proper arguments
    return NEW (FunExpr)(varExpr->to_string(), body);
}

PTR(Expr) parse_addend(std::istream &in) {
    //creates expression's lhs
    PTR(Expr) expr = parse_multicand(in);
    //consumes all white spaces
    skip_whitespace(in);
    //peeks next char from input
    int next = in.peek();
    //if multiplication
    if (next == '*') {
        consume(in, '*');// then consumes mult sign
        skip_whitespace(in);//skip white spaces
        //creates rhs expr
        PTR(Expr) rhs = parse_addend(in);
        //returns new mult with both left and right based on created expr
        return NEW (Mul)(expr, rhs);
    } else if (next == '/') {
        consume(in, '/');
        skip_whitespace(in);
        PTR(Expr) rhs = parse_addend(in);
        return NEW (Div) (expr, rhs);
    } else
        //handle "<expr> <expr> error ---"
        return expr;
}

PTR(Expr) parse_multicand(std::istream &in) {
    skip_whitespace(in);
    //parses input stream
    PTR(Expr) expr = parse_inner(in);
    while (in.peek() == '(') {//if function then
        consume(in, '(');//consume opening parenthesis
        //parses the actual expression
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');//consume closing parenthesis
        //sets callexpr
        expr = NEW (CallExpr)(expr, actual_arg);
    }
    return expr;
}//end of parse-multicand method

PTR(Expr) parse_inner(std::istream &in) {
    skip_whitespace(in);//skips the proper white spaces
    //peeks at the next char ... if ...
    int c = in.peek();
    if (c == '-' || isdigit(c)) { //is negative or any digit then parse num
        return parse_num(in);
    } else if (isalpha(c)) {//if is variables then parse var
        return parse_var(in);
    } else if (c == '_') { //if underscore then either
    //let, if, fun, true or false
        consume(in, c);
        std::string keyword = parse_keyword(in);
        if (keyword == "let") { //if let returns parse let
            return parse_let(in);
        }
        if (keyword == "if") { //if if returns parse if
            return parse_if(in);
        }
        if (keyword == "fun") { //if fun returns parse fun
            return parse_fun(in);
        }
        if (keyword == "true") { //if true returns true bool expr
            return NEW (BoolExpr)(true);
        }
        if (keyword == "false") { //if false returns false bool expr
            return NEW (BoolExpr)(false);
        }
        //if reached here // no valid options for _ + keyword
        throw std::runtime_error("Invalid keyword" + keyword);
    } else if (c == '(') {//handles extra expressions in parentheses
        consume(in, '(');//consumes paretheses
        PTR(Expr) expr = parse_expr(in);//parses input
        consume(in, ')');//consumes external parenthesis
        return expr;//returns new parsed expr
    } else {
        consume(in, c);
        throw std::runtime_error("Invalid input from multicand parser!");
    }
}//end of parse inner method bracket

//helper method to skip white spaces
static void skip_whitespace(std::istream &in) {
    //enters loop until all spaces are consumed
    while (true) {
        //peek next char without consuming it
        int next = in.peek();
        //if different than space then break the loop
        if (!isspace(next)) {
            break;
        }
        //consumes white space
        consume(in, next);
    }
}

//helper method that works with consume white spaces
static void consume(std::istream &in, int expect) {
    //gets the next char from the stream
    int next = in.get();
    //if different then throws runtime error
    if (next != expect) {
        throw std::runtime_error("Mismatch from consume method");
    }
}

//helper method to consume specific words in given input
static void consumeWord(std::istream &in, std::string word) {
    //iterates through each char in the given word
    for (char character : word) {
        int next = in.peek();
        if (next == character) {
            //consume the char if it matches
            consume(in, next);
        }
    }
}

PTR(Expr) parse_if(std::istream &in) {
    //try without skipping white spaces --
    skip_whitespace(in);
    // std::string msg = "Wrong format for If Expression";
    //parses the test part
    PTR(Expr) test_part = parse_expr(in);
    skip_whitespace(in);//skips white spaces
    //removes the word _then from input
    consumeWord(in, "_then");
    //parses the then_part
    PTR(Expr) then_part = parse_expr(in);
    skip_whitespace(in);//skips next whitespaces
    consumeWord(in, "_else");//consumes the word _else from input
    skip_whitespace(in);//skips whitespaces
    //parses the else_part
    PTR(Expr) else_part = parse_expr(in);
    //returns new formed ifexpr based on parsed input
    return NEW (IfExpr)(test_part, then_part, else_part);
}
// <expr> = <comparg> | <comparg> == <expr>
// <comparg> = <addend> | <addend> + <comparg>
//<addend> = <multicand> | <multicand * <addend>
//<multicand> = <number> | (<expr>) | <variable> |
//_let <variable> = <expr> _in <expr> | _true | _false |
//_if <expr> _then <expr> _else <expr>
PTR(Expr) parse_comparg(std::istream &in) {
    //parses the lhs firs by callind parse_addend
    PTR(Expr) left = parse_addend(in);
    skip_whitespace(in);//skips white spaces

    int next = in.peek();//peeks at the next char
    if (next == '+') {//if next is an addition then consumes sign
        consume(in, '+');
        //parses the rhs by calling parse_comparg
        PTR(Expr) right = parse_comparg(in);
        //returs new addition expression
        return NEW (Add)(left, right);
    } else if(next == '-') {
        consume(in, '-');
        PTR(Expr) right = parse_comparg(in);
        return NEW (Subt)(left, right);
    }
    //else no addition expr simply returns the lhs
    return left;
}
//helper method to check for let, fun, if expressions
std::string parse_keyword(std::istream &in) {
    std::string keyword;//string to be built in
    
    while (true) {
        int ch = in.peek();//sees the next char/int
        if (isalpha(ch)) {//if it is alpha(letters)
            consume(in, ch);//removes it from input and
            //builds new keyword
            char c = ch;
            keyword += c;
        } else
        break;//end of letters then breaks while loop
    }
    return keyword;//returns new formed keyword
}
//helper method for using qt tool
PTR(Expr) parse_string(std::string s) {
    std::istringstream in(s);
    return parse_expr(in);
}