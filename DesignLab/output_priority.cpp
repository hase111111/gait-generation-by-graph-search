#include "output_priority.h"


std::string std::to_string(EOutputPriority priority)
{
	switch (priority)
	{
	case EOutputPriority::SYSTEM:
		return "SYSTEM";
	case EOutputPriority::ERROR:
		return "ERROR";
	case EOutputPriority::WARNING:
		return "WARNING";
	case EOutputPriority::INFO:
		return "INFO";
	case EOutputPriority::DEBUG:
		return "DEBUG";
	default:
		return "UNKNOWN";
	}
}


EOutputPriority std::toOutputPriority(const std::string& str)
{
	if (str == "SYSTEM" || str == "system" || str == "System")
	{
		return EOutputPriority::SYSTEM;
	}
	else if (str == "ERROR" || str == "error" || str == "Error")
	{
		return EOutputPriority::ERROR;
	}
	else if (str == "WARNING" || str == "warning" || str == "Warning")
	{
		return EOutputPriority::WARNING;
	}
	else if (str == "INFO" || str == "info" || str == "Info")
	{
		return EOutputPriority::INFO;
	}
	else if (str == "DEBUG" || str == "debug" || str == "Debug")
	{
		return EOutputPriority::DEBUG;
	}
	else
	{
		return EOutputPriority::SYSTEM;
	}

}
