#include "boot_mode.h"


std::string std::to_string(const BootMode boot_mode)
{
	switch (boot_mode)
	{
	case BootMode::kSimulation:
		return "SIMULATION";
	case BootMode::kViewer:
		return "VIEWER";
	case BootMode::kDisplayTest:
		return "DISPLAY_TEST";
	case BootMode::kResultViewer:
		return "RESULT_VIEWER";
	default:
		return "UNKNOWN";
	}
}


BootMode std::sToMode(const std::string str)
{
	if (str == "SIMULATION" || str == "simulation" || str == "Simulation")
	{
		return BootMode::kSimulation;
	}
	else if (str == "VIEWER" || str == "viewer" || str == "Viewer")
	{
		return BootMode::kViewer;
	}
	else if (str == "DISPLAY_TEST" || str == "display_test" || str == "Display_test" || str == "Display_Test")
	{
		return BootMode::kDisplayTest;
	}
	else if (str == "RESULT_VIEWER" || str == "result_viewer" || str == "Result_viewer" || str == "Result_Viewer")
	{
		return BootMode::kResultViewer;
	}
	else
	{
		return BootMode::kSimulation;
	}
}