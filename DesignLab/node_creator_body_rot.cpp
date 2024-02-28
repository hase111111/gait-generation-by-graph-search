
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "node_creator_body_rot.h"


namespace designlab
{

NodeCreatorBodyRot::NodeCreatorBodyRot(
    const DividedMapState& divided_map,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
    const ::designlab::Vector3& rot_axis,
    enums::HexapodMove next_move) :
    map_(divided_map),
    next_move_(next_move),
    converter_ptr_(converter_ptr),
    checker_ptr_(checker_ptr),
    rot_axis_(rot_axis)
{
}

void NodeCreatorBodyRot::Create(const RobotStateNode& current_node,
                                int current_num, std::vector<RobotStateNode>* output_graph) const
{
    for (int i = 0; i < kBodyYawRotAngleDivNum; ++i)
    {
        const Quaternion quat = Quaternion::MakeByAngleAxis(candidate_angle_[i], rot_axis_);

        RobotStateNode node = current_node;
        node.ChangePosture(converter_ptr_, current_node.posture * quat);

        if (checker_ptr_->IsBodyInterferingWithGround(node, map_))
        {
            continue;
        }

        bool is_valid = true;

        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            if (!checker_ptr_->IsLegInRange(j, node.leg_pos[j]))
            {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) { continue; }

        node.ChangeToNextNode(current_num, next_move_);

        output_graph->push_back(node);
    }

    if (output_graph->empty())
    {
        // 1つも回転動作を追加できない場合は，回転しないノードを追加する．
        RobotStateNode node = current_node;
        node.ChangeToNextNode(current_num, next_move_);

        output_graph->push_back(node);
    }
}

}  // namespace designlab
