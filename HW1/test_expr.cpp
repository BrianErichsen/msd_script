// #define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"

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
}//end of ...