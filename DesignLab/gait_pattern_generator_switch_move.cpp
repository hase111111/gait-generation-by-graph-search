
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "gait_pattern_generator_switch_move.h"

#include <string>
#include <utility>

#include <magic_enum.hpp>

#include "cassert_define.h"


namespace designlab
{

GaitPatternGeneratorSwitchMove::GaitPatternGeneratorSwitchMove(
    std::unique_ptr<IGaitPatternGenerator>&& gpg_for_straight_ptr,
    std::unique_ptr<IGaitPatternGenerator>&& gpg_for_turn_spot_ptr) :
    gpg_for_straight_ptr_(std::move(gpg_for_straight_ptr)),
    gpg_for_turn_spot_ptr_(std::move(gpg_for_turn_spot_ptr))
{
    assert(gpg_for_straight_ptr_);
    assert(gpg_for_turn_spot_ptr_);
}

GraphSearchResult GaitPatternGeneratorSwitchMove::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node,
    const MapState& map,
    const RobotOperation& operation,
    RobotStateNode* output_node)
{
    using enum enums::RobotOperationType;

    if (operation.operation_type == kStraightMoveVector ||
        operation.operation_type == kStraightMovePosition)
    {
        return gpg_for_straight_ptr_->GetNextNodeByGraphSearch(
            current_node, map, operation, output_node);
    }
    else if (operation.operation_type == kSpotTurnRotAxis ||
             operation.operation_type == kSpotTurnLastPosture)
    {
        return gpg_for_turn_spot_ptr_->GetNextNodeByGraphSearch(
            current_node, map, operation, output_node);
    }
    else
    {
        assert(false);

        return { enums::Result::kFailure,
            "A non-existent behavior was specified." };
    }
}

}  // namespace designlab
