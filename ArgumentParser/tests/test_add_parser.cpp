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
			subParser.add_argument("positional").nargs(3).argumentType<int>();
			
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
		parser.subParsersHelp("git commands");
		ArgumentParser& commit = parser.add_parser("commit");
		
		WHEN("'commit' is parsed'")
		{
			parser.parse_argument({ "commit" });
			THEN("isActive returns true")
			{
				auto expected = true;
				auto result = parser.isActive("commit");
				
				REQUIRE(expected == result);
			}
		}

	}

	GIVEN("One subParser")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& commit = parser.add_parser("commit");
		commit.add_argument("-m", "--message").help("adds a message");
		
		WHEN("'commit' and '-m' is parsed")
		{
			parser.parse_argument({ "commit", "-m" });
			THEN("isActive returns true for both '-m' and '--mesage'")
			{
				auto expected = true;
				auto result1 = commit.isActive("-m");
				auto result2 = commit.isActive("--message");

				REQUIRE(expected == result1);
				REQUIRE(expected == result2);
			}
		}

	}

	GIVEN("Two subParsers, commit and pull")
	{
		ArgumentTest parser;
		parser.subParsersHelp("git commands");
		ArgumentParser& commit = parser.add_parser("commit");
		ArgumentParser& pull = parser.add_parser("pull");

		commit.add_argument("-m", "--message")
			  .help("adds a message to a current commit").nargs(1).argumentType<std::string>();
		commit.add_argument("-a", "--all").help("add all modified files");
		
		//commit  ""

		pull.add_argument("-q", "--quiet").help("minimal output during fetching");
		pull.add_argument("-v", "--verbose").help("passes --verbose to git-feth and git-merge");
		
		WHEN("'commit -m inital commit' is parsed")
		{
				
			parser.parse_argument({ "commit", "-m", "inital commit" });
			
			THEN("isActive returns true for 'commit' and returns false for 'pull'")
			{
				auto expected = true;
				auto result1 = parser.isActive("commit");
				auto result2 = parser.isActive("pull");

				REQUIRE(expected == result1);
				REQUIRE_FALSE(expected == result2);
			}

			THEN("value of '-m' is 'inital commit'")
			{
				auto expected = "inital commit";
				auto result1 = commit.getValues<std::string>("-m").front();

				REQUIRE(expected == result1);
			}
		}

		WHEN("'pull' is parsed")
		{

			parser.parse_argument({ "pull", "-q" });

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
		ArgumentParser& config = parser.add_parser("config");
		ArgumentParser& global = config.add_parser("--global");

		global.add_argument("--user.name")
			.help("adds user name to git").nargs(1).argumentType<std::string>();
		global.add_argument("--user.mail").
			help("adds user email to git").nargs(1).argumentType<std::string>();


		WHEN("'config --global user.name userName'")
		{

			parser.parse_argument({ "config", "--global", "--user.name", "userName" });

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

			parser.parse_argument({ "config", "--global", "--user.mail", "userEmail" });

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
		ArgumentParser& checkout = parser.add_parser("checkout").help("switches to a differet brnach");
		
		checkout.add_argument("-b")
			.help("adds a branch").nargs(1).argumentType<std::string>();
		checkout.add_argument("-B")
			.help("branch is created if it does not exist, otherwise branch is reset").nargs(1).argumentType<std::string>();

		WHEN("'checkout -b newBranch'")
		{

			parser.parse_argument({ "checkout", "-b", "newBranch" });

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

			parser.parse_argument({ "checkout", "-B", "newBranch" });

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