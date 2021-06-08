// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"


SCENARIO("ArgumentParser can have no arguments added", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);

		WHEN("No arguments are added")
		{
			THEN("Help is only argument in optionalArguments")
			{
				const auto expected = "-h, --help";
				const auto result = parser.getOptionalArguments().front().getName();

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("ArgumentParser can have ether optional or positional arguments added", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);

		WHEN("One optional argument is added ")
		{
			parser.addArgument("-i");

			THEN("Argument is in vector optionalArguments")
			{
				const auto expected = "-i";
				const auto result = parser.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("Two optional arguments are added")
		{
			parser.addArgument("-i", "--info");

			THEN("Argument is in vector optionalArguments")
			{
				const auto expected = "-i, --info";
				const auto result = parser.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("One positional argument is added")
		{
			parser.addArgument("integers");

			THEN("Argument is in vector positionalArguments")
			{
				const auto expected = "integers";
				const auto result = parser.getPositionalArguments().front().getName();

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("Argument can't have negative number of arguments", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);

		WHEN("Negative number of arguments is added")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("integers").nargs(-5));
			}
		}
	}
}

SCENARIO("Argument names can't be over 15 characters long", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);

		WHEN("argument name is to long ")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("--longParsedArgumentName"));
			}
		}

		WHEN("1st argument name parameter is to long")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("-longParsedArgumentName", "--shortName"));
			}
		}

		WHEN("2nd argument name parameter is to long")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("-shortName", "--longParsedArgumentName"));
			}
		}
	}
}

SCENARIO("Optional arguments have to begin with '-'", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);

		WHEN("a short name of an optional argument does not begin with '-'")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("i", "--info"));
			}


		}

		WHEN("a long name of an optional argument does not begin with '--'")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("-i", "info"));
			}
		}
	}
}

SCENARIO("Arguments of the same name can't be parsed", "[addArgument]")
{
	GIVEN("ArgumentParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);


		WHEN("two arguments with the same short argument name are added")
		{
			parser.addArgument("-g", "--get");
			
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("-g"));
			}
		}

		WHEN("two arguments with the same long argument name are added")
		{
			parser.addArgument("-g", "--get");

			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("--get"));
			}
		}
	}
}