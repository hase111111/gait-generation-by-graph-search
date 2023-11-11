//! @file node_initializer.h
//! @brief ノードの初期化を行うクラス．

#ifndef DESIGNLAB_NODE_INITIALIZER_H_
#define DESIGNLAB_NODE_INITIALIZER_H_


#include "robot_state_node.h"


//! @class NodeInitializer
//! @brief ノードの初期化を行うクラス．
//! @n シミュレーション時にノードの初期値を設定するために使用する．
class NodeInitializer final
{
public:

	RobotStateNode InitNode() const;
};


#endif	// DESIGNLAB_NODE_INITIALIZER_H_