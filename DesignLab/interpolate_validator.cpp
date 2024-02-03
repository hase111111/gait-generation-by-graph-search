
/// @author    Hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "interpolate_validator.h"


namespace designlab
{

InterpolateValidator::InterpolateValidator(const std::shared_ptr<const IHexapodCoordinateConverter>& converter,
                                           const std::shared_ptr<const IHexapodJointCalculator>& calculator) :
    converter_(converter),
    calculator_(calculator),
    interpolated_node_creator_{ converter_ }
{
}


bool InterpolateValidator::IsValid(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
    // 補完するノード群を生成する．
    const std::vector<RobotStateNode> interpolated_nodes =
        interpolated_node_creator_.CreateInterpolatedNode(current_node, next_node);

    // まず，現在のノードが可動範囲内であるか確認する．
    const auto joint_current = calculator_->CalculateAllJointState(current_node);
    if (!calculator_->IsValidAllJointState(current_node, joint_current))
    {
        return false;
    }

    // 次のノードが可動範囲内であるか確認する．
    const auto joint_next = calculator_->CalculateAllJointState(next_node);
    if (!calculator_->IsValidAllJointState(next_node, joint_next))
    {
        return false;
    }

    // 補完されたノードが可動範囲内であるか確認する．
    for (const auto& node : interpolated_nodes)
    {
        const auto joint = calculator_->CalculateAllJointState(node);
        if (!calculator_->IsValidAllJointState(node, joint))
        {
            return false;
        }
    }

    return true;
}

}