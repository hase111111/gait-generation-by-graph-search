#include "output_detail.h"


std::string std::to_string(OutputDetail detail)
{
	switch (detail)
	{
	case OutputDetail::kSystem:
		return "SYSTEM";
	case OutputDetail::kError:
		return "ERROR";
	case OutputDetail::kWarning:
		return "WARNING";
	case OutputDetail::kInfo:
		return "INFO";
	case OutputDetail::kDebug:
		return "DEBUG";
	default:
		return "UNKNOWN";
	}
}


OutputDetail std::toOutputPriority(const std::string& str)
{
	if (str == "SYSTEM" || str == "system" || str == "System")
	{
		return OutputDetail::kSystem;
	}
	else if (str == "ERROR" || str == "error" || str == "Error")
	{
		return OutputDetail::kError;
	}
	else if (str == "WARNING" || str == "warning" || str == "Warning")
	{
		return OutputDetail::kWarning;
	}
	else if (str == "INFO" || str == "info" || str == "Info")
	{
		return OutputDetail::kInfo;
	}
	else if (str == "DEBUG" || str == "debug" || str == "Debug")
	{
		return OutputDetail::kDebug;
	}
	else
	{
		return OutputDetail::kSystem;
	}

}
