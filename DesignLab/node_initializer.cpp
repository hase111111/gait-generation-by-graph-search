#include "node_initializer.h"

#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace dl = ::designlab;
namespace dllf = designlab::leg_func;


RobotStateNode NodeInitializer::InitNode() const
{
	using designlab::Vector3;

	RobotStateNode res;

	//脚状態
	res.leg_state = dllf::MakeLegStateBit(
		DiscreteComPos::kCenterBack,
		{ true, true, true, true, true, true },
		{ DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
		DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter }
	);

	//脚付け根を原点とした，脚先の位置を初期化する．
	const float z_base = 0.0f;
	const float kComZ = 30.f + z_base;	// ロボットの重心のZ座標

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		res.leg_pos[i] = res.leg_reference_pos[i] = {
			170 * cos(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
			170 * sin(PhantomXMkIIConst::kCoxaDefaultAngle[i]),
			-kComZ
		};
	}

	//グローバル座標の重心位置．グローバル座標(0,0,0)を中心とした，下の変数 _x，_yを半径とする楕円形のなかに重心を移動する．
	const float kAngle = 0; // do_random ? dlm::GenerateRandomNumber(0.0f, 2.0f * dlm::kFloatPi) : 0;
	const float kEx = 0; // do_random ? dlm::GenerateRandomNumber(0.0f, 1.0f) : 0;


	const float kX = 3000.f * 0.25f;
	const float kY = 2000.f / 2.0f * 0.8f;

	res.global_center_of_mass = dl::Vector3(kEx * kX * cos(kAngle), kEx * kY * sin(kAngle), kComZ);

	//ロールピッチヨーで回転を表現する．ロボットの重心を中心にして回転する． 
	res.quat = dl::Quaternion::MakeByAngleAxis(0.0f, dl::Vector3::GetUpVec());

	res.next_move = HexapodMove::kComUpDown;
	res.parent_num = -1;
	res.depth = 0;

	return res;
}
