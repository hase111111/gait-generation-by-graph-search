#pragma once

#include "interface_node_creator.h"
#include "hexapod_state_calculator.h"


//! @class LegUpDownNodeCreator
//! @date 2023/08/12
//! @author 長谷川
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @note 回転を考慮していないので注意
class LegUpDownNodeCreator final : public INodeCreator
{
public:

	LegUpDownNodeCreator(const MapState* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move);
	~LegUpDownNodeCreator() = default;

	void create(const SNode& current_node, int current_node_index, std::vector<SNode>* output_graph) override;

private:

	//脚が接地可能か調べる．地面に干渉するかどうかを調べていないので注意．実際に接地するとしたらどこになるかをoutput_ground_posで出力する．
	bool isGroundableLeg(int leg_num, const SNode& current_node, dl_vec::SVector* output_ground_pos);

	//離散化した脚位置の4のグローバル座標，候補点のグローバル座標，付け根のグローバル座標．現在の脚状態(1～7)，これらを利用して候補点が離散化した脚位置に適しているか調べる．
	bool isAbleLegPos(const SNode& node, int leg_num);


	const float LEG_MARGIN = 20.0f;		//これだけ動かせば現在の脚位置でも届くのならば，脚位置4判定となる．
	const float HIGH_MARGIN = 5.0f;		//縦方向（Z軸方向）のマージン．この範囲内ならば真ん中にあるとみなす．


	const MapState* const mp_map;

	HexapodStateCalclator_Old m_calclator;	//ロボットの座標計算クラス．
	const std::shared_ptr<const AbstractHexapodStateCalculator> mp_calclator;	//ロボットの座標計算クラス．
};


//! @file leg_up_down_node_creator.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @n 行数 : @lineinfo
