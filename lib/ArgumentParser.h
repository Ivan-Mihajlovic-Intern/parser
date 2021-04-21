#pragma once
#ifndef _ARGUMENT_PARSER_HPP
#define _ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>



class Argument;

class ArgumentParser
{
public:
    //Calculates its own help message
    ArgumentParser();

    //~ArgumentParser();

    //TODO:needs to be a template
    //template<typename T> 
    Argument& add_argument(std::string arg);

    //optional arguments
    //template<typename T>
    Argument& add_argument(std::string arg1, std::string arg2);

    void parse_argument(int argc, char* argv[]);
    void parse_argument(std::vector<std::string> parsedAgruments);

    ArgumentParser& usage(std::string usage);
    ArgumentParser& prog(std::string programName);
    ArgumentParser& description(std::string description);
    ArgumentParser& epilog(std::string epilog);
    ArgumentParser& prefix_chars(std::string prefixChars);
    ArgumentParser& from_file_prefix_chars(std::string fromFilePrefixChars);

    void addUsage(std::string msg);

    //getters for testing purpose
    std::vector<Argument> getOptionalArguments();
    std::vector<Argument> getPositionalArguments();
    std::map<std::string, bool> getActiveArguments();
    std::string getProgramName();


    friend std::ostream& operator<<(std::ostream& out, const ArgumentParser& arg_pars);

private:
    void programNameFromArgv(std::string);
    std::string _programName;
    std::string _usage = "";
    std::string _description;
    std::string _epilog;
    std::vector<Argument> _optionalArguments;
    std::vector<Argument> _positionalArguments;
    std::vector<ArgumentParser> _parents;
    std::string _prefixChars;
    std::string _fromFilePrefixChars;
    //if argument is called in command line bool becomes true, default false
    std::map<std::string, bool> _activeArguments;
    
    
    //argumentDefault def:none
    //add_help def: true
    //allow_abbrev def: true
    //exit_on_error def: true
};

class Argument
{
public:
    friend class ArgumentParser;
    Argument();

    Argument& name(std::string arg1, std::string arg2);
    Argument& action(std::string action);
    Argument& nargs(int nargs);
    Argument& constant(std::string constant);
    Argument& def(std::string def);
    Argument& help(std::string help);
    Argument& metavar(std::string metavar);

    void actionCheck(const std::string action);
    std::string getName() const;
    std::string getHelp() const;
    unsigned getNargs() const;
    std::string getMetavar() const;


    std::string generateSpaces(std::string name) const;
    const unsigned helpLength = 15;

private:
    std::vector<std::string> _name;
    std::string _action;
    unsigned _nargs = 0;
    std::string _constant;
    std::string _def;
    std::string _help;
    std::string _metavar;
    std::vector<std::string> _positionalValues;
    //missing type, choice and dest

    std::vector<std::string> allowedActions{ "store", "store_const", "store_true",
                                            "store_false", "append", "append_const",
                                            "count", "help"};
};

#endif // _ARGUMRNT_PARSER_HPP
