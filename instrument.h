#pragma once

#include "schedule.h"

//**************************************************************************************************
//	Instrument
//--------------------------------------------------------------------------------------------------
class Instrument
{
public:
	Instrument()												= default;
	Instrument(const Instrument&)								= delete;
	Instrument(Instrument&&)									= delete;
	virtual ~Instrument()										= default;

	Instrument&					operator=(const Instrument&)	= delete;
	Instrument&					operator=(Instrument&&)			= delete;

	void						SetShedules(const std::shared_ptr<Schedules>& _schedules)	{ schedules = _schedules; }
	void						SetShedules(std::shared_ptr<Schedules>&& _schedules)		{ schedules = _schedules; }
	std::shared_ptr<Schedules>	GetSchedules()	const										{ return schedules; }
	void						Clear()														{ schedules->clear(); }

protected:
	std::shared_ptr<Schedules>	schedules;
};