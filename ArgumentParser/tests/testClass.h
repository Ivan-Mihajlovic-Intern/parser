#pragma once

#ifndef _TESTCLASS_HPP
#define _TESTCLASS_HPP

#include "ArgumentParser.h"


class ArgumentTest : public ArgumentParser
{
public:
	ArgumentTest()
	{

	}

	std::vector<Argument> getOptionalArguments() const 
	{
		return _optionalArguments;
	}

	std::vector<Argument> getPositionalArguments() const
	{
		return _positionalArguments;
	}

	std::map<std::string, bool> getActiveArguments() const
	{
		return _activeArguments;
	}

	std::string getProgramName() const
	{
		return _programName;
	}

	std::list<ArgumentParser> getSubParsers() const
	{
		return _subParsers;
	}

private:
	//some maps or set of arguments
};

#endif //_TESTCLASS_HPP