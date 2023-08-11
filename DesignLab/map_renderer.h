#pragma once

#include "map_state.h"
#include "Node.h"


//! @class MapRenderer
//! @date 2023/08/09
//! @auther 長谷川
//! @brief Mapの描画を行うクラス．
class MapRenderer
{
public:
	MapRenderer();
	~MapRenderer() = default;

	void setNode(const SNode& node);

	//! @brief マップの描画を行う．
	//! @param [in] map マップの状態．
	void draw(const MapState& map) const;

private:

	SNode m_node;
	const unsigned int COLOR_GRAY;
	const unsigned int COLOR_LIGHT_GRAY;
	const float CUBE_SIZE = 15.0f;
};


//! @file map_renderer.h
//! @date 2023/08/09
//! @author	長谷川
//! @brief	マップの描画を行うMapRendererクラス．
//! @n 行数 : @lineinfo