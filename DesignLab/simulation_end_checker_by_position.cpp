#include "simulation_end_checker_by_position.h"


namespace designlab
{

SimulationEndCheckerByPosition::SimulationEndCheckerByPosition(
	const Vector3& goal_position,
	const float allowable_error
) :
	goal_position_(goal_position),
	allowable_error_(allowable_error)
{
}

bool SimulationEndCheckerByPosition::IsEnd(const RobotStateNode& node) const
{
	//位置を取得し，誤差を計算
	float dif = (node.global_center_of_mass - goal_position_).GetLength();

	if (dif > allowable_error_)
	{
		return false;
	}

	return true;
}

} // namespace designlab