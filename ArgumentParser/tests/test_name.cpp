#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument names", "[name]")
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