#include "catch.hpp"
#include "testClass.h"

SCENARIO("Actions", "[action]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;

		WHEN("argument has constant and action is set to 'sore_const'")
		{
			parser.add_argument("integer").constant<int>(5).action("store_const");
			parser.parse_argument({});

			THEN("value of 'integer' is the value passed by constant")
			{
				const auto result = parser.getValues<int>("integer");
				std::vector expected = {5};

				REQUIRE(expected == result);
			}
		}

		WHEN("argument has constant and action is set to 'sore_const'")
		{
			parser.add_argument("string").constant<std::string>("value").action("store_const");
			parser.parse_argument({});

			THEN("value of 'string' is the value passed by constant")
			{
				const auto result = parser.getValues<std::string>("string").front();
				const auto expected =  "value" ;

				REQUIRE(expected == result);
			}
		}

		WHEN("action is set to 'sore_true'")
		{
			parser.add_argument("boolean").action("store_true");
			parser.parse_argument({});

			THEN("value of 'boolean' is set to true")
			{
				const auto result = parser.getValues<bool>("boolean");
				std::vector<bool> expected = { true };

				REQUIRE(expected == result);
			}
		}

		WHEN("action is set to 'sore_false'")
		{
			parser.add_argument("boolean").action("store_false");
			parser.parse_argument({});

			THEN("value of 'boolean' is set to false")
			{
				const auto result = parser.getValues<bool>("boolean");
				std::vector<bool> expected = { false };

				REQUIRE(expected == result);
			}
		}
	}
}