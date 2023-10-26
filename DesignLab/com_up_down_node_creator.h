//! @file com_up_down_node_creator.h
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．

#ifndef DESIGNLAB_COM_UP_DOWN_NODE_CREATOR_H_
#define DESIGNLAB_COM_UP_DOWN_NODE_CREATOR_H_


#include "interface_node_creator.h"

#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "map_state.h"


//! @class ComUpDownNodeCreator
//! @brief 重心の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
class ComUpDownNodeCreator final : public INodeCreator
{
public:
	ComUpDownNodeCreator(const DevideMapState& devide_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, HexapodMove next_move);
	~ComUpDownNodeCreator() = default;

	void Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const override;

private:

	// グローバル座標の重心の最低位置と最高位置から，重心を上下に変化させたノードを追加する．
	void pushNodeByMaxAndMinPosZ(const RobotStateNode& current_node, int current_num, float high, float low, std::vector<RobotStateNode>* output_graph) const;


	static constexpr int DISCRETIZATION = 5;	//離散化数．最大位置を最小位置を何分割するのか．

	static constexpr float MARGIN = 10.0f;		//脚を伸ばし切らないようにするためのマージン[mm]．数値は先輩のプログラムからとってきたのでなぜこの数値が良いのかはわからない．


	const DevideMapState map_;

	const std::shared_ptr<const AbstractHexapodStateCalculator> calclator_;	//!< ロボットの座標計算クラス．

	const HexapodMove next_move_;	//!< 次の動作．
};


#endif	//DESIGNLAB_COM_UP_DOWN_NODE_CREATOR_H_