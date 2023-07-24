#pragma once
#include "InterfaceNodeCreator.h"
#include "HexapodStateCalculator.h"


class ComUpDownNodeCreator final : public INodeCreator
{
public:
	ComUpDownNodeCreator(const MapState* const _p_map, const EHexapodMove _next_move) : INodeCreator(_p_map, _next_move), mp_Map(_p_map) {};
	~ComUpDownNodeCreator() = default;

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph) override;

private:

	const MapState* const mp_Map;
	HexapodStateCalclator m_HexaCalc;

	// グローバル座標の重心の最低位置と最高位置から，重心を上下に変化させたノードを追加する．
	void pushNodeByMaxAndMinPosZ(const SNode& _current_node, const int _current_num, const float _high, const float _low, std::vector<SNode>& _output_graph);


	const int DISCRETIZATION = 5;	//離散化数．最大位置を最小位置を何分割するのか．
	const float MARGIN = 10.0f;		//脚を伸ばし切らないようにするためのマージン[mm]．数値は先輩のプログラムからとってきたのでなぜこの数値が良いのかはわからない．
	const EHexapodMove m_next_move = EHexapodMove::COM_MOVE;	//次の動作
};

//! @file ComUpDownNodeCreator.h
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @date 2023/7/24
//! @auther 長谷川

//! @class ComUpDownNodeCreator
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @date 2023/7/24
//! @auther 長谷川