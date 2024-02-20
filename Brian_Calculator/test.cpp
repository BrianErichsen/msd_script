// #define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "test.h"
#include <iostream>
#include <sstream>
#include <limits>

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
        CHECK((new Num(2))->interp() == 2);
        //check if the result of the 2 != expressions are correct
        CHECK((new Add(new Num(2), new Num(3)))->interp() ==
        (new Mul(new Num(1), new Num(5)))->interp());

        //Mul class
        CHECK((new Mul(new Num(2),new Num(3)))->equals
        (new Mul(new Num(6),new Num(1)))==false);

        CHECK((new Mul(new Num(2),new Num(3)))->equals
        (new Mul(new Num(2),new Num(3)))==true);

        CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );
    
    }//end of section
    SECTION("bool has_variables and other testing") {
        CHECK( (new Mul(new Num(3), new Num(2)))
        ->interp()==6 );
        CHECK((new Add(new VarExpr("x"), new Num(1)))->has_variable() == true );
        CHECK((new Mul(new Num(2), new Num(1)))->has_variable() == false );
        CHECK((new Mul(new Num(-99), new Num(1)))->has_variable() == false );
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
        CHECK(test69->interp() == 10);
        Expr* test70 = new Mul(
            new Mul(
                new Num(2),
                new Let("x", new Num(5), new Add(new VarExpr("x"), new Num(1)))
            ),
            new Num(3)
        );
        CHECK(test70->to_pretty_string() == "(2 * _let x = 5\n     _in  x + 1) * 3");
        CHECK(test70->interp() == 36);
        CHECK(test69->interp() == 10);
    }//end of test case bracket


}//end of ...test case

// Expr* parse_str(const std::string& str) {
// std::istringstream iss(str);
//     return parse(iss);
// }

Expr* parse_str(const std::string &str) {
    std::istringstream iss(str);
    return parse(iss);
}

TEST_CASE("parse") {
    // CHECK_THROWS_WITH( parse_str("()"), "invalid input" );

    CHECK( parse_str("(1)")->equals(new Num(1)) );
    CHECK( parse_str("(((1)))")->equals(new Num(1)) );

    // CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );

    CHECK( parse_str("1")->equals(new Num(1)) );
    CHECK( parse_str("10")->equals(new Num(10)) );
    CHECK( parse_str("-3")->equals(new Num(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(new Num(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "Invalid input" );
    CHECK_THROWS_WITH( parse_str(" -   5  "), "Invalid input" );

    CHECK( parse_str("x")->equals(new VarExpr("x")) );
    CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "Invalid input" );

    CHECK( parse_str("x + y")->equals(new Add(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("x * y")->equals(new Mul(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("z * x + y")
        ->equals(new Add(new Mul(new VarExpr("z"), new VarExpr("x")),
        new VarExpr("y"))) );

    CHECK( parse_str("z * (x + y)")
        ->equals(new Mul(new VarExpr("z"),
        new Add(new VarExpr("x"), new VarExpr("y"))) ));
}//end of test case bracket

// TEST_CASE("testing_let_parse"){
//     CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(new _let("x", new Num(5), new Add(new VarExpr("x"), new Num(7)))));

//     CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(new _let("x", new Num(5), new Add(new VarExpr("x"), new Num(7)))));
//     CHECK(parse_str(("_let x=10 _in (y+10)"))->equals(new _let("x", new Num(10), new Add(new VarExpr("y"), new Num(10)))));
//     CHECK(parse_str(("(_let x=5 _in ((_let y=3 _in (y+2))+x))"))
//                   ->equals(new _let("x", new Num(5),
//                                    new Add(new _let("y", new Num(3), new Add(new VarExpr("y"), new Num(2))), new VarExpr("x")))));
//     CHECK(parse_str(("(_let x=5 _in (x+7))"))->equals((new _let("x", new Num(5), new Add(new VarExpr("x"), new Num(7))))));
// }