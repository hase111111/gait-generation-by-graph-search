#include "node_creator_body_rot.h"


namespace dl = ::designlab;


NodeCreatorBodyRot::NodeCreatorBodyRot(
	const DevideMapState& devide_map, 
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr, 
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	const ::designlab::Vector3& rot_axis,
	HexapodMove next_move):
	map_(devide_map),
	next_move_(next_move),
	converter_ptr_(converter_ptr),
	checker_ptr_(checker_ptr),
	rot_axis_(rot_axis)
{
}

void NodeCreatorBodyRot::Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const
{
	for (int i = 0; i < kBodyYawRotAngleDivNum; ++i)
	{
		const dl::Quaternion quat = dl::Quaternion::MakeByAngleAxis(candiate_angle_[i], rot_axis_);

		RobotStateNode node = current_node;
		node.ChangeQuat(converter_ptr_, current_node.quat * quat);

		if (checker_ptr_->IsBodyInterferingWithGround(node, map_)) 
		{
			continue;
		}

		bool is_vaild = true;

		for (int j = 0; j < HexapodConst::kLegNum; j++)
		{
			if (! checker_ptr_->IsLegInRange(j, node.leg_pos[j]) )
			{
				is_vaild = false;
				break;
			}
		}

		if (! is_vaild) { continue; }

		node.ChangeToNextNode(current_num, next_move_);

		output_graph->push_back(node);
	}

	if (output_graph->empty()) 
	{
		//1つも回転動作を追加できない場合は，回転しないノードを追加する．
		RobotStateNode node = current_node;
		node.ChangeToNextNode(current_num, next_move_);

		output_graph->push_back(node);
	}
}