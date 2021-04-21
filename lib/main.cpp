#include <iostream>
#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;
    
    arg_pars.add_argument("N").help("int argument required").nargs(3).metavar("x");
    arg_pars.add_argument("integer").help("no [] after and no metavar").nargs(1);
    arg_pars.add_argument("-i", "--info").help("shows information about program");
    arg_pars.add_argument("-o").help("just a shorcut");
    arg_pars.add_argument("--option").help("just a command");
   
    arg_pars.parse_argument(argc, argv);
    
}
