#include "target_updater_once.h"


TargetUpdaterOnce::TargetUpdaterOnce(const TargetRobotState& target) : target_(target)
{
}

TargetRobotState TargetUpdaterOnce::Init() const
{
	return target_;
}

TargetRobotState TargetUpdaterOnce::Update([[maybe_unused]] const RobotStateNode& state) const
{
	return target_;
}