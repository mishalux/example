#pragma once

#include <string>
#include <iostream>

//**************************************************************************************************
//	iStream
//--------------------------------------------------------------------------------------------------
class iStream
{
public:
	iStream()									= default;
	iStream(const iStream&)						= delete;
	iStream(iStream&&)							= delete;
	virtual ~iStream()							= default;

	iStream&		operator=(const iStream&)	= delete;
	iStream&		operator=(iStream&&)		= delete;

	virtual bool	Init()									= 0;
	virtual void	Read(std::string& _str)					= 0;
	virtual void	Write(const std::string& _str)	const	= 0;
};

//**************************************************************************************************
//	StreamConsole
//--------------------------------------------------------------------------------------------------
class StreamConsole : public iStream
{
public:
	StreamConsole()									= default;
	StreamConsole(const StreamConsole&)				= delete;
	StreamConsole(StreamConsole&&)					= delete;
	virtual ~StreamConsole()						= default;

	StreamConsole&	operator=(const StreamConsole&)	= delete;
	StreamConsole&	operator=(StreamConsole&&)		= delete;

	virtual bool	Init()									override	{ return true; }
	virtual void	Read(std::string& _str)					override	{ std::cin >> _str; }
	virtual void	Write(const std::string& _str)	const	override	{ std::cout << _str << std::endl; }
};