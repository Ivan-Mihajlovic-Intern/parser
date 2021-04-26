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
		
		parser.parse_argument({"-i", "--info"});
		const auto result = parser.getActiveArguments()["-i"];
		const auto expected = true;

		REQUIRE(expected == result);
	}
	//TODO: fix
	SECTION("When --help is called, all arguments after help are ignored")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");
		
		parser.parse_argument({"-i", "--info"});
		const auto result = parser.getActiveArguments()["--info"];
		const auto expected = true;

		REQUIRE(expected == result);
	}

	SECTION("When -i is called, isActive(-i) returns ture")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");
		
		parser.parse_argument({ "-i", "--info"});
		const auto result = parser.isActive("-i");
		const auto expected = true;

		REQUIRE(expected == result);
	}

	SECTION("When -i is called, isActive(--info) return true")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({ "-i"});
		const auto result = parser.isActive("--info");
		const auto expected = true;

		REQUIRE(expected == result);
	}

	SECTION("When --info is called, isActive(-i) return true")
	{
		ArgumentParser parser;

		parser.add_argument("-i", "--info").help("information about program");

		parser.parse_argument({ "--info" });
		const auto result = parser.isActive("-i");
		const auto expected = true;

		REQUIRE(expected == result);
	}

	SECTION("When three integers are parsed, getValues<int> returns thoes integers")
	{
		ArgumentParser parser;

		parser.add_argument("integers").help("parsing 3 integers").nargs(3).argumentType("int");

		std::vector<std::string> arguments;
		arguments.push_back("1");
		arguments.push_back("2");
		arguments.push_back("3");
		
		parser.parse_argument(arguments);
		auto result = parser.getValues<int>("integers");
		std::vector<int> expected = {1,2,3};

		REQUIRE(expected == result);
	}

	SECTION("When argumentType isnt specified, getValues<int> returns empty vector")
	{
		ArgumentParser parser;

		parser.add_argument("integers").help("parsing 3 integers").nargs(3);

		std::vector<std::string> arguments;
		arguments.push_back("1");
		arguments.push_back("2");
		arguments.push_back("3");

		parser.parse_argument(arguments);
		auto result = parser.getValues<int>("integers");
		std::vector<int> expected = { };

		REQUIRE(expected == result);
	}

	SECTION("When three strings are parsed, getValues<std::string> returns thoes strings")
	{
		ArgumentParser parser;

		parser.add_argument("strings").help("parsing 3 strings").nargs(3).argumentType("string");

		std::vector<std::string> arguments;
		arguments.push_back("value1");
		arguments.push_back("value2");
		arguments.push_back("value3");

		parser.parse_argument(arguments);
		auto result = parser.getValues<std::string>("strings");
		std::vector<std::string> expected = {"value1", "value2", "value3"};

		REQUIRE(expected == result);
	}

	SECTION("When argumentType is not specified, getValues<(std::string)> returns empty vector")
	{
		ArgumentParser parser;

		parser.add_argument("strings").help("parsing 3 strings").nargs(3);

		std::vector<std::string> arguments;
		arguments.push_back("value1");
		arguments.push_back("value2");
		arguments.push_back("value3");

		parser.parse_argument(arguments);
		auto result = parser.getValues<std::string>("strings");
		std::vector<std::string> expected = {  };

		REQUIRE(expected == result);
	}

}

TEST_CASE("Constant and action tested", "[constant][action]")
{
	SECTION("When constant is defined and action is set to 'store_const', getValues<int> returns value of constant")
	{
		ArgumentParser parser;

		parser.add_argument("constant").help("constant value parsed")
			.constant<int>(45).action("store_const");

		parser.parse_argument({});
		auto result = parser.getValues<int>("constant");
		std::vector<int> expected = {45};

		REQUIRE(expected == result);
	}

	SECTION("When constant is defined and action is set to 'store_const', getValues<std::string> returns value of constant")
	{
		ArgumentParser parser;

		parser.add_argument("constant").help("constant value parsed")
			.constant<std::string>("stored").action("store_const");

		parser.parse_argument({});
		auto result = parser.getValues<std::string>("constant");
		std::vector<std::string> expected = { "stored" };

		REQUIRE(expected == result);
	}

	SECTION("When action is set to 'store_true', getValues<bool> returns true")
	{
		ArgumentParser parser;

		parser.add_argument("boolean").help("returns true value").action("store_true");

		parser.parse_argument({});
		auto result = parser.getValues<bool>("boolean");
		std::vector<bool> expected = { true };

		REQUIRE(expected == result);
	}

	SECTION("When action is set to 'store_false', getValues<bool> returns false")
	{
		ArgumentParser parser;

		parser.add_argument("boolean").help("returns false value").action("store_false");

		parser.parse_argument({});
		auto result = parser.getValues<bool>("boolean");
		std::vector<bool> expected = { false };

		REQUIRE(expected == result);
	}
}
