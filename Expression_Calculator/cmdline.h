/*Author: Brian Erichsen Fagundes                            //
// MSD Script Expression Calculator                          //
// MSD - UofU - CS6015 Software Engineering - Spring semester*/

#ifndef CMDLINE_H
#define CMDLINE_H

typedef enum {

  do_tests,
  do_help,
  do_interp,
  do_print,
  do_pretty_print,

} run_mode_t;
/**
 * \brief Parses and uses command line arguments.
 * \param argc Number of command line arguments.
 * \param argv Array of command line argument strings.
 */
run_mode_t use_arguments(int argc, char* argv[]);

#endif //CMDLINE_H