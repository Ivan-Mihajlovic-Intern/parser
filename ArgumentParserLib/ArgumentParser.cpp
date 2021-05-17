#include "pch.h"
#include "ArgumentParser.h"


/*********************ARGUMENT_PARSER***********************/

ArgumentParser::ArgumentParser(std::string parserName) : _parserName(parserName)
{
    //-h --help option by default
    _optionalArguments.push_back(Argument().name("-h", "--help")
        .help("shows this message and exits"));
    _optionalArguments.push_back(Argument().name("-v", "--version")
        .help("shows version and exits"));
}

//template<typename T>
Argument& ArgumentParser::add_argument(std::string shortParameterName, std::string longParameterName)
{

    std::string msg = "";
    //check if arg begins with '-' or not
    if (longParameterName == "") 
    {
        if (shortParameterName[0] == '-')
        {
            _optionalArguments.push_back(Argument().name(shortParameterName, longParameterName));
            _activeArguments.insert({shortParameterName, false });
            
            msg = "[" + shortParameterName + "]";
            addUsage(msg);

            return _optionalArguments.back();
        }
        else
        {
            _positionalArguments.push_back(Argument().name(shortParameterName, longParameterName));
            _activeArguments.insert({shortParameterName, false });
            //TODO: fix usage
            msg += shortParameterName;
            unsigned nargs = _positionalArguments.back()._nargs;
            if (nargs != 0) {
                msg += "[";
                for (int i = 0; i < nargs; i++)
                {
                    msg += shortParameterName + " ";
                }
                msg += "]";
            }
            addUsage(msg);
            return _positionalArguments.back();
        }
    }
    else {
        //TODO: '-' can be redefined by user
        if (shortParameterName[0] != '-' || longParameterName[0] != '-'
            || longParameterName[1] != '-')
            throw std::invalid_argument("arguments have to begin with '-'");

        _optionalArguments.push_back(Argument().name(shortParameterName, longParameterName));

        //usage
        msg = "[" + shortParameterName + "]";
        addUsage(msg);

        //TODO:if arg1 is called from command line, agr2 needs to be true as well
        _activeArguments.insert({ shortParameterName, false });
        _activeArguments.insert({ longParameterName, false });
        return _optionalArguments.back();
    }
}

void ArgumentParser::parse_argument(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << _usage << std::endl;
        std::exit(0);
    }

    std::string path = argv[0];
    if (path.find('\\') != std::string::npos)
    {
        programNameFromArgv(path);
    }
    else
    {
        _programName = path;
    }

    std::vector<std::string> parsedArguments;
    //maybe push back optional 1st then positional
    for (int i = 1; i < argc; i++)
    {
        parsedArguments.push_back(argv[i]);
    }

    parse_argument(*this,parsedArguments);
}

void ArgumentParser::parse_argument(std::vector<std::string> parsedArguments)
{
    parse_argument(*this, parsedArguments);
}

void ArgumentParser::parse_argument(ArgumentParser& argparse, std::vector<std::string> parsedAgruments)
{
    //resetting all values to default
    resetParsedValues(argparse);
    resetActiveArguments(argparse);
    
    unsigned positionalArgumentsCount = 0;
    unsigned optionalArgumentsCount = 0;
    unsigned subParserCount = 0;
    //help is allways the first optional argument
    auto firstOptional = argparse._optionalArguments.front();
    //version is allways the second optional argument
    auto secondOptional = argparse._optionalArguments[1];
    auto numOfParsedArguments = parsedAgruments.size();
    for (int j = 0; j < numOfParsedArguments; ++j)
    {
        //check if it is subparser
        if (!argparse._subParserCalled)
        {
            for (auto subParser = argparse._subParsers.begin(); subParser!= argparse._subParsers.end(); subParser++)
            {
                if (subParser->_parserName == parsedAgruments[j])
                {
                    argparse._subParserCalled = true;
                    argparse._activeSubParsers[subParser->_parserName] = true;
                    parse_argument(*subParser, makeSubParserVector(parsedAgruments, j));
                    
                    //argparse._subParsers.push_back(*subParser);
                    //argparse._subParsers.erase(subParser);
                    
                    //subparsed is added last so we exit the function when its parse is done
                    return;
                }
                subParserCount++;
            }
        }

        if (parsedAgruments[j] == firstOptional._name[0] 
            || parsedAgruments[j] == firstOptional._name[1])
        {
            std::cout << argparse;
            std::exit(0);
        }

        if (parsedAgruments[j] == secondOptional._name[0]
            || parsedAgruments[j] == secondOptional._name[1])
        {
            std::cout << "TODO: version";
            std::exit(0);
        }

        //TODO: different arguments for positional
        //TODO: make a function for some of the code
        if (parsedAgruments[j][0] == '-') //optional arguments
        {
            auto search = argparse._activeArguments.find(parsedAgruments[j]);
            if(search != argparse._activeArguments.end())
            { 
                activateArguments(argparse, parsedAgruments[j]);
            }
            else
            {
                throw std::invalid_argument("Invalid argument: " + parsedAgruments[j]);
            }

            Argument& currentOptionalArgument = argparse.getArgument(parsedAgruments[j]);
            auto args = currentOptionalArgument._nargs;
            if (args > 0)
            {
                unsigned i;
                for (i = 0; i < args; i++)
                {
                    if (numOfParsedArguments == j)
                    {
                        throw std::invalid_argument("To fiew positional arguments");
                    }

                    j++;
                    currentOptionalArgument.resolveArgumentTypes(parsedAgruments[j]);
                }

                //j--;
            }
            optionalArgumentsCount++;
        }
        else //positional arguments
        {
            if (argparse._positionalArguments.size() < positionalArgumentsCount+1)
            {
                throw std::invalid_argument("To many positional arguments: " + std::to_string(_positionalArguments.size())
                                            + " expected, but more were parsed");
            }
            
            auto args = argparse._positionalArguments[positionalArgumentsCount]._nargs;
            unsigned i;
            for (i=0; i < args; i++)
            {
                if (numOfParsedArguments == j)
                {
                    throw std::invalid_argument("To fiew positional arguments");
                }

                argparse._positionalArguments[positionalArgumentsCount].resolveArgumentTypes(parsedAgruments[j]);
                j++;
            }

            j--;
            positionalArgumentsCount++;
        }
    }
    
    if (positionalArgumentsCount + 1 < argparse._positionalArguments.size())
    {
        throw std::invalid_argument("To fiew positional arguments");
    }

}

ArgumentParser& ArgumentParser::usage(std::string usage) 
{
    _usage = usage;
    return *this;
}

ArgumentParser& ArgumentParser::prog(std::string programName)
{
    _programName = programName;
    return *this;
}

ArgumentParser& ArgumentParser::help(std::string help)
{
    _help = help;
    return *this;
}

ArgumentParser& ArgumentParser::description(std::string description) 
{
    _description = description;
    return *this;
}

ArgumentParser& ArgumentParser::epilog(std::string epilog)
{
    _epilog = epilog;
    return *this;
}

ArgumentParser& ArgumentParser::prefix_chars(std::string prefixChars)
{
    _prefixChars = prefixChars;
    return *this;
}

ArgumentParser& ArgumentParser::from_file_prefix_chars(std::string fromFilePrefixChars)
{
    _fromFilePrefixChars = fromFilePrefixChars;
    return *this;
}

ArgumentParser& ArgumentParser::add_parser(std::string parserName)
{
    _subParsers.push_back(ArgumentParser(parserName));
    _activeSubParsers.insert({ parserName, false });
    return _subParsers.back();
}

ArgumentParser& ArgumentParser::subParsersTitle(std::string subParserTitle)
{
    _subParsersTitle = subParserTitle;
    return *this;
}

ArgumentParser& ArgumentParser::subParsersDescription(std::string subParserDescription)
{
    _subParsersDescription = subParserDescription;
    return *this;
}

ArgumentParser& ArgumentParser::subParsersHelp(std::string subParserHelp)
{
    _subParsersHelp = subParserHelp;
    return *this;
}

ArgumentParser& ArgumentParser::getSubParser(std::string parserName)
{
    for (auto& subParser : _subParsers)
    {
        if (subParser._parserName == parserName)
        {
            return subParser;
        }
    }

    throw std::invalid_argument("No such subparser exists");
}

void ArgumentParser::addUsage(std::string msg)
{
    _usage += msg + " ";
}

bool ArgumentParser::isActive(std::string argumentName)
{
    //if argument and subparser have the same name isActive would not work as inteded
    if (_activeArguments[argumentName] && _activeSubParsers[argumentName])
        throw std::invalid_argument("Argument and subparser of same name are parsed");

    return _activeArguments[argumentName] || _activeSubParsers[argumentName];
}

void ArgumentParser::activateArguments(ArgumentParser& parser,const std::string parsedArgument)
{
    for (const auto& argument : parser._optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (parsedArgument == argumentNames[0] || parsedArgument == argumentNames[1])
        {
            if (argumentNames[1] != "")
            {
                parser._activeArguments[argumentNames[0]] = true;
                parser._activeArguments[argumentNames[1]] = true;
            }
            else
                parser._activeArguments[parsedArgument] = true;

            return;
        }
    }
}

Argument& ArgumentParser::getArgument(std::string argumentName)
{
    for (auto& argument : _optionalArguments)
    {
        auto names = argument.getNames();
        if (names[0] == argumentName || names[1] == argumentName)
            return argument;
    }

    for (auto& argument : _positionalArguments)
    {
        if (argument.getName() == argumentName)
            return argument;
    }

    throw std::invalid_argument("No such argument exists");
}

void ArgumentParser::resetParsedValues(ArgumentParser& parser)
{
    for (auto& argument : parser._positionalArguments)
    {
        /*for (int i = 0; i < argument._parsedValues.size(); i++)
        {
            argument._parsedValues.pop_back();
        }*/
        argument._parsedValues.clear();
        if (argument._action == "store_const")
        {
            argument._parsedValues.push_back(argument._constant);
        }
        else if (argument._action == "store_true")
        {
            argument._parsedValues.push_back(true);
        }
        else if (argument._action == "store_false")
        {
            argument._parsedValues.push_back(false);
        }
        else
        {
            //nothing
        }
    }

    for (auto& argument : parser._optionalArguments)
    {
        /*for (int i = 0; i < argument._parsedValues.size(); i++)
        {
            argument._parsedValues.pop_back();
        }*/
        argument._parsedValues.clear();
        if (argument._action == "store_const")
        {
            argument._parsedValues.push_back(argument._constant);
        }
        else if (argument._action == "store_true")
        {
            argument._parsedValues.push_back(true);
        }
        else if (argument._action == "store_false")
        {
            argument._parsedValues.push_back(false);
        }
        else
        {
            //nothing
        }
    }

}

void ArgumentParser::resetActiveArguments(ArgumentParser& parser)
{
    /*for (const auto& argument : parser._optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
            if (argumentNames[1] != "")
            {
                parser._activeArguments[argumentNames[0]] = false;
                parser._activeArguments[argumentNames[1]] = false;
            }
            else
                parser._activeArguments[argumentNames[1]] = false;

    }*/

    for (auto& [_, v] : parser._activeArguments)
        v = false;
}

void ArgumentParser::programNameFromArgv(std::string argv)
{
    std::string program_name = argv;
    std::size_t position = program_name.rfind('\\');

    program_name = program_name.substr(position+1);
    if (_programName.empty())
    {
        _programName = program_name;
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
    out << "Usage: " << arg_pars._programName << " ";

    for (const auto& argument : arg_pars._optionalArguments)
    {
        std::size_t comaPosition = argument.getName().find(',');
        out << "[" << argument.getName().substr(0, comaPosition) << "]" << " ";
    }
    
    for (auto& argument : arg_pars._positionalArguments)
    {
        if (argument.getNargs() > 1)
        {
            for (int i = 0; i < argument.getNargs(); i++)
            {
                if (argument.getMetavar().empty())
                    out << argument.getName() << " ";
                else
                    out << argument.getMetavar() << " ";
            }
        }

        if(argument.getNargs() == 1)
           out << argument.getName() << " ";
    }

    out << "{";
    std::string tmp;
    for (auto& subParser : arg_pars._subParsers)
    {
        tmp +=  subParser._parserName + ",";
        
    }

    tmp = tmp.substr(0, tmp.size() - 1);
    out << tmp;
    out << "}";

    //out << arg_pars._usage;
    out << "\n\n";

    if (!arg_pars._description.empty())
    {
        out << arg_pars._description << "\n\n";
    }

    if (!arg_pars._positionalArguments.empty() || 
        (!arg_pars._subParsers.empty() && arg_pars._subParsersTitle.empty()))
    {
        out << "positional arguments:" << "\n";
        
        if (!arg_pars._subParsers.empty())
        {
            out << arg_pars.printAllSubParsers(arg_pars._subParsers);
        }

        for (const auto& argument : arg_pars._positionalArguments)
        {
            std::string spaces = argument.generateSpaces(argument.getName());
            out << " " << argument.getName() << spaces << argument.getHelp() << "\n";
        }
        
        out << "\n";
    }

    if (!arg_pars._optionalArguments.empty())
    {
        out << "optional arguments:" << "\n";
        for (const auto& argument : arg_pars._optionalArguments)
        {
            std::string spaces = argument.generateSpaces(argument.getName());
            out << " " << argument.getName() << spaces << argument.getHelp() << "\n";
        }

        out << "\n";
    }

    if (!arg_pars._subParsersTitle.empty() || !arg_pars._subParsersDescription.empty())
    {
        out << "\n";
        if (!arg_pars._subParsersTitle.empty())
        {
            out << arg_pars._subParsersTitle << ":\n";
        }
        else
        {
            out << "subcommands" << ":\n";
        }

        if (!arg_pars._subParsersDescription.empty())
        {
            out << arg_pars._subParsersDescription << "\n";
        }

        out << arg_pars.printAllSubParsers(arg_pars._subParsers);

        for (auto& subParser : arg_pars._subParsers)
        {
            out << subParser.printSubParser(subParser);
        }

    }

    return out;
}


/***************************SUBPARSER*********************************/


std::string ArgumentParser::printAllSubParsers(std::list<ArgumentParser> subParsers) const 
{
    std::string result = " {";
    for (const auto& subParer : subParsers)
    {
        result += subParer._parserName + ",";
    }
    result = result.substr(0, result.size() - 1);
    result += "}";
    
    std::string spaces(helpLength - result.length() + 1, ' ');
    result +=  spaces + _subParsersHelp + "\n";

    return result;
}

std::string ArgumentParser::printSubParser(const ArgumentParser& subParser) const 
{
    std::string result;
    std::string spaces(helpLength - subParser._parserName.length(), ' ');
    result += " " + subParser._parserName + spaces + subParser._help + "\n";

    return result;
}


/***************************ARGUMENT********************************/

Argument::Argument() : _nargs(0)
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
    _name.push_back(shortArgumentName);
    _name.push_back(longArgumentName);
    return *this;
}

//TODO:
Argument& Argument::action(std::string action)
{
    actionCheck(action);
    _action = action;
    performAcion(action);
    return *this;
}

Argument& Argument::nargs(int nargs)
{   
    if (nargs < 0)
        throw std::invalid_argument("Nargs has to be a positive number");
    _nargs = nargs;
    return *this;
}

Argument& Argument::help(std::string help)
{
    _help = help;
    return *this;
}

//maybe different type for metavar
Argument& Argument::metavar(std::string metavar)
{
    _metavar = metavar;
    return *this;
}

Argument& Argument::argumentType(std::string argumentType)
{
    _type = argumentType;
    return *this;
}

std::string Argument::getName() const 
{
    std::string result;
    result = _name.front();

    if (!_name.back().empty())
    {
        result += ", " + _name.back();
    }

    return result;
}

std::vector<std::string> Argument::getNames() const
{
    return _name;
}

std::string Argument::getHelp() const
{
    return _help;
}

//TODO: add to _usage
unsigned Argument::getNargs() const
{
    return _nargs;
}

std::string Argument::getMetavar() const
{
    return _metavar;
}

std::string Argument::generateSpaces(std::string argumentName) const
{
    std::string result (helpLength-argumentName.length(), ' ');
    return result;
}

void Argument::actionCheck(const std::string action)
{
    for (int i = 0; i != allowedActions.size(); i++)
    {
        if (action == allowedActions[i])
            return;
    }
    //TODO: maybe it is a differend throw
    throw std::invalid_argument("Action: " + action + " is not a valid option");
}

//TODO: implement actions
void Argument::performAcion(const std::string action)
{
    if (action == "store_const")
    {
        
        _parsedValues.push_back(_constant);
    }
    else if (action == "store_true")
    {
        _parsedValues.push_back(true);
    }
    else if (action == "store_false")
    {
        _parsedValues.push_back(false);
    }
    else if (action == "append")
    {

    }
    else if (action == "append_const")
    {

    }
    else if (action == "count")
    {

    }
    else if (action == "help")
    {
        
    }
    else 
    {
        //nothing        
    }
}

void Argument::resolveArgumentTypes(std::string value)
{
    //no need for conversion if it is a string
    if (_type == "string")
    {
        _parsedValues.push_back(value);
    }
    else if (_type == "int")
    {
        //conversion fail?
        int a = std::stoi(value);
        _parsedValues.push_back(a);
    }
    else
    {
        throw std::invalid_argument("Not a right type");
    }
}