#include "boot_mode.h"


std::string std::to_string(const EBootMode boot_mode)
{
	switch (boot_mode)
	{
	case EBootMode::SIMULATION:
		return "SIMULATION";
	case EBootMode::VIEWER:
		return "VIEWER";
	default:
		return "UNKNOWN";
	}
}


EBootMode std::sToMode(const std::string str)
{
	if (str == "SIMULATION" || str == "simulation" || str == "Simulation")
	{
		return EBootMode::SIMULATION;
	}
	else if (str == "VIEWER" || str == "viewer" || str == "Viewer")
	{
		return EBootMode::VIEWER;
	}
	else
	{
		return EBootMode::SIMULATION;
	}
}