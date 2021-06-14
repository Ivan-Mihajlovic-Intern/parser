// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Abbreviations of positional arguments are enabled by default", "[allowAbbrev]")
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
			THEN("'--func' will be recognized as '-f, --function'")
			{

				const auto result = parser.getOptionalArguments().back().getName();

				const auto expected = "-f, --function";

				REQUIRE(expected == result);
			}
		}

		WHEN("allowAbbrev is set to true and '--fun' is parsed")
		{
			THEN("'--fun' is parsed error will occur, because '--fun' matches 2 arguments")
			{

				REQUIRE_THROWS(parser.parseArgument({ "--fun" }));
			}
		}

		WHEN("allowAbbrev is set to false")
		{
			parser.allowAbbrev(false);
			THEN("'--func' is parsed error will occur")
			{

				REQUIRE_THROWS(parser.parseArgument({ "--func" }));
			}
		}
	}
}