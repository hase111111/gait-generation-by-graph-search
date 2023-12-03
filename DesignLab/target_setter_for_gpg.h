#ifndef DESIGNLAB_TARGET_SETTER_FOR_GPG_H_
#define DESIGNLAB_TARGET_SETTER_FOR_GPG_H_

#include <vector>

#include "designlab_math_util.h"
#include "robot_state_node.h"
#include "target_robot_state.h"


//! @class TargetSetterForGpg
//! @brief global path planningを行うための仮置きクラス．
class TargetSetterForGpg final
{
public:

	TargetSetterForGpg();

	TargetRobotState GetTarget(const RobotStateNode& node) const;

private:

	static constexpr float kAllowableAngleError{ ::designlab::math_util::ConvertDegToRad(20.0f) };	//!< 目標角度と現在の角度の許容誤差．

	std::vector<::designlab::Vector3> global_route_;	//!< グローバルパス．
};

#endif  // DESIGNLAB_TARGET_SETTER_FOR_GPG_H_