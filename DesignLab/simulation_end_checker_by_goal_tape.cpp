#include "simulation_end_checker_by_goal_tape.h"


SimulationEndCheckerByGoalTape::SimulationEndCheckerByGoalTape(const float goal_tape_x) : kGoalTape(goal_tape_x)
{
}

bool SimulationEndCheckerByGoalTape::IsEnd(const RobotStateNode& node) const
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