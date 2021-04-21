#include "catch.hpp"

#include "../lib/ArgumentParser.h"

TEST_CASE("Creating the ArgumentParser", "[constructor][add_argument]")
{
	SECTION("When ArgumentParser is constructed, --help is a default argument")
	{
		ArgumentParser parser;

		const auto expected = "-h, --help";
		const auto result = parser.getOptionalArguments().front().getName();
		
		REQUIRE(expected == result);
	}

	SECTION("Adding an positionl argument 'integer'")
	{
		ArgumentParser parser;
		
		const auto expected = "integer";
		
		parser.add_argument("integer").help("takes one integer").nargs(1);
		const auto result = parser.getPositionalArguments().front().getName();

		REQUIRE(expected == result);
	}

	SECTION("Adding a optional agrument '-i, --info'")
	{
		ArgumentParser parser;
	
		const auto expected = "-i, --info";
		
		parser.add_argument("-i", "--info").help("information about program");
		const auto result = parser.getOptionalArguments().back().getName();

		REQUIRE(expected == result);
	}
}
//TODO:Better description
TEST_CASE("Recognizing arguments")
{
	SECTION("When '-i' is given as an argument, its walue will be true in active arguments map")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({"path\name.exe", "-i", "--info", "1", "2"});
		const auto result = parser.getActiveArguments()["-i"];
		const auto expected = true;

		REQUIRE(expected == result);
	}

	SECTION("When --help is called, all arguments after help are ignored")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({ "path\name.exe", "-i", "-h" "--info", "1", "2" });
		const auto result = parser.getActiveArguments()["--info"];
		const auto expected = true;

		REQUIRE_FALSE(expected == result);
	}

	SECTION("If program path is the first argument, then only progrma name will be saved")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({ "path\\name.exe", "-i", "-h" "--info", "1", "2" });
		const auto result = parser.getProgramName();
		const auto expected = "name.exe";

		REQUIRE(expected == result);
	}

	SECTION("If only program name is the first argument, it will be saved")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({ "name.exe", "-i", "-h" "--info", "1", "2" });
		const auto result = parser.getProgramName();
		const auto expected = "name.exe";

		REQUIRE(expected == result);
	}
}
