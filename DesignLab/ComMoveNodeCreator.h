#pragma once
#include "InterfaceNodeCreator.h"
#include "my_polygon.h"
#include "ComType.h"


class ComMoveNodeCreator final : public INodeCreator
{
public:

	ComMoveNodeCreator(const MapState* const _p_map, const EHexapodMove _next_move);
	~ComMoveNodeCreator();

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>* _output_graph) override;

private:

	const MapState* const mp_Map;

	std::vector<my_vec::SPolygon2> m_Polygons;

	const bool DO_DEBUG_PRINT = false;
};

//! @file ComMoveNodeCreator.h
//! @brief 重心の平行移動を行うクラス．
//! @date 2023/7/25
//!	@auther 長谷川

//! @class ComMoveNodeCreator
//! @brief 重心の平行移動を行うクラス．
//! @date 2023/7/25
//! @auther 長谷川