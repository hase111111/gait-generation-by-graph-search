#include "node_initializer.h"

#include "designlab_math_util.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"

namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;


RobotStateNode NodeInitializer::InitNode() const
{
	RobotStateNode res;

	//脚状態
	res.leg_state = dllf::MakeLegStateBit(
		DiscreteComPos::kCenterBack,
		{ true, true, true, true, true, true },
		{ DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
		DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter }
	);


	const float base_z = 0.0f;			// 地面のZ座標	
	const float com_z = 30.f + base_z;	// ロボットの重心のZ座標

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		res.leg_pos[i] = res.leg_reference_pos[i] = {
			170.f * cos(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
			170.f * sin(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
			-com_z
		};
	}

	res.global_center_of_mass = dl::Vector3{ 0.f, 0.f, com_z };
	//res.global_center_of_mass += dl::Vector3(
	//	designlab::math_util::GenerateRandomNumber(-100.f, 100.f),
	//	designlab::math_util::GenerateRandomNumber(-400.f, 400.f),
	//	0
	//);

	//ロールピッチヨーで回転を表現する．ロボットの重心を中心にして回転する． 
	res.quat = dl::Quaternion::MakeByAngleAxis(0.f, dl::Vector3::GetUpVec());

	res.next_move = HexapodMove::kComUpDown;
	res.parent_index = -1;
	res.depth = 0;

	return res;
}