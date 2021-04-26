#include "ArgumentParser.h"

/*********************ARGUMENT_PARSER***********************/

ArgumentParser::ArgumentParser()
{
    //-h --help option by default
    _optionalArguments.push_back(Argument().name("-h", "--help")
        .help("shows this message and exits"));
}

//template<typename T>
Argument& ArgumentParser::add_argument(std::string arg)
{   
    std::string msg = "";
    //check if arg begins with '-' or not
    if(arg[0] == '-')
    {
        _optionalArguments.push_back(Argument().name(arg, ""));
        _activeArguments.insert({arg, false});
        
        msg = "[" + arg + "]";
        addUsage(msg);
        
        return _optionalArguments.back();
    }
    else
    {
        _positionalArguments.push_back(Argument().name(arg, ""));
        _activeArguments.insert({ arg, false });
        //TODO: fix usage
        msg += arg;
        unsigned nargs = _positionalArguments.back()._nargs;
        if (nargs != 0) {
            msg += "[";
            for (int i = 0; i < nargs; i++)
            {
                msg += arg + " ";
            }
            msg += "]";
        }
        addUsage(msg);
        return _positionalArguments.back();
    }
}

//template<typename T>
Argument& ArgumentParser::add_argument(std::string shortParameterName, std::string longParameterName)
{
    //TODO: '-' can be redefined by user
    if (shortParameterName[0] != '-' || longParameterName[0] != '-' 
        || longParameterName[1] != '-')
        throw std::invalid_argument("arguments have to begin with '-'");
   
   _optionalArguments.push_back(Argument().name(shortParameterName, longParameterName));
   
   //usage
   std::string msg = "";
   msg = "[" + shortParameterName + "]";
   addUsage(msg);

   //TODO:if arg1 is called from command line, agr2 needs to be true as well
   _activeArguments.insert({shortParameterName, false});
   _activeArguments.insert({longParameterName, false});
    return _optionalArguments.back();
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

    parse_argument(parsedArguments);
}

void ArgumentParser::parse_argument(std::vector<std::string> parsedAgruments)
{
    
    unsigned positionalArgumentsCount = 0;
    //help is allways the first optional argument
    auto firstOptional = _optionalArguments.front();
    for (int j = 0; j < parsedAgruments.size(); ++j)
    {
        if (parsedAgruments[j] == firstOptional._name[0] 
            || parsedAgruments[j] == firstOptional._name[1])
        {
            std::cout << *this;
            std::exit(0);
        }

        //TODO: different arguments for positional
        //TODO: make a function for some of the code
        if (parsedAgruments[j][0] == '-') //optional arguments
        {
            auto search = _activeArguments.find(parsedAgruments[j]);
            if(search != _activeArguments.end())
            { 
                activateArguments(parsedAgruments[j]);
            }
            else
            {
                std::cerr << "Invalid argument: " << parsedAgruments[j] << std::endl;
                std::exit(0);
            }
        }
        else //positional arguments
        {
            if (_positionalArguments.size() < positionalArgumentsCount+1)
            {
                std::cerr << "To many positional arguments: " << _positionalArguments.size()
                    << " expected, but more were parsed"<< std::endl;
                std::exit(0);
            }
            auto args = _positionalArguments[positionalArgumentsCount]._nargs;
            for (unsigned i = 0; i < args; i++)
            { 
                
                _positionalArguments[positionalArgumentsCount].resolveArgumentTypes(parsedAgruments[j]);
                j++;
            }
            j--;
            positionalArgumentsCount++;
        }
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

void ArgumentParser::addUsage(std::string msg)
{
    _usage += msg + " ";
}

bool ArgumentParser::isActive(std::string argumentName)
{
    return _activeArguments[argumentName];
}

void ArgumentParser::activateArguments(const std::string parsedArgument)
{
    for (const auto& argument : _optionalArguments)
    {
        std::vector<std::string> argumentNames = argument.getNames();
        if (parsedArgument == argumentNames[0] || parsedArgument == argumentNames[1])
        {
            if (argumentNames[1] != "")
            {
                _activeArguments[argumentNames[0]] = true;
                _activeArguments[argumentNames[1]] = true;
            }
            else
                _activeArguments[parsedArgument] = true;
        }
    }
}

std::vector<Argument> ArgumentParser::getOptionalArguments() const 
{
    return _optionalArguments;
}

std::vector<Argument> ArgumentParser::getPositionalArguments() const 
{
    return _positionalArguments;
}

std::map<std::string, bool> ArgumentParser::getActiveArguments() const 
{
    return _activeArguments;
}

std::string ArgumentParser::getProgramName() const
{
    return _programName;
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

std::string Argument::generateSpaces(std::string argumentName) const 
{
    std::string result = "";
    int length = helpLength - argumentName.length();
    if (length<= 0)
    {
        throw std::invalid_argument("command name is over" +
            std::to_string(helpLength) + "characters long");
    }

    for (unsigned i = 0; i < length; i++)
    {
        result += " ";
    }

    return result;
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
    
    //out << arg_pars._usage;
    out << "\n\n";

    if (!arg_pars._description.empty())
    {
        out << arg_pars._description << "\n\n";
    }

    if (!arg_pars._positionalArguments.empty())
    {
        out << "positional arguments:" << "\n";
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

    return out;
}

/***************************ARGUMENT********************************/

Argument::Argument()
{

}

Argument& Argument::name(std::string shortArgumentName, std::string longArgumentName)
{
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
        throw std::invalid_argument("nargs has to be a positive number");
    _nargs = nargs;
    return *this;
}

Argument& Argument::def(std::string def)
{
    _def = def;
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

void Argument::actionCheck(const std::string action)
{
    for (int i = 0; i != allowedActions.size(); i++)
    {
        if (action == allowedActions[i])
            return;
    }
    //TODO: maybe it is a differend throw
    throw std::invalid_argument("no such action exists");
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
        std::cerr << "Invalid action: " << action << std::endl;
        std::exit(0);
    }
}

void Argument::resolveArgumentTypes(std::string value)
{
    //no need for conversion if it is a string
    if (_type == "string")
    {
        _parsedValues.push_back(value);
    }

    if (_type == "int")
    {
        //conversion fail?
        int a = std::stoi(value);
        _parsedValues.push_back(a);
    }
}