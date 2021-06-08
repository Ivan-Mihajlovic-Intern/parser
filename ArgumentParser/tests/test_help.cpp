#include "catch.hpp"
#include "testClass.h"

SCENARIO("Argument Parser help", "[help]")
{
	GIVEN("Argument parser")
	{
		ArgumentTest parser;

		WHEN("Arguments '-s, --sum' and 'inegers' are added")
		{
			parser.addArgument("-s", "--sum").help("returns a sum of integers");
			parser.addArgument("intgers").help("integers for sum").nargs(3).argumentType<int>().metavar("x");

			THEN("Help message look like this")
			{
				std::stringstream helpMessage;
				helpMessage << "Usage: program.exe [-h] [-v] [-s] x x x \n\n"
					<< "positional arguments:\n"
					<< " intgers        integers for sum\n\n"
					<< "optional arguments:\n"
					<< " -h, --help     shows this message and exits\n"
					<< " -v, --version  shows version and exits\n"
					<< " -s, --sum      returns a sum of integers\n\n";
					
				const auto expected = parser.printHelp();

				REQUIRE(expected == helpMessage.str());
			}
		}

		WHEN("Description, epilog, prog(programName), usage and arguments '-s, --sum' and 'inegers' are added")
		{
			parser.description("Parser description");
			parser.epilog("Parser epilog");
			parser.prog("calculator");
			parser.usage("--sum number number number");
			parser.addArgument("-s", "--sum").help("returns a sum of integers");
			parser.addArgument("intgers").help("integers for sum").nargs(3).argumentType<int>().metavar("x");

			THEN("Help message look like this")
			{
				std::stringstream helpMessage;
				helpMessage << "Usage: calculator --sum number number number\n\n"
					<< "Parser description\n\n"
					<< "positional arguments:\n"
					<< " intgers        integers for sum\n\n"
					<< "optional arguments:\n"
					<< " -h, --help     shows this message and exits\n"
					<< " -v, --version  shows version and exits\n"
					<< " -s, --sum      returns a sum of integers\n\n"
					<< "Parser epilog";

				const auto expected = parser.printHelp();

				REQUIRE(expected == helpMessage.str());
			}
		}

		WHEN("Arguments '-s, --sum' and 'inegers' are added")
		{
			parser.addArgument("-s", "--sum").help("returns a sum of integers");
			parser.addArgument("intgers").help("integers for sum").nargs(3).argumentType<int>().metavar("x");
			
			parser.subParsersTitle("Git commands")
				.subParsersDescription("Commands added to subParser")
				.subParsersHelp("git command commit");
			ArgumentParser& commit = parser.addParser("commit");
			commit.addArgument("-m", "--message")
				.help("adds a message to a current commit").nargs(1).argumentType<std::string>();
			commit.addArgument("-a", "--all").help("add all modified files");
			THEN("Help message look like this")
			{
				std::stringstream helpMessage;
				helpMessage << "Usage: program.exe [-h] [-v] [-s] x x x {commit}\n\n"
					<< "positional arguments:\n"
					<< " {commit}       git command commit\n"
					<< " intgers        integers for sum\n\n"
					<< "optional arguments:\n"
					<< " -h, --help     shows this message and exits\n"
					<< " -v, --version  shows version and exits\n"
					<< " -s, --sum      returns a sum of integers\n\n"
					<< "\nGit commands:\n"
					<< "Commands added to subParser\n"
					<< " {commit}       git command commit\n"
					<< " commit         \n";
				const auto expected = parser.printHelp();

				REQUIRE(expected == helpMessage.str());
			}
		}
	}
}