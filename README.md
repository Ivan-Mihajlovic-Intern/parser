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



