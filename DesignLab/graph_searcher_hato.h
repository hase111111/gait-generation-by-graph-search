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
	size_t getParentNodeIndex(const std::vector<RobotStateNode>& graph) const;

	//! @brief 見つからないとfalseがかえる．MAX_DEPTHさかのぼっても見つからない場合はfalseがかえる
	//! @param [in] graph グラフ
	//! @param [in] max_depth_node_index 最大深さのノードのインデックス
	//! @param [out] put_node 親ノードの情報を格納する
	//! @return bool 見つかったかどうか
	bool getDepth1NodeFromMaxDepthNode(const std::vector<RobotStateNode>& graph, size_t max_depth_node_index, RobotStateNode* output_node) const;

	void initEvaluationValue(const RobotStateNode& parent_node, const TargetRobotState& target);

	//! @brief 前進するための評価値を計算する
	float calcMoveFrowardEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;

	//! @brief 脚の平均回転量の評価値を計算する
	float calcLegRotEvaluationValue(const RobotStateNode& current_node, const TargetRobotState& target) const;


	const float MARGIN_OF_MOVE = 10;


	RobotStateNode m_parent_node;

	const std::shared_ptr<const AbstractHexapodStateCalculator> mp_calculator;	//!< ヘキサポッドの状態を計算するクラス
};


#endif	//DESIGNLAB_GRAPH_SEARCHER_HATO_H_