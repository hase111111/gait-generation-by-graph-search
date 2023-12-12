﻿#include "simulation_end_checker_by_posture.h"

#include "designlab_rot_converter.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;


SimulationEndCheckerByPosture::SimulationEndCheckerByPosture(
	const::designlab::Quaternion& goal_orientation,
	const float allowable_error
) :
	goal_orientation_(goal_orientation),
	goal_euler_(dl::ToEulerXYZ(goal_orientation)),
	allowable_error_(allowable_error)
{
}

bool SimulationEndCheckerByPosture::IsEnd(const RobotStateNode& node) const
{
	//角度を取得し，誤差を計算
	auto now = dl::ToEulerXYZ(node.quat);

	float error_x = std::abs(now.x_angle - goal_euler_.x_angle);
	error_x = (std::min)(error_x, 2 * dlm::kFloatPi - error_x);

	if (error_x > allowable_error_)
	{
		return false;
	}

	float error_y = std::abs(now.y_angle - goal_euler_.y_angle);
	error_y = (std::min)(error_y, 2 * dlm::kFloatPi - error_y);

	if (error_y > allowable_error_)
	{
		return false;
	}

	float error_z = std::abs(now.z_angle - goal_euler_.z_angle);
	error_z = (std::min)(error_z, 2 * dlm::kFloatPi - error_z);

	if (error_z > allowable_error_)
	{
		return false;
	}

	return true;
}