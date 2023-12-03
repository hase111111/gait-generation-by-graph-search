#include "target_setter_for_gpg.h"

#include <iostream>

#include "designlab_rot_converter.h"


namespace dl = ::designlab;
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

TargetSetterForGpg::TargetSetterForGpg() : global_route_{
{4130,210,40},
{4110,230,40},
{4090,250,40},
{4070,270,40},
{4050,290,40},
{4030,310,40},
{4010,330,40},
{3990,350,40},
{3970,370,40},
{3950,390,40},
{3930,410,40},
{3910,430,40},
{3890,450,40},
{3870,470,40},
{3850,490,40},
{3830,510,40},
{3810,530,40},
{3790,550,40},
{3770,570,40},
{3750,590,40},
{3730,610,40},
{3710,990,40},//
{3710,970,40},
{3710,950,40},
{3710,930,40},
{3710,910,40},
{3710,890,40},
{3710,870,40},
{3710,850,40},
{3710,830,40},
{3710,810,40},
{3710,790,40},
{3710,770,40},
{3710,750,40},
{3710,730,40},
{3710,710,40},
{3710,690,40},
{3710,670,40},
{3710,650,40},
{3710,630,40},
{3690,1030,40},
{3690,1010,40},
{3670,1050,40},
{3650,1070,40},
{3630,1090,40},
{3610,1110,40},
{3590,1130,40},
{3570,1130,40},
{3550,1150,40},
{3530,1150,40},
{3510,1150,40},
{3490,1150,40},
{3470,1150,40},
{3450,1150,40},
{3430,1150,40},
{3410,1150,40},
{3390,1150,40},
{3370,1150,40},
{3350,1150,40},
{3330,1150,40},
{3310,1150,40},
{3290,1150,40},
{3270,1150,40},
{3250,1150,40},
{3230,1150,40},
{3210,1150,40},
{3190,1150,40},
{3170,1150,40},
{3150,1150,40},
{3130,1150,40},
{3110,1150,40},
{3090,1150,40},
{3070,1150,40},
{3050,1150,40},
{3030,1150,40},
{3010,1150,40},
{2990,1150,40},
{2970,1150,40},
{2950,1150,40},
{2930,1150,40},
{2910,1150,40},
{2890,1150,40},
{2870,1150,40},
{2850,1150,40},
{2830,1150,40},
{2810,1150,40},
{2790,1150,40},
{2770,1150,40},
{2750,1150,40},
{2730,1150,40},
{2710,1150,40},
{2690,1150,40},
{2670,1150,40},
{2650,1150,40},
{2630,1150,40},
{2610,1150,40},
{2590,1150,40},
{2570,1150,40},
{2550,1150,40},
{2530,1150,40},
{2510,1150,40},
{2490,1150,40},
{2470,1150,40},
{2450,1150,40},
{2430,1150,40},
{2410,1150,40},
{2390,1150,40},
{2370,1150,40},
{2350,1150,40},
{2330,1150,40},
{2310,1150,40},
{2290,1130,40},
{2270,1130,40},
{2250,1110,40},
{2230,1090,40},
{2210,1070,40},
{2190,1050,40},
{2170,1030,40},
{2150,1010,40},
{2130,990,40},
{2110,970,40},
{2090,950,40},
{2070,930,40},
{2050,910,40},
{2030,890,40},
{2010,870,40},
{1990,850,40},
{1970,830,40},
{1950,810,40},
{1930,790,40},
{1910,770,40},
{1890,750,40},
{1870,730,40},
{1850,710,40},
{1830,690,40},
{1810,670,40},
{1790,650,40},
{1770,630,40},
{1750,610,40},
{1730,590,40},
{1710,570,40},
{1690,550,40},
{1670,530,40},
{1650,510,40},
{1630,490,40},
{1610,470,40},
{1590,450,40},
{1570,430,40},
{1550,410,40},
{1530,390,40},
{1510,370,40},
{1490,350,40},
{1470,330,40},
{1450,310,40},
{1430,290,40},
{1410,270,40},
{1390,250,40},
{1370,230,40},
{1350,230,40},
{1330,230,40},
{1310,230,40},
{1290,230,40},
{1270,230,40},
{1250,230,40},
{1230,230,40},
{1210,230,40},
{1190,230,40},
{1170,230,40},
{1150,230,40},
{1130,230,40},
{1110,230,40},
{1090,230,40},
{1070,230,40},
{1050,230,40},
{1030,230,40},
{1010,230,40},
{990,230,40},
{970,230,40},
{950,230,40},
{930,230,40},
{910,230,40},
{890,230,40},
{870,230,40},
{850,230,40},
{830,230,40},
{810,230,40},
{790,230,40},
{770,230,40},
{750,230,40},
{730,230,40},
{710,230,40},
{690,230,40},
{670,230,40},
{650,230,40},
{630,230,40},
{610,230,40},
{590,230,40},
{570,230,40},
{550,230,40},
{530,230,40},
{510,230,40},
{490,230,40},
{470,230,40},
{450,230,40},
{430,230,40},
{410,230,40},
{390,230,40},
{370,230,40},
{350,230,40},
{330,230,40},
{310,230,40},
{290,230,40},
{270,210,40}
}
{
}

TargetRobotState TargetSetterForGpg::GetTarget(const RobotStateNode& node) const
{
	//まず，現在の重心位置から最も近い点を探す
	int most_near_index = 0;
	float distance = 100000.f;

	for (int i = 0; i < global_route_.size(); ++i)
	{
		if (distance > global_route_[i].GetDistanceFrom(node.global_center_of_mass))
		{
			distance = global_route_[i].GetDistanceFrom(node.global_center_of_mass);

			most_near_index = i;
		}
	}

	std::cout << "most_near_index: " << most_near_index << std::endl;

	//次の地点への角度を計算する
	const float euler_z_angle = NormalizeAngle(dl::ToEulerXYZ(node.quat).z_angle);
	const dl::Vector3 diff_vector = global_route_[most_near_index - 1] - global_route_[most_near_index];
	const float target_angle = NormalizeAngle(atan2(diff_vector.y, diff_vector.x));
	const float rot_dif = target_angle - euler_z_angle;

	std::cout << "target_angle : " << dlm::ConvertRadToDeg(target_angle) << "/ now_angle : " << dlm::ConvertRadToDeg(euler_z_angle) << std::endl;

	if (abs(rot_dif) > kAllowableAngleError)
	{
		TargetRobotState target_robot_state;
		target_robot_state.SetSpotTurnLastPosture(dl::Quaternion::MakeByAngleAxis(target_angle, dl::Vector3::GetUpVec()));

		std::cout << "target_quat : " << target_robot_state.GetSpotTurnLastPosture();
		std::cout << "/ now_quat : " << node.quat << std::endl;
		return target_robot_state;
	}

	const int loop_num = 10;
	dl::Vector3 target_vector;

	for (int i = 0; i < loop_num; i++)
	{
		if (most_near_index - i < 0) { break; }

		target_vector += (global_route_[most_near_index - i] - node.global_center_of_mass) * (float)(i + 1);
	}

	std::cout << "target_vector: " << target_vector << "/ normalized" << target_vector.GetNormalized() << std::endl;

	TargetRobotState target_robot_state;
	target_robot_state.SetStraightMoveVector((dl::Vector3{ target_vector.x,target_vector.y,0 }).GetNormalized());

	return target_robot_state;
}
