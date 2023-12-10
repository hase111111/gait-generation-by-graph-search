#include "simulation_end_checker_factory.h"

#include "cassert_define.h"
#include "designlab_quaternion.h"
#include "simulation_end_checker_by_goal_tape.h"
#include "simulation_end_checker_by_position.h"
#include "simulation_end_checker_by_posture.h"

namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;


std::unique_ptr<ISimulationEndChecker> SimulationEndCheckerFactory::Create(const std::shared_ptr<const SimulationSettingRecord>& record)
{
	if (record->simulation_end_check_type == SimulationEndCheckType::kGoalTape)
	{
		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByGoalTape>(record->goal_tape_position_x);

		return std::move(simulation_end_checker);
	}
	else if (record->simulation_end_check_type == SimulationEndCheckType::kPosture)
	{
		const dl::Quaternion x = dl::Quaternion::MakeByAngleAxis(record->target_posture_x_angle, dl::Vector3::GetFrontVec());
		const dl::Quaternion y = dl::Quaternion::MakeByAngleAxis(record->target_posture_y_angle, dl::Vector3::GetUpVec());
		const dl::Quaternion z = dl::Quaternion::MakeByAngleAxis(record->target_posture_z_angle, dl::Vector3::GetLeftVec());

		const dl::Quaternion goal_orientation = x * y * z;

		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByPosture>(goal_orientation, dlm::ConvertDegToRad(record->target_posture_allowable_error));

		return std::move(simulation_end_checker);
	}
	else if (record->simulation_end_check_type == SimulationEndCheckType::kPosition)
	{
		const dl::Vector3 goal_position(record->target_position_x, record->target_position_y, record->target_position_z);

		auto simulation_end_checker = std::make_unique<SimulationEndCheckerByPosition>(goal_position, record->target_position_allowable_error);

		return std::move(simulation_end_checker);
	}
	else
	{
		assert(false);
	}

	return nullptr;
}