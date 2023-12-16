#include "interpolated_node_creator.h"

#include "cassert_define.h"
#include "designlab_math_util.h"
#include "hexapod_const.h"


namespace designlab 
{

InterpolatedNodeCreator::InterpolatedNodeCreator(const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) : 
	converter_ptr_(converter_ptr)
{
}

std::vector<RobotStateNode>  InterpolatedNodeCreator::CreateInterpolatedNode(
	const RobotStateNode& current_node, 
	const RobotStateNode& next_node) const
{
	if (IsNoChange(current_node, next_node)) 
	{ 
		return { current_node }; 
	}

	if (IsBodyRot(current_node, next_node))
	{
		return CreateBodyRotInterpolatedNode(current_node, next_node);
	}

	if (IsBodyMove(current_node, next_node)) 
	{
		return CreateBodyMoveInterpolatedNode(current_node, next_node);
	}

	return CreateLegMoveInterpolatedNode(current_node, next_node);
}

bool InterpolatedNodeCreator::IsNoChange(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	bool res = true;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (current_node.leg_pos[i] != next_node.leg_pos[i])
		{
			res = false;
			break;
		}
	}

	if (current_node.global_center_of_mass != next_node.global_center_of_mass)
	{
		res = false;
	}

	if (current_node.quat != next_node.quat)
	{
		res = false;
	}

	return res;
}

bool InterpolatedNodeCreator::IsBodyMove(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	return current_node.global_center_of_mass != next_node.global_center_of_mass;
}

bool InterpolatedNodeCreator::IsBodyRot(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	return current_node.quat.GetNormalized() != next_node.quat.GetNormalized();
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateBodyMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	std::vector<RobotStateNode> res;

	for (int i = 0; i < kBodyMoveInterpolatedNodeNum; i++)
	{
		RobotStateNode temp_node = current_node;
		const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kBodyMoveInterpolatedNodeNum));

		temp_node.ChangeGlobalCenterOfMass(
			temp_node.global_center_of_mass + (next_node.global_center_of_mass - temp_node.global_center_of_mass) * ex,
			true
			);

		//for (int j = 0; j < HexapodConst::kLegNum; j++)
		//{
		//	temp_node.leg_pos[j] = temp_node.leg_pos[j] + (next_node.leg_pos[j] - temp_node.leg_pos[j]) * ex;
		//}

		res.push_back(temp_node);
	}

	return res;
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateBodyRotInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	std::vector<RobotStateNode> res;

	for (int i = 0; i < kBodyMoveInterpolatedNodeNum; i++)
	{
		RobotStateNode temp_node = current_node;
		const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kBodyMoveInterpolatedNodeNum));

		const Quaternion quat = SlerpQuaternion(current_node.quat, next_node.quat, ex).GetNormalized();

		temp_node.ChangeQuat(converter_ptr_, quat);

		res.push_back(temp_node);
	}

	return res;
}

std::vector<RobotStateNode> InterpolatedNodeCreator::CreateLegMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	std::vector<RobotStateNode> res;

	// 接地脚の平行移動，下降動作，遊脚の上昇動作，平行移動を行う．
	RobotStateNode last_state = current_node;

	//接地
	{
		std::vector<int> ground_move_index = GetGroundMoveIndex(current_node, next_node);

		for (int i = 0; i < kGroundInterpolatedNodeNum; i++)
		{
			RobotStateNode temp_node = last_state;
			const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kGroundInterpolatedNodeNum));

			for (int j = 0; j < ground_move_index.size(); j++)
			{
				temp_node.leg_pos[ground_move_index[j]].x = last_state.leg_pos[ground_move_index[j]].x +
					(next_node.leg_pos[ground_move_index[j]].x - last_state.leg_pos[ground_move_index[j]].x) * ex;

				temp_node.leg_pos[ground_move_index[j]].y = last_state.leg_pos[ground_move_index[j]].y +
					(next_node.leg_pos[ground_move_index[j]].y - last_state.leg_pos[ground_move_index[j]].y) * ex;
			}

			res.push_back(temp_node);
		}

		last_state = res.back();

		for (int i = 0; i < kGroundInterpolatedNodeNum; i++)
		{
			RobotStateNode temp_node = last_state;
			const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kGroundInterpolatedNodeNum));

			for (int j = 0; j < ground_move_index.size(); j++)
			{
				temp_node.leg_pos[ground_move_index[j]].z = last_state.leg_pos[ground_move_index[j]].z +
					(next_node.leg_pos[ground_move_index[j]].z - last_state.leg_pos[ground_move_index[j]].z) * ex;
			}

			res.push_back(temp_node);
		}

		last_state = res.back();
	}

	//遊脚
	{
		std::vector<int> free_move_index = GetFreeMoveIndex(current_node, next_node);

		for (int i = 0; i < kFreeInterpolatedNodeNum; i++)
		{
			RobotStateNode temp_node = last_state;
			const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kFreeInterpolatedNodeNum));

			for (int j = 0; j < free_move_index.size(); j++)
			{
				temp_node.leg_pos[free_move_index[j]].z = last_state.leg_pos[free_move_index[j]].z +
					(next_node.leg_pos[free_move_index[j]].z - last_state.leg_pos[free_move_index[j]].z) * ex;
			}

			res.push_back(temp_node);
		}

		last_state = res.back();

		for (int i = 0; i < kFreeInterpolatedNodeNum; i++)
		{
			RobotStateNode temp_node = last_state;
			const float ex = (static_cast<float>(i) + 1.0f) / (static_cast<float>(kFreeInterpolatedNodeNum));

			for (int j = 0; j < free_move_index.size(); j++)
			{
				temp_node.leg_pos[free_move_index[j]].x = last_state.leg_pos[free_move_index[j]].x +
					(next_node.leg_pos[free_move_index[j]].x - last_state.leg_pos[free_move_index[j]].x) * ex;

				temp_node.leg_pos[free_move_index[j]].y = last_state.leg_pos[free_move_index[j]].y +
					(next_node.leg_pos[free_move_index[j]].y - last_state.leg_pos[free_move_index[j]].y) * ex;
			}

			res.push_back(temp_node);
		}
	}

	return res;
}

std::vector<int> InterpolatedNodeCreator::GetGroundMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	// 脚先座標の差分を計算．
	std::array<Vector3, HexapodConst::kLegNum> dif;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		dif[i] = next_node.leg_pos[i] - current_node.leg_pos[i];
	}

	//indexを返す．
	std::vector<int> res_index;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dif[i].z < 0)
		{
			res_index.push_back(i);
		}
	}

	return res_index;
}

std::vector<int> InterpolatedNodeCreator::GetFreeMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const
{
	// 脚先座標の差分を計算．
	std::array<Vector3, HexapodConst::kLegNum> dif;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		dif[i] = next_node.leg_pos[i] - current_node.leg_pos[i];
	}

	// indexを返す．
	std::vector<int> res_index;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (!dif[i].ProjectedXY().IsZero())
		{
			res_index.push_back(i);
		}
	}

	return res_index;
}

} // namespace designlab