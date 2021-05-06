#include "catch.hpp"
#include "testClass.h"


SCENARIO("Adding subparsers", "[add_parser]")
{
	GIVEN("One subParser")
	{
		ArgumentTest parser;
		ArgumentParser subParser = parser.add_parser("subparser");

		WHEN("One positional argument named 'positional' is added")
		{
			subParser.add_argument("positional").nargs(3).argumentType("int");
			
			THEN("The name of the first positional arument is 'positional'")
			{
				const auto expected = "positional";
				const auto cast =static_cast<ArgumentTest&>(subParser);
				const auto result = cast.getPositionalArguments().front().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("No arguments are added")
		{
			
			THEN("Help is in optionalArguments by default")
			{
				const auto expected = "-h, --help";
				const auto cast = static_cast<ArgumentTest&>(subParser);
				const auto result = cast.getOptionalArguments().front().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("No arguments are added")
		{

			THEN("Version is in optionalArguments by default")
			{
				const auto expected = "-v, --version";
				const auto cast = static_cast<ArgumentTest&>(subParser);
				const auto result = cast.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}

		WHEN("One optional argument named '--info' is added")
		{
			subParser.add_argument("-i", "--info").help("some information is displayed");

			THEN("The name of the last optional arument is '-i, --info'")
			{
				const auto expected = "-i, --info";
				const auto cast = static_cast<ArgumentTest&>(subParser);
				const auto result = cast.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}

		
	}

	GIVEN("One subParser")
	{
		ArgumentTest parser;
		ArgumentParser subParser1 = parser.add_parser("subparser1");
		ArgumentParser subParser2 = parser.add_parser("subparser2");

		WHEN("One positional argument named 'positional' is added")
		{
			subParser1.add_argument("positional").nargs(3).argumentType("int");
			subParser2.add_argument("-o, --option").help("shows some options");
			THEN("values of argument 'positional' is {1, 2, 3}")
			{
				parser.parse_argument({ "subparser1", "1", "2", "3" });
				std::vector<int> expected = { 1 , 2, 3 };
				std::vector<int> result = subParser1.getValues<int>("positional");
				//const auto result1 = subParser2.isActive("")
				//const auto cast = static_cast<ArgumentTest&>(subParser1);
				//const auto result = cast.getPositionalArguments().front().get;

				REQUIRE(expected == result);
			}
		}

	}
}