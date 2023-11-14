//! @file interpolated_node_creator.h
//! @brief ノード間を補間するクラス


#ifndef DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_
#define DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_

#include <array>
#include <vector>

#include "robot_state_node.h"


//! @class InterpolatedNodeCreator
//! @brief ノード間を補間するクラス．矩形軌道を生成する
class InterpolatedNodeCreator final
{
public:
	InterpolatedNodeCreator() = default;


	//! @brief ノード間を補間する．
	//! @param[in]	node 現在のノード．
	//! @param[in]	next_node 次のノード．
	//! @return std::vector<RobotStateNode> 補間されたノード．
	std::vector<RobotStateNode> CreateInterpolatedNode(const RobotStateNode& node, const RobotStateNode& next_node) const;

private:
	static constexpr int kGroundInterpolatedNodeNum = 10;	//脚が接地する際の補間ノード数
	static constexpr int kBodyMoveInterpolatedNodeNum = 10;	//胴体が移動する際の補間ノード数
	static constexpr int kFreeInterpolatedNodeNum = 10;		//脚が遊脚する際の補間ノード数

	//! @return 補間が必要ないならばtrueを返す．
	bool IsNoChange(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	//! @return 胴体が移動するならtrueを返す．
	bool IsBodyMove(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	std::vector<RobotStateNode> CreateBodyMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	std::vector<RobotStateNode> CreateLegMoveInterpolatedNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	//! @brief 接地動作をする脚のindexを取得する．
	std::vector<int> GetGroundMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const;

	//! @brief 遊脚動作をする脚のindexを取得する．
	std::vector<int> GetFreeMoveIndex(const RobotStateNode& current_node, const RobotStateNode& next_node) const;
};


#endif	//	DESIGNLAB_INTERPOLATED_NODE_CREATOR_H_