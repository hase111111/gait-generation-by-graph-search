#pragma once
#include "InterfaceNodeCreator.h"
#include "HexapodStateCalculator.h"

class LegDownNodeCreator final : public INodeCreator
{
public:
	LegDownNodeCreator(const MapState* const p_map, const EHexapodMove next_move) : INodeCreator(p_map, next_move), mp_map(p_map) {};
	~LegDownNodeCreator() = default;

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	HexapodStateCalclator m_calculator;
	const MapState* const mp_map;

	const float LEG_MARGIN = 20.0f;		//これだけ動かせば現在の脚位置でも届くのならば，脚位置4判定となる．
	const float HIGH_MARGIN = 5.0f;		//縦方向（Z軸方向）のマージン．この範囲内ならば真ん中にあるとみなす．

	//脚が接地可能か調べる．地面に干渉するかどうかを調べていないので注意．実際に接地するとしたらどこになるかをoutput_ground_posで出力する．
	bool isGroundableLeg(const int leg_num, const SNode& current_node, my_vec::SVector& output_ground_pos);

	//離散化した脚位置の4のグローバル座標，候補点のグローバル座標，付け根のグローバル座標．現在の脚状態(1～7)，これらを利用して候補点が離散化した脚位置に適しているか調べる．
	bool isAbleLegPos(const SNode& node, const int leg_num);
};


//! @file LegUpDownNodeCreator.h
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @date 2023/7/24
//! @auther 長谷川

//! @class LegUpDownNodeCreator
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @date 2023/7/24
//! @auther 長谷川