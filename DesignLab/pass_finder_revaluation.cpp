#include "pass_finder_revaluation.h"

#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


PassFinderRevaluation::PassFinderRevaluation(
	std::unique_ptr<IGraphTreeCreator>&& graph_tree_creator_ptr,
	std::unique_ptr<IGraphTreeCreator>&& graph_tree_creator_revaluation_ptr,
	std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr
) : 
	graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
	graph_tree_creator_revaluation_ptr_(std::move(graph_tree_creator_revaluation_ptr)),
	graph_searcher_ptr_(std::move(graph_searcher_ptr))
{
}

GraphSearchResult PassFinderRevaluation::GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map_state, const TargetRobotState& target, RobotStateNode* output_node)
{
	assert(output_node != nullptr);	// output_nodeはnullptrでない
	assert(graph_tree_creator_ptr_ != nullptr);	// graph_tree_creator_ptr_はnullptrでない
	assert(graph_tree_creator_revaluation_ptr_ != nullptr);	// graph_tree_creator_revaluation_ptr_はnullptrでない
	assert(graph_searcher_ptr_ != nullptr);	// graph_searcher_ptr_はnullptrでない


	dlio::Output("PassFinderBasic::GetNextNodebyGraphSearch．\nまずは初期化する．(マップを分割する)\n", OutputDetail::kDebug);

	DevideMapState devide_map;
	devide_map.Init(map_state, current_node.global_center_of_mass);

	graph_tree_creator_ptr_->Init(devide_map);
	graph_tree_creator_revaluation_ptr_->Init(devide_map);

	graph_tree_.clear();

	{
		dlio::Output("初期化終了．", OutputDetail::kDebug);


		// グラフ探索をするための，歩容パターングラフを生成する
		dlio::Output("グラフ木を作成する", OutputDetail::kDebug);

		RobotStateNode parent_node = current_node;
		parent_node.ChangeParentNode();

		GraphSearchResult result = graph_tree_creator_ptr_->CreateGraphTree(parent_node, GraphSearchConst::kMaxDepth, &graph_tree_);

		if (result != GraphSearchResult::kSuccess)
		{
			dlio::Output("グラフ木の作成に失敗．", OutputDetail::kDebug);
			return result;
		}

		dlio::Output("グラフ木の作成終了．", OutputDetail::kDebug);
		dlio::Output("グラフのサイズ" + std::to_string(graph_tree_.size()), OutputDetail::kDebug);


		// グラフ探索を行う
		dlio::Output("グラフ木を評価する", OutputDetail::kDebug);

		result = graph_searcher_ptr_->SearchGraphTree(graph_tree_, target, output_node);

		if (result != GraphSearchResult::kSuccess)
		{
			dlio::Output("グラフ木の評価に失敗．", OutputDetail::kDebug);
			return result;
		}

		if (IsVaildNode(current_node, (*output_node))) 
		{
			dlio::Output("グラフ木の評価終了．グラフ探索に成功した", OutputDetail::kDebug);

			return GraphSearchResult::kSuccess;
		}
	}

	{
		// グラフ探索をするための，歩容パターングラフを生成する
		dlio::Output("脚軌道生成に失敗したため，再評価を行う", OutputDetail::kDebug);

		RobotStateNode parent_node = current_node;
		parent_node.ChangeParentNode();

		graph_tree_.clear();
		GraphSearchResult result = graph_tree_creator_revaluation_ptr_->CreateGraphTree(parent_node, GraphSearchConst::kMaxDepth, &graph_tree_);

		if (result != GraphSearchResult::kSuccess)
		{
			dlio::Output("グラフ木の作成に失敗．", OutputDetail::kDebug);
			return result;
		}

		dlio::Output("グラフ木の作成終了．", OutputDetail::kDebug);
		dlio::Output("グラフのサイズ" + std::to_string(graph_tree_.size()), OutputDetail::kDebug);


		// グラフ探索を行う
		dlio::Output("グラフ木を評価する", OutputDetail::kDebug);

		result = graph_searcher_ptr_->SearchGraphTree(graph_tree_, target, output_node);

		if (result != GraphSearchResult::kSuccess)
		{
			dlio::Output("グラフ木の評価に失敗した．", OutputDetail::kDebug);
			return result;
		}

		if (IsVaildNode(current_node, (*output_node))) 
		{
			dlio::Output("グラフ木の評価終了．グラフ探索に成功した", OutputDetail::kDebug);

			return GraphSearchResult::kSuccess;
		}
	}

	dlio::Output("脚軌道生成に失敗した", OutputDetail::kDebug);

	return GraphSearchResult::kFailureByLegPathGenerationError;
}

int PassFinderRevaluation::GetMadeNodeNum() const
{
	return static_cast<int>(graph_tree_.size());
}

void PassFinderRevaluation::GetGraphTree(std::vector<RobotStateNode>* output_graph) const
{
	assert(output_graph != nullptr);
	assert((*output_graph).size() == 0);

	(*output_graph) = graph_tree_;
}

bool PassFinderRevaluation::IsVaildNode([[maybe_unused]]const RobotStateNode& current_node, [[maybe_unused]] const RobotStateNode& next_node) const
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
