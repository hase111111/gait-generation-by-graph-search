#pragma once

#include "interface_node_creator.h"


//! @class ComUpDownNodeCreator
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
class ComUpDownNodeCreator final : public INodeCreator
{
public:
	ComUpDownNodeCreator(const MapState* const p_map, const EHexapodMove next_move);
	~ComUpDownNodeCreator();

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	// グローバル座標の重心の最低位置と最高位置から，重心を上下に変化させたノードを追加する．
	void pushNodeByMaxAndMinPosZ(const SNode& current_node, const int current_num, const float high, const float low, std::vector<SNode>* output_graph);


	static constexpr int DISCRETIZATION = 5;	//離散化数．最大位置を最小位置を何分割するのか．

	static constexpr float MARGIN = 10.0f;		//脚を伸ばし切らないようにするためのマージン[mm]．数値は先輩のプログラムからとってきたのでなぜこの数値が良いのかはわからない．


	const MapState* const mp_map;
};


//! @file com_up_down_node_creator.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @n 行数 : @lineinfo
