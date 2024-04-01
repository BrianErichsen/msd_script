/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

// #define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "test.h"
#include <iostream>
#include <sstream>
#include <limits>
#include "val.h"
#include <string>

TEST_CASE("do nothing") {
  std::cout << "test passed!" << std::endl;
}

TEST_CASE("Equals method tests") {
    SECTION("Expr equals") {
        //VarExpr class
        CHECK((new VarExpr("x"))->equals(new VarExpr("x")) == true);
        CHECK((new VarExpr("x"))->equals(new VarExpr("y")) == false);
        //different classes
        CHECK((new Num(1))->equals(new VarExpr("x")) == false );
        //Add class
        CHECK((new Add(new Num(2),new Num(3)))->equals
        (new Add(new Num(2),new Num(3)))==true);
        //int max / min and negative numbers
        CHECK((new Add(new Num(2),new Num(3)))->equals
        (new Add(new Num(3),new Num(2)))==false);

        CHECK((new Add(new Num(-1),new Num(3)))->equals
        (new Add(new Num(3),new Num(2)))==false);
        //checking for max num added into num
        int max = INT_MAX;
        Num* num1 = new Num(max);
        CHECK((new Add(num1,new Num(-1)))->equals
        (new Add(new Num(2147483647),new Num(-1)))==true);
        //check for proper value
        CHECK((new Num(2))->interp()->equals(new NumVal(2)));
        //check if the result of the 2 != expressions are correct
        // CHECK((new Add(new Num(2), new Num(3)))->interp() ==
        // (new Mul(new Num(1), new Num(5)))->interp());
        //still working how to test this

        //Mul class
        CHECK((new Mul(new Num(2),new Num(3)))->equals
        (new Mul(new Num(6),new Num(1)))==false);

        CHECK((new Mul(new Num(2),new Num(3)))->equals
        (new Mul(new Num(2),new Num(3)))==true);

        CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );
    
    }//end of section
    SECTION("bool has_variables and other testing") {
        CHECK( (new Mul(new Num(3), new Num(2)))
        ->interp()->equals(new NumVal(6)));
    }
    SECTION("Subst method test case") {
        CHECK( (new Add(new VarExpr("x"), new Num(7)))
        ->subst("x", new VarExpr("y"))
        ->equals(new Add(new VarExpr("y"), new Num(7))) );
        CHECK( (new VarExpr("x"))
        ->subst("x", new Add(new VarExpr("y"),new Num(7)))
        ->equals(new Add(new VarExpr("y"),new Num(7))) );
        CHECK( (new VarExpr("x"))
        ->subst("x", new Mul(new VarExpr("y"),new Num(7)))
        ->equals(new Mul(new VarExpr("y"),new Num(7))) );
        CHECK( (new VarExpr("y"))
        ->subst("y", new Mul(new VarExpr("x"),new Num(-12)))
        ->equals(new Mul(new VarExpr("x"),new Num(-12))) );
    }
    SECTION("Printing methods test cases") {
        CHECK( (new Num(10))->to_string() == "10" );
        CHECK ( (new Mul(new Num(1), new Add(new Num(2),
        new Num(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
        CHECK ( (new Mul(new Mul(new Num(8), new Num(1)),
        new VarExpr("y")))->to_pretty_string() ==  "(8 * 1) * y" );
        CHECK ( (new Mul(new Add(new Num(3), new Num(5)),
        new Mul(new Num(6), new Num(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
        CHECK ( (new Mul(new Mul(new Num(7), new Num(7)),
        new Add(new Num(9), new Num(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
        Add *addPrintExp = new Add(new Num(-1), new Add(new Num(-2), new Num(-3)));
        std::string exp = "(-1+(-2+-3))";
        CHECK( addPrintExp->to_string() == exp );
        // Check simple Num printing
        CHECK((new Num(42))->to_string() == "42");
        // Check VarExpr printing
        CHECK((new VarExpr("x"))->to_string() == "x");
        CHECK((new Add(new Num(1), new Num(2)))->to_string() == "(1+2)");
        CHECK((new Add(new Num(1), new VarExpr("x")))->to_pretty_string() == "1 + x");
        CHECK((new Mul(new Num(3), new VarExpr("y")))->to_pretty_string() == "3 * y");
        CHECK( (new Add(new Add(new VarExpr("x"), new VarExpr("y")),
        new Num(1)))->to_pretty_string() == "(x + y) + 1" );
        CHECK( (new Add(new Add(new Num(-4), new Num(3)), new Add(new Num(2),
        new Num(-1))) )->to_pretty_string() == "(-4 + 3) + 2 + -1" );
    }
    SECTION("Let expression test cases for its methods") {
        Let letExpr("x", new Num(5), new VarExpr("x"));
        CHECK(letExpr.to_string() == "(_let x=5 _in x)");
        //test case given from assignment
        Expr* test69 = new Let("x", new Num(5), new Add(new Let("y", new Num(3),
        new Add(new VarExpr("y"), new Num(2))), new VarExpr("x")));
        CHECK(test69->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        CHECK(test69->to_pretty_string() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
        CHECK(test69->interp()->equals(new NumVal(10)));
        Expr* test70 = new Mul(
            new Mul(
                new Num(2),
                new Let("x", new Num(5), new Add(new VarExpr("x"), new Num(1)))
            ),
            new Num(3)
        );
        CHECK(test70->to_pretty_string() == "(2 * _let x = 5\n     _in  x + 1) * 3");
        CHECK(test70->interp()->equals(new NumVal(36)));
    }//end of test case bracket
}//end of ...test case

PTR(Expr) parse_str(const std::string &str) {
    std::istringstream iss(str);
    return parse(iss);
}

static std::string run(std::string s) {
  return parse_str(s)->interp()->to_string();
}
//beginning of new test case
TEST_CASE("parse") {
    CHECK_THROWS_WITH( parse_str("()"), "Invalid input from multicand parser!");

    CHECK( parse_str("(1)")->equals(new Num(1)) );
    CHECK( parse_str("(((1)))")->equals(new Num(1)) );

    CHECK_THROWS_WITH( parse_str("(1"), "Mismatch from consume method");

    CHECK( parse_str("1")->equals(new Num(1)) );
    CHECK( parse_str("10")->equals(new Num(10)) );
    CHECK( parse_str("-3")->equals(new Num(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(new Num(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "Invalid input" );
    CHECK_THROWS_WITH( parse_str(" -   5  "), "Invalid input" );

    CHECK( parse_str("x")->equals(new VarExpr("x")) );
    CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
    // CHECK_THROWS_WITH( parse_str("x_z"), "Invalid input from Expr* parser" );

    CHECK( parse_str("x + y")->equals(new Add(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("x * y")->equals(new Mul(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("z * x + y")
        ->equals(new Add(new Mul(new VarExpr("z"), new VarExpr("x")),
        new VarExpr("y"))) );

    CHECK( parse_str("z * (x + y)")
        ->equals(new Mul(new VarExpr("z"),
        new Add(new VarExpr("x"), new VarExpr("y"))) ));
}//end of test case bracket

TEST_CASE("Parsing Let Expressions") {
    SECTION("Simple Let Expression") {
        std::istringstream input("_let x = 5 _in x");
        Expr* result = parse_let(input);
        CHECK(result->to_string() == "(_let x=5 _in x)");

        //tests the subst method for nested cases of let expression
        CHECK( (new Let("x", new VarExpr("x"), new VarExpr("x")))->subst
        ("x", new Num(2)) ->equals(new Let("x", new Num(2), new VarExpr("x"))) );

        // checks proper interp
        CHECK( (new Let("x", new Num(1), new Let("x",
        new Num(2), new VarExpr("x")))) ->interp()->equals(new NumVal(2)));

        CHECK( (new Let("x", new Num(1), new Add(new Let("x",
        new Num(2), new VarExpr("x")), new VarExpr("x")))) ->interp()->equals(new NumVal(3)));

        //pretty print testing with nested let expressions
        CHECK( (new Let("x", new Num(5), new Add(new VarExpr("x"), new Let("y",
        new Num(3), new Add(new VarExpr("y"), new Num(2))))))
        ->to_pretty_string() == ((std::string)"" + "_let x = 5\n" + "_in  x + _let y = 3\n" + "         _in  y + 2") );

        CHECK( (new Let("x", new Num(5), new Mul(new VarExpr("x"),
        new Let("y", new Num(3), new Mul(new VarExpr("y"), new Num(2))))))
         ->to_pretty_string() == ((std::string)"" + "_let x = 5\n" + "_in  x * _let y = 3\n" + "         _in  y * 2") );

        CHECK( (new Let("x", new Num(5), new VarExpr("x")))
     ->to_pretty_string()
     == ((std::string)"" +
       "_let x = 5\n" +
       "_in  x") );
       //I have one extra space after _in__ instead of _in_
       //check with TA
  CHECK( (new Let("x", new Num(5),
          new Add(new Let("y", new Num(3),
                  new Add(new VarExpr("y"), new Num(2))),
              new VarExpr("x"))))
     ->to_pretty_string()
     == ((std::string)"" +
       "_let x = 5\n" +
       "_in  (_let y = 3\n" +
       "      _in  y + 2) + x") );
  CHECK( (new Let("x", new Num(5),
          new Add(new VarExpr("x"),
              new Let("y", new Num(3),
                  new Add(new VarExpr("y"), new Num(2))))))
     ->to_pretty_string()
     == ((std::string)"" +
       "_let x = 5\n" +
       "_in  x + _let y = 3\n" +
       "         _in  y + 2") );
    }

    // SECTION("Nested Let Expression") {
        // std::istringstream input("_let x = 5 _in (_let y = 3 _in (x + y))");
        // Expr* result = parse_let(input);
        // CHECK(result->to_string() == "(_let x=5 _in ((_let y=3 _in (x+y))+x))");
        //check this one with TA -- might be wrong the CHECK
    // }

    SECTION("Let Expression with Addition") {
        std::istringstream input("_let x = 10 _in (_let y = 7 _in (x + y))");
        Expr* result = parse_let(input);
        CHECK(result->to_string() == "(_let x=10 _in (_let y=7 _in (x+y)))");
        //I might have fixed this one =)
    }
}//end of test case
TEST_CASE("Parse positive number", "[parse_num]") {
  std::istringstream input("123");
  Expr* result = parse_num(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->interp()->equals(new NumVal(123)));
}
TEST_CASE("Parse negative number", "[parse_num]") {
  std::istringstream input("-456");
  Expr* result = parse_num(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->interp()->equals(new NumVal(-456)));
}
TEST_CASE("Invalid input with negative sign only", "[parse_num]") {
  std::istringstream input("-");
  REQUIRE_THROWS_AS(parse_num(input), std::runtime_error);
}
TEST_CASE("Parse variable with lowercase letters", "[parse_var]") {
  std::istringstream input("abc");
  Expr* result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "abc");
}
TEST_CASE("Parse variable with uppercase letters", "[parse_var]") {
  std::istringstream input("VariableName");
  Expr* result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "VariableName");
}
TEST_CASE("Parse single lowercase letter", "[parse_var]") {
  std::istringstream input("x");
  Expr* result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "x");
}
TEST_CASE("Parse variable with trailing spaces", "[parse_var]") {
  std::istringstream input("var  ");
  Expr* result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "var");
}
TEST_CASE("Parse variable with mixed case", "[parse_var]") {
  std::istringstream input("VaRiAbLe");
  Expr* result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "VaRiAbLe");
}
TEST_CASE("Parse basic Let expression", "[parse_let]") {
  std::istringstream input("_let x = 5 _in x");
  Expr* result = parse_let(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Let*>(result) != nullptr);
  REQUIRE(result->to_string() == "(_let x=5 _in x)");
}
TEST_CASE("Empty input should throw an error", "[parse_let]") {
  std::istringstream input("");
  REQUIRE_THROWS_AS(parse_let(input), std::runtime_error);
}
TEST_CASE("Invalid input without equals sign", "[parse_let]") {
  std::istringstream input("_let x 5 _in x");
  REQUIRE_THROWS_AS(parse_let(input), std::runtime_error);
}
TEST_CASE("Parse basic Addend expression", "[parse_addend]") {
  std::istringstream input("5");
  Expr* result = parse_addend(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->to_string() == "5");
}
TEST_CASE("Parse Addend expression with single operand", "[parse_addend]") {
  std::istringstream input("4");
  Expr* result = parse_addend(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->to_string() == "4");
}
TEST_CASE("Parse basic Multicand expression with number", "[parse_multicand]") {
  std::istringstream input("42");
  Expr* result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->to_string() == "42");
}
TEST_CASE("Parse Multicand expression with negative number", "[parse_multicand]") {
  std::istringstream input("-7");
  Expr* result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Num*>(result) != nullptr);
  REQUIRE(result->to_string() == "-7");
}
TEST_CASE("Parse Multicand expression with parentheses", "[parse_multicand]") {
  std::istringstream input("(3 * 7)");
  Expr* result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<Mul*>(result) != nullptr);
  REQUIRE(result->to_string() == "(3*7)");
}
TEST_CASE("Parse Multicand expression with variable", "[parse_multicand]") {
  std::istringstream input("variable");
  Expr* result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(dynamic_cast<VarExpr*>(result) != nullptr);
  REQUIRE(result->to_string() == "variable");
}
TEST_CASE("Invalid input without operand", "[parse_multicand]") {
  std::istringstream input("* 3");
  REQUIRE_THROWS_AS(parse_multicand(input), std::runtime_error);
}
TEST_CASE("Missing closing parenthesis should throw an error",
"[parse_multicand]") {
  std::istringstream input("(3 * 7");
  REQUIRE_THROWS_AS(parse_multicand(input), std::runtime_error);
}//end of test case bracket
TEST_CASE("Val class") {
//tests for to_expr // of 2 different objects num val to expr equals new num
CHECK((new NumVal(2))->to_expr()->equals(new Num(2)));
NumVal* numVal1 = new NumVal(33);//same int values
NumVal* numVal2 = new NumVal(33);
NumVal* different = new NumVal(-77);//negative int value
//tests equals
CHECK(numVal1->equals(numVal2));
CHECK(numVal2->equals(numVal1));
CHECK_FALSE(numVal1->equals(different));

//tests to_string method
CHECK(numVal1->to_string() == "33");
CHECK(parse_str("1 + 2")->interp()->equals(new NumVal(3)));
CHECK(parse_str("1 + 2")->interp()->to_string() == "3");

//mult class expr
CHECK((new Add(new Num(1), new Num(2)))->interp()->equals(new NumVal(3)));
}
TEST_CASE("BoolVal && BoolExpr classes methods") {
  SECTION("BoolExpr class") {
    BoolExpr trueExpr(true);
        BoolExpr falseExpr(false);

        // Test interp method
        Val* trueVal = trueExpr.interp();
        Val* falseVal = falseExpr.interp();


        // Test equals method
        BoolExpr trueExprCopy(true);
        BoolExpr falseExprCopy(false);

        CHECK(trueExpr.equals(&trueExprCopy));
        CHECK(falseExpr.equals(&falseExprCopy));
        CHECK_FALSE(trueExpr.equals(&falseExpr));
        CHECK_FALSE(falseExpr.equals(&trueExpr));

        // Test subst method
        Expr* substTrueExpr = trueExpr.subst("x", new Num(5));
        Expr* substFalseExpr = falseExpr.subst("y", new Num(10));

        CHECK(substTrueExpr->equals(&trueExpr));
        CHECK(substFalseExpr->equals(&falseExpr));
  }
  SECTION("EqExpr, BoolVal methods") {
    CHECK((new EqExpr(new Num(1), new Num(2)))->interp()->
    equals(new BoolVal(false)));
    CHECK((new EqExpr(new Num(1), new Num(1)))->interp()->
    equals(new BoolVal(true)));
    Expr* testExpr = new EqExpr(new Add(new Num(1), new Num(1)), new Add(new Num(2), new Num(0)));
    Val* result = testExpr->interp();
    CHECK((result)->to_string() == "_true");

    CHECK( (new Let("x", new Add(new Num(1), new Num(4)),
            new Mul(new VarExpr("x"), new VarExpr("x"))))
            ->interp()
            ->equals(new NumVal(25)));

    //Parsers BoolExpr
    CHECK(parse_str(("_true"))->equals(new BoolExpr(true)));
    CHECK(parse_str(("_false"))->equals(new BoolExpr(false)));

    CHECK(parse_str(("_if _true _then 5 _else 6"))->equals(
      new IfExpr(new BoolExpr(true), new Num(5), new Num(6))
    ));
    CHECK(parse_str(("_if _false _then 66 _else 1"))->equals(
      new IfExpr(new BoolExpr(false), new Num(66), new Num(1))
    ));
    CHECK(parse_str("1 == 2")->interp()->equals(new BoolVal(false)));
    CHECK(parse_str("5 == 88")->interp()->equals(new BoolVal(false)));
    CHECK(parse_str("5 == 5")->interp()->equals(new BoolVal(true)));
    CHECK(parse_str("1 == 2")->interp()->to_string() == "_false");
    CHECK(run("53 == -4") == "_false");
    CHECK(run("5 == 5") == "_true");
    CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp())->to_string()) == "6");
    CHECK((parse_str("1 + 2 == 3 + 0"))->interp()->to_string() == "_true");
  }
  SECTION("Pretty print for EqExpr") {
    Expr* expr1 = new Add(new Let("x", new Num(1), new Add(new VarExpr("x"), new Num(1))),
                          new Let("y", new Num(2), new Add(new VarExpr("y"), new Num(2))));
    Expr* expr2 = new Num(6);
    Expr* equal_expr = new EqExpr(expr1, expr2);
    CHECK(equal_expr->to_pretty_string() == "(_let x = 1\n"
    " _in  x + 1) + (_let y = 2\n"
    "                _in  y + 2) == 6");
    // delete equal_expr;
    IfExpr* if_base = new IfExpr(new BoolExpr(true), new Num(1), new Num(2));
    EqExpr* equal_expr1 = new EqExpr(new Num(2), new Add(new Num(3), if_base));
        CHECK(equal_expr1->to_pretty_string() == "2 == 3 + _if _true\n"
        "         _then 1\n"
        "         _else 2");
  }
}
TEST_CASE("CallExpr and FunExpr") {
  FunExpr* f1 = new FunExpr("x", new Add(new VarExpr("x"), new Num(1)));
  // FunExpr* f2 = new FunExpr("y", new Add(new VarExpr("y"), new Num(2)));
  Expr* ca1 = new CallExpr(f1, new Num(2));
  std::string s = "_let fib = _fun (fib)\n"
                  "             _fun (x)\n"
                  "               _if x == 0\n"
                  "               _then 0\n"
                  "               _else _if x == 1\n"
                  "                     _then 1\n"
                  "                     _else fib(fib)(x + -2) + fib(fib)(x + -1)\n"
                  "_in  fib(fib)(10)";
  std::string s1 = "(_fun (x) (x+1))(2)";
  std::string s2 = "(_fun (x) \n"
                          "   x + 1)(2)";
  // section gap
  SECTION("Parsing CallExpr & FunExpr, interp, and equals") {
    CHECK(ca1->equals(new CallExpr(new FunExpr("x", new Add(new VarExpr("x"),
    new Num(1))), new Num(2))));
    CHECK(parse_str("_let f = _fun (x) x + 1 _in f(10)")->interp()->
    equals(new NumVal(11)));
    CHECK(parse_str("(_fun (x) x + 1)(24)")->interp()->
    equals(new NumVal(25)));
    CHECK(parse_str("(_fun (x) x * 66)(-1)")->interp()->
    equals(new NumVal(-66)));
    CHECK(parse_str(s)->interp()->equals(new NumVal(55)));
    CHECK(ca1->interp()->equals(new NumVal(3)));
    CHECK(ca1->to_string() == s1);
    CHECK(ca1->to_pretty_string() == s2);
  }
  SECTION("Factorial example") {
    Expr* fac = new Let("factr1",
    new FunExpr("factr1",
    new FunExpr("x",
    new IfExpr(new EqExpr(new VarExpr("x"), new Num(1)),
    new Num(1), new Mul(new VarExpr("x"),
    new CallExpr(new CallExpr(new VarExpr("factr1"), new VarExpr("factr1") ),
    new Add(new VarExpr("x"), new Num(-1)))))
    )
    ), new CallExpr(new CallExpr(new VarExpr("factr1"), new VarExpr("factr1")), new Num(10)));
    CHECK(fac->interp()->equals(new NumVal(3628800)));

    std::string fac1 = "_let factr1 = _fun (factr1) \n"
    "                _fun (x) \n"
    "                  _if x == 1\n"
    "                  _then 1\n"
    "                  _else x * factr1(factr1)(x + -1)\n"
    "_in  factr1(factr1)(10)";
    CHECK(fac->to_pretty_string() == fac1);
  }
}