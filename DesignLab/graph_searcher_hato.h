//! @file graph_searcher_hato.h
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．

#ifndef DESIGNLAB_GRAPH_SEARCHER_HATO_H_
#define DESIGNLAB_GRAPH_SEARCHER_HATO_H_


#include "interface_graph_searcher.h"


//! @class GraphSearcherHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
class GraphSearcherHato final : public IGraphSearcher
{
public:

	GraphSearcherHato(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc);
	~GraphSearcherHato();

	GraphSearchResult SearchGraphTree(const std::vector<RobotStateNode>& graph, const TargetRobotState& target, RobotStateNode* output_result) override;

private:

	//! @brief 見つからないと-1がかえる
	size_t GetParentNodeIndex(const std::vector<RobotStateNode>& graph) const;

	//! @brief 見つからないとfalseがかえる．MAX_DEPTHさかのぼっても見つからない場合はfalseがかえる
	//! @param [in] graph グラフ
	//! @param [in] max_depth_node_index 最大深さのノードのインデックス
	//! @param [out] put_node 親ノードの情報を格納する
	//! @return bool 見つかったかどうか
	bool GetDepth1NodeFromMaxDepthNode(const std::vector<RobotStateNode>& graph, size_t max_depth_node_index, RobotStateNode* output_node) const;

	void InitEvaluationValue(const RobotStateNode& parent_node, const TargetRobotState& target);

	//! @brief 前進するための評価値を計算する
	float CalcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float CalcLegRotEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;


	const float kMarginOfMove = 10;


	RobotStateNode parent_node_;

	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ヘキサポッドの状態を計算するクラス
};


#endif	//DESIGNLAB_GRAPH_SEARCHER_HATO_H_