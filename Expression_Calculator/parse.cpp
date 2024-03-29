#include <iostream>
#include "parse.h"
#include <stack>

Expr* parse_expr(std::istream &in) {
    //creates an expression to represent lhs
    Expr* lhs = parse_comparg(in);
    skip_whitespace(in);// skip white spaces
    int next = in.peek();// peeks next char from input
    //if + sign then
    if (next == '=') {
        consume(in, '=');// consume the sign
        if (next != '=') {
            std::cerr << "Error: Expected '==' for equality check!";
        }
        consume(in, '=');
        Expr* rhs = parse_expr(in);//then parse the rhs recursively
        //return new Add expr from both parsed data (left and right)
        return new EqExpr(lhs, rhs);
    } else {
        return lhs;
    }
}

Expr* parse(std::istream &in) {
    skip_whitespace(in);// skip white spaces
    //attempts to parse
    Expr* expr = parse_expr(in);
    skip_whitespace(in);// skip white spaces

    //returns the ptr to the parsed expression
    return expr;
}

//parses a numeric expression
Expr* parse_num(std::istream &in) {
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
    return new Num(n);
}

//parses a variable expression
Expr* parse_var(std::istream &in) {
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
    return new VarExpr(var);
}

//Parses a Let expression
Expr* parse_let(std::istream &in) {
    //consumes white spaces
    skip_whitespace(in);
    //defines _let string to be consumed
    std::string _let = "_let";
    consumeWord(in, _let);//consumes the _let portion of it

    //consumes white spaces
    skip_whitespace(in);
    //parses the variable portion of it
    Expr* lhs = parse_var(in);
    skip_whitespace(in); // skip white spaces
    consume(in, '=');// consume the equal sign

    skip_whitespace(in);// skip white spaces
    //creates rhs by parsing it
    Expr* rhs = parse_expr(in);

    //defines string to be consumed
    std::string _in = "_in";
    //consumes _in from input
    consumeWord(in, _in);

    skip_whitespace(in);// skip white spaces
    //parses the body (last piece of it)
    Expr* body = parse(in);

    //creates new Let from parsed data and returns it
    return new class Let(lhs->to_string(), rhs, body);
}

Expr* parse_fun(std::istream &in) {
    skip_whitespace(in); //skip spaces
    consume(in, '('); //takes opening parenthesis out of input stream
    //creates new var expre from parsing the input stream
    Expr* varExpr = parse_var(in);
    consume(in, ')');//takes the closing parenthesis out of input stream
    //creates new expr from parsing the input stream
    Expr* body = parse_expr(in);
    //returns new FunExpr from parsed input using proper arguments
    return new FunExpr(varExpr->to_string(), body);
}

Expr* parse_addend(std::istream &in) {
    //creates expression's lhs
    Expr* expr = parse_multicand(in);
    //consumes all white spaces
    skip_whitespace(in);
    //peeks next char from input
    int next = in.peek();
    //if multiplication
    if (next == '*') {
        consume(in, '*');// then consumes mult sign
        skip_whitespace(in);//skip white spaces
        //creates rhs expr
        Expr* rhs = parse_addend(in);
        //returns new mult with both left and right based on created expr
        return new Mul(expr, rhs);
    } else
        //handle "<expr> <expr> error ---"
        return expr;
}

Expr* parse_multicand(std::istream &in) {
    skip_whitespace(in);
    //parses input stream
    Expr* expr = parse_inner(in);
    while (in.peek() == '(') {//if function then
        consume(in, '(');//consume opening parenthesis
        //parses the actual expression
        Expr* actual_arg = parse_expr(in);
        consume(in, ')');//consume closing parenthesis
        //sets callexpr
        expr = new CallExpr(expr, actual_arg);
    }
    return expr;
    // std::string keyword;
    // skip_whitespace(in);

    // int next = in.peek();
    // //if next is negative or any number then recursively parses it
    // if ((next == '-') || (isdigit(next))) {
    //     return parse_num(in);
    // }
    // //if given parentheses then parses inner expression
    // else if (next == '(') {
    //     //consumes opening parenthesis
    //     consume(in, '(');
    //     //parses given expression after parenthesis
    //     Expr* expr = parse_expr(in);
    //     //skips any next spaces
    //     skip_whitespace(in);
        
    //     // gets next char from input
    //     next = in.get();
    //     //checks for closing brackets and if not throws an error
    //     if (next != ')') {
    //         throw std::runtime_error("Missing closing pararenthesis");
    //     }
    //     //returns new parsed expr
    //     return expr;
    // }
    // //if next is a char -- then calls for parse_var method
    // else if (isalpha(next)) {
    //     return parse_var(in);
    // }
    // //if '_' char -- then calls for parse_let method
    // else if (next == '_') {
    //     consume(in, '_');
    //     keyword = parse_keyword(in);
        
    //     if (keyword == "let") {
    //         return parse_let(in);
    //     }
    //     else if (keyword == "if") {
    //         return parse_if(in);
    //     }
    //     else if (keyword == "true") {
    //         return new BoolExpr(true);
    //     }
    //     else if (keyword == "false") {
    //         return new BoolExpr(false);
    //     }
    //     else if (keyword == "fun") {
    //         return parse_fun(in);
    //     }

    // }
    // else {
    //     consume(in, next);
    //     throw std::runtime_error("Invalid input from multicand parser");
    // }

}//end of parse-multicand method

Expr* parse_inner(std::istream &in) {
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
            return new BoolExpr(true);
        }
        if (keyword == "false") { //if false returns false bool expr
            return new BoolExpr(false);
        }
        //if reached here // no valid options for _ + keyword
        throw std::runtime_error("Invalid keyword" + keyword);
    } else if (c == '(') {//handles extra expressions in parentheses
        consume(in, '(');//consumes paretheses
        Expr* expr = parse_expr(in);//parses input
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

Expr* parse_if(std::istream &in) {
    //try without skipping white spaces --
    skip_whitespace(in);
    // std::string msg = "Wrong format for If Expression";
    //parses the test part
    Expr* test_part = parse_expr(in);
    skip_whitespace(in);//skips white spaces
    //removes the word _then from input
    consumeWord(in, "_then");
    //parses the then_part
    Expr* then_part = parse_expr(in);
    skip_whitespace(in);//skips next whitespaces
    consumeWord(in, "_else");//consumes the word _else from input
    skip_whitespace(in);//skips whitespaces
    //parses the else_part
    Expr* else_part = parse_expr(in);
    //returns new formed ifexpr based on parsed input
    return new IfExpr(test_part, then_part, else_part);
}
// <expr> = <comparg> | <comparg> == <expr>
// <comparg> = <addend> | <addend> + <comparg>
//<addend> = <multicand> | <multicand * <addend>
//<multicand> = <number> | (<expr>) | <variable> |
//_let <variable> = <expr> _in <expr> | _true | _false |
//_if <expr> _then <expr> _else <expr>
Expr* parse_comparg(std::istream &in) {
    //parses the lhs firs by callind parse_addend
    Expr* left = parse_addend(in);
    skip_whitespace(in);//skips white spaces

    int next = in.peek();//peeks at the next char
    if (next == '+') {//if next is an addition then consumes sign
        consume(in, '+');
        //parses the rhs by calling parse_comparg
        Expr* right = parse_comparg(in);
        //returs new addition expression
        return new Add(left, right);
    }//else no addition expr simply returns the lhs
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