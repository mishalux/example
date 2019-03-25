#pragma once

#include "stdint.h"
#include <string>
#include <map>
#include <vector>

//**************************************************************************************************
//	TimeStamp
//--------------------------------------------------------------------------------------------------
class TimeStamp
{
public:
	enum Days : int32_t
	{
		INVALID	= -1,
		SUNDAY,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

public:
	TimeStamp(Days, int32_t offset);
	TimeStamp()									= delete;
	TimeStamp(const TimeStamp&)					= default;
	TimeStamp(TimeStamp&&)						= default;
	virtual ~TimeStamp()						= default;

	TimeStamp&		operator=(const TimeStamp&)	= default;
	TimeStamp&		operator=(TimeStamp&&)		= default;

	bool			operator==(const TimeStamp&)		const;
	bool			operator!=(const TimeStamp& _other)	const	{ return !operator==(_other); }
	bool			operator<(const TimeStamp&)			const;
	bool			operator<=(const TimeStamp&)		const;

	const Days		Day()								const	{ return day; }
	const int32_t	Offset()							const	{ return offset; }
	void			Day(const Days _day)						{ day = _day; }
	void			Offset(const int32_t _offset)				{ offset = _offset; }

protected:
	Days			day		= Days::INVALID;
	int32_t			offset	= 0;
};

//**************************************************************************************************
//	TimeStamp::TimeStamp
//--------------------------------------------------------------------------------------------------
using Days = std::map<std::string, TimeStamp::Days>;

static Days DAYS =
{
	{"Sunday",		TimeStamp::Days::SUNDAY},
	{"Monday",		TimeStamp::Days::MONDAY},
	{"Tuesday",		TimeStamp::Days::TUESDAY},
	{"Wednesday",	TimeStamp::Days::WEDNESDAY},
	{"Thursday",	TimeStamp::Days::THURSDAY},
	{"Friday",		TimeStamp::Days::FRIDAY},
	{"Saturday",	TimeStamp::Days::SATURDAY},
};

//**************************************************************************************************
//	TimeStamp::ToString
//--------------------------------------------------------------------------------------------------
std::string ToString(const std::vector<TimeStamp::Days>&);