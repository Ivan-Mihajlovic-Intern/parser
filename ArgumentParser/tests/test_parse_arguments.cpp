#include "catch.hpp"
#include "testClass.h"

SCENARIO("Parsing arguments", "[parse_arguments]")
{
	GIVEN("Arguments '-i', '-o' and '--option'")
	{
		ArgumentTest parser;
		parser.add_argument("-i");
		parser.add_argument("-o", "--option");
		
		WHEN("Argument '-i' is parsed")
		{
			parser.parse_argument({ "-i" });
			THEN("activeValues for that argument is set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["-i"];

				REQUIRE(expected == result);
			}
		}

		WHEN("Argument '-o' is parsed")
		{
			parser.parse_argument({ "-o" });
			THEN("activeValues for '--option' argument is allso set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["--option"];

				REQUIRE(expected == result);
			}
		}

		WHEN("Argument --option is parsed")
		{
			parser.parse_argument({ "--option" });
			THEN("activeValues for '-o' argument is allso set to true")
			{
				const auto expected = true;
				const auto result = parser.getActiveArguments()["-o"];

				REQUIRE(expected == result);
			}
		}
	}

	GIVEN("number of arguments and argument type")
	{
		ArgumentTest parser;
		parser.add_argument("integers").nargs(3).argumentType<int>();
		parser.add_argument("strings").nargs(2).argumentType<std::string>();

		WHEN("three integers are parsed")
		{
			parser.parse_argument({ "1","2","7","string1","string2" });
			THEN("size of integers argument is three")
			{
				const auto result = parser.getValues<int>("integers").size();
				const auto expected = 3;

				REQUIRE(expected == result);
			}
		}

		WHEN("two strings are parsed")
		{
			parser.parse_argument({ "1","2","7","string1","string2" });
			THEN("size of strings argument is two")
			{
				const auto result = parser.getValues<std::string>("strings").size();
				const auto expected = 2;

				REQUIRE(expected == result);
			}
		}

		WHEN("more arguments are parsed then specified in nargs")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parse_argument({ "1", "2", "3", "4", "5", "6" }));
			}
		}

		WHEN("less arguments are parsed then specified in nargs")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parse_argument({ "1", "2", "3", "4" }));
			}
		}

		WHEN("argument is parsed but not added")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.parse_argument({ "--option" }));
			}
		}

	}

	GIVEN("no number of arguments nor argument type")
	{
		ArgumentTest parser;
		parser.add_argument("integers");

		WHEN("empty string is parsed")
		{
			parser.parse_argument({ });
			THEN("size of integers argument is zero")
			{
				const auto result = parser.getValues<int>("integers").size();
				const auto expected = 0;

				REQUIRE(expected == result);
			}
		}
	}

	GIVEN("Argument --sum and some integers")
	{
		ArgumentTest parser;
		parser.add_argument("-s", "--sum").nargs(4).argumentType<int>();

		WHEN("'--sum 1 2 3 4' is parsed")
		{
			parser.parse_argument({ "--sum", "1", "2", "3", "4" });
			THEN("sum of integers is 10")
			{
				const auto integers = parser.getValues<int>("--sum");
				const auto expected = 10;
				int result = std::accumulate(integers.begin(), integers.end(), 0);

				REQUIRE(expected == result);
			}
		}
	}

	GIVEN("Argument --max and some integers")
	{
		ArgumentTest parser;
		parser.add_argument("-M", "--max").nargs(6).argumentType<int>();

		WHEN("'--max 1 2 3 4 1 3' is parsed")
		{
			parser.parse_argument({ "--max", "1", "2", "3", "4", "1", "3" });
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

	GIVEN("Argument --min and some integers")
	{
		ArgumentTest parser;
		parser.add_argument("-m", "--min").nargs(4).argumentType<int>();

		WHEN("'--min 2 3 1 4' is parsed")
		{
			parser.parse_argument({ "--min", "2", "3", "1", "4" });
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