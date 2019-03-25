#include "parser.h"

//**************************************************************************************************
//	Parser::Parse
//--------------------------------------------------------------------------------------------------
std::shared_ptr<Schedules> Parser::Parse(const Strings& _data)
{
	std::shared_ptr<Schedules> schedules = std::make_shared<Schedules>();

	for(const std::string& str : _data)
	{
		size_t posBegin = 0;
		std::string offsetBegin_t	= str.substr(posBegin, str.find(DELIMITER, posBegin));
		posBegin += offsetBegin_t.size() + DELIMITER.size();
		std::string offsetEnd_t	= str.substr(posBegin, str.find(DELIMITER, posBegin) - posBegin);
		posBegin += offsetEnd_t.size() + DELIMITER.size();

		int32_t offsetBegin	= atoi(offsetBegin_t.c_str());
		int32_t offsetEnd	= atoi(offsetEnd_t.c_str());

		size_t posEnd = 0;
		while((posEnd = str.find(DELIMITER, posBegin)) != std::string::npos)
		{
			MakeSchedule(schedules, str, posBegin, posEnd, offsetBegin, offsetEnd);
			posBegin += posEnd - posBegin + DELIMITER.size();
		}
		MakeSchedule(schedules, str, posBegin, std::string::npos, offsetBegin, offsetEnd);
	}
	return schedules;
}
//==================================================================================================
//	Parser::MakeSchedule
//--------------------------------------------------------------------------------------------------
void Parser::MakeSchedule(const std::shared_ptr<Schedules>&	_schedules,
						  const std::string&				_source,
						  size_t							_posBegin,
						  size_t							_posEnd,
						  int32_t							_offsetBegin,
						  int32_t							_offsetEnd)
{
	std::string day_t = _source.substr(_posBegin, _posEnd - _posBegin);
	TimeStamp::Days day = DAYS[day_t];
	_schedules->push_back(std::make_shared<Schedule>(TimeStamp(day, _offsetBegin), TimeStamp(day, _offsetEnd)));
}
