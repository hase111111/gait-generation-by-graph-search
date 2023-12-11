#include "gait_pattern_generator_switch_move.h"

#include "cassert_define.h"


namespace dle = ::designlab::enums;


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
	const RobotOperation& operation,
	RobotStateNode* output_node
)
{
	if (operation.operation_type == dle::RobotOperationType::kStraightMoveVector || operation.operation_type == dle::RobotOperationType::kStraightMovePosition)
	{
		return gait_pattern_generator_for_straigt_ptr_->GetNextNodebyGraphSearch(
			current_node,
			map,
			operation,
			output_node
		);
	}
	else if (operation.operation_type == dle::RobotOperationType::kSpotTurnRotAxis || operation.operation_type == dle::RobotOperationType::kSpotTurnLastPosture)
	{
		return gait_pattern_generator_for_turn_spot_ptr_->GetNextNodebyGraphSearch(
			current_node,
			map,
			operation,
			output_node
		);
	}
	else
	{
		assert(false);
		return GraphSearchResult::kFailure;
	}
}