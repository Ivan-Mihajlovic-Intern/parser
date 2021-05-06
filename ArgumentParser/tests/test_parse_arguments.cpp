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

		//WHEN("argc < 1") then error
	}

	GIVEN("number of arguments and argument type")
	{
		ArgumentTest parser;
		parser.add_argument("integers").nargs(3).argumentType("int");
		parser.add_argument("strings").nargs(2).argumentType("string");

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
}