#include "simulation_end_checker_factory.h"

#include "simulation_end_checker_by_goal_tape.h"

std::unique_ptr<ISimulationEndChecker> SimulationEndCheckerFactory::Create(const std::shared_ptr<const SimulationSettingRecord>& record)
{
	auto simulation_end_checker = std::make_unique<SimulationEndCheckerByGoalTape>(record->goal_tape_position);

	return std::move(simulation_end_checker);
}