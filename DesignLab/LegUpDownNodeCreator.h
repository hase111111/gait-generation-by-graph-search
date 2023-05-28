#pragma once
#include "MapState.h"
#include "HexapodStateCalculator.h"

//脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．胴体の回転を一切考慮していないので，それを考慮したい場合作り直すか新しいものを作ってください．
class LegUpDownNodeCreator final
{
public:

	// _output_graphの後ろに脚を上下させた新しいノードをpushする．
	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

};
