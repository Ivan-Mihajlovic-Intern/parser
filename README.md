# Argument Parser

##Description
Pythons argparse library implemented in C++

##Building code using Cmake:
...

##Code example:

```cpp
#include "ArgumentParser.h"

int main(int argc, char* argv[])
{
    ArgumentParser arg_pars;
    
    //positional
    arg_pars.addHelp(false);
    arg_pars.exitOnError(false);
    arg_pars.argumentDefault({365, 366});
    arg_pars.prefixChars("-+");
    
    //positional
    arg_pars.addArgument("integers").help("int argument required").nargs(3).metavar("x").argumentType<int>().choices({1,2,3});
    
    //optional
    arg_pars.addArgument("-s", "--summ").help("sum of integers");        
    
    arg_pars.parseArgument({"-s", "5", "7", "-10"});
    
    if(arg_pars.isActive("--summ"))
    {
    	std::vector<int> args = arg_pars.getValues<int>("N");
    	auto userName = subSubParser.getValues<std::string>("--user.name");
    	int sum = 0;
    	for (const auto& arg : args)
    	{
            std::cout << "N: " << arg << std::endl;
            sum += arg;
    	}
    	std::cout << sum << "\n";
    }
    return 0;
}
```

Output:

```bash
2
```

