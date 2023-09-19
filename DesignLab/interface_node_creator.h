#pragma once

#include <vector>
#include <memory>

#include "node.h"
#include "map_state.h"
#include "hexapod_next_move.h"
#include "abstract_hexapod_state_calculator.h"


//! @class INodeCreator
//! @date 2023/08/12
//! @author 長谷川
//! @brief ノード生成インターフェース
class INodeCreator
{
public:

	//! @brief コンストラクタでは次動作を設定する．またマップのポインタを受け取る
	INodeCreator(const MapState_Old* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move) : m_next_move(next_move) {};
	virtual ~INodeCreator() = default;


	//! @brief 重心を平行移動したノードを生成する
	//! @param[in] current_node 重心を平行移動するノード
	//! @param[in] current_node_index 重心を平行移動するノードの番号
	//! @param[out] output_graph 重心を平行移動したノードを格納するコンテナ
	virtual void create(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph) = 0;

protected:

	const EHexapodMove m_next_move;	//!< 次動作
};


//! @file interface_node_creator.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief ノード生成インターフェース
//! @n 行数 : @lineinfo
