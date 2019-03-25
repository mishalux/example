#include "time_stamp.h"

std::string	DELIMITER = ",";
int32_t		MINUTES_IN_DAY = 60 * 24;

//**************************************************************************************************
//	TimeStamp::TimeStamp
//--------------------------------------------------------------------------------------------------
TimeStamp::TimeStamp(Days _day, int32_t _offset)
{
	int32_t daysInOffset = _offset / MINUTES_IN_DAY;
	if(_offset < 0)
	{
		offset	= _offset % MINUTES_IN_DAY + MINUTES_IN_DAY;
		day		= (Days)(_day + daysInOffset - 1);
	}
	else if(daysInOffset)
	{
		offset	= _offset % MINUTES_IN_DAY;
		day		= (Days)(_day + daysInOffset);
	}
	else
	{
		day		= _day;
		offset	= _offset;
	}
}
//==================================================================================================
//	TimeStamp::operator==
//--------------------------------------------------------------------------------------------------
bool TimeStamp::operator==(const TimeStamp& _other) const
{
	return day == _other.day && offset == _other.offset;
}
//==================================================================================================
//	TimeStamp::operator<
//--------------------------------------------------------------------------------------------------
bool TimeStamp::operator<(const TimeStamp& _other) const
{
	if(day < _other.day)
	{
		return true;
	}
	else if(day > _other.day)
	{
		return false;
	}
	else
	{
		return offset < _other.offset;
	}
}
//==================================================================================================
//	TimeStamp::operator<=
//--------------------------------------------------------------------------------------------------
bool TimeStamp::operator<=(const TimeStamp& _other) const
{
	return *this < _other || *this == _other;
}

//**************************************************************************************************
//	TimeStamp::ToString
//--------------------------------------------------------------------------------------------------
std::string ToString(const std::vector<TimeStamp::Days>& _days)
{
	std::string days;
	for(TimeStamp::Days day : _days)
	{
		for(Days::const_iterator it = DAYS.begin(); it != DAYS.end(); ++it)
		{
			if(it->second == day) { days += (DELIMITER + it->first); }
		}
	}
	return days;
}
