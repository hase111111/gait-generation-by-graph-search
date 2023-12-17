#include "simulation_end_checker_factory.h"

#include "cassert_define.h"
#include "designlab_quaternion.h"
#include "designlab_rot_converter.h"
#include "simulation_end_checker_by_goal_tape.h"
#include "simulation_end_checker_by_position.h"
#include "simulation_end_checker_by_posture.h"

namespace designlab
{

std::unique_ptr<ISimulationEndChecker> SimulationEndCheckerFactory::Create(const SimulationSettingRecord& record)
{
	if (record.end_check_mode == enums::SimulationEndCheckMode::kGoalTape)
	{
		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByGoalTape>(record.goal_tape_position_x);

		return std::move(simulation_end_checker);
	}
	else if (record.end_check_mode == enums::SimulationEndCheckMode::kPosture)
	{
		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByPosture>(
			ToQuaternion(record.target_posture),
			math_util::ConvertDegToRad(record.target_posture_allowable_error_deg)
		);

		return std::move(simulation_end_checker);
	}
	else if (record.end_check_mode == enums::SimulationEndCheckMode::kPosition)
	{
		const Vector3 goal_position(record.target_position);

		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByPosition>(goal_position, record.target_position_allowable_error);

		return std::move(simulation_end_checker);
	}
	else
	{
		assert(false);
	}

	return nullptr;
}

} // namespace designlab