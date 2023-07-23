#pragma once
#include "MapState.h"
#include "HexapodStateCalculator.h"

//重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．胴体の回転を一切考慮していないので，それを考慮したい場合作り直すか新しいものを作ってください．
class ComUpDownNodeCreator final
{
public:
	void init(const MapState* const _p_Map);

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

	const MapState* mp_Map;
	HexapodStateCalclator m_HexaCalc;

	// グローバル座標の重心の最低位置と最高位置から，重心を上下に変化させたノードを追加する．
	void pushNodeByMaxAndMinPosZ(const SNode& _current_node, const int _current_num, const float _high, const float _low, std::vector<SNode>& _output_graph);


	const int DISCRETIZATION = 5;	//離散化数．最大位置を最小位置を何分割するのか．
	const float MARGIN = 10.0f;		//脚を伸ばし切らないようにするためのマージン[mm]．数値は先輩のプログラムからとってきたのでなぜこの数値が良いのかはわからない．
	const EHexapodMove m_next_move = EHexapodMove::COM_MOVE;	//次の動作
};
