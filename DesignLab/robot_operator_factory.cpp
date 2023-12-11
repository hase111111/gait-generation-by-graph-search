#include "robot_operator_factory.h"

#include "cassert_define.h"
#include "robot_operation.h"
#include "robot_operator_for_gpg.h"
#include "robot_operator_fixed.h"


std::unique_ptr<IRobotOperator> RobotOperatorFactory::Create(const SimulationSettingRecord& setting)
{
	if (setting.simulation_target_update_mode == SimulationTargetUpdateMode::kForGloabalPathGeneration)
	{
		return std::make_unique<RobotOperatorForGpg>();
	}
	else if (setting.simulation_target_update_mode == SimulationTargetUpdateMode::kOnce)
	{
		return std::make_unique<RobotOperatorFixed>(RobotOperation());
	}
	else
	{
		assert(false);
		return nullptr;
	}
}