#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;
    std::cout << "start\n";
    //positional
    //arg_pars.addHelp(false);
    arg_pars.exitOnError(false);
    arg_pars.argumentDefault({365, 366});
    arg_pars.prefixChars("-+");
    arg_pars.addArgument("N").help("int argument required").nargs(3).metavar("x").argumentType<int>().choices({1,2,3});
    //arg_pars.addArgument("M").help("int argument required").nargs(3).argumentType<int>();

    //optional
    arg_pars.addArgument("+p", "++plus").help("prefix is plus").nargs(2).argumentType<int>();
    arg_pars.addArgument("+b", "+-both").help("prefix is both + and -");
    arg_pars.addArgument("-i", "--info").help("shows information about program");
    arg_pars.addArgument("-o").help("just a shorcut").nargs(1).argumentType<std::string>();
    arg_pars.addArgument("--option").help("just a command");
    //arg_pars.addArgument("+p");
    //arg_pars.addArgument("++plus");
    //arg_pars.addArgument("N");


    //constant/action
    //arg_pars.addArgument("constant").help("constant value parsed")
     //   .constant<int>(45).action("store_const");
    
    //subParsers
    arg_pars.subParsersTitle("subParser Title").subParsersDescription("subParser description").subParsersHelp("subParser help");
    ArgumentParser& subParser = arg_pars.addParser("foo").help("foo help");
    //subParser.addArgument("posArg").nargs(3).argumentType("int");
    subParser.addArgument("-f", "--fchat").help("fs in the chat");

    ArgumentParser& subSubParser = subParser.addParser("--global");
    subSubParser.addArgument("--user.name").nargs(1).argumentType<std::string>();

    ArgumentParser& getParser = arg_pars.addParser("get").help("get help");
    getParser.addArgument("-g","--get").help("get function").nargs(1).argumentType<std::string>();
     

    //parseArgs
    //std::cout << arg_pars.printUsage() << "\n";
   // std::cout << arg_pars.printHelp() << "\n";
    arg_pars.parseArgument(argc, argv);
    //arg_pars.parseArgument({"-o", "4", "5", "6"});
    //arg_pars.parseArgument({ "--option", "24", "13", "14" });
    //passed args testing
    std::cout << "pst parse\n";
    std::vector<int> args = arg_pars.getValues<int>("N");
    auto userName = subSubParser.getValues<std::string>("--user.name");
    int sum = 0;
    for (const auto& arg : args)
    {
        std::cout << "N: " << arg << std::endl;
        sum += arg;
    }
    std::cout << sum << "\n";
    //std::vector<std::string> niska = arg_pars.getValues<std::string>("-o");
    //std::cout << niska[0] << std::endl;
    
    if (arg_pars.isActive("--option"))
    {
        std::cout << "--option is active" << "\n";
    }
    else
        std::cout << "--option is NOT active" << "\n";

    std::vector<int> argsOption = arg_pars.getValues<int>("--option");
    for (const auto& arg : argsOption)
    {
        std::cout << "--option: " << arg << std::endl;
    }

    return 0;
}
