#include "target_updater_factory.h"

#include "cassert_define.h"
#include "target_robot_state.h"
#include "target_updater_for_gpg.h"
#include "target_updater_once.h"


std::unique_ptr<ITargetUpdater> TargetUpdaterFactory::Create(const SimulationSettingRecord& setting)
{
	if (setting.simulation_target_update_mode == SimulationTargetUpdateMode::kForGloabalPathGeneration)
	{
		return std::make_unique<TargetUpdaterForGpg>();
	}
	else if (setting.simulation_target_update_mode == SimulationTargetUpdateMode::kOnce)
	{
		return std::make_unique<TargetUpdaterOnce>(TargetRobotState());
	}
	else
	{
		assert(false);
		return nullptr;
	}
}