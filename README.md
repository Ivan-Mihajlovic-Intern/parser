# Argument Parser

##Description
Pythons argparse library implemented in C++

## Building code using Cmake:


## Code example:

### Positional arguments

```cpp
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
    
    arg_pars.parseArgument({ "5", "7", "10"});
    
    
	std::vector<int> args = arg_pars.getValues<int>("integers");
	int sum = 0;
	for (const auto& arg : args)
	{
		sum += arg;
	}
	std::cout << sum << "\n";
    
    return 0;
}
```

Output:

```bash
22
```

### Optional arguments

```cpp
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
    
    //optional argument
    arg_pars.addArgument("-s", "--sum")
			.help("sum of integers");        
    
    arg_pars.parseArgument({"-s", "5", "7", "10"});
    
    if(arg_pars.isActive("--sum"))
    {
    	std::vector<int> args = arg_pars.getValues<int>("integers");
    	int sum = 0;
    	for (const auto& arg : args)
    	{
            sum += arg;
    	}
    	std::cout << sum << "\n";
    }
	else
	{
		std::cout << "'--sum' isn't parsed" << "\n";
	}
    return 0;
}
```

Output:

```bash
22
```

### Negative Numbers

```cpp
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
    
    //optional argument
    arg_pars.addArgument("-s", "--split")
			.help("splits negative and positive integers");        
    
    arg_pars.parseArgument({"-s", "-5", "-7", "10"});
    
    if(arg_pars.isActive("-s"))
    {
    	std::vector<int> args = arg_pars.getValues<int>("integers");
    	for (const auto& arg : args)
    	{
            if(arg < 0)
				std::cout << arg << " ";
    	}
    	std::cout << "\n";
    }
	
    return 0;
}
```

Output:

```bash
-5 -7
```

### Subparsers

```cpp
#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser parser;
    parser.prog("GuidanceGen")
        .usage("command [options]")
        .description("Generate guidance localization files");

    //dest='...' not implemented
    parser.addArgument("-s", "--settings")
        .metavar("SETINGS")
        .argumentType<std::string>()
        .nargs(1)
        .def<std::string>("settings.ini")
        .required(true)
        .help("Settings file.");

    parser.addArgument("-v", "--verbose")
        .help("Increase output verbosity")
        .action("store_true");

    parser.subParsersTitle("Operations")
        .subParsersHelp("Possible operations");

    ArgumentParser& parser_a = parser.addParser("generate")
        .help("Generate guidance files up to supported number of locales based on input.");

    parser_a.exitOnError(false);
    parser_a.addArgument("-r", "--release")
        .metavar("RELEASE")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Release version in format XX.YY (40.11).");

    parser_a.addArgument("-i", "--input")
        .metavar("INPUT_PATH")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Input path where source guidance file is located.");

    parser_a.addArgument("-d", "--destination")
        .metavar("DESTINATION_PATH")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Destination path to generate guidance files up to supported number of locales.");

    parser.addParser("parse")
        .help("Parse single guidance file based on input.")
        .addArgument("-i", "--input")
        .metavar("INPUT_PAT")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Path to input file.");

    parser.addParser("validate")
        .help("Validate guidance files in input path location.")
        .addArgument("-c", "--channel")
        .metavar("CHANNEL")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Promotion channel.");

    parser.addParser("build")
        .help("Build output aggregated \"userguidance.json\".")
        .addArgument("-c", "--channel")
        .metavar("CHANNEL")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Promotion channel.");
       
    parser.addParser("update")
        .help("Update guidance files according to \"en\" locale.")
        .addArgument("-i", "--input")
        .metavar("INPUT_PATH")
        .argumentType<std::string>()
        .nargs(1)
        .required(true)
        .help("Promotion channel.");

    parser.parseArgument(argc, argv);
	
    return 0;
}
```


