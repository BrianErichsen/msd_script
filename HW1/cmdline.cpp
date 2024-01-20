#define CATCH_CONFIG_RUNNER
#include <iostream>
#include <cstdlib>
#include "expr.h"
#include "catch.h"
#include "cmdline.h"

//created by Brian Erichsen Fagundes - CS6015 Software Engineering
//Spring semester

void use_arguments(int argc, char* argv[]) {
    bool testFlag = false;

    for (int i = 1; i < argc; i++) {
        //strcmp compares two strings and return an int; 0 for equal
        if (strcmp(argv[i], "--help") == 0) {
            //Prits help text and exits
            std::cout << "Usage: " << argv[0] << " [--help] [--test]\n";
            exit(0);
        } else if (strcmp(argv[i], "--test") == 0) {
            //tests if flag has already been seen
            if (testFlag) {
                std::cerr << "Error: --test argument can only be used once.\n";
                exit(1);
            } else {
                //sets the flag to true and continues
                testFlag = true;
                Catch::Session().run(1, argv);
                std::cout << "All tests passed!" << std::endl;
            }
        } else if (strcmp(argv[i], "--interp") == 0) {
            std::vector<std::string> tokens;
            for (int j = i + 1; j < argc; j++) {
                tokens.push_back(argv[j]);
            }
            size_t index = 0;
            Expr* result = Expr::parseExpr(tokens, index);
            if (result != nullptr) {
                std::cout << result->eval() << std::endl;
            }
            exit(0);
        } else {
            //if reached here; then given argument is invalid
            std::cerr << "Error: Unknown argument '" << argv[i] <<
            "'.\n";
            exit(1);
        }
    }//end of cmd method
}//end of method bracket