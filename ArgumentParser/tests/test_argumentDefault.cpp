// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Parser can have default values for arguments, witch are overridden when values are parsed", "[argumentDefault]")
{
	GIVEN("argumentDefault is set to '-5, 8, 13' and argument '-s, --sum'")
	{
		ArgumentTest parser;
		parser.argumentDefault(std::vector<int>{-5, 8, 13});
		parser.addArgument("-s", "--sum").nargs(3).argumentType<int>();

		WHEN("Argument '-s' is parsed")
		{
			parser.parseArgument({ "-s" });
			THEN("sum of '--sum' is 16")
			{
				const auto integers = parser.getValues<int>("--sum");
				const auto expected = 16;
				int result = std::accumulate(integers.begin(), integers.end(), 0);

				REQUIRE(expected == result);
			}
		}

		WHEN("'--sum 1 2 3' is parsed")
		{
			parser.parseArgument({ "--sum", "1", "2", "3" });
			THEN("sum of '--sum' is 6")
			{
				const auto integers = parser.getValues<int>("--sum");
				const auto expected = 6;
				int result = std::accumulate(integers.begin(), integers.end(), 0);

				REQUIRE(expected == result);
			}
		}
	}

	GIVEN("argumentDefault is set to 'admin' and argument '-u, --username'")
	{
		ArgumentTest parser;
		parser.argumentDefault(std::vector<std::string>{"admin"});
		parser.addArgument("-u", "--username").nargs(1).argumentType<std::string>();

		WHEN("Argument '-u' is parsed")
		{
			parser.parseArgument({ "-u" });
			THEN("username is admin")
			{
				const auto result = parser.getValues<std::string>("--username").front();
				const auto expected = "admin";
				
				REQUIRE(expected == result);
			}
		}

		WHEN("'--username Name.Lastname' is parsed")
		{
			parser.parseArgument({ "--username", "Name.Lastname" });
			THEN("username is Name.Lastname")
			{
				const auto result = parser.getValues<std::string>("--username").front();
				const auto expected = "Name.Lastname";

				REQUIRE(expected == result);
			}
		}
	}
}