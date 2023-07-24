#pragma once
#include "InterfaceNodeCreator.h"
#include "HexapodStateCalculator.h"


class LegUpDownNodeCreator final : public INodeCreator
{
private:

	const float LEG_MARGIN = 20.0f;		//これだけ動かせば現在の脚位置でも届くのならば，脚位置4判定となる．
	const float HIGH_MARGIN = 5.0f;		//縦方向（Z軸方向）のマージン．この範囲内ならば真ん中にあるとみなす．

public:

	LegUpDownNodeCreator(const MapState* const _p_Map, const EHexapodMove _next_move) : INodeCreator(_p_Map, _next_move), mp_Map(_p_Map) {};
	~LegUpDownNodeCreator() = default;

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph) override;

private:

	const MapState* const mp_Map;

	HexapodStateCalclator m_Calc;	//ロボットの座標計算クラス．

	constexpr EHexapodMove getNextMove(const EHexapodMove& _last_move) const
	{
		if (_last_move == EHexapodMove::LEG_UP_DOWN_NEXT_COM_MOVE)
		{
			return EHexapodMove::COM_MOVE;
		}
		else
		{
			return EHexapodMove::COM_UP_DOWN;
		}
	}

	//脚が接地可能か調べる．地面に干渉するかどうかを調べていないので注意．実際に接地するとしたらどこになるかをoutput_ground_posで出力する．
	bool isGroundableLeg(const int _leg_num, const SNode& _current_node, my_vec::SVector& _output_ground_pos);

	//離散化した脚位置の4のグローバル座標，候補点のグローバル座標，付け根のグローバル座標．現在の脚状態(1～7)，これらを利用して候補点が離散化した脚位置に適しているか調べる．
	bool isAbleLegPos(const SNode& _node, const int _leg_num);
};


//! @file LegUpDownNodeCreator.h
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @date 2023/7/24
//! @auther 長谷川

//! @class LegUpDownNodeCreator
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @note 回転を考慮していないので注意
//! @date 2023/7/24
//! @auther 長谷川