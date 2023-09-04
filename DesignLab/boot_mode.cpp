#include "boot_mode.h"


std::string std::to_string(const EBootMode boot_mode)
{
	switch (boot_mode)
	{
	case EBootMode::SIMULATION:
		return "SIMULATION";
	case EBootMode::VIEWER:
		return "VIEWER";
	case EBootMode::DISPLAY_TEST:
		return "DISPLAY_TEST";
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
	else if (str == "DISPLAY_TEST" || str == "display_test" || str == "Display_test" || str == "Display_Test")
	{
		return EBootMode::DISPLAY_TEST;
	}
	else
	{
		return EBootMode::SIMULATION;
	}
}