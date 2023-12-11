#include "target_updater_once.h"


TargetUpdaterOnce::TargetUpdaterOnce(const RobotOperation& operation) : operation_(operation)
{
}

RobotOperation TargetUpdaterOnce::Init() const
{
	return operation_;
}

RobotOperation TargetUpdaterOnce::Update([[maybe_unused]] const RobotStateNode& state) const
{
	return operation_;
}