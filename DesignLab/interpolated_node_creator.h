#pragma once

#include <vector>

#include "node.h"


class InterpolatedNodeCreator
{
public:
	InterpolatedNodeCreator() = default;


	//! @brief ノード間を補間する
	//! @param[in]	node 現在のノード
	//! @param[in]	next_node 次のノード
	//! @param[out]	interpolated_node 補間されたノード
	void createInterpolatedNode(const SNode& node, const SNode& next_node, std::vector<SNode>* interpolated_node) const;

private:

	const int INTERPOLATED_NODE_NUM1 = 5;		//補間するノードの数
	const int INTERPOLATED_NODE_NUM2 = 5;		//補間するノードの数
	const int INTERPOLATED_NODE_NUM = INTERPOLATED_NODE_NUM1 + INTERPOLATED_NODE_NUM2;		//補間するノードの数
};

