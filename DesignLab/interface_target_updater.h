//! @file interface_target_updater.h
//! @brief シミュレーション時にロボットの動作を決定するクラスのインターフェース．

#ifndef DESIGNLAB_INTERFACE_TARGET_UPDATER_H_
#define DESIGNLAB_INTERFACE_TARGET_UPDATER_H_

#include "robot_state_node.h"
#include "target_robot_state.h"


//! @class ITargetUpdater
//! @brief シミュレーション時にロボットの動作を決定するクラスのインターフェース．
class ITargetUpdater
{
public:

	virtual ~ITargetUpdater() = default;

	//! @brief ロボットの動作を初期化する．
	//! @return ロボットの動作．
	virtual TargetRobotState Init() const = 0;

	//! @brief ロボットの動作を更新する．
	//! @param[in] state ロボットの現在の状態．
	//! @return ロボットの動作．
	virtual TargetRobotState Update(const RobotStateNode& state) const = 0;
};


#endif // DESIGNLAB_INTERFACE_TARGET_UPDATER_H_