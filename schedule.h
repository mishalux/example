#pragma once

#include "time_stamp.h"
#include <vector>
#include <memory>

//**************************************************************************************************
//	Schedule
//--------------------------------------------------------------------------------------------------
class Schedule
{
public:
	Schedule(TimeStamp begin, TimeStamp end);
	Schedule()										= delete;
	Schedule(const Schedule&)						= default;
	Schedule(Schedule&&)							= default;
	virtual ~Schedule()								= default;

	Schedule&			operator=(const Schedule&)	= default;
	Schedule&			operator=(Schedule&&)		= default;

	const TimeStamp&	Begin()							const	{ return begin; }
	const TimeStamp&	End()							const	{ return end; }
	void				Begin(const TimeStamp& _begin)			{ begin	= _begin; }
	void				End(const TimeStamp& _end)				{ end	= _end; }
	bool				IsIntersected(const Schedule&)	const;

protected:
	TimeStamp			begin;
	TimeStamp			end;
};

//**************************************************************************************************
//	Schedules
//--------------------------------------------------------------------------------------------------
using Schedules = std::vector<std::shared_ptr<Schedule>>;