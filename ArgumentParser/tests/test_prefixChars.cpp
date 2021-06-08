// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"

SCENARIO("Optional arguments begin with '-', this can be redefined by prefixChars()", "[prefixChars]")
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