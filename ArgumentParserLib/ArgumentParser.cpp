// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"
#include "ArgumentParser.h"


/*********************ARGUMENT_PARSER***********************/

ArgumentParser::ArgumentParser(std::string parserName) : m_parserName(parserName)
{
    //-h --help option by default
    m_optionalArguments.push_back(Argument().isOptional().name("-h", "--help")
        .help("shows this message and exits"));
    m_optionalArguments.push_back(Argument().isOptional().name("-ve", "--version")
        .help("shows version and exits"));
}

//template<typename T>
Argument& ArgumentParser::addArgument(std::string shortParameterName, std::string longParameterName)
{

    if (conflictHandeler(*this, shortParameterName, longParameterName))
        throw std::invalid_argument("Argument: " + shortParameterName + " or " + longParameterName
                                    + " already exists");
    
    if (longParameterName == "") 
    {
        if (checkPrefix(shortParameterName[0]))
        {
            m_optionalArguments.push_back(Argument().isOptional().name(shortParameterName, longParameterName));
            m_activeArguments.insert({ shortParameterName, false });

            return m_optionalArguments.back();
        }
        else
        {
            m_positionalArguments.push_back(Argument().name(shortParameterName, longParameterName));
            m_activeArguments.insert({shortParameterName, false });
            
            return m_positionalArguments.back();
        }
    }
    else {
        if (!checkPrefix(shortParameterName[0]) || !checkPrefix(longParameterName[0])
            || !checkPrefix(longParameterName[1]))
            throw std::invalid_argument("arguments have to begin with '-'");

        m_optionalArguments.push_back(Argument().isOptional().name(shortParameterName, longParameterName));
        
        m_activeArguments.insert({ shortParameterName, false });
        m_activeArguments.insert({ longParameterName, false });
        return m_optionalArguments.back();
    }
}

void ArgumentParser::parseArgument(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << m_usage << std::endl;
        std::exit(0);
    }

    std::string path = argv[0];
    if (path.find('\\') != std::string::npos)
    {
        programNameFromArgv(path);
    }
    else
    {
        m_programName = path;
    }

    std::vector<std::string> parsedArguments;
    //maybe push back optional 1st then positional
    for (size_t i = 1; i < argc; i++)
    {
        parsedArguments.push_back(argv[i]);
    }

    parseArgument(*this,parsedArguments);
}

void ArgumentParser::parseArgument(std::vector<std::string> parsedArguments)
{
    parseArgument(*this, parsedArguments);
}

void ArgumentParser::parseArgument(ArgumentParser& argparse, std::vector<std::string> parsedArguments)
{
    //resetting all values to default
    resetParsedValues(argparse);
    resetActiveArguments(argparse);
    
    unsigned positionalArgumentsCount = 0;
    unsigned optionalArgumentsCount = 0;
    int numOfParsedArguments = parsedArguments.size();
    for (size_t j = 0; j < numOfParsedArguments; ++j)
    {
        //check if it is a subparser
        if (!argparse.m_subParserCalled)
        {
            for (auto subParser = argparse.m_subParsers.begin(); subParser!= argparse.m_subParsers.end(); subParser++)
            {
                if (subParser->m_parserName == parsedArguments[j])
                {
                    argparse.m_subParserCalled = true;
                    argparse.m_activeSubParsers[subParser->m_parserName] = true;
                    parseArgument(*subParser, makeSubParserVector(parsedArguments, j));
                  
                    return;
                }
            }
        }

        if (argparse.m_addHelp)
        {
            auto firstOptional = argparse.m_optionalArguments.front();
            auto secondOptional = argparse.m_optionalArguments[1];
            if (parsedArguments[j] == firstOptional.m_name[0]
                || parsedArguments[j] == firstOptional.m_name[1])
            {
                std::cout << argparse;
                std::exit(0);
            }


            if (parsedArguments[j] == secondOptional.m_name[0]
                || parsedArguments[j] == secondOptional.m_name[1])
            {
                std::cout << "ArgumentParser version 1.0.0";
                std::exit(0);
            }
        }
        //TODO: different arguments for positional
        //TODO: make a function for some of the code
        if (checkPrefix(parsedArguments[j][0]) && !isNegativeNumber(parsedArguments[j])) //optional arguments
        {
            if (argparse.m_abbrevAllowed)
            {
                abbreviationCheck(*this, parsedArguments[j]);
            }
            auto search = argparse.m_activeArguments.find(parsedArguments[j]);
            if(search != argparse.m_activeArguments.end())
            { 
                activateArguments(argparse, parsedArguments[j]);
            }
            else if((search == argparse.m_activeArguments.end()) && !argparse.m_exitOnError)
            {
                throw std::invalid_argument("Invalid argument: " + parsedArguments[j]);
            }
            else
            {
                std::cout << argparse.printUsage() << std::endl;
                exit(0);
            }

            Argument& currentOptionalArgument = argparse.getArgument(parsedArguments[j]);
            auto args = currentOptionalArgument.m_nargs;
            if (args > 0 && (args > numOfParsedArguments - j) && !argparse.m_hasDefaultValue)
                throw std::invalid_argument("To few optional arguments");
            if (args > 0 && (args < numOfParsedArguments - j))
            {
                //clearing default values
                currentOptionalArgument.m_parsedValues.clear();
                for (size_t i = 0; i < args; i++)
                {
                    if (numOfParsedArguments < (j+args-i) && !argparse.m_exitOnError)
                    {
                        throw std::invalid_argument("To few positional arguments");
                    }
                    else if (numOfParsedArguments == j && argparse.m_exitOnError)
                    {
                        std::cout << argparse.printUsage() << std::endl;
                        exit(0);
                    }

                    j++;
                    currentOptionalArgument.resolveArgumentTypes(parsedArguments[j]);
                }

                //j--;
            }
            optionalArgumentsCount++;
        }
        else //positional arguments
        {
            if ((argparse.m_positionalArguments.size() < positionalArgumentsCount+1) && !argparse.m_exitOnError)
            {
                throw std::invalid_argument("To many positional arguments: " + std::to_string(m_positionalArguments.size())
                                            + " expected, but more were parsed");
            }
            else if((argparse.m_positionalArguments.size() < positionalArgumentsCount + 1) && argparse.m_exitOnError)
            {
                std::cout << argparse.printUsage() << std::endl;
                exit(0);
            }

            auto args = argparse.m_positionalArguments[positionalArgumentsCount].m_nargs;
            //clearing defaults
            m_positionalArguments[positionalArgumentsCount].m_parsedValues.clear();
            for (size_t i=0; i < args; i++)
            {
                if ((numOfParsedArguments == j) && !argparse.m_exitOnError)
                {
                    throw std::invalid_argument("To few positional arguments");
                }
                else if ((numOfParsedArguments == j) && argparse.m_exitOnError)
                {
                    std::cout << argparse.printUsage() << std::endl;
                    exit(0);
                }

                argparse.m_positionalArguments[positionalArgumentsCount].resolveArgumentTypes(parsedArguments[j]);
                j++;
            }

            j--;
            positionalArgumentsCount++;
        }
    }
    
    if ((positionalArgumentsCount + 1 < argparse.m_positionalArguments.size()) && !argparse.m_exitOnError)
    {
        throw std::invalid_argument("To few positional arguments");
    }
    else if((positionalArgumentsCount + 1 < argparse.m_positionalArguments.size()) && argparse.m_exitOnError)
    {
        std::cout << argparse.printUsage() << std::endl;
        exit(0);
    }

    checkForRequired(argparse);

}

ArgumentParser& ArgumentParser::usage(std::string usage) 
{
    m_usage = "Usage: ";
    if (!m_programName.empty())
        m_usage += m_programName + " ";
    else
        m_usage += "program.exe ";

    m_usage += usage;
    return *this;
}

ArgumentParser& ArgumentParser::prog(std::string programName)
{
    m_programName = programName;
    return *this;
}

ArgumentParser& ArgumentParser::help(std::string help)
{
    m_help = help;
    return *this;
}

ArgumentParser& ArgumentParser::description(std::string description) 
{
    m_description = description;
    return *this;
}

ArgumentParser& ArgumentParser::epilog(std::string epilog)
{
    m_epilog = epilog;
    return *this;
}

ArgumentParser& ArgumentParser::prefixChars(std::string prefixChars)
{
    m_prefixChars = prefixChars;
    return *this;
}

ArgumentParser& ArgumentParser::addHelp(bool addHelp)
{
    std::vector<Argument> tmp;
    if (!addHelp)
    {
        for (size_t i = 2; i < m_optionalArguments.size(); i++)
        {
            tmp.push_back(m_optionalArguments[i]);
        }
    }
    m_optionalArguments.clear();
    for (size_t i = 0; i < tmp.size(); i++)
    {
        m_optionalArguments.push_back(tmp[i]);
    }
    m_addHelp = addHelp;
    return *this;
}

ArgumentParser& ArgumentParser::argumentDefault(std::variant<std::vector<std::string>, std::vector<int>> defaultArgumentValue)
{
    std::swap(m_defaultArgumentVector, defaultArgumentValue);
    m_hasDefaultValue = true;
    return *this;
}

bool ArgumentParser::conflictHandeler(ArgumentParser& argumentParser, std::string shortArgumentName, std::string longArgumentName)
{

    for (auto& argument : argumentParser.m_optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (shortArgumentName == argumentNames[0] || shortArgumentName == argumentNames[1]
            || ((longArgumentName == argumentNames[0] || longArgumentName == argumentNames[1]) && !longArgumentName.empty()))
            return true;
    }

    for (auto& argument : argumentParser.m_positionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (shortArgumentName == argumentNames[0] || shortArgumentName == argumentNames[1]
            || ((longArgumentName == argumentNames[0] || longArgumentName == argumentNames[1]) && !longArgumentName.empty()))
            return true;
    }

    return false;
}

ArgumentParser& ArgumentParser::allowAbbrev(bool abbrevAllowed)
{
    m_abbrevAllowed = abbrevAllowed;
    return *this;
}

ArgumentParser& ArgumentParser::exitOnError(bool exitOnError)
{
    m_exitOnError = exitOnError;
    return *this;
}

ArgumentParser& ArgumentParser::addParser(std::string parserName)
{
    m_subParsers.push_back(ArgumentParser(parserName));
    m_activeSubParsers.insert({ parserName, false });
    return m_subParsers.back();
}

ArgumentParser& ArgumentParser::subParsersTitle(std::string subParserTitle)
{
    m_subParsersTitle = subParserTitle;
    return *this;
}

ArgumentParser& ArgumentParser::subParsersDescription(std::string subParserDescription)
{
    m_subParsersDescription = subParserDescription;
    return *this;
}

ArgumentParser& ArgumentParser::subParsersHelp(std::string subParserHelp)
{
    m_subParsersHelp = subParserHelp;
    return *this;
}

ArgumentParser& ArgumentParser::getSubParser(std::string parserName)
{
    for (auto& subParser : m_subParsers)
    {
        if (subParser.m_parserName == parserName)
        {
            return subParser;
        }
    }

    throw std::invalid_argument("No such subparser exists");
}

std::string ArgumentParser::printUsage() const
{
    if (!m_usage.empty())
        return m_usage;
    
    std::string result = "Usage: ";

    if (m_programName.empty())
        result += "program.exe ";
    else
        result += m_programName + " ";

    for (auto& argument : m_optionalArguments)
    {
        result += argument.m_usage;
    }

    for (auto& argument : m_positionalArguments)
    {
        result += argument.m_usage;
    }

    if (!m_subParsers.empty())
    {
        result += "{";
        std::string tmp;
        for (auto& subParser : m_subParsers)
        {
            tmp += subParser.m_parserName + ",";
        }

        tmp = tmp.substr(0, tmp.size() - 1);
        result += tmp;
        result += "}";
    }

    return result;
}

std::string ArgumentParser::printHelp() const
{
    std::stringstream buffer;
    buffer << *this;

    return buffer.str();
}

void ArgumentParser::parseKnownArgs(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << m_usage << std::endl;
        std::exit(0);
    }

    std::string path = argv[0];
    if (path.find('\\') != std::string::npos)
    {
        programNameFromArgv(path);
    }
    else
    {
        m_programName = path;
    }

    std::vector<std::string> parsedArguments;
    //maybe push back optional 1st then positional
    for (size_t i = 1; i < argc; i++)
    {
        parsedArguments.push_back(argv[i]);
    }

    parseKnownArgs(parsedArguments);
}

void ArgumentParser::parseKnownArgs(std::vector<std::string> parsedArguments)
{
    std::vector<std::string> result;
    int numberOfPositionalArguments = getNumberOfPositionalArguments();
    for (auto& parsedArgument : parsedArguments)
    {
        if (checkPrefix(parsedArgument[0]))
        {
            if (checkIfOpionalIsAdded(parsedArgument))
            {
                numberOfPositionalArguments += getArgument(parsedArgument).m_nargs;
                result.push_back(parsedArgument);
                continue;
            }
            else
            {
                m_unusedValues.push_back(parsedArgument);
                continue;
            }
        }
        else
        {
            if (numberOfPositionalArguments > 0)
            {
                result.push_back(parsedArgument);
                numberOfPositionalArguments--;
            }
            else
            {
                m_unusedValues.push_back(parsedArgument);
            }
        }
    }

    parseArgument(result);
}

std::vector<std::string> ArgumentParser::getUnusedValues()
{
    return m_unusedValues;
}

/*****************************PRIVATE FUNCTIONS****************************************/
int ArgumentParser::getNumberOfPositionalArguments()
{
    int result = 0;
    for (auto& positionalArgument : m_positionalArguments)
    {
        result += positionalArgument.m_nargs;
    }

    return result;
}

bool ArgumentParser::checkIfOpionalIsAdded(const std::string parsedArgument)
{
    for (auto& argument : m_optionalArguments)
    {
        if (argument.getNames().front() == parsedArgument || argument.getNames().back() == parsedArgument)
            return true;
    }

    return false;
}

void ArgumentParser::checkForRequired(ArgumentParser& argumentParser)
{
    for (auto& argument : argumentParser.m_optionalArguments)
    {
        if (argument.m_required)
        {
            if (!argumentParser.m_activeArguments[argument.getNames()[0]] ||
                (!argumentParser.m_activeArguments[argument.getNames()[1]] && (!argument.getNames()[1].empty())))
            {
                if (argumentParser.m_exitOnError)
                {
                    std::cout << argumentParser.printUsage() << std::endl;
                    exit(0);
                }
                else
                {
                    throw std::invalid_argument("Argument: " + argument.getName() + " is required but isn't parsed");
                }
            }
        }
    }
}

void ArgumentParser::abbreviationCheck(ArgumentParser& argumentParser, std::string& parsedArgument)
{
    int numberOfArgumentsMatched = 0;
    std::vector<std::string> possibleMatches;
    for (auto& argument : argumentParser.m_optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (argumentNames[0].rfind(parsedArgument, 0) == 0) {
            numberOfArgumentsMatched++;
            possibleMatches.push_back(argumentNames[0]);
        }
        if (argument.getNames()[1].rfind(parsedArgument, 0) == 0)
        {
            numberOfArgumentsMatched++;
            possibleMatches.push_back(argumentNames[1]);
        }
    }

    if (numberOfArgumentsMatched == 1)
    {
        parsedArgument = possibleMatches.front();
    }

    if ((numberOfArgumentsMatched > 1) && !argumentParser.m_exitOnError)
    {
        throw std::invalid_argument("Argument " + parsedArgument + " matches more than 1 value");
    }
    else if ((numberOfArgumentsMatched == 0) && !argumentParser.m_exitOnError)
    {
        throw std::invalid_argument("Argument " + parsedArgument + " is parsed but not added");
    }
    else if ((numberOfArgumentsMatched > 1) && argumentParser.m_exitOnError)
    {
        std::cout << argumentParser.printUsage();
        exit(0);
    }
    
}

bool ArgumentParser::isNegativeNumber(std::string parsedArgument)
{
    if (parsedArgument[0] != '-')
        return false;

    for (size_t i = 1; i < parsedArgument.length(); i++)
    {
        if (!std::isdigit(parsedArgument[i]))
            return false;
    }

    return true;
}

bool ArgumentParser::isActive(std::string argumentName)
{
    //if argument and subparser have the same name isActive would not work as intended
    return m_activeArguments[argumentName] || m_activeSubParsers[argumentName];
}

void ArgumentParser::activateArguments(ArgumentParser& parser,const std::string parsedArgument)
{
    for (const auto& argument : parser.m_optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (parsedArgument == argumentNames[0] || parsedArgument == argumentNames[1])
        {
            if (argumentNames[1] != "")
            {
                parser.m_activeArguments[argumentNames[0]] = true;
                parser.m_activeArguments[argumentNames[1]] = true;
            }
            else
                parser.m_activeArguments[parsedArgument] = true;

            return;
        }
    }
}

Argument& ArgumentParser::getArgument(std::string argumentName)
{
    for (auto& argument : m_optionalArguments)
    {
        auto names = argument.getNames();
        if (names[0] == argumentName || names[1] == argumentName)
            return argument;
    }

    throw std::invalid_argument("No such argument exists");
}

bool ArgumentParser::checkPrefix(const char prefix)
{
    if (m_prefixChars.find(prefix) != std::string::npos)
        return true;
    else
        return false;
}

void ArgumentParser::resetParsedValues(ArgumentParser& parser)
{
    for (auto& argument : parser.m_positionalArguments)
    {
        /*for (int i = 0; i < argument._parsedValues.size(); i++)
        {
            argument._parsedValues.pop_back();
        }*/
        argument.m_parsedValues.clear();
        if (argument.m_action == "store_const")
        {
            argument.m_parsedValues.push_back(argument.m_constant);
        }
        else if (argument.m_action == "store_true")
        {
            argument.m_parsedValues.push_back(true);
        }
        else if (argument.m_action == "store_false")
        {
            argument.m_parsedValues.push_back(false);
        }
        else
        {
            //nothing
        }
    }

    for (auto& argument : parser.m_optionalArguments)
    {
        /*for (int i = 0; i < argument._parsedValues.size(); i++)
        {
            argument._parsedValues.pop_back();
        }*/
        argument.m_parsedValues.clear();
        if (argument.m_action == "store_const")
        {
            argument.m_parsedValues.push_back(argument.m_constant);
        }
        else if (argument.m_action == "store_true")
        {
            argument.m_parsedValues.push_back(true);
        }
        else if (argument.m_action == "store_false")
        {
            argument.m_parsedValues.push_back(false);
        }
        else
        {
            //nothing
        }
    }

}

void ArgumentParser::resetActiveArguments(ArgumentParser& parser)
{
    for (auto& [_, v] : parser.m_activeArguments)
        v = false;
}

void ArgumentParser::programNameFromArgv(std::string argv)
{
    std::string program_name = argv;
    std::size_t position = program_name.rfind('\\');

    program_name = program_name.substr(position+1);
    if (m_programName.empty())
    {
        m_programName = program_name;
    }
}

std::vector<std::string> ArgumentParser::makeSubParserVector(std::vector<std::string> parsedArguments, int currentArgument)
{
    int numOfParsedArguments = parsedArguments.size();
    std::vector<std::string>::const_iterator first = parsedArguments.begin() + currentArgument + 1;
    std::vector<std::string>::const_iterator last = parsedArguments.begin() + numOfParsedArguments;
    std::vector<std::string> subParserArguments(first, last);

    return subParserArguments;
}

/********************OPERATORS_FOR_ARGUMENT_PARSER******************/
std::ostream& operator<<(std::ostream& out, const ArgumentParser& arg_pars)
{
    out << arg_pars.printUsage();
    out << "\n\n";

    if (!arg_pars.m_description.empty())
    {
        out << arg_pars.m_description << "\n\n";
    }

    if (!arg_pars.m_positionalArguments.empty() || 
        (!arg_pars.m_subParsers.empty() && arg_pars.m_subParsersTitle.empty()))
    {
        out << "positional arguments:" << "\n";
        
        if (!arg_pars.m_subParsers.empty())
        {
            out << arg_pars.printAllSubParsers(arg_pars.m_subParsers);
        }

        for (const auto& argument : arg_pars.m_positionalArguments)
        {
            std::string spaces = argument.generateSpaces(argument.getName());
            out << " " << argument.getName() << spaces << argument.getHelp() << "\n";
        }
        
        out << "\n";
    }

    if (!arg_pars.m_optionalArguments.empty())
    {
        out << "optional arguments:" << "\n";
        for (const auto& argument : arg_pars.m_optionalArguments)
        {
            std::string spaces = argument.generateSpaces(argument.getName());
            out << " " << argument.getName() << spaces << argument.getHelp() << "\n";
        }

        out << "\n";
    }

    if (!arg_pars.m_subParsersTitle.empty() || !arg_pars.m_subParsersDescription.empty())
    {
        out << "\n";
        if (!arg_pars.m_subParsersTitle.empty())
        {
            out << arg_pars.m_subParsersTitle << ":\n";
        }
        else
        {
            out << "subcommands" << ":\n";
        }

        if (!arg_pars.m_subParsersDescription.empty())
        {
            out << arg_pars.m_subParsersDescription << "\n";
        }

        out << arg_pars.printAllSubParsers(arg_pars.m_subParsers);

        for (auto& subParser : arg_pars.m_subParsers)
        {
            out << subParser.printSubParser();
        }
    }

    if (!arg_pars.m_epilog.empty())
    {
        out <<arg_pars.m_epilog;
    }

    return out;
}


/***************************SUBPARSER*********************************/


std::string ArgumentParser::printAllSubParsers(std::list<ArgumentParser> subParsers) const 
{
    std::string result = " {";
    for (const auto& subParer : subParsers)
    {
        result += subParer.m_parserName + ",";
    }
    result = result.substr(0, result.size() - 1);
    result += "}";
    
    if (result.length() + 1 < helpLength)
    {
        std::string spaces(helpLength - result.length() + 1, ' ');
        result += spaces + m_subParsersHelp + "\n";
    }
    else
    {
        std::string spaces(helpLength, ' ');
        result += "\n" + spaces + m_subParsersHelp + "\n";
    }
    return result;
}

std::string ArgumentParser::printSubParser() const 
{
    std::string result;
    std::string spaces(helpLength - this->m_parserName.length(), ' ');
    result += " " + this->m_parserName + spaces + this->m_help + "\n";

    return result;
}


/***************************ARGUMENT********************************/

Argument::Argument()
{
    
}

Argument& Argument::name(std::string shortArgumentName, std::string longArgumentName)
{
    int lengthShort = helpLength - shortArgumentName.length();
    int lengthLong = helpLength - longArgumentName.length();
    if (lengthShort <= 0 || lengthLong <= 0)
    {
        throw std::invalid_argument("Command name is over" +
            std::to_string(helpLength) + "characters long");
    }
    m_name.push_back(shortArgumentName);
    m_name.push_back(longArgumentName);

    //generating usage
    if (m_isOptional)
        m_usage = '[' + shortArgumentName + "] ";
    else
        m_usage = shortArgumentName + " ";
    
    return *this;
}

//TODO:
Argument& Argument::action(std::string action)
{
    actionCheck(action);
    m_action = action;
    performAcion(action);
    return *this;
}

Argument& Argument::nargs(int nargs)
{   
    if (nargs < 0)
        throw std::invalid_argument("Nargs has to be a positive number");
    m_nargs = nargs;

    //rewriting usage
    if (m_isOptional)
        m_usage = "[" + m_name.front() + " ";
    else
        m_usage = "";
    for (size_t i = 0; i < nargs; i++)
    {
        if (m_metavar == "")
            m_usage += m_name.front() + ' ';
        else
            m_usage += m_metavar + ' ';
    }
    
    if (m_isOptional)
    {
        m_usage = m_usage.substr(0, m_usage.size() - 1);
        m_usage += "] ";
    }

    return *this;
}

Argument& Argument::choices(std::vector<std::variant<std::string, int>> choices)
{
    m_choices = choices;
    return *this;
}

Argument& Argument::required(bool required)
{
    m_required = required;
    return *this;
}

Argument& Argument::help(std::string help)
{
    m_help = help;
    return *this;
}

Argument& Argument::metavar(std::string metavar)
{
    m_metavar = metavar;

    //rewriting usage
    std::string nameForReplacement = m_name.front();
    size_t start_pos = 0;
    while ((start_pos = m_usage.find(nameForReplacement, start_pos)) != std::string::npos) {
        m_usage.replace(start_pos, nameForReplacement.length(), m_metavar);
        start_pos += m_metavar.length();
    }

    return *this;
}

std::string Argument::getName() const 
{
    std::string result;
    result = m_name.front();

    if (!m_name.back().empty())
    {
        result += ", " + m_name.back();
    }

    return result;
}

std::vector<std::string> Argument::getNames() const
{
    return m_name;
}

std::string Argument::getHelp() const
{
    return m_help;
}

std::string Argument::generateSpaces(std::string argumentName) const
{
    std::string result (helpLength-argumentName.length(), ' ');
    return result;
}

Argument& Argument::isOptional()
{
    m_isOptional = true;
    return *this;
}

void Argument::actionCheck(const std::string action)
{
    for (size_t i = 0; i != allowedActions.size(); i++)
    {
        if (action == allowedActions[i])
            return;
    }
    //TODO: maybe it is a different throw
    throw std::invalid_argument("Action: " + action + " is not a valid option");
}

bool Argument::checkForChoices(std::variant<std::string, int> parsedValue)
{
    for (auto& value : m_choices)
    {
        if (value == parsedValue)
            return true;
    }

    return false;
}

//TODO: implement actions
void Argument::performAcion(const std::string action)
{
    if (action == "store_const")
    {
        
        m_parsedValues.push_back(m_constant);
    }
    else if (action == "store_true")
    {
        m_parsedValues.push_back(true);
    }
    else if (action == "store_false")
    {
        m_parsedValues.push_back(false);
    }
    else 
    {
        //nothing        
    }
}

void Argument::resolveArgumentTypes(std::string value)
{
    //no need for conversion if it is a string
    //_parsedValues::value_type s;
    if (std::holds_alternative<std::string>(m_argumentType))
    {
        if (!m_choices.empty())
            if (!checkForChoices(value))
                throw std::invalid_argument("Argument value is different from its choices");
        m_parsedValues.push_back(value);
    }
    else if (std::holds_alternative<int>(m_argumentType))
    {
        //conversion fail?
        int a = std::stoi(value);
        
        if (!m_choices.empty())
            if (!checkForChoices(a))
                throw std::invalid_argument("Argument value is different from its choices");

        m_parsedValues.push_back(a);
    }
    else
    {
        throw std::invalid_argument("Not a right type");
    }
}