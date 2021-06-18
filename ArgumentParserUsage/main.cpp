// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;

    //positional argument
    arg_pars.addArgument("integers")
        .help("int argument required")
        .nargs(3)
        .metavar("x")
        .argumentType<int>();


    arg_pars.parseArgument({ "5", "7", "10" });

    try {
        arg_pars.parseArgument({ "5", "7", "10" });
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << arg_pars;
        exit(0);
    }


    std::vector<int> args = arg_pars.getValues<int>("integers");
    int sum = 0;
    for (const auto& arg : args)
    {
        sum += arg;
    }
    std::cout << sum << "\n";

    //optional argument
    arg_pars.addArgument("-s", "--sum")
        .help("sum of integers");

    try {
        arg_pars.parseArgument({ "-s", "5", "7", "10" });
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << arg_pars;
        exit(0);
    }

    if (arg_pars.isActive("--sum"))
    {
        std::vector<int> args = arg_pars.getValues<int>("integers");
        int sum = 0;
        
        try {
            for (const auto& arg : args)
            {
                sum += arg;
            }
        }
        catch (const std::runtime_error& err) {
            std::cout << err.what() << std::endl;
            exit(0);
        }

        std::cout << sum << "\n";
    }
    else
    {
        std::cout << "'--sum' isn't parsed" << "\n";
    }

    //optional argument
    arg_pars.addArgument("-sp", "--split")
        .help("splits negative and positive integers");

    try {
        arg_pars.parseArgument({ "-sp", "-5", "-7", "10" });
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << arg_pars;
        exit(0);
    }

    if (arg_pars.isActive("-sp"))
    {
        std::vector<int> args = arg_pars.getValues<int>("integers");
        try {
            for (const auto& arg : args)
            {
                if (arg < 0)
                    std::cout << arg << " ";
            }
        }
        catch (const std::runtime_error& err) {
            std::cout << err.what() << std::endl;
            exit(0);
        }

        std::cout << "\n";
    }

    return 0;
}
