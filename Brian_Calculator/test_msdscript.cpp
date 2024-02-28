//-----
//-----
//Created by Brian Erichsen Fagundes for grammar calculator exe part VII
//February, 2024 --- Spring Semester UofU - MSD
#include <iostream>
#include "test_msdscript.h"
#include <string>
#include "exec.h"
#include <ctime>

int main(int argc, char **argv) {
    srand(clock());//different each time rand runs

    if (argc == 2) {
        const char *const interp_argv[] = {argv[1], "--interp"};
        const char *const print_argv[] = {argv[1], "--print"};
        const char *const pretty_print_argv[] = {argv[1], "--pretty-print"};

        for (int i = 0; i < 50; i++) {
            std::string in = random_expr_string();
            // std::cout << "Trying" << in << std::endl;

            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pp_result = exec_program(2, pretty_print_argv, in);

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);

            if (interp_again_result.out != interp_result.out) {
                throw std::runtime_error("The 2 results are not identical");
            }

        }//end of for loop
        std::cout << "Reached end of for loop; no discrepencies found!!" << std::endl;

    }//end of if argc == 2
    if (argc == 3) {
        const char *const interp_argv[] = {argv[1], "--interp"};
        const char *const interp_tester_argv[] = {argv[2], "--interp"};

        const char *const print_argv[] = {argv[1], "--print"};
        const char *const print_tester_argv[] = {argv[2], "--print"};

        const char *const pretty_print_argv[] = {argv[1], "--pretty-print"};
        const char* const pp_tester_argv[] = {argv[2], "--pretty-print"};

        for (int i = 0; i < 50; i++) {
            std::string in = random_expr_string();
            // std::cout << "Trying" << in << std::endl;

            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult interp_tester_result = exec_program(2, interp_tester_argv, in);

            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult print_tester_result = exec_program(2, print_tester_argv, in);

            ExecResult pp_result = exec_program(2, pretty_print_argv, in);
            ExecResult pp_tester_result = exec_program(2, pp_tester_argv, in);

            if (interp_result.out != interp_tester_result.out) {
                std::cout << "msdscript Interp Result: " << interp_result.out;
                std::cout << "tester Interp Result: " << interp_tester_result.out <<
                std::endl << std::endl;

                // throw std::runtime_error("The 2 results are not identical");
            }

            if (print_result.out != print_tester_result.out) {
                std::cout << "msdscript print Result: " << print_result.out;
                std::cout << "tester print Result: " << print_result.out <<
                std::endl << std::endl;

                // throw std::runtime_error("The 2 results are not identical");
            }
            
            if (pp_result.out != pp_tester_result.out) {
                std::cout << "msdscript pretty_print Result: " << interp_result.out;
                std::cout << "tester pretty_print Result: " << interp_tester_result.out <<
                std::endl << std::endl;

                // throw std::runtime_error("The 2 results are not identical");
            }
        }
    }//end of if argc == 3


return 0;
}//end of main bracket

std::string random_expr_string() {
    if ((rand() % 10) < 6) {
        return std::to_string(rand());
    } else {
        int random = rand() % 100;
        switch(random) {
            case 0 ... 15:
                return "(" + random_expr_string() + ")";
            case 16 ... 30:
                return random_expr_string() + "+" + random_expr_string();
            case 31 ... 45:
                return random_expr_string() + "*" + random_expr_string();
            case 46 ... 50:
                return rand_var();
            case 51 ... 75:
                return ("_let " + rand_var() + " = " + random_expr_string() +
                "_in" + rand_let(random_expr_string()));
            case 76 ... 99:
                return "-" + std::to_string(rand());
                //if none of the others then it proceeds with default case
            default:
                return random_expr_string();
        }
    }
}//end of random_expr_string method

std::string rand_var() {
    //starting with an empty string at first to be built on
    std::string str = "";
    //loops over random number between 0 and 6
    for (int i = 0; i < rand() % 6; i++) {
        //concartenate string with 26 options + 'a' -- 0 for a -- 1 for b ...
        str += rand() % 26 + 'a';
    }
    return str;// returns newly built string
}//end of rand_var method bracket

std::string rand_let(std::string left) {
    int random = rand() % 100;

    switch(random) {
        case 0 ... 24:
            return left + "+" + random_expr_string();
        case 25 - 50:
            return left + "*" + random_expr_string();
        case 51 - 75:
            return random_expr_string() + "+" + left;
        case 76 - 99:
            return random_expr_string() + "*" + left;
    }
}//end of rand_let method bracket