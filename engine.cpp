#include "engine.h"
#include "schedule.h"
#include "stream.h"
#include <algorithm>
#include <vector>

extern int32_t MINUTES_IN_DAY;

//**************************************************************************************************
//	Engine::Engine
//--------------------------------------------------------------------------------------------------
Engine::Engine(std::shared_ptr<iStream> _stream)
: stream(_stream)
{}
//==================================================================================================
//	Engine::Start
//--------------------------------------------------------------------------------------------------
void Engine::Start()
{
	while(Input())
	{
		Output(Process());
		Done();
	}
}
//==================================================================================================
//	Engine::Done
//--------------------------------------------------------------------------------------------------
void Engine::Done()
{
	firstInstrument.Clear();
	secondInstrument.Clear();
}
//==================================================================================================
//	Engine::Input
//--------------------------------------------------------------------------------------------------
bool Engine::Input()
{
	InputMode();
	if(mode == Mode::EXIT)
	{
		return false;
	}
	if(!InputInstrument(firstInstrument) || !InputInstrument(secondInstrument))
	{
		return false;
	}
	return true;
}
//==================================================================================================
//	Engine::InputMode
//--------------------------------------------------------------------------------------------------
void Engine::InputMode()
{
	std::string mode_t;
	do
	{
		std::cout << "Input:" << std::endl;

		stream->Read(mode_t);
		if(MODES.find(mode_t) == MODES.end())
		{
			stream->Write("There is no the next mode: " + mode_t + "\nmodes: intersect, unite, exit\n");
		}
		else
		{
			break;
		}
	}while(true);

	mode = MODES.at(mode_t);
}
//==================================================================================================
//	Engine::InputInstrument
//--------------------------------------------------------------------------------------------------
bool Engine::InputInstrument(Instrument& _instrument)
{
	std::string countSchedules_t;
	stream->Read(countSchedules_t);
	int64_t countSchedules = atoi(countSchedules_t.c_str());
	if(countSchedules <= 0)
	{
		stream->Write("[ERROR][Engine::Input] Number of schedules is not valid.");
		return false;
	}
	Strings schedules;
	for(int32_t i(0); i < countSchedules; ++i)
	{
		std::string inputData;
		stream->Read(inputData);
		if(inputData.empty())
		{
			stream->Write("[ERROR][Engine::Input] There is no input data.");
			return false;
		}
		schedules.push_back(inputData);
	}
	_instrument.SetShedules(parser.Parse(schedules));
	return true;
}
//==================================================================================================
//	Engine::Process
//--------------------------------------------------------------------------------------------------
std::shared_ptr<Schedules_t> Engine::Process()
{
	switch(mode)
	{
	case Engine::INTERSECT:	return GetIntersectingSchedules();
	case Engine::UNION:		return GetUnionSchedules();
	default:				return nullptr;
	}
}
//==================================================================================================
//	Engine::Output
//--------------------------------------------------------------------------------------------------
void Engine::Output(std::shared_ptr<Schedules_t> _schedules)
{
	std::cout << "Output:" << std::endl;

	for(Schedules_t::const_iterator schedule = _schedules->begin(); schedule != _schedules->end(); ++schedule)
	{
		stream->Write(std::to_string(schedule->first.first)
					  + DELIMITER
					  + std::to_string(schedule->first.second)
					  + ToString(schedule->second));
	}
}
//==================================================================================================
//	Engine::GetIntersectingSchedules
//--------------------------------------------------------------------------------------------------
std::shared_ptr<Schedules_t> Engine::GetIntersectingSchedules()
{
	std::shared_ptr<Schedules> intersectingSchedules = std::make_shared<Schedules>();

	for(std::shared_ptr<Schedule> scheduleFirstInstrument : *firstInstrument.GetSchedules())
	{
		for(std::shared_ptr<Schedule> scheduleSecondInstrument : *secondInstrument.GetSchedules())
		{
			if (scheduleFirstInstrument->IsIntersected(*scheduleSecondInstrument))
			{
				std::shared_ptr<Schedule> intersectingSchedule
					= std::make_shared<Schedule>(std::max(scheduleFirstInstrument->Begin(), scheduleSecondInstrument->Begin()),
												 std::min(scheduleFirstInstrument->End(), scheduleSecondInstrument->End()));
				intersectingSchedules->push_back(intersectingSchedule);
			}
		}
	}
	return Unify(intersectingSchedules);
}
//==================================================================================================
//	Engine::GetUnionSchedules
//--------------------------------------------------------------------------------------------------
std::shared_ptr<Schedules_t> Engine::GetUnionSchedules()
{
	Schedules commonInstrument(firstInstrument.GetSchedules()->size() + secondInstrument.GetSchedules()->size());

	std::copy(firstInstrument.GetSchedules()->begin(), firstInstrument.GetSchedules()->end(), commonInstrument.begin());
	std::copy(secondInstrument.GetSchedules()->begin(),
			  secondInstrument.GetSchedules()->end(),
			  commonInstrument.begin() + firstInstrument.GetSchedules()->size());

	std::sort(commonInstrument.begin(), commonInstrument.end(), [](const std::shared_ptr<Schedule>& _scheduleFirst,
																   const std::shared_ptr<Schedule>& _scheduleSecond) -> bool
	{
		return _scheduleFirst->Begin() < _scheduleSecond->Begin();
	});

	std::shared_ptr<Schedules> unionSchedules = std::make_shared<Schedules>();
	bool isPreviousIntersected = false;
	for(Schedules::iterator first = commonInstrument.begin(); first != commonInstrument.end(); ++first)
	{
		Schedules::iterator second = first + 1;
		if(second != commonInstrument.end() && (*second)->Begin() < (*first)->End())
		{
			unionSchedules->push_back(std::make_shared<Schedule>((*first)->Begin(), (*second)->Begin()));
			unionSchedules->push_back(std::make_shared<Schedule>((*second)->Begin(), std::min((*second)->End(), (*first)->End())));

			Schedules::iterator third = second + 1;
			if(third != commonInstrument.end() && (*third)->Begin() < (*first)->End())
			{
				unionSchedules->push_back(std::make_shared<Schedule>((*second)->End(),(*third)->Begin()));
				unionSchedules->push_back(std::make_shared<Schedule>((*third)->Begin(),(*first)->End()));
				(*third)->Begin((*first)->End());
			}
			else
			{
				unionSchedules->push_back(std::make_shared<Schedule>(std::min((*first)->End(), (*second)->End()),
																	 std::max((*first)->End(), (*second)->End())));
			}
			isPreviousIntersected = true;
		}
		else
		{
			if(!isPreviousIntersected)
			{
				unionSchedules->push_back(*first);
			}
			isPreviousIntersected = false;
		}
	}
	return Unify(unionSchedules);
}
//==================================================================================================
//	Engine::Unify
//--------------------------------------------------------------------------------------------------
std::shared_ptr<Schedules_t> Engine::Unify(const std::shared_ptr<Schedules>& _schedules)
{
	std::shared_ptr<Schedules_t> intersectingSchedules = std::make_shared<Schedules_t>();

	for(const std::shared_ptr<Schedule>& schedule : *_schedules)
	{
		TimeStamp begin	= schedule->Begin();
		TimeStamp end	= schedule->End();
		if(begin.Day() != end.Day())
		{
			int32_t diff = end.Day() - begin.Day();
			begin.Day((TimeStamp::Days)(begin.Day() + diff));
			begin.Offset(begin.Offset() - diff * MINUTES_IN_DAY);
		}

		std::pair<int32_t, int32_t> key(begin.Offset(), end.Offset());
		(*intersectingSchedules)[key].push_back(begin.Day());
	}
	return intersectingSchedules;
}
