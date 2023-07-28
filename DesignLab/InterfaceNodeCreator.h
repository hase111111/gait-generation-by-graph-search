#pragma once
#include "Node.h"
#include "MapState.h"
#include "HexapodNextMove.h"

class INodeCreator
{
public:
	INodeCreator(const MapState* const p_map, const EHexapodMove next_move) : m_next_move(next_move) {};
	virtual ~INodeCreator() = default;

	//! @brief 重心を平行移動したノードを生成する
	//! @param[in] current_node 重心を平行移動するノード
	//! @param[in] current_node_index 重心を平行移動するノードの番号
	//! @param[out] output_graph 重心を平行移動したノードを格納するコンテナ
	virtual void create(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph) = 0;

protected:
	const EHexapodMove m_next_move;

};
