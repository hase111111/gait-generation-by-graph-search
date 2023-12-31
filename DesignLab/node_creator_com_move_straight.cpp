
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_com_move_straight.h"

#include <numbers>


namespace designlab
{

NodeCreatorComMoveStraight::NodeCreatorComMoveStraight(
  const DividedMapState& divided_map,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  enums::HexapodMove next_move
) :
    map_(divided_map),
    next_move_(next_move),
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr)
{
    for (size_t i = 0; i < kCandidateDirectionNum; i++)
    {
        const float rad = static_cast<float>(i) * 2.f * std::numbers::pi_v<float> /
            static_cast<float>(kCandidateDirectionNum);

        candidate_directions_[i] = Vector3(std::cos(rad), std::sin(rad), 0.0f);
    }
}

void NodeCreatorComMoveStraight::Create(const RobotStateNode& current_node,
                                        int current_num,
                                        std::vector<RobotStateNode>* output_graph) const
{
    RobotStateNode next_node;

    std::array<Vector3, kCandidateDirectionNum> candidate_directions_rotated;

    for (size_t i = 0; i < kCandidateDirectionNum; i++)
    {
        candidate_directions_rotated[i] = RotateVector3(candidate_directions_[i],
                                                        current_node.posture);
    }

    for (size_t i = 0; i < kCandidateDirectionNum; i++)
    {
        bool is_able = false;

        next_node = current_node;
        int able_count = 0;

        for (float j = kMoveDistanceStep; j <= kMaxMoveDistance; j += kMoveDistanceStep)
        {
            next_node.ChangeGlobalCenterOfMass(current_node.center_of_mass_global_coord +
                                               candidate_directions_rotated[i] * j, false);

            // IsLegInRange，IsStable，IsBodyInterfering を確認する．
            if (checker_ptr_->IsAllLegInRange(next_node.leg_state, next_node.leg_pos) &&
              checker_ptr_->IsStable(next_node.leg_state, next_node.leg_pos) &&
              !checker_ptr_->IsBodyInterferingWithGround(next_node, map_))
            {
                is_able = true;
                able_count = static_cast<int>(j);
            }
            else
            {
                break;
            }
        }

        if (is_able)
        {
            next_node.ChangeGlobalCenterOfMass(current_node.center_of_mass_global_coord +
                                               candidate_directions_rotated[i] *
                                               static_cast<float>(able_count), false);

            next_node.ChangeToNextNode(current_num, next_move_);

            // discreate_leg_posを更新する．
            for (int j = 0; j < HexapodConst::kLegNum; j++)
            {
                leg_func::ChangeDiscreteLegPos(j, enums::DiscreteLegPos::kCenter,
                                               &next_node.leg_state);
            }

            output_graph->push_back(next_node);
        }
    }
}


}  // namespace designlab
