#include "gait_pattern_generator_switch_move.h"

#include "cassert_define.h"


GaitPatternGeneratorSwitchMove::GaitPatternGeneratorSwitchMove(
	std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_straigt,
	std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_for_turn_spot
) :
	gait_pattern_generator_for_straigt_ptr_(std::move(gait_pattern_generator_for_straigt)),
	gait_pattern_generator_for_turn_spot_ptr_(std::move(gait_pattern_generator_for_turn_spot))
{
}

GraphSearchResult GaitPatternGeneratorSwitchMove::GetNextNodebyGraphSearch(
	const RobotStateNode& current_node,
	const MapState& map,
	const TargetRobotState& target,
	RobotStateNode* output_node
)
{
	if (target.GetTargetMode() == TargetMode::kStraightMoveVector || target.GetTargetMode() == TargetMode::kStraightMovePosition)
	{
		return gait_pattern_generator_for_straigt_ptr_->GetNextNodebyGraphSearch(
			current_node,
			map,
			target,
			output_node
		);
	}
	else if (target.GetTargetMode() == TargetMode::kSpotTurnRotAxis || target.GetTargetMode() == TargetMode::kSpotTurnLastPosture)
	{
		return gait_pattern_generator_for_turn_spot_ptr_->GetNextNodebyGraphSearch(
			current_node,
			map,
			target,
			output_node
		);
	}
	else
	{
		assert(false);
		return GraphSearchResult::kFailure;
	}
}