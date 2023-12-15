#include "robot_operator_free.h"

#include "designlab_rot_converter.h"

namespace dl = ::designlab;
namespace dle = ::designlab::enums;
namespace dlm = ::designlab::math_util;


namespace
{
	// -π～πの範囲に収める
	float NormalizeAngle(float angle)
	{
		while (angle > dlm::kFloatPi)
		{
			angle -= dlm::kFloatPi;
		}

		while (angle < -dlm::kFloatPi)
		{
			angle += dlm::kFloatPi;
		}

		return angle;
	}
}

RobotOperatorFree::RobotOperatorFree()
{
	global_route_ =
	{
		dl::Vector3{400,400,40},
		dl::Vector3{1300,400,40},
		dl::Vector3{2450,1400,40},
		dl::Vector3{3900,1400,40},
		dl::Vector3{3900,400,40},
	};
}

RobotOperation RobotOperatorFree::Init() const
{
	RobotOperation operation;

	operation.operation_type = dle::RobotOperationType::kStraightMoveVector;
	operation.straight_move_vector_ = dl::Vector3::GetFrontVec();

	return operation;
}

RobotOperation RobotOperatorFree::Update(const RobotStateNode& node)
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
	const float euler_z_angle = NormalizeAngle(dl::ToEulerXYZ(node.quat).z_angle);
	const float rot_dif = angle - euler_z_angle;

	if (abs(rot_dif) > kAllowableAngleError)
	{
		RobotOperation operation;
		operation.operation_type = dle::RobotOperationType::kSpotTurnLastPosture;
		operation.spot_turn_last_posture_ = dl::Quaternion::MakeByAngleAxis(angle, dl::Vector3::GetUpVec());

		return operation;
	}

	RobotOperation operation_straight;

	operation_straight.operation_type = dle::RobotOperationType::kStraightMoveVector;
	operation_straight.straight_move_vector_ = global_route_[most_near_index_] - global_route_[(most_near_index_ - 1) < 0 ? global_route_.size() - 1 : (most_near_index_ - 1)];

	return operation_straight;
}
