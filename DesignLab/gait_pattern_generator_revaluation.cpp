#include "gait_pattern_generator_revaluation.h"

#include "cassert_define.h"
#include "cmdio_util.h"
#include "gait_pattern_graph_tree.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


GaitPatternGeneratorRevaluation::GaitPatternGeneratorRevaluation(
	std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
	std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_revaluation_ptr
) :
	gait_pattern_generator_ptr_(std::move(gait_pattern_generator_ptr)),
	gait_pattern_generator_revaluation_ptr_(std::move(gait_pattern_generator_revaluation_ptr))
{
	assert(gait_pattern_generator_ptr_ != nullptr);	// gait_pattern_generator_ptr_はnullptrでない
	assert(gait_pattern_generator_revaluation_ptr_ != nullptr);	// gait_pattern_generator_revaluation_ptr_はnullptrでない
}

GraphSearchResult GaitPatternGeneratorRevaluation::GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map_state, const TargetRobotState& target, RobotStateNode* output_node)
{
	assert(output_node != nullptr);	// output_nodeはnullptrでない

	const GraphSearchResult result = gait_pattern_generator_ptr_->GetNextNodebyGraphSearch(current_node, map_state, target, output_node);

	if (result != GraphSearchResult::kSuccess)
	{
		// グラフ探索に失敗した場合は終了．
		return result;
	}

	//成功した場合は，逆運動学計算で脚軌道生成が可能であるか確認する．

	//! @todo 続きを書こう！

	return result;
}

bool GaitPatternGeneratorRevaluation::IsVaildNode([[maybe_unused]] const RobotStateNode& current_node, [[maybe_unused]] const RobotStateNode& next_node) const
{
	////逆運動学で間接角度を計算する
	////間接角度が範囲内に収まっているかを確認する
	//std::array<HexapodJointState, HexapodConst::kLegNum> joint_state;

	////現在のノードの間接角度を計算する
	//hexapod_state_calculator_ptr_->CalculateAllJointState(current_node, &joint_state);

	////解が正しいか確かめる
	//if (!hexapod_state_calculator_ptr_->IsVaildJointState(current_node, joint_state))
	//{
	//	return false;
	//}

	////次のノードの間接角度を計算する
	//hexapod_state_calculator_ptr_->CalculateAllJointState(next_node, &joint_state);

	////解が正しいか確かめる
	//if (!hexapod_state_calculator_ptr_->IsVaildJointState(next_node, joint_state))
	//{
	//	return false;
	//}

	//std::vector<RobotStateNode> interpolated_node;

	//interpolated_node_creator_.CreateInterpolatedNode(current_node, next_node, &interpolated_node);

	//for (const auto &i : interpolated_node)
	//{
	//	hexapod_state_calculator_ptr_->CalculateAllJointState(i, &joint_state);

	//	//解が正しいか確かめる
	//	if (!hexapod_state_calculator_ptr_->IsVaildJointState(i, joint_state))
	//	{
	//		return false;
	//	}
	//}

	return true;
}
