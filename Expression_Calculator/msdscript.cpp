#define CATCH_CONFIG_RUNNER
#include "cmdline.h"
#include "catch.h"
#include <iostream>
#include "expr.h"
#include "parse.h"
#include "val.h"


/**
* \mainpage MSDScript
* \author Brian Erichsen Fagundes
* \date 06-02-2024
*/
int main(int argc, char* argv[]) {
    run_mode_t type = use_arguments(argc, argv);

    if (type == do_tests) {
        int result = Catch::Session().run(1, argv);
        if (result != 0) {
            std::cerr << "Tests did not pass -- exiting the program";
            exit(1);
        } else {
            Catch::Session().run(1, argv);
            "All tests passed!!";
            exit(0);
        }
    }
    if (type == do_help) {
        std::cout << "Available options are : \n";
        std::cout << "--test, --interp, --print, --pretty-print\n";
        exit(0);
    }
    try {
        Expr* expr = parse(std::cin);
        if (type == do_interp) {
            Val* result = expr->interp();
            std::cout << result->to_string() << std::endl;
            exit(0);
        }
        else if (type == do_print) {
            std::cout << expr->to_string() << std::endl;
            exit(0);
        }
        else if (type == do_pretty_print) {
            std::cout << expr->to_pretty_string() << std::endl;
            exit(0);
        }
    } catch (const std::runtime_error& exit) {
        std::cerr << "Sorry, bad input!" << exit.what() << std::endl;
        return 1;
    }
}//end of main bracket