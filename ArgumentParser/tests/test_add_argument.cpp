#include "catch.hpp"
#include "testClass.h"


SCENARIO("Creating the ArgumentParser", "[addArgument]")
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

		WHEN("One oprional argument is added ")
		{
			parser.addArgument("-i");

			THEN("Argument is in vector optionalArguments")
			{
				const auto expected = "-i";
				const auto result = parser.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("Two oprional arguments are added")
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

		WHEN("Negative number of arguments is added")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("integers").nargs(-5));
			}
		}

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