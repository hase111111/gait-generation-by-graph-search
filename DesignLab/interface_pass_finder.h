//! @file interface_pass_finder.h
//! @brief グラフ探索による歩容パターン生成を行うクラスのインターフェース．

#ifndef DESIGNLAB_INTERFACE_PASS_FINDER_H_
#define DESIGNLAB_INTERFACE_PASS_FINDER_H_

#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "graph_search_result_recoder.h"
#include "map_state.h"
#include "robot_state_node.h"
#include "target.h"


//! @class IPassFinder
//! @brief グラフ探索による歩容パターン生成を行うクラスのインターフェース．
//! @details 
//! @n 波東さんのプログラムで言うところのPassFindingクラス．
//! @n 実体は作成できないのでこれを継承してたクラスを使うこと．
//! @n 継承をするクラスのデストラクタはvirtualにしておく．
//! @n 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html
class IPassFinder
{
public:

	IPassFinder() = default;
	virtual ~IPassFinder() = default;


	//! @brief グラフ探索を行い，次の動作として最適なノードを返す．
	//! @param [in] current_node 現在の状態を表すノード
	//! @param [in] map 現在のマップの状態
	//!	@param [in] target 目標の状態
	//! @param [out] output_node 結果のノード
	//! @return GraphSearchResult グラフ探索の結果を返す．成功か失敗か
	virtual GraphSearchResult GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map, const STarget& target, RobotStateNode* output_node) = 0;

	//! @brief 作成したグラフのノードの数を返す
	//! @return int 作成したグラフの数
	virtual int GetMadeNodeNum() const = 0;

	//! @brief 作成したグラフ木を返す．
	//! @n output_graphは空であること．
	//! @param [out] output_graph 作成したグラフ木を渡す．
	virtual void GetGraphTree(std::vector<RobotStateNode>* output_graph) const = 0;
};


#endif	// DESIGNLAB_INTERFACE_PASS_FINDER_H_