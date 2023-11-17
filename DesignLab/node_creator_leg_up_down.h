﻿//! @file node_creator_leg_up_down.h
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．

#ifndef DESIGNLAB_NODE_CREATOR_LEG_UP_DOWN_H_
#define DESIGNLAB_NODE_CREATOR_LEG_UP_DOWN_H_


#include "interface_node_creator.h"

#include <memory>

#include "devide_map_state.h"
#include "hexapod_next_move.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_vaild_checker.h"


//! @class NodeCreatorLegUpDown
//! @brief 脚の上げ下げをするエッジ(辺，ノードとノードを繋ぐ物)の処理をするクラス．
//! @note 回転を考慮していないので注意
class NodeCreatorLegUpDown final : public INodeCreator
{
public:

	NodeCreatorLegUpDown(
		const DevideMapState& devide_map, 
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		HexapodMove next_move
	);
	~NodeCreatorLegUpDown() = default;

	void Create(const RobotStateNode& current_node, int current_node_index, std::vector<RobotStateNode>* output_graph) const override;

private:

	//! @brief 脚が接地可能か調べる．地面に干渉するかどうかを調べていないので注意．実際に接地するとしたらどこになるかをoutput_ground_posで出力する．
	bool IsGroundableLeg(int leg_num, const RobotStateNode& current_node, designlab::Vector3* output_ground_pos) const;

	//! @brief 離散化した脚位置の4のグローバル座標，候補点のグローバル座標，付け根のグローバル座標．現在の脚状態(1～7)，これらを利用して候補点が離散化した脚位置に適しているか調べる．
	bool IsAbleLegPos(const RobotStateNode& node, int leg_num) const;


	const float kLegMargin;		//!< これだけ動かせば現在の脚位置でも届くのならば，脚位置4判定となる．
	const float kHighMargin;	//!< 縦方向（Z軸方向）のマージン．この範囲内ならば真ん中にあるとみなす．

	const DevideMapState map_;	//!< マップの状態．

	const HexapodMove next_move_;	//!< 次の動作．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_NODE_CREATOR_LEG_UP_DOWN_H_