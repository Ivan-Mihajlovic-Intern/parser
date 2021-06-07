#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument Parser addHelp", "[addHelp]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;
		WHEN("Arguments '-s, --sum' and '-f' are added and addHelp is set to false")
		{

			parser.addArgument("-s", "--sum");
			parser.addArgument("-f");
			parser.addHelp(false);

			THEN("'-h, --help' and '-v, --version' are not in added by default")
			{
				
				const auto result1 = parser.getOptionalArguments().front().getName();
				const auto result2 = parser.getOptionalArguments()[1].getName();

				const auto expected1 = "-h, --help";
				const auto expected2 = "-v, --version";

				REQUIRE_FALSE(expected1 == result1);
				REQUIRE_FALSE(expected2 == result2);
			}
		}
	}
}