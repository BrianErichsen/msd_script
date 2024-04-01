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
#include "pointer.h"

TEST_CASE("do nothing") {
  std::cout << "test passed!" << std::endl;
}

TEST_CASE("Equals method tests") {
    SECTION("Expr equals") {
        //VarExpr class
        CHECK((NEW (VarExpr)("x"))->equals(NEW (VarExpr)("x")) == true);
        CHECK((NEW (VarExpr)("x"))->equals(NEW (VarExpr)("y")) == false);
        //different classes
        CHECK((NEW (Num)(1))->equals(NEW (VarExpr)("x")) == false );
        //Add class
        CHECK((NEW (Add)(NEW (Num)(2),NEW (Num)(3)))->equals
        (NEW (Add)(NEW (Num)(2),NEW (Num)(3)))==true);
        //int max / min and negative numbers
        CHECK((NEW (Add)(NEW (Num)(2),NEW (Num)(3)))->equals
        (NEW (Add)(NEW (Num)(3),NEW (Num)(2)))==false);

        CHECK((NEW (Add)(NEW (Num)(-1),NEW (Num)(3)))->equals
        (NEW (Add)(NEW (Num)(3),NEW (Num)(2)))==false);
        //checking for max num added into num
        int max = INT_MAX;
        PTR(Num) num1 = NEW (Num)(max);
        CHECK((NEW (Add)(num1,NEW (Num)(-1)))->equals
        (NEW (Add)(NEW (Num)(2147483647),NEW (Num)(-1)))==true);
        //check for proper value
        CHECK((NEW (Num)(2))->interp()->equals(NEW (NumVal)(2)));
        //check if the result of the 2 != expressions are correct
        // CHECK((NEW Add(NEW Num(2), NEW Num(3)))->interp() ==
        // (NEW Mul(NEW Num(1), NEW Num(5)))->interp());
        //still working how to test this

        //Mul class
        CHECK((NEW (Mul)(NEW (Num)(2),NEW (Num)(3)))->equals
        (NEW (Mul)(NEW (Num)(6), NEW (Num)(1)))==false);

        CHECK((NEW (Mul)(NEW (Num)(2),NEW (Num)(3)))->equals
        (NEW (Mul)(NEW (Num)(2),NEW (Num)(3)))==true);

        CHECK_THROWS_WITH( (NEW (VarExpr)("x"))->interp(), "no value for variable" );
    
    }//end of section
    SECTION("bool has_variables and other testing") {
        CHECK( (NEW (Mul)(NEW (Num)(3), NEW (Num)(2)))
        ->interp()->equals(NEW (NumVal)(6)));
    }
    SECTION("Subst method test case") {
        CHECK( (NEW (Add)(NEW (VarExpr)("x"), NEW (Num)(7)))
        ->subst("x", NEW (VarExpr)("y"))
        ->equals(NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(7))) );
        CHECK( (NEW (VarExpr)("x"))
        ->subst("x", NEW (Add)(NEW (VarExpr)("y"),NEW (Num)(7)))
        ->equals(NEW (Add)(NEW (VarExpr)("y"),NEW (Num)(7))) );
        CHECK( (NEW (VarExpr)("x"))
        ->subst("x", NEW (Mul)(NEW (VarExpr)("y"),NEW (Num)(7)))
        ->equals(NEW (Mul)(NEW (VarExpr)("y"),NEW (Num)(7))) );
        CHECK( (NEW (VarExpr)("y"))
        ->subst("y", NEW (Mul)(NEW (VarExpr)("x"),NEW (Num)(-12)))
        ->equals(NEW (Mul)(NEW (VarExpr)("x"),NEW (Num)(-12))) );
    }
    SECTION("Printing methods test cases") {
        CHECK( (NEW (Num)(10))->to_string() == "10" );
        CHECK ( (NEW (Mul)(NEW (Num)(1), NEW (Add)(NEW (Num)(2),
        NEW (Num)(3))))->to_pretty_string() ==  "1 * (2 + 3)" );
        CHECK ( (NEW (Mul)(NEW (Mul)(NEW (Num)(8), NEW (Num)(1)),
        NEW (VarExpr)("y")))->to_pretty_string() ==  "(8 * 1) * y" );
        CHECK ( (NEW (Mul)(NEW (Add)(NEW (Num)(3), NEW (Num)(5)),
        NEW (Mul)(NEW (Num)(6), NEW (Num)(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );
        CHECK ( (NEW (Mul)(NEW (Mul)(NEW (Num)(7), NEW (Num)(7)),
        NEW (Add)(NEW (Num)(9), NEW (Num)(2))) )->to_pretty_string() ==  "(7 * 7) * (9 + 2)" );
        PTR(Add) addPrintExp = NEW (Add)(NEW (Num)(-1), NEW (Add)(NEW (Num)(-2), NEW (Num)(-3)));
        std::string exp = "(-1+(-2+-3))";
        CHECK( addPrintExp->to_string() == exp );
        // Check simple Num printing
        CHECK((NEW (Num)(42))->to_string() == "42");
        // Check VarExpr printing
        CHECK((NEW (VarExpr)("x"))->to_string() == "x");
        CHECK((NEW (Add)(NEW (Num)(1), NEW (Num)(2)))->to_string() == "(1+2)");
        CHECK((NEW (Add)(NEW (Num)(1), NEW (VarExpr)("x")))->to_pretty_string() == "1 + x");
        CHECK((NEW (Mul)(NEW (Num)(3), NEW (VarExpr)("y")))->to_pretty_string() == "3 * y");
        CHECK( (NEW (Add)(NEW (Add)(NEW (VarExpr)("x"), NEW (VarExpr)("y")),
        NEW (Num)(1)))->to_pretty_string() == "(x + y) + 1" );
        CHECK( (NEW (Add)(NEW (Add)(NEW (Num)(-4), NEW (Num)(3)), NEW (Add)(NEW (Num)(2),
        NEW (Num)(-1))) )->to_pretty_string() == "(-4 + 3) + 2 + -1" );
    }
    SECTION("Let expression test cases for its methods") {
        Let letExpr("x", NEW (Num)(5), NEW (VarExpr)("x"));
        CHECK(letExpr.to_string() == "(_let x=5 _in x)");
        //test case given from assignment
        PTR(Expr) test69 = NEW (Let)("x", NEW (Num)(5), NEW (Add)(NEW (Let)("y", NEW (Num)(3),
        NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(2))), NEW (VarExpr)("x")));
        CHECK(test69->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        CHECK(test69->to_pretty_string() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
        CHECK(test69->interp()->equals(NEW (NumVal)(10)));
        PTR(Expr) test70 = NEW (Mul)(
            NEW (Mul)(
                NEW (Num)(2),
                NEW (Let)("x", NEW (Num)(5), NEW (Add)(NEW (VarExpr)("x"), NEW (Num)(1)))
            ),
            NEW (Num)(3)
        );
        CHECK(test70->to_pretty_string() == "(2 * _let x = 5\n     _in  x + 1) * 3");
        CHECK(test70->interp()->equals(NEW (NumVal)(36)));
    }//end of test case bracket
}//end of ...test case

PTR(Expr) parse_str(const std::string &str) {
    std::istringstream iss(str);
    return parse(iss);
}

static std::string run(std::string s) {
  return parse_str(s)->interp()->to_string();
}
//beginning of NEW test case
TEST_CASE("parse") {
    CHECK_THROWS_WITH( parse_str("()"), "Invalid input from multicand parser!");

    CHECK( parse_str("(1)")->equals(NEW (Num)(1)) );
    CHECK( parse_str("(((1)))")->equals(NEW (Num)(1)) );

    CHECK_THROWS_WITH( parse_str("(1"), "Mismatch from consume method");

    CHECK( parse_str("1")->equals(NEW (Num)(1)) );
    CHECK( parse_str("10")->equals(NEW (Num)(10)) );
    CHECK( parse_str("-3")->equals(NEW (Num)(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(NEW (Num)(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "Invalid input" );
    CHECK_THROWS_WITH( parse_str(" -   5  "), "Invalid input" );

    CHECK( parse_str("x")->equals(NEW (VarExpr)("x")) );
    CHECK( parse_str("xyz")->equals(NEW (VarExpr)("xyz")) );
    CHECK( parse_str("xYz")->equals(NEW (VarExpr)("xYz")) );
    // CHECK_THROWS_WITH( parse_str("x_z"), "Invalid input from Expr* parser" );

    CHECK( parse_str("x + y")->equals(NEW (Add)(NEW (VarExpr)("x"), NEW (VarExpr)("y"))) );

    CHECK( parse_str("x * y")->equals(NEW (Mul)(NEW (VarExpr)("x"), NEW (VarExpr)("y"))) );

    CHECK( parse_str("z * x + y")
        ->equals(NEW (Add)(NEW (Mul)(NEW (VarExpr)("z"), NEW (VarExpr)("x")),
        NEW (VarExpr)("y"))) );

    CHECK( parse_str("z * (x + y)")
        ->equals(NEW (Mul)(NEW (VarExpr)("z"),
        NEW (Add)(NEW (VarExpr)("x"), NEW (VarExpr)("y"))) ));
}//end of test case bracket

TEST_CASE("Parsing Let Expressions") {
    SECTION("Simple Let Expression") {
        std::istringstream input("_let x = 5 _in x");
        PTR(Expr) result = parse_let(input);
        CHECK(result->to_string() == "(_let x=5 _in x)");

        //tests the subst method for nested cases of let expression
        CHECK( (NEW (Let)("x", NEW (VarExpr)("x"), NEW (VarExpr)("x")))->subst
        ("x", NEW (Num)(2)) ->equals(NEW (Let)("x", NEW (Num)(2), NEW (VarExpr)("x"))) );

        // checks proper interp
        CHECK( (NEW (Let)("x", NEW (Num)(1), NEW (Let)("x",
        NEW (Num)(2), NEW (VarExpr)("x")))) ->interp()->equals(NEW (NumVal)(2)));

        CHECK( (NEW (Let)("x", NEW (Num)(1), NEW (Add)(NEW (Let)("x",
        NEW (Num)(2), NEW (VarExpr)("x")), NEW (VarExpr)("x")))) ->interp()->equals(NEW (NumVal)(3)));

        //pretty print testing with nested let expressions
        CHECK( (NEW (Let)("x", NEW (Num)(5), NEW (Add)(NEW (VarExpr)("x"), NEW (Let)("y",
        NEW (Num)(3), NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(2))))))
        ->to_pretty_string() == ((std::string)"" + "_let x = 5\n" + "_in  x + _let y = 3\n" + "         _in  y + 2") );

        CHECK( (NEW (Let)("x", NEW (Num)(5), NEW (Mul)(NEW (VarExpr)("x"),
        NEW (Let)("y", NEW (Num)(3), NEW (Mul)(NEW (VarExpr)("y"), NEW (Num)(2))))))
         ->to_pretty_string() == ((std::string)"" + "_let x = 5\n" + "_in  x * _let y = 3\n" + "         _in  y * 2") );

        CHECK( (NEW (Let)("x", NEW (Num)(5), NEW (VarExpr)("x")))
     ->to_pretty_string()
     == ((std::string)"" +
       "_let x = 5\n" +
       "_in  x") );
       //I have one extra space after _in__ instead of _in_
       //check with TA
  CHECK( (NEW (Let)("x", NEW (Num)(5),
          NEW (Add)(NEW (Let)("y", NEW (Num)(3),
                  NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(2))),
              NEW (VarExpr)("x"))))
     ->to_pretty_string()
     == ((std::string)"" +
       "_let x = 5\n" +
       "_in  (_let y = 3\n" +
       "      _in  y + 2) + x") );
  CHECK( (NEW (Let)("x", NEW (Num)(5),
          NEW (Add)(NEW (VarExpr)("x"),
              NEW (Let)("y", NEW (Num)(3),
                  NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(2))))))
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
        PTR(Expr) result = parse_let(input);
        CHECK(result->to_string() == "(_let x=10 _in (_let y=7 _in (x+y)))");
        //I might have fixed this one =)
    }
}//end of test case
TEST_CASE("Parse positive number", "[parse_num]") {
  std::istringstream input("123");
  PTR(Expr) result = parse_num(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->interp()->equals(NEW (NumVal)(123)));
}
TEST_CASE("Parse negative number", "[parse_num]") {
  std::istringstream input("-456");
  PTR(Expr) result = parse_num(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->interp()->equals(NEW (NumVal)(-456)));
}
TEST_CASE("Invalid input with negative sign only", "[parse_num]") {
  std::istringstream input("-");
  REQUIRE_THROWS_AS(parse_num(input), std::runtime_error);
}
TEST_CASE("Parse variable with lowercase letters", "[parse_var]") {
  std::istringstream input("abc");
  PTR(Expr) result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
  REQUIRE(result->to_string() == "abc");
}
TEST_CASE("Parse variable with uppercase letters", "[parse_var]") {
  std::istringstream input("VariableName");
  PTR(Expr) result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
  REQUIRE(result->to_string() == "VariableName");
}
TEST_CASE("Parse single lowercase letter", "[parse_var]") {
  std::istringstream input("x");
  PTR(Expr) result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
  REQUIRE(result->to_string() == "x");
}
TEST_CASE("Parse variable with trailing spaces", "[parse_var]") {
  std::istringstream input("var  ");
  PTR(Expr) result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
  REQUIRE(result->to_string() == "var");
}
TEST_CASE("Parse variable with mixed case", "[parse_var]") {
  std::istringstream input("VaRiAbLe");
  PTR(Expr) result = parse_var(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
  REQUIRE(result->to_string() == "VaRiAbLe");
}
TEST_CASE("Parse basic Let expression", "[parse_let]") {
  std::istringstream input("_let x = 5 _in x");
  PTR(Expr) result = parse_let(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Let)(result) != nullptr);
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
  PTR(Expr) result = parse_addend(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->to_string() == "5");
}
TEST_CASE("Parse Addend expression with single operand", "[parse_addend]") {
  std::istringstream input("4");
  PTR(Expr) result = parse_addend(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->to_string() == "4");
}
TEST_CASE("Parse basic Multicand expression with number", "[parse_multicand]") {
  std::istringstream input("42");
  PTR(Expr) result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->to_string() == "42");
}
TEST_CASE("Parse Multicand expression with negative number", "[parse_multicand]") {
  std::istringstream input("-7");
  PTR(Expr) result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Num)(result) != nullptr);
  REQUIRE(result->to_string() == "-7");
}
TEST_CASE("Parse Multicand expression with parentheses", "[parse_multicand]") {
  std::istringstream input("(3 * 7)");
  PTR(Expr) result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(Mul)(result) != nullptr);
  REQUIRE(result->to_string() == "(3*7)");
}
TEST_CASE("Parse Multicand expression with variable", "[parse_multicand]") {
  std::istringstream input("variable");
  PTR(Expr) result = parse_multicand(input);
  REQUIRE(result != nullptr);
  REQUIRE(CAST(VarExpr)(result) != nullptr);
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
//tests for to_expr // of 2 different objects num val to expr equals NEW num
CHECK((NEW (NumVal)(2))->to_expr()->equals(NEW (Num)(2)));
PTR(NumVal) numVal1 = NEW (NumVal)(33);//same int values
PTR(NumVal) numVal2 = NEW (NumVal)(33);
PTR(NumVal) different = NEW (NumVal)(-77);//negative int value
//tests equals
CHECK(numVal1->equals(numVal2));
CHECK(numVal2->equals(numVal1));
CHECK_FALSE(numVal1->equals(different));

//tests to_string method
CHECK(numVal1->to_string() == "33");
CHECK(parse_str("1 + 2")->interp()->equals(NEW (NumVal)(3)));
CHECK(parse_str("1 + 2")->interp()->to_string() == "3");

//mult class expr
CHECK((NEW (Add)(NEW (Num)(1), NEW (Num)(2)))->interp()->equals(NEW (NumVal)(3)));
}
TEST_CASE("BoolVal && BoolExpr classes methods") {
  SECTION("BoolExpr class") {
    // BoolExpr trueExpr(true);
    //     BoolExpr falseExpr(false);

    //     // Test interp method
    //     PTR(Val) trueVal = trueExpr.interp();
    //     PTR(Val) falseVal = falseExpr.interp();

    //     // Test equals method
    //     BoolExpr trueExprCopy(true);
    //     BoolExpr falseExprCopy(false);

    //     CHECK(trueExpr.equals(&trueExprCopy));
    //     CHECK(falseExpr.equals(&falseExprCopy));
    //     CHECK_FALSE(trueExpr.equals(&falseExpr));
    //     CHECK_FALSE(falseExpr.equals(&trueExpr));

    //     // Test subst method
    //     PTR(Expr) substTrueExpr = trueExpr.subst("x", NEW (Num)(5));
    //     PTR(Expr) substFalseExpr = falseExpr.subst("y", NEW (Num)(10));

    //     CHECK(substTrueExpr->equals(&trueExpr));
    //     CHECK(substFalseExpr->equals(&falseExpr));
    // have to fix smart pointers here to make it work
    //it was passing tests before switching to new pointers
  }
  SECTION("EqExpr, BoolVal methods") {
    CHECK((NEW (EqExpr)(NEW (Num)(1), NEW (Num)(2)))->interp()->
    equals(NEW (BoolVal)(false)));
    CHECK((NEW (EqExpr)(NEW (Num)(1), NEW (Num)(1)))->interp()->
    equals(NEW (BoolVal)(true)));
    PTR(Expr) testExpr = NEW (EqExpr)(NEW (Add)(NEW (Num)(1), NEW (Num)(1)), NEW (Add)(NEW (Num)(2), NEW (Num)(0)));
    PTR(Val) result = testExpr->interp();
    CHECK((result)->to_string() == "_true");

    CHECK( (NEW (Let)("x", NEW (Add)(NEW (Num)(1), NEW (Num)(4)),
            NEW (Mul)(NEW (VarExpr)("x"), NEW (VarExpr)("x"))))
            ->interp()
            ->equals(NEW (NumVal)(25)));

    //Parsers BoolExpr
    CHECK(parse_str(("_true"))->equals(NEW (BoolExpr)(true)));
    CHECK(parse_str(("_false"))->equals(NEW (BoolExpr)(false)));

    CHECK(parse_str(("_if _true _then 5 _else 6"))->equals(
      NEW (IfExpr)(NEW (BoolExpr)(true), NEW (Num)(5), NEW (Num)(6))
    ));
    CHECK(parse_str(("_if _false _then 66 _else 1"))->equals(
      NEW (IfExpr)(NEW (BoolExpr)(false), NEW (Num)(66), NEW (Num)(1))
    ));
    CHECK(parse_str("1 == 2")->interp()->equals(NEW (BoolVal)(false)));
    CHECK(parse_str("5 == 88")->interp()->equals(NEW (BoolVal)(false)));
    CHECK(parse_str("5 == 5")->interp()->equals(NEW (BoolVal)(true)));
    CHECK(parse_str("1 == 2")->interp()->to_string() == "_false");
    CHECK(run("53 == -4") == "_false");
    CHECK(run("5 == 5") == "_true");
    CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp())->to_string()) == "6");
    CHECK((parse_str("1 + 2 == 3 + 0"))->interp()->to_string() == "_true");
  }
  SECTION("Pretty print for EqExpr") {
    PTR(Expr) expr1 = NEW (Add)(NEW (Let)("x", NEW (Num)(1), NEW (Add)(NEW (VarExpr)("x"), NEW (Num)(1))),
                          NEW (Let)("y", NEW (Num)(2), NEW (Add)(NEW (VarExpr)("y"), NEW (Num)(2))));
    PTR(Expr) expr2 = NEW (Num)(6);
    PTR(Expr) equal_expr = NEW (EqExpr)(expr1, expr2);
    CHECK(equal_expr->to_pretty_string() == "(_let x = 1\n"
    " _in  x + 1) + (_let y = 2\n"
    "                _in  y + 2) == 6");
    // delete equal_expr;
    PTR(IfExpr) if_base = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (Num)(1), NEW (Num)(2));
    PTR(EqExpr) equal_expr1 = NEW (EqExpr)(NEW (Num)(2), NEW (Add)(NEW (Num)(3), if_base));
        CHECK(equal_expr1->to_pretty_string() == "2 == 3 + _if _true\n"
        "         _then 1\n"
        "         _else 2");
  }
}
TEST_CASE("CallExpr and FunExpr") {
  PTR(FunExpr) f1 = NEW (FunExpr)("x", NEW (Add)(NEW (VarExpr)("x"), NEW (Num)(1)));
  // FunExpr* f2 = NEW FunExpr("y", NEW Add(NEW VarExpr("y"), NEW Num(2)));
  PTR(Expr) ca1 = NEW (CallExpr)(f1, NEW (Num)(2));
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
    CHECK(ca1->equals(NEW (CallExpr)(NEW (FunExpr)("x", NEW (Add)(NEW (VarExpr)("x"),
    NEW (Num)(1))), NEW (Num)(2))));
    CHECK(parse_str("_let f = _fun (x) x + 1 _in f(10)")->interp()->
    equals(NEW (NumVal)(11)));
    CHECK(parse_str("(_fun (x) x + 1)(24)")->interp()->
    equals(NEW (NumVal)(25)));
    CHECK(parse_str("(_fun (x) x * 66)(-1)")->interp()->
    equals(NEW (NumVal)(-66)));
    CHECK(parse_str(s)->interp()->equals(NEW (NumVal)(55)));
    CHECK(ca1->interp()->equals(NEW (NumVal)(3)));
    CHECK(ca1->to_string() == s1);
    CHECK(ca1->to_pretty_string() == s2);
  }
  SECTION("Factorial example") {
    PTR(Expr) fac = NEW (Let)("factr1",
    NEW (FunExpr)("factr1",
    NEW (FunExpr)("x",
    NEW (IfExpr)(NEW (EqExpr)(NEW (VarExpr)("x"), NEW (Num)(1)),
    NEW (Num)(1), NEW (Mul)(NEW (VarExpr)("x"),
    NEW (CallExpr)(NEW (CallExpr)(NEW (VarExpr)("factr1"), NEW (VarExpr)("factr1") ),
    NEW (Add)(NEW (VarExpr)("x"), NEW (Num)(-1)))))
    )
    ), NEW (CallExpr)(NEW (CallExpr)(NEW (VarExpr)("factr1"), NEW (VarExpr)("factr1")), NEW (Num)(10)));
    CHECK(fac->interp()->equals(NEW (NumVal)(3628800)));

    std::string fac1 = "_let factr1 = _fun (factr1) \n"
    "                _fun (x) \n"
    "                  _if x == 1\n"
    "                  _then 1\n"
    "                  _else x * factr1(factr1)(x + -1)\n"
    "_in  factr1(factr1)(10)";
    CHECK(fac->to_pretty_string() == fac1);
  }
}