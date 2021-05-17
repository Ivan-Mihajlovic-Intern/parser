#include <iostream>
#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;
    
    //positional
    arg_pars.add_argument("N").help("int argument required").nargs(3).metavar("x").argumentType<int>();
    
    //optional
    arg_pars.add_argument("-i", "--info").help("shows information about program");
    arg_pars.add_argument("-o").help("just a shorcut").nargs(1).argumentType<std::string>();
    arg_pars.add_argument("--option").help("just a command");
    
    //constant/action
    arg_pars.add_argument("constant").help("constant value parsed")
        .constant<int>(45).action("store_const");
    
    //subParsers
    arg_pars.subParsersTitle("subParser Title").subParsersDescription("subParser description").subParsersHelp("subParser help");
    ArgumentParser& subParser = arg_pars.add_parser("foo").help("foo help");
    //subParser.add_argument("posArg").nargs(3).argumentType("int");
    subParser.add_argument("-f", "--fchat").help("fs in the chat");

    ArgumentParser& subSubParser = subParser.add_parser("--global");
    subSubParser.add_argument("--user.name").nargs(1).argumentType<std::string>();

    ArgumentParser& getParser = arg_pars.add_parser("get").help("get help");
    getParser.add_argument("-g","--get").help("get function").nargs(1).argumentType<std::string>();
     

    //parseArgs
    
    arg_pars.parse_argument(argc, argv);
    //arg_pars.parse_argument({"-o", "4", "5", "6"});
    //arg_pars.parse_argument({ "--option", "24", "13", "14" });
    //passed args testing
    
    std::vector<int> args = arg_pars.getValues<int>("N");
    auto userName = subSubParser.getValues<std::string>("--user.name");
    for (const auto& arg : args)
    {
        std::cout << "subParser " << arg << std::endl;
    }

    std::vector<std::string> niska = arg_pars.getValues<std::string>("-o");
    std::cout << niska[0] << std::endl;
    
    std::cout << userName.front() << std::endl;
    if(subSubParser.isActive("--global"))
        std::cout << "radi" << std::endl;
    else
        std::cout << "ne radi" << std::endl;
    return 0;
}
