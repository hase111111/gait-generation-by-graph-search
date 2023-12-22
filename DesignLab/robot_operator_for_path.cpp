#include "robot_operator_for_path.h"

#include "designlab_rot_converter.h"


namespace
{
// -π～πの範囲に収める
float NormalizeAngle(float angle)
{
	while (angle > designlab::MathConst<float>::kPi)
	{
		angle -= designlab::MathConst<float>::kPi;
	}

	while (angle < -designlab::MathConst<float>::kPi)
	{
		angle += designlab::MathConst<float>::kPi;
	}

	return angle;
}

}


namespace designlab
{

RobotOperatorForPath::RobotOperatorForPath(const std::vector<Vector3>& path) :
	global_route_(path),
	most_near_index_(0)
{
}

RobotOperation RobotOperatorForPath::Init() const
{
	RobotOperation operation;

	operation.operation_type = enums::RobotOperationType::kStraightMoveVector;
	operation.straight_move_vector_ = Vector3::GetFrontVec();

	return operation;
}

RobotOperation RobotOperatorForPath::Update(const RobotStateNode& node)
{
	if (
		(global_route_[most_near_index_].ProjectedXY() - node.global_center_of_mass.ProjectedXY()).GetLength() < 100.f
		)
	{
		most_near_index_++;
	}

	if (most_near_index_ >= global_route_.size())
	{
		most_near_index_ = 0;
	}

	const auto diff = global_route_[most_near_index_].ProjectedXY() - node.global_center_of_mass.ProjectedXY();

	const float angle = atan2(diff.y, diff.x);
	const float euler_z_angle = NormalizeAngle(ToEulerXYZ(node.quat).z_angle);
	const float rot_dif = angle - euler_z_angle;

	if (abs(rot_dif) > kAllowableAngleError)
	{
		RobotOperation operation;
		operation.operation_type = enums::RobotOperationType::kSpotTurnLastPosture;
		operation.spot_turn_last_posture_ = Quaternion::MakeByAngleAxis(angle, Vector3::GetUpVec());

		return operation;
	}

	RobotOperation operation_straight;

	operation_straight.operation_type = enums::RobotOperationType::kStraightMoveVector;

	if (most_near_index_ == 0)
	{
		operation_straight.straight_move_vector_ = global_route_[most_near_index_] - node.global_center_of_mass;
	}
	else
	{
		operation_straight.straight_move_vector_ = global_route_[most_near_index_] - global_route_[most_near_index_ - 1];
	}

	return operation_straight;
}

} // namespace designlab