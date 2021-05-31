#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument Parser allowAbbrev", "[allowAbbrev]")
{
	GIVEN("Argument parser and arguments '-f, --function' and '--funtain'")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("--funtain");
		parser.addArgument("-f", "--function");
		WHEN("allowAbbrev is set to true and '--func' is parsed")
		{
			parser.parseArgument({"--func"});
			THEN("'--func' will be recognised as '-f, --function'")
			{

				const auto result = parser.getOptionalArguments().back().getName();

				const auto expected = "-f, --function";

				REQUIRE(expected == result);
			}
		}

		WHEN("allowAbbrev is set to true and '--fun' is parrsed")
		{
			THEN("'--fun' is parsed error will accour, because '--fun' matches 2 arguments")
			{

				REQUIRE_THROWS(parser.parseArgument({ "--fun" }));
			}
		}

		WHEN("allowAbbrev is set to false")
		{
			parser.allowAbbrev(false);
			THEN("'--func' is parsed error will accour")
			{

				REQUIRE_THROWS(parser.parseArgument({ "--func" }));
			}
		}
	}
}