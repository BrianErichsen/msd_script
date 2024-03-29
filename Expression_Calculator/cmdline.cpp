/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#include <iostream>
#include <cstdlib>
#include "expr.h"
#include "cmdline.h"

run_mode_t use_arguments(int argc, char* argv[]) {
    bool testFlag = false;

    for (int i = 1; i < argc; i++) {
        //strcmp compares two strings and return an int; 0 for equal
        if (strcmp(argv[i], "--help") == 0) {
            return do_help;
        } else if (strcmp(argv[i], "--test") == 0) {
            //tests if flag has already been seen exits the program
            if (testFlag) {
                std::cerr << "Error: --test argument can only be used once.\n";
                exit(1);
            } else {
                //sets the flag to true and continues
                testFlag = true;
                return do_tests;
            }
            //--interp 'starting here where arguments are parsed
        } else if (strcmp(argv[i], "--interp") == 0) {
            return do_interp;
        } else if (strcmp(argv[i], "--print") == 0) {
            return do_print;
        } else if(strcmp(argv[i], "--pretty-print") == 0) {
            return do_pretty_print;
        } else {
            //if reached here; then given argument is invalid
            std::cerr << "Error: Unknown argument '" << argv[i] <<
            "'.\n";
            exit(1);
        }
    }//end of cmd method
}//end of method bracket