// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Using parseKnownArgs allows us to parse more arguments than we need, without throwing exceptions", "[parseKnownArgs]")
{
	GIVEN("Argument --sum and some integers")
	{
		ArgumentTest parser;
		parser.addArgument("-s", "--sum").nargs(4).argumentType<int>();

		WHEN("'--sum 1 2 3 4' is parsed")
		{
			parser.parseKnownArgs({ "--sum", "1", "2", "3", "4", "--max", "45", "-506" });
			THEN("sum of integers is 10")
			{
				const auto integers = parser.getValues<int>("--sum");
				const auto expected = 10;
				int result = std::accumulate(integers.begin(), integers.end(), 0);

				REQUIRE(expected == result);
			}

			THEN("unused arguments are '--max', '45' and '-506'")
			{
				const auto result = parser.getUnusedValues();
				std::vector<std::string> expected = { "--max", "45", "-506" };

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("Order and type of positional arguments is still important", "[parseKnownArgs]")
{
	GIVEN("Argument --min and some integers")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("-m", "--min").nargs(4).argumentType<int>();

		WHEN("'--min 2 randomString 1 4 5' is parsed")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseKnownArgs({ "--min", "2", "randomString", "1", "4", "5" }));
			}
		}
	}
}

SCENARIO("ArgumentParser takes command-line arguments and passes them to parser", "[parseKnownArgs]")
{
	GIVEN("Argument --min and some integers")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("-m", "--min").nargs(4).argumentType<int>();

		WHEN("'--min 2 3 1 4' is parsed as vector of char pointers")
		{
			char* parseArgs[6] =
			{
				"program.exe",
				"--min",
				"2",
				"3",
				"1",
				"4"
			};

			parser.parseKnownArgs(6, parseArgs);
			THEN("min element is 1")
			{
				const auto integers = parser.getValues<int>("--min");
				const auto expected = 1;
				const auto position = std::min_element(integers.begin(), integers.end());

				int result = integers[std::distance(integers.begin(), position)];

				REQUIRE(expected == result);
			}
		}
	}

	GIVEN("Argument --max and some integers")
	{
		ArgumentTest parser;
		parser.addArgument("-M", "--max").nargs(6).argumentType<int>();

		WHEN("'--max 1 2 3 4 1 3' is parsed")
		{
			char* parseArgs[8] =
			{
				"pathToExe\\program.exe",
				"--max",
				"2",
				"3",
				"1",
				"4",
				"1",
				"3"
			};

			parser.parseKnownArgs(8, parseArgs);
			THEN("max element is 4")
			{
				const auto integers = parser.getValues<int>("--max");
				const auto expected = 4;
				const auto position = std::max_element(integers.begin(), integers.end());

				int result = integers[std::distance(integers.begin(), position)];

				REQUIRE(expected == result);
			}
		}
	}
}