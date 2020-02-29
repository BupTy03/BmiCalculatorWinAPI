#pragma once

#include <string>


namespace logging_impl
{
	void LogToConsole(const char* tag, const std::string& message);
}


#define LogMsg(tag, message) (logging_impl::LogToConsole(tag, message))


#ifndef NDEBUG
	#define LogDebug(message) LogMsg("DEBUG", message)
#else
	#define LogDebug(message) ((void)0)
#endif // !NDEBUG


