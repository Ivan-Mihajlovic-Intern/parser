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
		return m_optionalArguments;
	}

	std::vector<Argument> getPositionalArguments() const
	{
		return m_positionalArguments;
	}

	std::map<std::string, bool> getActiveArguments() const
	{
		return m_activeArguments;
	}

	std::string getProgramName() const
	{
		return m_programName;
	}

	std::list<ArgumentParser> getSubParsers() const
	{
		return m_subParsers;
	}

private:
	//some maps or set of arguments
};

#endif //_TESTCLASS_HPP