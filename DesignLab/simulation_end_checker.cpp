#include "simulation_end_checker.h"


bool SimulationEndChecker::IsEnd(const RobotStateNode& node) const
{
	if (node.global_center_of_mass.x > kGoalTape)
	{
		return true;
	}
	else
	{
		return false;
	}
}