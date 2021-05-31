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
#include <sstream>

class Argument;
class SubParsers;

class ArgumentParser
{
public:
    /// <summary>
    /// The first step in using the argparse is creating an ArgumentParser object:
    ///\code{ .cpp }
    /// ArgumentParser arg_parse;
    /// \endcode 
    /// The ArgumentParser object will hold all the information necessary
    /// to parse the command line into C++ data types.
    /// </summary>
    /// <param name="parserName">="" (by default)</param>
    ArgumentParser(std::string parserName="");

    /// <summary>
    /// Filling an ArgumentParser with information about program arguments is done by making
    /// calls to the addArgument() method. Generally, these calls tell the ArgumentParser 
    /// how to take the strings on the command line and turn them into objects. 
    /// This information is stored and used when parseArgs() is called. For example:
    /// \code{.cpp}
    /// parser.addArgument('integers')
    ///         .metavar('N')
    ///         .argumentType<int>()
    ///         .nargs(3)
    ///         .help("an integer for the accumulator");
    /// parser.addArgument("-s", "--sum")
    ///         .action("store_const")
    ///         .help("sum the integers")
    /// \endcode
    /// </summary>
    /// <param name="shortArgumentName">This parameter holds eather full name of a positional argument
    /// or shortcut of a optional argument(shortcut of '--foo' can be '-f'), it needs to begin with '-'.
    /// </param>
    /// <param name="longArgumentName">This parameter is usually empty for posiional arguments. For optional
    /// arguments it contains the full name of an argument, it needs to begin with '--'.
    /// </param>
    /// <returns>Reference to newly created Argument object</returns>
    Argument& addArgument(std::string shortArgumentName, std::string longArgumentName="");

    /// <summary>
    /// Converts contents of argv into string and passes it to
    /// parseArgument(ArgumentParser&, std::vector<std::string>)
    /// \code{.cpp}
    /// parser.parseArgs(argc, argv);
    /// \endcode
    /// </summary>
    /// <param name="argc">Argument count of command-line arguments passed from main function</param>
    /// <param name="argv">Array of character pointers listing all command-line arguments passed from main function</param>
    void parseArgument(int argc, char* argv[]);

    /// <summary>
    /// Passes *this and parsedArguments to parseArgument(ArgumentParser&, std::vector<std::string>),
    /// usefull in cases like:
    /// \code{.cpp}
    /// parser.parseArgs({"-s", "5", "12", "-9"});
    /// \endcode
    /// </summary>
    /// <param name="parsedArguments">Vector of arguments made to mimic command-line</param>
    void parseArgument(std::vector<std::string> parsedArguments);

    /// <summary>
    /// ArgumentParser parses arguments through the parseArgs() method.
    /// This will inspect the command line, convert each argument to the appropriate type 
    /// and then invoke the appropriate action.
    /// </summary>
    /// <param name="argparse">Since subParsers are allso instances of ArgumentParser, we need
    /// to know with wich ArgumentParser we are working with. 
    /// </param>
    /// <param name="parsedAgruments">Vector of arguments parsed from command-line.</param>
    /// @warning Fix argparse descritpion
    void parseArgument(ArgumentParser& argparse, std::vector<std::string> parsedAgruments);

    /// <summary>
    /// By default, ArgumentParser objects use argv[0] to
    /// determine how to display the name of the program in help 
    /// messages. This default is almost always desirable because it will
    /// make the help messages match how the program was invoked on 
    /// the command line. For example, consider a file named myprogram.cpp with the following code:
    /// \code{.cpp}
    /// ArgumentParser parser;
    /// parser.add_argument("--foo").help("foo help");
    /// parser.parseArgs();
    /// \endcode
    /// 
    /// The help for this program will display myprogram.cpp 
    /// as the program name:
    /// 
    /// \code{.cpp}
    /// ./myprogram.exe --help
    /// usage: myprogram.cpp [-h] [--foo FOO]
    ///
    /// optional arguments :
    /// -h, --help  show this help messageand exit
    /// --foo FOO   foo help
    /// \endcode
    /// 
    /// To change this default behavior, another value can be supplied 
    /// using the prog() method on ArgumentParser:
    /// 
    /// \code{.cpp}
    /// ArgumentParser parser;
    /// parser.prog("myProgram")
    /// python myProgram --help
    /// usage: myProgram [-h] [--foo FOO]
    ///
    /// optional arguments :
    /// -h, --help  show this help messageand exit
    /// --foo FOO   foo help
    /// \endcode
    /// </summary>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& prog(std::string programName);

    /// <summary>
    /// 
    /// By default, ArgumentParser calculates the usage message from
    /// the arguments it contains:
    ///
    /// \code{cpp}
    /// parser = argparse.ArgumentParser(prog = 'PROG')
    /// parser.add_argument('--foo', nargs = '?', help = 'foo help')
    /// parser.add_argument('bar', nargs = '+', help = 'bar help')
    /// parser.print_help()
    /// \endcode
    /// 
    /// Output:
    /// \code{cpp}
    /// usage: PROG[-h][--foo[FOO]] bar[bar ...]
    ///
    ///    positional arguments :
    /// bar          bar help
    ///
    ///    optional arguments :
    ///  -h, --help   show this help messageand exit
    ///     --foo[FOO]  foo help
    /// \endcode
    /// 
    /// The default message can be overridden with the usage() method:
    ///
    /// \code{cpp}
    /// parser = argparse.ArgumentParser(prog = 'PROG', usage = '%(prog)s [options]')
    /// parser.add_argument('--foo', nargs = '?', help = 'foo help')
    /// parser.add_argument('bar', nargs = '+', help = 'bar help')
    /// parser.print_help()
    /// \endcode
    /// 
    /// Output:
    /// \code{cpp}
    ///  usage: PROG[options]
    ///
    ///  positional arguments :
    ///   bar          bar help
    ///
    /// optional arguments :
    ///  -h, --help   show this help message and exit
    ///  --foo[FOO]  foo help
    /// \endcode    
    /// </summary>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& usage(std::string usage);

    /// <summary>
    /// Most calls to the ArgumentParser constructor will use the 
    /// description() method argument. This method gives a brief 
    /// description of what the program does and how it works. In help 
    /// messages, the description is displayed between the command-line 
    /// usage string and the help messages for the various arguments:
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.description("A foo that bars");
    /// parser.printHelp();
    /// \endcode
    /// 
    /// Output:
    /// \code{cpp}
    /// usage: argparse.py [-h]
    /// 
    /// A foo that bars
    /// 
    /// optional arguments:
    ///  -h, --help  show this help message and exit
    /// \endcode
    /// </summary>

    /// <param name="description"></param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& description(std::string description);


    /// <summary>
    /// Some programs like to display additional description of the 
    /// program after the description of the arguments. Such text can be 
    /// specified using the epilog() method no ArgumentParser:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.epilog("And that's how you'd foo a bar");
    /// parser.printHelp();
    /// \endcode
    /// 
    /// Output:
    /// \code{cpp}
    /// usage: argparse.py [-h]
    /// 
    /// A foo that bars
    /// 
    /// optional arguments:
    ///  -h, --help  show this help message and exit
    /// 
    /// And that's how you'd foo a bar
    /// \endcode
    /// </summary>
    /// <param name="epilog"></param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& epilog(std::string epilog);

    /// <summary>
    /// Setter for m_help
    /// </summary>
    /// <param name="help"></param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& help(std::string help);

    /// <summary>
    /// Most command-line options will use - as the prefix, e.g. -f/--foo.
    /// Parsers that need to support different or additional prefix 
    /// characters, e.g. for options like +f or /foo, may specify them 
    /// using the prefix_chars() method on the ArgumentParser constructor:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("+f");
    /// parser.addArgument("++bar");
    /// parser.parseArgs({"+f", ++bar});
    /// \endcode
    /// 
    /// The prefix_chars= argument defaults to '-'. Supplying a set of characters
    /// that does not include - will cause -f/--foo options to be disallowed.
    /// </summary>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& prefixChars(std::string prefixChars);

    /// <summary>
    /// Setter for m_fromFilePrefixChars
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @warning Not implemented
    ArgumentParser& fromFilePrefixChars(std::string fromFilePrefixChars);

    /// <summary>
    /// Non vector variant of argumentdefault
    /// </summary>
    /// <param name="defaultArgumentValue">Default value given to all arguments, can be changed if
    /// different value is parsed to argument.
    /// </param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& argumentDefault(std::variant<std::string, int, bool> defaultArgumentValue);
    
    /// <summary>
    /// Generally, argument defaults are specified either by passing a 
    /// default to addArgument(). Sometimes however, it may be useful 
    /// to specify a single parser-wide default for arguments. This can 
    /// be accomplished by passing the argument_default() method on ArgumentParser:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.argumentDefault([1,2,3]);
    /// parser.addArgument("--foo");
    /// parser.addArgument("bar").nargs(3);
    /// \endcode
    /// 
    /// Both '--foo' and 'bar' have values '1','2' and '3' 
    /// </summary>
    /// <param name="defaultArgumentValue">Vector of default values given to all arguments, can be changed
    /// if different set of values is parsed to argument.
    /// </param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& argumentDefault(std::vector<std::variant<std::string, int, bool>> defaultArgumentValue);
    
    /// <summary>
    /// Normally, when you pass an argument list to the parseArgs() 
    /// method of an ArgumentParser, it recognizes abbreviations of long options.
    /// This feature can be disabled by setting allow_abbrev to False:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.allowAbbrev(false);
    /// parser.addArgument("--foobar");
    /// parser.parseArgs({"--foob"});
    /// \endcode
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @warning Only usage appears, error message not implemented
    ArgumentParser& allowAbbrev(bool abbrevAllowed);

    /// <summary>
    /// ArgumentParser objects do not allow two actions with the same 
    /// option string. By default, ArgumentParser objects raise an 
    /// exception if an attempt is made to create an 
    /// argument with an option string that is already in use:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("-f", "--foo");
    /// parser.addArgument("--foo");
    /// \endcode
    /// </summary> 
    /// <returns></returns>
    /// @warning 'resolve' need to be implemented 
    bool conflictHandeler(ArgumentParser& argumentParser, std::string shortArgumentName, std::string longArgumentName);

    /// <summary>
    /// By default, ArgumentParser objects add an option which simply 
    /// displays the parser’s help message. For example, consider a file 
    /// named myprogram.cpp containing the following code:
    /// </summary>
    /// <returns></returns>
    ArgumentParser& addHelp(bool addHelp);

    /// <summary>
    /// Normally, when you pass an invalid argument list to the 
    /// parseArgs() method of an ArgumentParser, it will exit with
    /// error info. If the user would like to catch errors manually, 
    /// the feature can be enabled by callinf exitOnError(false) method:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.exitOnError(false);
    /// parser.addArgument("--integers").argumentType<int>();
    /// try{
    ///     parser.parseArgs({"--integers", 'a'});
    /// } catch{
    ///     std::cout << "Catching argument error";
    /// }
    /// \endcode
    /// </summary>
    /// <returns></returns>
    ArgumentParser& exitOnError(bool exitOnError);

    /// <summary>
    /// After parsing the arguments using parseArgs() method, good way to see 
    /// if certain argument was parsed is by using isActive() method on ArgumentParser.
    /// This can be usefull to specifie what should be done if certain argument
    /// was parsed. This function is mainly used for optional arguments, positioanl arguments
    /// have to be parsed so there is no use in checking them.
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("-f","--foo");
    /// parser.addArgument("bar");
    /// parser.parseArgs({"-f"});
    /// 
    /// if(parser.isActive("-f"))
    ///     std::cout << "'-f' was parsed \n";
    /// 
    /// if(parser.isActive("--foo"))
    ///     std::cout << "'--foo' was parsed \n";
    /// \endcode
    /// 
    /// Output:
    /// 
    /// \code{cpp}
    /// '-f' was parsed
    /// '--foo' was parsed
    /// \endcode
    /// 
    /// Note that even tho "--foo" wasn't parsed it is set as active, that is because
    /// '-f' and '--foo' are the part of the same Argument object, so if one of them
    /// is parsed the other one will allso be considered as parsed.
    /// 
    /// This function can allso be used to se if a certian subParser is parsed.
    /// </summary>
    /// <param name="argumentName">Eather short or long name of a optional argument</param>
    /// @see addArgument(std::string shortArgumentName, std::string longArgumentName="")
    /// <returns>True if argument is in m_activeArguments, false otherwise</returns>
    bool isActive(std::string argumentName);


    /// <summary>
    /// After parsing values to argument object:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("integers").nargs(3).argumentType<int>();
    /// parser.parseArgs({"integers", "1", "2", "3"});
    /// \endcode 
    /// 
    /// Accessing those values parsed to argument "integers" is 
    /// achived by using getValues<type>(argumentName) method on ArgumentParser.
    /// argumentName is the name of argument whos values we want to get,
    /// in this chase argumentName is "integers", type should be the same as
    /// type specified in argumentType<type>() method, in this case type is int.
    /// If types do not match error will occour.
    /// 
    /// \code{cpp}
    /// std::vector<int> values = parser.getValues<int>("integers");
    /// for(auto& value: values)
    ///     std::cout << value << " ";
    /// \endcode
    /// 
    /// Output:
    /// 
    /// \code{cpp}
    /// 1 2 3
    /// \endcode
    /// 
    /// </summary>
    /// <typeparam name="T">Type of value hat needs to be fetched from vector of parsed values</typeparam>
    /// <param name="argumentName">Full positinal argument name or eather long or short optioanl argument name</param>
    /// @see addArgument(std::string shortArgumentName, std::string longArgumentName="")
    /// <returns>Vector of values parsed with the object</returns>
   
    template<typename T>
    std::vector<T> getValues(std::string argumentName) const;

    /// <summary>
    /// Operator<< is used to generate help message, help message gets
    /// printed by eather parsing -h or --help or by printing the whole
    /// ArgumentParser object:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("--foo").help("help message for foo");
    /// parser.addArgument("bar").help("help message for bar");
    /// std::cout << parser;
    /// \endcode
    ///   
    /// Output:
    /// 
    /// \code
    /// usage: frobble [-h] [--foo] bar
    /// 
    /// positional arguments:
    ///  bar     help message for bar
    /// 
    /// optional arguments:
    ///  -h, --help  show this help message and exit
    ///  --foo   help mesage for foo
    /// \endcode
    /// 
    /// </summary>
    /// <returns></returns>
    friend std::ostream& operator<<(std::ostream& out, const ArgumentParser& arg_pars);

    //SUBPARSER

    /// <summary>
    /// Makes a new ArgumentParser object and puts it in m_subParsers
    /// </summary>
    /// <param name="parserName"></param>
    /// <returns>Reference to a newly created ArgumentParser object</returns>
    ArgumentParser& addParser(std::string parserName);

    /// <summary>
    /// Setter for m_subParsersTitle
    /// </summary>
    /// <param name="subParserTitle"></param>
    /// <returns>Reference to current object</returns>
    ArgumentParser& subParsersTitle(std::string subParserTitle);

    /// <summary>
    /// Setter for m_subParserdescrition
    /// </summary>
    /// <param name="subParserDescription"></param>
    /// <returns>Reference to current object</returns>
    ArgumentParser& subParsersDescription(std::string subParserDescription);
    
    /// <summary>
    /// Setter for m_subParsersHelp
    /// </summary>
    /// <param name="subParserHelp"></param>
    /// <returns>Reference to a current object</returns>
    ArgumentParser& subParsersHelp(std::string subParserHelp);
    
    std::string printUsage() const;
    std::string printHelp() const;

    //Should be private
    ArgumentParser& getSubParser(std::string parserName);
    std::string printAllSubParsers(std::list<ArgumentParser> subParsers) const;
    std::string printSubParser(const ArgumentParser& subParser) const;
    Argument& getArgument(std::string argumentName);
    std::list<ArgumentParser> getSubParsers()
    {
        return m_subParsers;
    }


    //for testing purpose
protected:
    std::vector<Argument> m_optionalArguments;
    std::vector<Argument> m_positionalArguments;
    std::map<std::string, bool> m_activeArguments;
    std::map<std::string, bool> m_activeSubParsers;
    std::string m_programName;
    std::list<ArgumentParser> m_subParsers;
    const unsigned helpLength = 15;

    //subparser
    std::string m_subParsersTitle;
    std::string m_subParsersDescription;
    std::string m_subParsersHelp;
    
private:
    void activateArguments(ArgumentParser& parser,const std::string parsedArgument);

    void checkForRequired(ArgumentParser& argumentParser);
    void abbreviationCheck(ArgumentParser& argumentParser,std::string& parsedArgument);
    bool isNegativeNumber(std::string parsedArgument);
    bool checkPrefix(const char prefix);
    void resetParsedValues(ArgumentParser& parser);
    void resetActiveArguments(ArgumentParser& parser);
    void programNameFromArgv(std::string);
    std::vector<std::string> makeSubParserVector(std::vector<std::string> parsedArguments, int currentArgument);
    
    bool m_exitOnError = true;
    bool m_abbrevAllowed = true;
    std::vector<std::variant<std::string, int, bool>> m_defaultArgumentValue;
    bool m_addHelp = true;
    std::string m_parserName;
    std::string m_usage;
    std::string m_help;
    std::string m_description;
    std::string m_epilog;
    std::vector<ArgumentParser> m_parents;
    std::string m_prefixChars = "-";
    std::string m_fromFilePrefixChars;
    bool m_subParserCalled = false;  
};

/**************************************************************************

***************************************************************************/



/**************************************************************************

***************************************************************************/
class Argument
{
public:
    friend class ArgumentParser;

    /// <summary>
    /// Define how a single command-line argument should be parsed.
    /// </summary>
    Argument();

    /// <summary>
    /// The add_argument() method must know whether an optional 
    /// argument, like -f or --foo, or a positional argument, like a 
    /// list of filenames, is expected. The first arguments passed to 
    /// addArgument() must therefore be either a series of flags, or 
    /// a simple argument name. For example, an optional argument 
    /// could be created like:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// //optional argument
    /// parser.addArgument('-f', '--foo');
    /// 
    /// //positional argument
    /// parser.addArgument('bar')
    /// \endcode
    /// 
    /// When parseArgs() is called, optional arguments will be identified 
    /// by the - prefix, and the remaining arguments will be assumed to 
    /// be positional.
    /// 
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @warning Error message not implemented
    Argument& name(std::string shortArgumentName, std::string longArgumentName);

    /// <summary>
    /// Setter for m_action
    /// </summary>
    /// <param name="action"></param>
    /// <returns>Reference to a current object</returns>
    Argument& action(std::string action);

    /// <summary>
    /// ArgumentParser objects usually associate a single command-line 
    /// argument with a single action to be taken. The nargs keyword 
    /// argument associates a different number of command-line arguments 
    /// with a single action.N (an integer). N arguments from the command 
    /// line will be gathered together into a list. For example:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("integers").nargs(3).argumentType<int>();
    /// parser.parseArgs({"integers", "1", "2", "3"});
    /// //1, 2 and 3 are associated with "integers"
    /// \endcode
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @warning '+', '*' and '?' not implemented
    Argument& nargs(int nargs);
    
    /// <summary>
    /// The const argument of addArgument() is used to hold constant 
    /// values that are not read from the command line but are required 
    /// for the various ArgumentParser actions.
    /// </summary>
    /// <returns>Reference to a current object</returns>
    template<typename T>
    Argument& constant(T constant)
    {
        m_constant = constant;
        return *this;
    }
    
    /// <summary>
    /// default in python
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value"></param>
    /// <returns></returns>
    /// @warning Not implemented
    template<typename T>
    Argument& def(T value);

    /// <summary>
    /// By default, the parser reads command-line arguments in as 
    /// simple strings. However, quite often the command-line string 
    /// should instead be interpreted as another type, such as a float or int. 
    /// The type keyword for addArgument() allows any 
    /// necessary type-checking and type conversions to be performed.
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("integers").nargs(3).argumentType<int>();
    /// parser.parserArgs({"integers", "1", "2", "3"});
    /// \endcode
    /// 
    /// In example written above, argument "integers" is associated
    /// with "1", "2" and "3" witch are strings, method parseArgs()
    /// tkes those strings and converts them into types specified in 
    /// argumentType<type>(), in this case it converts them to int
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @warning Currently supported types are int, string and bool
    template<typename T>
    Argument& argumentType()
    {
        //_parsedValues<T>{};
        if (typeid(T) == typeid(std::string))
            m_type = "string";
        else
            m_type = typeid(T).name();
        //_parsed
        return *this;
    }

    /// <summary>
    /// Some command-line arguments should be selected from a 
    /// restricted set of values. These can be handled by passing a 
    /// container object as the choices keyword argument to addArgument(). 
    /// When the command line is parsed, argument values will be checked, 
    /// and an error message will be displayed if the argument was not one 
    /// of the acceptable values:
    /// </summary>
    /// <param name="choices"></param>
    /// <returns></returns>
    /// @warning Not implemented
    Argument& choices(std::vector<std::variant<std::string, int, bool>> choices);

    /// <summary>
    /// In general, the argparse module assumes that flags like 
    /// -f and --bar indicate optional arguments, which can always 
    /// be omitted at the command line. To make an option required, 
    /// method required(true) can be called on Argument object:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.add_argument("--foo").required(true);
    /// parser.parseArgs({});
    /// \endcode
    /// 
    /// Output:
    /// 
    /// \code{cpp}
    /// usage: [-h] --foo FOO
    /// \endcode
    /// </summary>
    /// <returns></returns>
    /// @note Required options are generally considered bad form because users expect options to be optional, and thus they should be avoided when possible.
    /// @warning Error mesage not implemented, only usage appears
    Argument& required(bool required);

    /// <summary>
    /// The help value is a string containing a brief description of the argument. 
    /// When a user requests help (usually by using -h or --help 
    /// at the command line), these help descriptions will be displayed 
    /// with each argument:
    /// 
    /// \code{cpp}
    /// ArgumentParser parser;
    /// parser.addArgument("--foo").help("help message for foo");
    /// parser.addArgument("bar").help("help message for bar");
    /// parser.parseArgs({"-h"})
    /// \endcode
    /// 
    /// Output:
    /// 
    /// \code{cpp}
    /// usage: frobble [-h] [--foo] bar
    /// 
    /// positional arguments:
    ///  bar     help message for bar
    /// 
    /// optional arguments:
    ///  -h, --help  show this help message and exit
    ///  --foo   help mesage for foo
    /// \endcode
    /// </summary>
    /// <returns>Reference to a current object</returns>
    /// @Warning Format specifiers not implemented
    Argument& help(std::string help);

    /// <summary>
    /// When ArgumentParser generates help messages, it needs some 
    /// way to refer to each expected argument. By default, 
    /// ArgumentParser objects use value as the “name” of each object. 
    /// By default, for positional argument actions, the dest value is used directly, 
    /// and for optional argument actions, the dest value is uppercased. 
    /// So, a single positional argument with dest='bar' will be referred to as bar. 
    /// A single optional argument --foo that should be followed by a single command-line 
    /// argument will be referred to as FOO. An example:
    /// 
    /// </summary>
    /// <param name="metavar"></param>
    /// <returns>Reference to a current object</returns>
    Argument& metavar(std::string metavar);

    
    /// <summary>
    /// String representation of the argument name
    /// </summary>
    std::string getName() const;

    /// <summary>
    /// Vector of all names, need for cases like: "-i", "--info"
    /// so "--info" will also be eligible for use even if only
    /// "-i" was parsed
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> getNames() const;

    /// <summary>
    /// Getter for m_help
    /// </summary>
    std::string getHelp() const;

    //Should be private, mabye with (Argument& x, ...)
    std::string generateSpaces(std::string argumentName) const;
    
protected:
    const unsigned helpLength = 15;
    Argument& isOptional();

private:
    void performAcion(const std::string action);
    void actionCheck(const std::string action);
    void resolveArgumentTypes(std::string argumentType);

    std::vector<std::variant<std::string, int, bool>> m_choices;
    bool m_required = false;
    bool m_isOptional = false;
    std::string m_usage;
    std::vector<std::string> m_name;
    std::string m_action;
    unsigned m_nargs = 0;
    std::variant<std::string, int, bool> m_constant;
    std::string m_def;
    std::string m_help;
    std::string m_metavar;
    std::string m_type;

    //variant of vectors
    std::vector<std::variant<std::string, int, bool>> m_parsedValues;
    //std::variant<std::vector<std::string>, std::vector<int>, std::vector<bool>> _parsedValues;


    std::vector<std::string> allowedActions{ "store", "store_const", "store_true",
                                            "store_false", "append", "append_const",
                                            "count", "help"};
};

//TODO: rename variables
template<typename T>
std::vector<T> ArgumentParser::getValues(std::string argumentName) const
{
    std::vector<T> result;
    for (const auto& arg : m_positionalArguments)
    {
        if (arg.getName() == argumentName)
        {
            if (!arg.m_parsedValues.empty()) 
            {
                for (int i = 0; i < arg.m_parsedValues.size(); i++)
                {
                    T tmp = std::get<T>(arg.m_parsedValues[i]);
                    result.push_back(tmp);
                }
                    return result;
            }
            else
            {
                for (int i = 0; i < m_defaultArgumentValue.size(); i++)
                {
                    T tmp = std::get<T>(m_defaultArgumentValue[i]);
                    result.push_back(tmp);
                }
                    return result;
            }
        }
    }

    for (const auto& arg : m_optionalArguments)
    {
        if (arg.getNames()[0] == argumentName || (arg.getNames()[1] == argumentName))
        {
            if (!arg.m_parsedValues.empty())
            {
                for (int i = 0; i < arg.m_parsedValues.size(); i++)
                {
                    T tmp = std::get<T>(arg.m_parsedValues[i]);
                    result.push_back(tmp);
                }
                    return result;
            }
            else
            {
                for (int i = 0; i < m_defaultArgumentValue.size() ; i++)
                {
                    T tmp = std::get<T>(m_defaultArgumentValue[i]);
                    result.push_back(tmp);
                }
                    return result;
            }
        }
    }
}

template<typename T>
Argument& Argument::def(T value)
{


    return *this;
}

#endif // _ARGUMRNT_PARSER_HPP
