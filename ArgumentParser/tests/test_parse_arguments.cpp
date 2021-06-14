// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Optional arguments are parsed with prefix '-'", "[parseArguments]")
{
	GIVEN("Arguments '-i', '-o' and '--option'")
	{
		ArgumentTest parser;
		parser.addArgument("-i");
		parser.addArgument("-o", "--option");

		WHEN("Argument '-i' is parsed")
		{
			parser.parseArgument({ "-i" });
			THEN("activeValues for that argument is set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["-i"];

				REQUIRE(expected == result);
			}
		}

		WHEN("Argument '-o' is parsed")
		{
			parser.parseArgument({ "-o" });
			THEN("activeValues for '--option' argument is also set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["--option"];

				REQUIRE(expected == result);
			}
		}

		WHEN("Argument --option is parsed")
		{
			parser.parseArgument({ "--option" });
			THEN("activeValues for '-o' argument is also set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["-o"];

				REQUIRE(expected == result);
			}
		}
	}
}
SCENARIO("Positional arguments have no prefix, they only take values no name is required to be parsed", "[parseArguments]")
{
	GIVEN("number of arguments and argument type")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		parser.addArgument("integers").nargs(3).argumentType<int>();
		parser.addArgument("strings").nargs(2).argumentType<std::string>();
		parser.addArgument("-u", "--username").nargs(1).argumentType<std::string>();

		WHEN("three integers are parsed")
		{
			parser.parseArgument({ "-1","2","7","string1","string2" });
			THEN("size of integers argument is three")
			{
				const auto result = parser.getValues<int>("integers").size();
				const auto expected = 3;

				REQUIRE(expected == result);
			}
		}

		WHEN("two strings are parsed")
		{
			parser.parseArgument({ "1","2","7","string1","string2" });
			THEN("size of strings argument is two")
			{
				const auto result = parser.getValues<std::string>("strings").size();
				const auto expected = 2;

				REQUIRE(expected == result);
			}
		}

		/*WHEN("optional argument is parsed without additional arguments, but its nargs is greater than 0")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseArgument({ "1", "2", "3", "string1", "string2", "-u" }));
			}
		}*/

		WHEN("more arguments are parsed then specified in nargs")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseArgument({ "1", "2", "3", "4", "5", "6" }));
			}
		}

		WHEN("less arguments are parsed then specified in nargs")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseArgument({ "1", "2", "3", "4" }));
			}
		}

		WHEN("less arguments are parsed then specified in nargs")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseArgument({ "1", "2" }));
			}
		}

		WHEN("argument is parsed but not added")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parseArgument({ "--option" }));
			}
		}

	}
}

SCENARIO("Nothing can be parsed", "[parseArguments]")
{
	GIVEN("no number of arguments nor argument type")
	{
		ArgumentTest parser;
		parser.addArgument("integers");

		WHEN("empty string is parsed")
		{
			parser.parseArgument({ });
			THEN("size of integers argument is zero")
			{
				const auto result = parser.getValues<int>("integers").size();
				const auto expected = 0;

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("ArgumentParser can be used to preform addition", "[parseArguments]")
{
	GIVEN("Argument --sum and some integers")
	{
		ArgumentTest parser;
		parser.addArgument("-s", "--sum").nargs(4).argumentType<int>();

		WHEN("'--sum 1 2 3 4' is parsed")
		{
			parser.parseArgument({ "--sum", "1", "2", "3", "4" });
			THEN("sum of integers is 10")
			{
				const auto integers = parser.getValues<int>("--sum");
				const auto expected = 10;
				int result = std::accumulate(integers.begin(), integers.end(), 0);

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("ArgumentParser can be used to find a max element", "[parseArguments]")
{
	GIVEN("Argument --max and some integers")
	{
		ArgumentTest parser;
		parser.addArgument("-M", "--max").nargs(6).argumentType<int>();

		WHEN("'--max 1 2 3 4 1 3' is parsed")
		{
			parser.parseArgument({ "--max", "1", "2", "3", "4", "1", "3" });
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

SCENARIO("ArgumentParser can be used to find a min element", "[parseArguments]")
{
	GIVEN("Argument --min and some integers")
	{
		ArgumentTest parser;
		parser.addArgument("-m", "--min").nargs(4).argumentType<int>();

		WHEN("'--min 2 3 1 4' is parsed")
		{
			parser.parseArgument({ "--min", "2", "3", "1", "4" });
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
}
SCENARIO("parseKnownArgs takes command-line arguments and passes them to parser", "[parseArguments]")
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

			parser.parseArgument(6, parseArgs);
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

			parser.parseArgument(8, parseArgs);
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