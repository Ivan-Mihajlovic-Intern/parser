// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "catch.hpp"
#include "testClass.h"


SCENARIO("Subparsers can have no arguments, positional or optional arguments added", "[addParser]")
{
	GIVEN("One subParser")
	{
		ArgumentTest parser;
		parser.exitOnError(false);
		ArgumentParser subParser = parser.addParser("subparser");

		WHEN("One positional argument named 'positional' is added")
		{
			subParser.addArgument("positional").nargs(3).argumentType<int>();

			THEN("The name of the first positional argument is 'positional'")
			{
				const auto expected = "positional";
				const auto cast = static_cast<ArgumentTest&>(subParser);
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
			subParser.addArgument("-i", "--info").help("some information is displayed");

			THEN("The name of the last optional argument is '-i, --info'")
			{
				const auto expected = "-i, --info";
				const auto cast = static_cast<ArgumentTest&>(subParser);
				const auto result = cast.getOptionalArguments().back().getName();

				REQUIRE(expected == result);
			}
		}
	}
}

SCENARIO("Parser can check if subparser is parsed", "[addParser]")
{
	GIVEN("One subParser")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& commit = parser.addParser("commit");

		WHEN("'commit' is parsed'")
		{
			parser.parseArgument({ "commit" });
			THEN("isActive returns true")
			{
				auto expected = true;
				auto result = parser.isActive("commit");

				REQUIRE(expected == result);
			}
		}

	}
}

SCENARIO("Subparsers can check if specific argument is parsed", "[addParser]")
{
	GIVEN("One subParser")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		parser.addParser("commit");
		ArgumentParser& commit = parser.getSubParser("commit");
		commit.addArgument("-m", "--message").help("adds a message");

		WHEN("'commit' and '-m' is parsed")
		{
			parser.parseArgument({ "commit", "-m" });
			THEN("isActive returns true for both '-m' and '--message'")
			{
				auto expected = true;
				auto result1 = commit.isActive("-m");
				auto result2 = commit.isActive("--message");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);

			}
		}
	}
}

SCENARIO("Multiple subparsers can be added, but only one can be used at a time", "[addParser]")
{
	GIVEN("Two subParsers, commit and pull")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& commit = parser.addParser("commit");
		ArgumentParser& pull = parser.addParser("pull");

		commit.addArgument("-m", "--message")
			  .help("adds a message to a current commit").nargs(1).argumentType<std::string>();
		commit.addArgument("-a", "--all").help("add all modified files");

		pull.addArgument("-q", "--quiet").help("minimal output during fetching");
		pull.addArgument("-ve", "--verbose").help("passes --verbose to git-fetch and git-merge");
		
		WHEN("'commit -m initial commit' is parsed")
		{
				
			parser.parseArgument({ "commit", "-m", "initial commit" });
			
			THEN("isActive returns true for 'commit' and returns false for 'pull'")
			{
				auto expected = true;
				auto result1 = parser.isActive("commit");
				auto result2 = parser.isActive("pull");

				REQUIRE(expected == result1);
				REQUIRE_FALSE(expected == result2);
			}

			THEN("value of '-m' is 'initial commit'")
			{
				auto expected = "initial commit";
				auto result1 = commit.getValues<std::string>("-m").front();

				REQUIRE(expected == result1);
			}
		}

		WHEN("'pull' is parsed")
		{

			parser.parseArgument({ "pull", "-q" });

			THEN("isActive returns true for 'pull' and returns false for 'commit'")
			{
				auto expected = true;
				auto result1 = parser.isActive("pull");
				auto result2 = parser.isActive("commit");

				REQUIRE(expected == result1);
				REQUIRE_FALSE(expected == result2);
			}
		}

	}

	GIVEN("Two subParsers, config and --global")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& config = parser.addParser("config");
		ArgumentParser& global = config.addParser("--global");

		global.addArgument("--user.name")
			.help("adds user name to git").nargs(1).argumentType<std::string>();
		global.addArgument("--user.mail").
			help("adds user email to git").nargs(1).argumentType<std::string>();


		WHEN("'config --global user.name userName'")
		{

			parser.parseArgument({ "config", "--global", "--user.name", "userName" });

			THEN("isActive returns true for 'commit' and '--global'")
			{
				auto expected = true;
				auto result1 = parser.isActive("config");
				auto result2 = config.isActive("--global");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);
			}

			THEN("value of 'user.name' is 'userName'")
			{
				auto expected = "userName";
				auto result1 =  global.getValues<std::string>("--user.name").front();

				REQUIRE(expected == result1);
			}
		}

		WHEN("'config --global user.main userEmail'")
		{

			parser.parseArgument({ "config", "--global", "--user.mail", "userEmail" });

			THEN("isActive returns true for 'config' and '--global'")
			{
				auto expected = true;
				auto result1 = parser.isActive("config");
				auto result2 = config.isActive("--global");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);
			}

			THEN("value of 'user.mail' is 'userEmail'")
			{
				auto expected = "userEmail";
				auto result1 = global.getValues<std::string>("--user.mail").front();

				REQUIRE(expected == result1);
			}
		}
	}

	GIVEN("Two subParsers, config and --global")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& checkout = parser.addParser("checkout").help("switches to a different branch");
		
		checkout.addArgument("-b")
			.help("adds a branch").nargs(1).argumentType<std::string>();
		checkout.addArgument("-B")
			.help("branch is created if it does not exist, otherwise branch is reset").nargs(1).argumentType<std::string>();

		WHEN("'checkout -b newBranch'")
		{

			parser.parseArgument({ "checkout", "-b", "newBranch" });

			THEN("isActive returns true for 'checkout' and '-b'")
			{
				auto expected = true;
				auto result1 = parser.isActive("checkout");
				auto result2 = checkout.isActive("-b");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);
			}

			THEN("value of '-b' is 'newBranch'")
			{
				auto expected = "newBranch";
				auto result1 = checkout.getValues<std::string>("-b").front();

				REQUIRE(expected == result1);
			}
		}

		WHEN("'checkout -B newBranch'")
		{

			parser.parseArgument({ "checkout", "-B", "newBranch" });

			THEN("isActive returns true for 'checkout' and '-B'")
			{
				auto expected = true;
				auto result1 = parser.isActive("checkout");
				auto result2 = checkout.isActive("-B");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);
			}

			THEN("value of '-B' is 'newBranch'")
			{
				auto expected = "newBranch";
				auto result1 = checkout.getValues<std::string>("-B").front();

				REQUIRE(expected == result1);
			}
		}
	}
}