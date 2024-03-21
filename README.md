This program takes 1 or more algebraic expressions as an input and computes the final result of a given expression!
To run it; choose the following modes:
--help for instructions
--test to test all case bases
--interp to compute expression final result
--print to print a string version of given expression
--pretty_print for a prettier printed version of expression

Grammar:
<Expr> = <number> | <boolean> | <expr> == <expr> | <expr> + <expr> |
<expr> * <expr> | <expr> (<expr>) | <variable> | _let <variable> =
<expr> _in <expr> | _if <expr> _then <expr> _else <expr> |
_fun (<variable>) <expr>

Example using a _fun expression:
  _let factrl = _fun (factrl)
                _fun (x)
                  _if x == 1
                  _then 1
                  _else x * factrl(factrl)(x + -1)
_in  factrl(factrl)(10) --> 3628800;

or a simpler _fun example:
  _let f = _fun (x) x + 1 _in f(10) --> 11;

or (_fun (x) x + 1)(24) --> 25;
How to run the program?
  Use the make file by typying make msdscript and run the program by ./msdscript [your argument]

  if in interp mode:
  input your expression and exit input by crt + d; then the program returns the value of given expression as input.
  Example: ./msdscript --interp
2 * 18888 + _let x = 10 _in x * 88 //insert any expression
38656 //this is the output of the program --calculates final expression

  if in print mode:
  input your expression and exit input by crt + d; then the program returns a printed string version of given expression as input

  if in pretty_print mode:
    input your expression and exit input by crt + d; then the program returns a prettier printed string version of given expression as an  input