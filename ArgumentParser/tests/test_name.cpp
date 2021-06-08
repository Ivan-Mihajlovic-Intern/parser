// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Optional arguments can have only the short name", "[name]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;

		WHEN("Argument '-i' is added")
		{
			parser.addArgument("-i");

			THEN("size of _name for that argument is two, second string being empty")
			{
				const auto names = parser.getOptionalArguments().back().getNames();
				const auto result = names.size();
				const auto expected1 = 2;
				const auto expected2 = "";

				REQUIRE(expected1 == result);
				REQUIRE(expected2 == names[1]);
			}
		}
	}
}

SCENARIO("Optional arguments have to begin with '-', and '--' if long name is provided", "[name]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;

		WHEN("Arguments '-o' and '--option' are added")
		{
			parser.addArgument("-o", "--option");

			THEN("size of _name for that argument is two")
			{
				const auto result = parser.getOptionalArguments().back().getNames().size();
				const auto expected = 2;

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("Positional arguments can't start with '-' and they don't have long name", "[name]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;

		WHEN("Argument 'integers' is added")
		{
			parser.addArgument("integers").nargs(3).argumentType<int>();

			THEN("size of _name for that argument is two, second string being empty")
			{
				const auto names = parser.getPositionalArguments().back().getNames();
				const auto result = names.size();
				const auto expected1 = 2;
				const auto expected2 = "";

				REQUIRE(expected1 == result);
				REQUIRE(expected2 == names[1]);
			}
		}

	}

}