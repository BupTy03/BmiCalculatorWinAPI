#include "logging.h"

#include <iostream>
#include <iomanip>


void logging_impl::LogToConsole(const char* tag, const std::string& message)
{
	std::time_t currTime = std::time(nullptr);
	std::tm pLocalTime;
	localtime_s(&pLocalTime, &currTime);
	std::cout << std::put_time(&pLocalTime, "%F") << ' ' << std::put_time(&pLocalTime, "%T") << ' ' 
		<< tag << ' ' << message << std::endl;
}
