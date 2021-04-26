#include <iostream>
#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;
    
    arg_pars.add_argument("N").help("int argument required").nargs(3).metavar("x").argumentType("int");
    arg_pars.add_argument("integer").help("no [] after and no metavar").nargs(1).argumentType("int");
    //arg_pars.add_argument("integer").help(":").nargs(1).argument_type<int>();
    arg_pars.add_argument("-i", "--info").help("shows information about program");
    arg_pars.add_argument("-o").help("just a shorcut");
    arg_pars.add_argument("--option").help("just a command");
    arg_pars.add_argument("constant").help("constant value parsed")
        .constant<int>(45).action("store_const");
    arg_pars.parse_argument(argc, argv);

    std::vector<int> arg = arg_pars.getValues<int>("N");
    auto arg1 = arg_pars.getValues<int>("integer");
    auto arg2 = arg_pars.getValues<int>("constant");
    
   
    return 0;
}
