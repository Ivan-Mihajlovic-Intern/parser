#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument Parser required", "[required]")
{
	GIVEN("Argument parser and argument 'integers'")
	{
		ArgumentTest parser;
		
		parser.exitOnError(false);
		parser.addArgument("integers").nargs(3).argumentType<int>();
		
		WHEN("argument '-s, --sum' is added with required is set to true")
		{
			parser.addArgument("-s", "--sum").required(true);
			THEN("parsing '--summ, 1, 56, -17' will not result in a error")
			{
				REQUIRE_NOTHROW(parser.parseArgument({ "--sum", "1", "56", "-17" }));
			}
		}

		WHEN("argument '-s, --sum' is added with required is set to true")
		{
			parser.addArgument("-s", "--sum").required(true);
			THEN("parsing '1, 56, -17' will result in a error")
			{
				REQUIRE_THROWS(parser.parseArgument({ "1", "56", "-17" }));
			}
		}
		
	}
}