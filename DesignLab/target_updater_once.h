//! @file target_updater_once.h
//! @brief ターゲットの更新をせず，初期化時のみに更新するクラス．


#ifndef DESIGNLAB_TARGET_UPDATER_ONCE_H_
#define DESIGNLAB_TARGET_UPDATER_ONCE_H_

#include "interface_target_updater.h"


//! @brief ターゲットの更新をせず，初期化時のみに更新するクラス．
class TargetUpdaterOnce : public ITargetUpdater
{
public:

	TargetUpdaterOnce() = delete;

	//! @brief コンストラクタで指定したターゲットを常に返す．
	//! @param[in] target ターゲット.
	TargetUpdaterOnce(const TargetRobotState& target);

	TargetRobotState Init() const override;

	TargetRobotState Update(const RobotStateNode& state) const override;

private:

	const TargetRobotState target_;
};


#endif // DESIGNLAB_TARGET_UPDATER_ONCE_H_