// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Arguments can store constant values", "[action]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		WHEN("argument has constant and action is set to 'sore_const'")
		{
			parser.addArgument("integer").constant<int>(5).action("store_const");
			parser.parseArgument({});

			THEN("value of 'integer' is the value passed by constant")
			{
				const auto result = parser.getValues<int>("integer");
				std::vector expected = { 5 };

				REQUIRE(expected == result);
			}
		}

		WHEN("argument has constant and action is set to 'sore_const'")
		{
			parser.addArgument("string").constant<std::string>("value").action("store_const");
			parser.parseArgument({});

			THEN("value of 'string' is the value passed by constant")
			{
				const auto result = parser.getValues<std::string>("string").front();
				const auto expected = "value";

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("Arguments can store true or false", "[action]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		WHEN("action is set to 'sore_true'")
		{
			parser.addArgument("boolean").action("store_true");
			parser.parseArgument({});

			THEN("value of 'boolean' is set to true")
			{
				const auto result = parser.getValues<bool>("boolean");
				std::vector<bool> expected = { true };

				REQUIRE(expected == result);
			}
		}

		WHEN("action is set to 'sore_false'")
		{
			parser.addArgument("boolean").action("store_false");
			parser.parseArgument({});

			THEN("value of 'boolean' is set to false")
			{
				const auto result = parser.getValues<bool>("boolean");
				std::vector<bool> expected = { false };

				REQUIRE(expected == result);
			}
		}

		WHEN("action that is not allowed is added")
		{
			THEN("error is thrown")
			{
				REQUIRE_THROWS(parser.addArgument("-i", "--info").action("invalid_action"));
			}
		}
	}
}