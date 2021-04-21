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
Argument& ArgumentParser::add_argument(std::string arg1, std::string arg2)
{
    //check if arg1 and arg2 begins with '-'
    //TODO: '-' can be redefined by user
    if (arg1[0] != '-' || arg2[0] != '-' || arg2[1] != '-')
        throw std::invalid_argument("arguments have to begin with '-'");
   
   _optionalArguments.push_back(Argument().name(arg1, arg2));
   
   //usage
   std::string msg = "";
   msg = "[" + arg1 + "]";
   addUsage(msg);

   //TODO:if arg1 is called from command line, agr2 needs to be true as well
   _activeArguments.insert({arg1, false});
   _activeArguments.insert({arg2, false});
    return _optionalArguments.back();
}

void ArgumentParser::parse_argument(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << _usage << std::endl;
        std::exit(0);
    }

    std::vector<std::string> parsedArguments;
    //maybe push back optional 1st then positional
    for (int i = 0; i < argc; i++)
    {
        parsedArguments.push_back(argv[i]);
    }

    parse_argument(parsedArguments);
}

void ArgumentParser::parse_argument(std::vector<std::string> parsedAgruments)
{
    std::string path = parsedAgruments.front();
    if (path.find('\\') != std::string::npos)
    {
        programNameFromArgv(path);
    }
    else
    {
        _programName = path;
    }

    unsigned posArgsCount = 0;
    for (int j = 1; j < parsedAgruments.size(); ++j)
    {
        //help is allways the first optional argument
        auto firstOptional = _optionalArguments.front();
        if (parsedAgruments[j] == firstOptional._name[0] 
            || parsedAgruments[j] == firstOptional._name[1])
        {
            std::cout << *this;
            std::exit(0);
        }

        //if it is an optional argument
        //TODO: wrong input and different arguments for positional
        //TODO: make a function for some of the code
        if (parsedAgruments[j][0] == '-') 
        {
            _activeArguments[parsedAgruments[j]] = true;
        }
        else
        {
            if (_positionalArguments.size() < posArgsCount+1)
            {
                std::cerr << "To much positional arguments" << std::endl;
                std::exit(0);
            }
            auto args = _positionalArguments[posArgsCount]._nargs;
            for (unsigned i = 0; i < args; i++)
            { 
                _positionalArguments[posArgsCount]._positionalValues.push_back(parsedAgruments[j]);
                j++;
            }
            j--;
            posArgsCount++;
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

std::vector<Argument> ArgumentParser::getOptionalArguments()
{
    return _optionalArguments;
}

std::vector<Argument> ArgumentParser::getPositionalArguments()
{
    return _positionalArguments;
}

std::map<std::string, bool> ArgumentParser::getActiveArguments()
{
    return _activeArguments;
}

std::string ArgumentParser::getProgramName()
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

std::string Argument::generateSpaces(std::string name) const 
{
    std::string result = "";
    int length = helpLength - name.length();
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

    //TODO:nargs changes the output
    for (auto& argument : arg_pars._positionalArguments)
    {
        if (argument.getNargs() > 1)
        {
            if (argument.getMetavar().empty())
                out << argument.getName() << " " << "[" << argument.getName() << " ...] ";
            else
                out << argument.getMetavar() << " " << "[" << argument.getMetavar() << " ...] ";
        }
        else
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
            //TODO:equal blank spaces between name and help for all args
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
            //TODO:equal blank spaces between name and help for all args
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

Argument& Argument::name(std::string arg1, std::string arg2)
{
    _name.push_back(arg1);
    _name.push_back(arg2);
    return *this;
}

//TODO:
Argument& Argument::action(std::string action)
{
    actionCheck(action);
    _action = action;
    return *this;
}

Argument& Argument::nargs(int nargs)
{   
    if (nargs < 0)
        throw std::invalid_argument("nargs has to be a positive number");
    _nargs = nargs;
    return *this;
}

Argument& Argument::constant(std::string constant)
{
    _constant = constant;
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

std::string Argument::getHelp() const
{
    return _help;
}

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