//! @file pass_finder_revaluation.h
//! @brief 再評価手法を実装したクラス


#ifndef PASS_FINDER_REVALUATION_H_
#define PASS_FINDER_REVALUATION_H_


#include <memory>
#include <vector>

#include "interface_pass_finder.h"
#include "interface_graph_searcher.h"
#include "interface_graph_tree_creator.h"
#include "interpolated_node_creator.h"
#include "robot_state_node.h"


class PassFinderRevaluation final : public IPassFinder
{
public:


	PassFinderRevaluation(
		std::unique_ptr<IGraphTreeCreator>&& graph_tree_creator_ptr,
		std::unique_ptr<IGraphTreeCreator>&& graph_tree_creator_revaluation_ptr,
		std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
		const std::shared_ptr<const AbstractHexapodStateCalculator>& hexapod_state_calculator_ptr
	);

	~PassFinderRevaluation() = default;


	GraphSearchResult GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map_ref, const TargetRobotState& target, RobotStateNode* output_node) override;

	int GetMadeNodeNum() const;

	void GetGraphTree(std::vector<RobotStateNode>* output_graph) const;

private:

	std::vector<RobotStateNode> graph_tree_;	//!< グラフ探索の結果得られた木構造のグラフ

	const std::unique_ptr<IGraphTreeCreator> graph_tree_creator_ptr_;	//!< グラフ探索を行う木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphTreeCreator> graph_tree_creator_revaluation_ptr_;	//!< 再評価用の木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;			//!< グラフ探索を行うクラス

	const std::shared_ptr<const AbstractHexapodStateCalculator> hexapod_state_calculator_ptr_;	//!< ヘキサポッドの状態を計算するクラス

	InterpolatedNodeCreator interpolated_node_creator_;	//!< 補間ノードを作成するクラス

	bool IsVaildNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;
};

#endif