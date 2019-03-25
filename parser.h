#pragma once

#include "schedule.h"

extern std::string DELIMITER;

using Strings = std::vector<std::string>;

//**************************************************************************************************
//	Parser
//--------------------------------------------------------------------------------------------------
class Parser
{
public:
	Parser()							= default;
	Parser(const Parser&)				= default;
	Parser(Parser&&)					= default;
	virtual ~Parser()					= default;

	Parser&	operator=(const Parser&)	= default;
	Parser&	operator=(Parser&&)			= default;

	std::shared_ptr<Schedules>	Parse(const Strings& data);

protected:
	void						MakeSchedule(const std::shared_ptr<Schedules>&,
											 const std::string&	source,
											 size_t				posBegin,
											 size_t				posEnd,
											 int32_t			offsetBegin,
											 int32_t			offsetEnd);
};