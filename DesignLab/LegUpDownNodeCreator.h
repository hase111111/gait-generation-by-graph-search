#pragma once
#include "MapState.h"
#include "HexapodStateCalculator.h"
#include "MapState.h"

//脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．胴体の回転を一切考慮していないので，それを考慮したい場合作り直すか新しいものを作ってください．
class LegUpDownNodeCreator final
{
public:

	// _output_graphの後ろに脚を上下させた新しいノードをpushする．
	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

	const MapState* mp_Map;

	EHexapodMove getNextMove(const EHexapodMove& _last_move) const;

	//脚が接地可能か調べる．地面に干渉するかどうかを調べていないので注意．実際に接地するとしたらどこになるかをoutput_ground_posで出力する．
	bool isGroundableLeg(const int _leg_num, const SNode& _current_node, SNode& _output_ground_pos);

	const float HORIZONTAL_MARGIN = 50.0f;	//これだけ動かせば現在の脚位置でも届くのならば，脚位置4判定となる．
};
