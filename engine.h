#pragma once

#include "instrument.h"
#include "parser.h"
#include <memory>

class iStream;

//**************************************************************************************************
//	Schedules_t
//--------------------------------------------------------------------------------------------------
using Schedules_t = std::map<std::pair<int32_t, int32_t>, std::vector<TimeStamp::Days>>;

//**************************************************************************************************
//	Engine
//--------------------------------------------------------------------------------------------------
class Engine
{
public:
	enum Mode
	{
		INVALID		= -1,
		INTERSECT,
		UNION,
		EXIT
	};

public:
	Engine(std::shared_ptr<iStream>);
	Engine()													= delete;
	Engine(const Engine&)										= delete;
	Engine(Engine&&)											= delete;
	virtual ~Engine()											= default;

	Engine&							operator=(const Engine&)	= delete;
	Engine&							operator=(Engine&&)			= delete;

	void							Start();

protected:
	void							Done();
	bool							Input();
	void							InputMode();
	bool							InputInstrument(Instrument&);
	std::shared_ptr<Schedules_t>	Process();
	void							Output(std::shared_ptr<Schedules_t>);
	std::shared_ptr<Schedules_t>	GetIntersectingSchedules();
	std::shared_ptr<Schedules_t>	GetUnionSchedules();
	std::shared_ptr<Schedules_t>	Unify(const std::shared_ptr<Schedules>&);

	std::shared_ptr<iStream>		stream;
	Parser							parser;
	Instrument						firstInstrument;
	Instrument						secondInstrument;

	Mode							mode	= Mode::INVALID;
};

//**************************************************************************************************
//	MODES
//--------------------------------------------------------------------------------------------------
static std::map<std::string, Engine::Mode>	MODES =
{
	{"intersect",	Engine::Mode::INTERSECT},
	{"unite",		Engine::Mode::UNION},
	{"exit",		Engine::Mode::EXIT}
};
