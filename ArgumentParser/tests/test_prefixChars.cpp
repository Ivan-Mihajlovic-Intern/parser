#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument Parser prefixChars", "[prefixChars]")
{
	GIVEN("Argument parser and argument prefixChars is set to '-+*'")
	{
		ArgumentTest parser;
		parser.prefixChars("-+*");
		parser.exitOnError(false);

		WHEN("argument '-s, --sum' is added")
		{
			THEN("no throw is expected")
			{
				REQUIRE_NOTHROW(parser.addArgument("-s", "--sum"));
			}
		}

		WHEN("argument '-s, -+sum' is added")
		{
			THEN("no throw is expected")
			{
				REQUIRE_NOTHROW(parser.addArgument("-s", "-+sum"));
			}
		}

		WHEN("argument '-s, -*sum' is added")
		{
			THEN("no throw is expected")
			{
				REQUIRE_NOTHROW(parser.addArgument("-s", "-*sum"));
			}
		}

		WHEN("argument '+s, ++sum' is added")
		{
			THEN("no throw is expected")
			{
				REQUIRE_NOTHROW(parser.addArgument("+s", "++sum"));
			}
		}

		WHEN("argument '*s, **sum' is added")
		{
			THEN("no throw is expected")
			{
				REQUIRE_NOTHROW(parser.addArgument("*s", "**sum"));
			}
		}
	}
}