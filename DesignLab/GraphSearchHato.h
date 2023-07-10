#pragma once
#include "MapState.h"
#include "Node.h"
#include "IGraphTreeCreator.h"
#include "IGraphSearcher.h"
#include "InterfaceGraphSearch.h"
#include <memory>

class GraphSearchHato final :public IGraphSearch
{
public:
	GraphSearchHato() = default;
	~GraphSearchHato() = default;

	//! @brief グラフ探索を行い，次の動作として最適なノードを返す．
	//! @param [in] _current_node 現在の状態を表すノード
	//! @param [in] _p_map マップの状態を表すクラスへのポインタ
	//! @param [in] _target 動作の目標
	//! @param [out] _output_node 結果のノード
	//! @return bool グラフ探索に失敗した場合falseを返す
	bool getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) override;

private:
	std::unique_ptr<IGraphTreeCreator> mp_GraphTreeCreator;
	std::unique_ptr<IGraphSearcher> mp_GraphSearcher;
};


//! @file GraphSearch.h 
//! @brief グラフ探索を行うクラスの実装．
//! @author 長谷川

//! @class GraphSearch
//! @brief グラフ探索を行うクラス．名前通り
//! @author 長谷川