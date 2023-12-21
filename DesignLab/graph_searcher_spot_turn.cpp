#include "graph_searcher_spot_turn.h"

#include "cassert_define.h"
#include "graph_search_const.h"


namespace designlab
{

GraphSearcherSpotTurn::GraphSearcherSpotTurn(const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	checker_ptr_(checker_ptr)
{
}

std::tuple<GraphSearchResult, int, int> GraphSearcherSpotTurn::SearchGraphTree(
	const GaitPatternGraphTree& graph,
	const RobotOperation& operation,
	[[maybe_unused]] const DevideMapState& devide_map_state,
	const int max_depth
) const
{
	assert(operation.operation_type == enums::RobotOperationType::kSpotTurnLastPosture ||
		   operation.operation_type == enums::RobotOperationType::kSpotTurnRotAxis);

	if (!graph.HasRoot())
	{
		const GraphSearchResult result = { enums::Result::kFailure, "ルートノードがありません．" };
		return { result, -1, -1 };
	}

	//初期化
	EvaluationValue max_evaluation_value;
	InitialValue init_value;

	if (operation.operation_type == enums::RobotOperationType::kSpotTurnLastPosture)
	{
		init_value.target_quat = operation.spot_turn_last_posture_;
	}
	else
	{
		assert(false);
	}

	init_value.target_z_value = InitTargetZValue(graph.GetRootNode(), devide_map_state, { 0,0,0 });


	//Calcなどの関数をvectorに格納する．
	std::vector<std::function<EvaluationResult(const int, const GaitPatternGraphTree&, const EvaluationValue&, const InitialValue&, EvaluationValue*)>> update_evaluation_value_func_vec
	{
		std::bind(&GraphSearcherSpotTurn::UpdateEvalutionValueByZDiff, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),
		std::bind(&GraphSearcherSpotTurn::UpdateEvaluationValueByAmoutOfTurn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),
		std::bind(&GraphSearcherSpotTurn::UpdateEvalutionValueByLegRot, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),

	};

	for (int i = 0; i < graph.GetGraphSize(); i++)
	{
		//最大深さのノードのみを評価する
		if (graph.GetNode(i).depth == max_depth)
		{
			EvaluationValue candiate_evaluation_value;
			candiate_evaluation_value.index = i;

			bool do_update = false;

			for (size_t j = 0; j < update_evaluation_value_func_vec.size(); ++j)
			{
				const EvaluationResult result = update_evaluation_value_func_vec[j](i, graph, max_evaluation_value, init_value, &candiate_evaluation_value);

				if (max_evaluation_value.index != -1)
				{
					if (result == EvaluationResult::kUpdate)
					{
						do_update = true;
					}

					if (!do_update && result == EvaluationResult::kNotUpdate)
					{
						break;
					}
				}
				else
				{
					do_update = true;
				}
			}

			if (do_update)
			{
				max_evaluation_value = candiate_evaluation_value;
			}
		}
	}

	// index が範囲外ならば失敗
	if (max_evaluation_value.index < 0 || max_evaluation_value.index >= graph.GetGraphSize())
	{
		std::string error_message = "最大評価値のインデックスが範囲外です．" + std::to_string(max_evaluation_value.index);
		const GraphSearchResult result = { enums::Result::kFailure,error_message };
		return { result, -1, -1 };
	}

	const GraphSearchResult result = { enums::Result::kSuccess, max_evaluation_value.ToString() };

	return { result, graph.GetParentNodeIndex(max_evaluation_value.index, 1), max_evaluation_value.index };
}

std::string GraphSearcherSpotTurn::EvaluationValue::ToString() const
{
	std::string result = "index:" + std::to_string(index) + "/";
	result += "turn:" + math_util::ConvertFloatToString(turn, 3, 7) + "/";
	result += "leg_rot:" + math_util::ConvertFloatToString(leg_rot, 3, 7) + "/";
	result += "z_diff:" + math_util::ConvertFloatToString(z_diff, 3, 7);

	return result;
}

float GraphSearcherSpotTurn::InitTargetZValue(const RobotStateNode& node, const DevideMapState& devide_map_state, const Vector3& move_direction) const
{
	const float move_length = 100.0f;

	const Vector3 target_position = move_direction * move_length;

	const int div = 50;
	const float min_z = -150.0f;
	const float max_z = 150.0f;

	for (int i = 0; i < div; i++)
	{
		const float z = min_z + (max_z - min_z) / static_cast<float>(div) * static_cast<float>(i);

		Vector3 pos = node.global_center_of_mass;
		pos += target_position;
		pos.z += z;

		RobotStateNode temp_node = node;
		temp_node.ChangeGlobalCenterOfMass(pos, false);

		if (!checker_ptr_->IsBodyInterferingWithGround(temp_node, devide_map_state))
		{
			std::cout << node.global_center_of_mass.z + z << std::endl;
			return node.global_center_of_mass.z + z;
		}
	}

	return node.global_center_of_mass.z;
}

GraphSearcherSpotTurn::EvaluationResult GraphSearcherSpotTurn::UpdateEvalutionValueByLegRot(
	const int index,
	const GaitPatternGraphTree& tree,
	const EvaluationValue& max_evaluation_value,
	[[maybe_unused]] const InitialValue& init_value,
	EvaluationValue* candiate
) const
{
	assert(0 <= index && index < tree.GetGraphSize());	//indexが範囲内であることを確認する．
	assert(candiate != nullptr);	//candiateがnullptrでないことを確認する．

	float result = 0.0f;

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (leg_func::IsGrounded(tree.GetNode(index).leg_state, i))
		{
			result += (tree.GetNode(index).leg_pos[i].ProjectedXY() - tree.GetRootNode().leg_pos[i].ProjectedXY()).GetLength();
		}
		else
		{
			result += (tree.GetNode(index).leg_pos[i] - tree.GetRootNode().leg_pos[i]).GetLength();
		}
	}

	const float margin = 50.0f;

	if (max_evaluation_value.leg_rot + margin < result)
	{
		(*candiate).leg_rot = result;
		return EvaluationResult::kUpdate;
	}
	else if (abs(max_evaluation_value.leg_rot - result) < margin)
	{
		(*candiate).leg_rot = result;
		return EvaluationResult::kEqual;
	}

	return EvaluationResult::kNotUpdate;
}

GraphSearcherSpotTurn::EvaluationResult GraphSearcherSpotTurn::UpdateEvalutionValueByZDiff(
	const int index,
	const GaitPatternGraphTree& tree,
	const EvaluationValue& max_evaluation_value,
	const InitialValue& init_value,
	EvaluationValue* candiate
) const
{
	assert(0 <= index && index < tree.GetGraphSize());	//indexが範囲内であることを確認する．
	assert(candiate != nullptr);	//candiateがnullptrでないことを確認する．

	//const float result = abs(tree.GetNode(index).global_center_of_mass.z - tree.GetRootNode().global_center_of_mass.z);
	const float result = abs(tree.GetNode(index).global_center_of_mass.z - init_value.target_z_value);

	if (result < max_evaluation_value.z_diff)
	{
		(*candiate).z_diff = result;
		return EvaluationResult::kUpdate;
	}
	else if (math_util::IsEqual(result, max_evaluation_value.z_diff))
	{
		(*candiate).z_diff = result;
		return EvaluationResult::kEqual;
	}

	return EvaluationResult::kNotUpdate;
}



GraphSearcherSpotTurn::EvaluationResult GraphSearcherSpotTurn::UpdateEvaluationValueByAmoutOfTurn(
	const int index,
	const GaitPatternGraphTree& tree,
	const EvaluationValue& max_evaluation_value,
	const InitialValue& init_value,
	EvaluationValue* candiate
) const
{
	const float target_weight = 100000.f;	//方向指定の際のターゲットの重み．


	//最終姿勢を表すクォータニオンとの差分を計算する
	const Quaternion target_quat = init_value.target_quat;
	const Quaternion current_quat = tree.GetNode(index).quat;

	const Quaternion target_to_current = current_quat.GetInverse() * target_quat;

	//回転角を計算する．
	const float rot_angle = math_util::ConvertRadToDeg(2.f * asin(target_to_current.v.GetLength()));
	const float result = target_weight - rot_angle;

	const float margin = 1.0f;

	if (result > max_evaluation_value.turn + margin)
	{
		(*candiate).turn = result;
		return EvaluationResult::kUpdate;
	}
	else if (abs(result - max_evaluation_value.turn) < margin)
	{
		(*candiate).turn = result;
		return EvaluationResult::kEqual;
	}

	return EvaluationResult::kNotUpdate;
}


} // namespace designlab