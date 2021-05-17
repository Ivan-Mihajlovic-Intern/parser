#pragma once
#ifndef _ARGUMENT_PARSER_HPP
#define _ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <variant>
#include <typeinfo>
#include <functional>
#include <numeric>
#include <algorithm>

class Argument;
class SubParsers;

class ArgumentParser
{
public:
    //Calculates its own help message
    ArgumentParser(std::string parserName="");

    Argument& add_argument(std::string shortArgumentName, std::string longArgumentName="");

    void parse_argument(int argc, char* argv[]);
    void parse_argument(std::vector<std::string> parsedArguments);
    void parse_argument(ArgumentParser& argparse, std::vector<std::string> parsedAgruments);

    ArgumentParser& usage(std::string usage);
    ArgumentParser& prog(std::string programName);
    ArgumentParser& help(std::string help);
    ArgumentParser& description(std::string description);
    ArgumentParser& epilog(std::string epilog);
    ArgumentParser& prefix_chars(std::string prefixChars);
    ArgumentParser& from_file_prefix_chars(std::string fromFilePrefixChars);

    void addUsage(std::string msg);
    bool isActive(std::string argumentName);
    void activateArguments(ArgumentParser& parser,const std::string parsedArgument);

    template<typename T>
    std::vector<T> getValues(std::string argumentName) const;

    Argument& getArgument(std::string argumentName);

    std::list<ArgumentParser> getSubParsers()
    {
        return _subParsers;
    }

    ArgumentParser& getSubParser(std::string parserName);
    std::string printAllSubParsers(std::list<ArgumentParser> subParsers) const;
    std::string printSubParser(const ArgumentParser& subParser) const;

    friend std::ostream& operator<<(std::ostream& out, const ArgumentParser& arg_pars);

    //SUBPARSER
    ArgumentParser& add_parser(std::string parserName);
    ArgumentParser& subParsersTitle(std::string subParserTitle);
    ArgumentParser& subParsersDescription(std::string subParserDescription);
    ArgumentParser& subParsersHelp(std::string subParserHelp);

    //for testing purpose
protected:
    std::vector<Argument> _optionalArguments;
    std::vector<Argument> _positionalArguments;
    std::map<std::string, bool> _activeArguments;
    std::map<std::string, bool> _activeSubParsers;
    std::string _programName;
    std::list<ArgumentParser> _subParsers;
    const unsigned helpLength = 15;

    //subparser
    std::string _subParsersTitle;
    std::string _subParsersDescription;
    std::string _subParsersHelp;
    
private:
    void resetParsedValues(ArgumentParser& parser);
    void resetActiveArguments(ArgumentParser& parser);
    void programNameFromArgv(std::string);
    std::vector<std::string> makeSubParserVector(std::vector<std::string> parsedArguments, int currentArgument);
    
    std::string _parserName;
    std::string _usage;
    std::string _help;
    std::string _description;
    std::string _epilog;
    std::vector<ArgumentParser> _parents;
    std::string _prefixChars;
    std::string _fromFilePrefixChars;
    bool _subParserCalled = false;  
};

/**************************************************************************

***************************************************************************/



/**************************************************************************

***************************************************************************/
class Argument
{
public:
    friend class ArgumentParser;
    Argument();

    Argument& name(std::string shortArgumentName, std::string longArgumentName);
    Argument& action(std::string action);
    Argument& nargs(int nargs);
    
    //TODO:lambda function
    template<typename T>
    Argument& def(T value);

    Argument& help(std::string help);
    Argument& metavar(std::string metavar);
    Argument& argumentType(std::string argumentType);
    template<typename T>
    Argument& argumentType()
    {
        if (typeid(T) == typeid(std::string))
            _type = "string";
        else
            _type = typeid(T).name();
        
        return *this;
    }


    template<typename T>
    Argument& constant(T constant)
    {
        _constant = constant;
        return *this;
    }
    
    void actionCheck(const std::string action);
    void performAcion(const std::string action);

    //string representation of the name
    std::string getName() const;
    //vector of all names, need for cases like: "-i", "--info"
    //so "--info" will also be eligible for use even if only
    //"-i" was parsed
    std::vector<std::string> getNames() const;
    std::string getHelp() const;
    std::string getMetavar() const;
    unsigned getNargs() const;

    std::string generateSpaces(std::string argumentName) const;
    void resolveArgumentTypes(std::string argumentType);
    
protected:
    const unsigned helpLength = 15;

private:
    std::vector<std::string> _name;
    std::string _action;
    unsigned _nargs = 0;
    std::variant<std::string, int, bool> _constant;
    std::string _def;
    std::string _help;
    std::string _metavar;
    std::string _type;

    std::vector<std::variant<std::string, int, bool>> _parsedValues;
    //missing type, choice and dest

    std::vector<std::string> allowedActions{ "store", "store_const", "store_true",
                                            "store_false", "append", "append_const",
                                            "count", "help"};
};

//TODO: rename variables
template<typename T>
std::vector<T> ArgumentParser::getValues(std::string argumentName) const
{
    std::vector<T> result;
    for (const auto& arg : _positionalArguments)
    {
        if (arg.getName() == argumentName)
        {
            for (int i = 0; i < arg._parsedValues.size(); i++)
            {
                T tmp = std::get<T>(arg._parsedValues[i]);
                result.push_back(tmp);
            }
        }
    }

    for (const auto& arg : _optionalArguments)
    {
        if (arg.getNames()[0] == argumentName || (arg.getNames()[1] == argumentName))
        {
            for (int i = 0; i < arg._parsedValues.size(); i++)
            {
                T tmp = std::get<T>(arg._parsedValues[i]);
                result.push_back(tmp);
            }
        }
    }

    return result;
}

template<typename T>
Argument& Argument::def(T value)
{


    return *this;
}

#endif // _ARGUMRNT_PARSER_HPP
