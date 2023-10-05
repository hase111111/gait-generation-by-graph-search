//! @file pass_finder_basic.h
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス

#ifndef DESIGNLAB_PASS_FINDER_BASIC_H_
#define DESIGNLAB_PASS_FINDER_BASIC_H_


#include <memory>
#include <vector>

#include "interface_pass_finder.h"
#include "interface_graph_searcher.h"
#include "interface_graph_tree_creator.h"
#include "robot_state_node.h"


//! @class PassFinderBasic
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス
class PassFinderBasic final : public IPassFinder
{
public:

	//! @param[in] graph_tree_creator グラフ探索を行う木構造のグラフを作成するクラス．unique_ptrで渡す
	//! @param[in] graph_searcher グラフ探索を行うクラス．unique_ptrで渡す
	//! @param[in] calculator_ptr ロボットの状態を計算するクラス．shared_ptrで渡す
	PassFinderBasic(
		std::unique_ptr<IGraphTreeCreator>&& graph_tree_creator_ptr, 
		std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
		const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr
	);

	~PassFinderBasic() = default;


	GraphSearchResult GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map_ref, const STarget& target, RobotStateNode* output_node) override;

	int GetMadeNodeNum() const;

	void GetGraphTree(std::vector<RobotStateNode>* output_graph) const;

private:

	std::vector<RobotStateNode> graph_tree_;	//!< グラフ探索の結果得られた木構造のグラフ

	const std::unique_ptr<IGraphTreeCreator> graph_tree_creator_ptr_;	//!< グラフ探索を行う木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;		//!< グラフ探索を行うクラス


	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス
};


#endif  // DESIGNLAB_PASS_FINDER_BASIC_H_