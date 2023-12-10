#ifndef DESIGNLAB_TARGET_UPDATER_FOR_GPG_H_
#define DESIGNLAB_TARGET_UPDATER_FOR_GPG_H_

#include <vector>

#include "designlab_math_util.h"
#include "interface_target_updater.h"


//! @class TargetUpdaterForGpg
//! @brief global path planningを行うための仮置きクラス．
class TargetUpdaterForGpg final : public ITargetUpdater
{
public:

	TargetUpdaterForGpg();

	TargetRobotState Init() const override;

	TargetRobotState Update(const RobotStateNode& node) const override;

private:

	static constexpr float kAllowableAngleError{ ::designlab::math_util::ConvertDegToRad(20.0f) };	//!< 目標角度と現在の角度の許容誤差．

	std::vector<::designlab::Vector3> global_route_;	//!< グローバルパス．
};

#endif  // DESIGNLAB_TARGET_UPDATER_FOR_GPG_H_