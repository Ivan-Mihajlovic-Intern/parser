// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Arguments can have certain values they can hold and no other value can be parsed to them", "[choices]")
{
	GIVEN("Argument parser and positional argument with choices 0 and 1")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("TrueOrFalse").nargs(1).argumentType<int>().choices({ 0,1 });

		WHEN("'0' is parsed")
		{
			parser.parseArgument({"0"});
			THEN("value of 'TrueOrFasle is 0'")
			{
				const auto result = parser.getValues<int>("TrueOrFalse").front();
				const auto expected = 0;

				REQUIRE(expected == result);
			}
		}

		WHEN("value other then 0 or 1 is parsed")
		{
			THEN("error is thrown")
			{
				
				REQUIRE_THROWS(parser.parseArgument({"7"}));
			}
		}
	}

	GIVEN("Argument parser and positional argument with choices 'yes' and 'no'")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("YesOrNo").nargs(1).argumentType<std::string>().choices({ "yes","no" });

		WHEN("'yes' is parsed")
		{
			parser.parseArgument({ "yes" });
			THEN("value of 'TrueOrFasle is 'yes'")
			{
				const auto result = parser.getValues<std::string>("YesOrNo").front();
				const auto expected = "yes";

				REQUIRE(expected == result);
			}
		}

		WHEN("value other then 'yes' or 'no' is parsed")
		{
			THEN("error is thrown")
			{

				REQUIRE_THROWS(parser.parseArgument({ "maybe" }));
			}
		}
	}
}