#include "catch.hpp"
#include "testClass.h"

TEST_CASE("Error throws", "[throw]")
{
	ArgumentTest parser;
	
	SECTION("When negative number of arguments is added, error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("integers").nargs(-5));
	}

	SECTION("When argument name is to long , error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("--longParsedArgumentName"));
	}

	SECTION("When 1st argument name parameter is to long , error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("-longParsedArgumentName", "--shortName"));
	}

	SECTION("When 2st argument name parameter is to long , error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("-shortName","--longParsedArgumentName"));
	}

	SECTION("When action that is not allowed is added, error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("-i", "--info").action("invalid_action"));
	}

	SECTION("When a short name of an optional argument does not begin with '-', error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("i", "--info"));
	}

	SECTION("When a long name of an optional argument does not begin with '--', error is thrown")
	{
		REQUIRE_THROWS(parser.add_argument("-i", "info"));
	}

	SECTION("When more arguments are parsed then specified in nargs, error is thrown")
	{
		parser.add_argument("integers").nargs(4).argumentType("int");
		REQUIRE_THROWS(parser.parse_argument({"1", "2", "3", "4", "5"}));
	}

	SECTION("When less arguments are parsed then specified in nargs, error is thrown")
	{
		parser.add_argument("integers").nargs(4).argumentType("int");
		REQUIRE_THROWS(parser.parse_argument({ "1", "2", "3" }));
	}

	SECTION("When argument is parsed but not added, error is thrown")
	{
		parser.add_argument("-i", "--info");
		REQUIRE_THROWS(parser.parse_argument({ "--option" }));
	}

}