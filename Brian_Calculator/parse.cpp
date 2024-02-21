#include <iostream>
#include "parse.h"
#include <stack>

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
        return expr;
}

Expr* parse_multicand(std::istream &in) {
    // bool isBalanced = isValid(in);
    // if (!isBalanced) {
    //     throw std::runtime_error("Missing closing pararenthesis");
    // }
    //consumes all white spaces
    skip_whitespace(in);

    int next = in.peek();
    //if next is negative or any number then recursively parses it
    if ((next == '-') || (isdigit(next))) {
        return parse_num(in);
    }
    //if given parentheses then parses inner expression
    else if (next == '(') {
        //consumes opening parenthesis
        consume(in, '(');
        //parses given expression after parenthesis
        Expr* expr = parse_expr(in);
        //skips any next spaces
        skip_whitespace(in);
        
        // gets next char from input
        next = in.get();
        //checks for closing brackets and if not throws an error
        if (next != ')') {
            throw std::runtime_error("Missing closing pararenthesis");
        }
        //returns new parsed expr
        return expr;
    }
    //if next is a char -- then calls for parse_var method
    else if (isalpha(next)) {
        return parse_var(in);
    }
    //if '_' char -- then calls for parse_let method
    else if (next == '_') {
        return parse_var(in);
    }
    else {
        consume(in, next);
        throw std::runtime_error("Invalid input");
    }
}

static Expr* parse_expr(std::istream &in) {
    //creates an expression to represent lhs
    Expr* expr = parse_addend(in);
    skip_whitespace(in);// skip white spaces
    int next = in.peek();// peeks next char from input
    //if + sign then
    if (next == '+') {
        consume(in, '+');// consume the sign
        Expr* rhs = parse_expr(in);//then parse the rhs recursively
        //return new Add expr from both parsed data (left and right)
        return new Add(expr, rhs);
    } else {
        return expr;
    }
}

Expr* parse(std::istream &in) {
    //attempts to parse
    Expr* expr = parse_expr(in);
    skip_whitespace(in);// skip white spaces

    //if different that end of input at this point
    if (!in.eof()) {
        //throws an error for any non white spaces remaining
        throw std::runtime_error("Invalid input");
    }
    //returns the ptr to the parsed expression
    return expr;
}

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

//helper method to see if given input is valid or not
bool isValid(std::istream& input) {
    //creates a copy of given input instream
    std::istream clone(input.rdbuf());
    std::stack<char> stack;

    char currentChar;
    while (clone.get(currentChar)) {
        if (currentChar == '(' || currentChar == '{' || currentChar == '[') {
            stack.push(currentChar);
        } else if (currentChar == ')' || currentChar == '}' || currentChar == ']') {
            if (!stack.empty() && isMatchingPairs(stack.top(), currentChar)) {
                stack.pop();
            } else {
                return false;
            }
        }
    }

    // If the stack is not empty, the expression is not balanced
    return stack.empty();
}

bool isMatchingPairs(char open, char close) {
    return (open == '(' && close == ')') || (open == '{' && close == '}') ||
            (open == '[' && close == ']');
}