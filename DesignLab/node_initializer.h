//! @file node_initializer.h
//! @brief ノードの初期化を行うクラス．

#ifndef DESIGNLAB_NODE_INITIALIZER_H_
#define DESIGNLAB_NODE_INITIALIZER_H_

#include "robot_state_node.h"


namespace designlab
{

//! @class NodeInitializer
//! @brief ノードの初期化を行うクラス．
//! @n シミュレーション時にノードの初期値を設定するために使用する．
class NodeInitializer final
{
public:
	NodeInitializer(const Vector3& pos, enums::HexapodMove move);

	//! @brief ノードの初期化を行う．
	//! @return RobotStateNode 初期化されたノード．
	RobotStateNode InitNode() const;

private:
	Vector3 pos_;
	enums::HexapodMove move_;
};

}	// namespace designlab


#endif	// DESIGNLAB_NODE_INITIALIZER_H_