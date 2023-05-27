#pragma once
#include "MapState.h"

//重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．胴体の回転を一切考慮していないので，それを考慮したい場合作り直すか新しいものを作ってください．
class ComUpDownNodeCreator final
{
public:
	void init();

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

	const MapState* mp_Map;

	const int DISCRETIZATION = 5;	//離散化数．最大位置を最小位置を何分割するのか．
	const float MARGIN = 10.0f;		//脚を伸ばし切らないようにするためのマージン[mm]．数値は先輩のプログラムからとってきたのでなぜこの数値が良いのかはわからない．
};
