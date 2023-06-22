#include "HexapodStateCalculator.h"
#include "MyMath.h"
#include <cmath>

using namespace my_vec;

float HexapodStateCalclator::m_leg_max_r[200] = {};
float HexapodStateCalclator::m_leg_min_r[200] = {};

HexapodStateCalclator::HexapodStateCalclator()
{
	//ジョイントの位置を初期化する．
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		m_local_femurjoint_pos[i] = m_local_tibiajoint_pos[i] = SVector(0, 0, 0);
	}
}

my_vec::SVector HexapodStateCalclator::getGlobalLegPos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num) + _node.leg_pos[_leg_num],_node.rot) + _node.global_center_of_mass;
}

my_vec::SVector HexapodStateCalclator::getLocalLegPos(const SNode& _node, my_vec::SVector _global_pos, const int _leg_num) const
{
	return rotVector(_global_pos - _node.global_center_of_mass, _node.rot * -1) - getLocalCoxaJointPos(_leg_num);
}

my_vec::SVector HexapodStateCalclator::getGlobalLeg2Pos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num) + _node.Leg2[_leg_num], _node.rot) + _node.global_center_of_mass;
}

my_vec::SVector HexapodStateCalclator::getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num), _node.rot) + _node.global_center_of_mass;
}

void HexapodStateCalclator::calclateJointPos(const SNode& _node)
{
	// 逆運動学的にジョイントの場所を計算する．
	//ノードの脚位置は正しい場所にあるという前提のもと計算するので，めちゃくちゃな値が代入されているとうまく動作しない．
	//チェックする機能を付けると重くなるので，そもそもそんなノードを生成しないように注意する．

	using namespace my_math;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		const float _coxa_joint_angle = atan2(_node.leg_pos[i].y,_node.leg_pos[i].x);

		m_local_femurjoint_pos[i] = SVector(HexapodConst::COXA_LENGTH * cos(_coxa_joint_angle), HexapodConst::COXA_LENGTH * sin(_coxa_joint_angle), 0);


		const float _L = std::sqrt(squared(_node.leg_pos[i].x - m_local_femurjoint_pos[i].x) + squared(_node.leg_pos[i].y - m_local_femurjoint_pos[i].y));				//脚先から第一関節までの長さ．
		const float _leg_to_fumur_len = std::sqrt(squared(_L) + squared(_node.leg_pos[i].z));

		const float _s1 = squared(_leg_to_fumur_len) + squared(HexapodConst::TIBIA_LENGTH) - squared(HexapodConst::FEMUR_LENGTH);
		const float _s2 = 2 * HexapodConst::TIBIA_LENGTH * _leg_to_fumur_len;
		
		const float _fumur_joint_angle = -(std::acos(_s1 / _s2) + std::atan(-_node.leg_pos[i].z / _L));

		m_local_tibiajoint_pos[i] = _node.leg_pos[i] - 
										SVector(HexapodConst::TIBIA_LENGTH * cos(_coxa_joint_angle) * cos(_fumur_joint_angle), 
												HexapodConst::TIBIA_LENGTH * sin(_coxa_joint_angle) * cos(_fumur_joint_angle),
												HexapodConst::TIBIA_LENGTH * sin(_fumur_joint_angle));

		if (abs((m_local_femurjoint_pos[i] - m_local_tibiajoint_pos[i]).length() - HexapodConst::FEMUR_LENGTH) > my_math::ALLOWABLE_ERROR)
		{
			const float _fumur_joint_angle = -(-std::acos(_s1 / _s2) + std::atan(-_node.leg_pos[i].z / _L));

			m_local_tibiajoint_pos[i] = _node.leg_pos[i] -
				SVector(HexapodConst::TIBIA_LENGTH * cos(_coxa_joint_angle) * cos(_fumur_joint_angle),
					HexapodConst::TIBIA_LENGTH * sin(_coxa_joint_angle) * cos(_fumur_joint_angle),
					HexapodConst::TIBIA_LENGTH * sin(_fumur_joint_angle));
		}
	}

}

my_vec::SVector HexapodStateCalclator::getGlobalFemurJointPos(const SNode& _node,const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num) + m_local_femurjoint_pos[_leg_num], _node.rot) + _node.global_center_of_mass;
}

my_vec::SVector HexapodStateCalclator::getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num) + m_local_tibiajoint_pos[_leg_num], _node.rot) + _node.global_center_of_mass;
}

void HexapodStateCalclator::initLegR()
{
	using namespace my_math;

	for (int _z = 0; _z < MAX_DIF_Z; _z++)
	{
		float _max_r = 0;
		float _min_r = 0;

		for (float _x = HexapodConst::COXA_LENGTH; _x < HexapodConst::COXA_LENGTH + HexapodConst::FEMUR_LENGTH + HexapodConst::TIBIA_LENGTH; _x++)
		{
			SVector _tmp_leg((float)_x, 0, -(float)_z);
			
			// 以下の三変数を辺とする三角形が成立するか調べる．
			float _a = HexapodConst::TIBIA_LENGTH;
			float _b = HexapodConst::FEMUR_LENGTH;
			float _c = sqrt(squared(_tmp_leg.x - HexapodConst::COXA_LENGTH) + squared(_tmp_leg.z));

			bool _is_vaild_triangle = true;
			if (_a + _b < _c)_is_vaild_triangle = false;
			if (_a + _c < _b)_is_vaild_triangle = false;
			if (_c + _b < _a)_is_vaild_triangle = false;

			//成立するのならば，そこに脚を伸ばすことができるはず．
			if (_is_vaild_triangle) 
			{
				if (_min_r == 0)_min_r = _x;
				_max_r = _x; 
			}

		}

		m_leg_max_r[_z] = _max_r;
		m_leg_min_r[_z] = _min_r;
	}
}

float HexapodStateCalclator::getMaxLegR(const float _coxa_z_to_leg_z) const
{
	int _dif_z = (int)abs(_coxa_z_to_leg_z);		// Z座標の差異を正の整数の値に変換する．誤差はでるけど高速に処理できるようになる．

	//値が範囲外であるならば，0を返す．
	if (_dif_z < 0) { return 0.0f; }
	if (MAX_DIF_Z < _dif_z ) { return 0.0f; }

	return m_leg_max_r[_dif_z];
}

float HexapodStateCalclator::getMinLegR(const float _coxa_z_to_leg_z) const
{
	int _dif_z = (int)abs(_coxa_z_to_leg_z);		// Z座標の差異を正の整数の値に変換する．誤差はでるけど高速に処理できるようになる．

	//値が範囲外であるならば，0を返す．
	if (_dif_z < 0) { return 0.0f; }
	if (MAX_DIF_Z < _dif_z) { return 0.0f; }

	return m_leg_min_r[_dif_z];
}

my_vec::SVector HexapodStateCalclator::getLocalCoxaJointPos(const int _leg_num) const
{
	//重心を原点とした座標．ロボットの正面をxの正，ロボットの上をzの正，右手座標系でy座標をとっている．
	//グローバル座標系のxyz軸とは別の軸なので，回転は考慮されていない．

	if (_leg_num == 0)		{ return SVector(HexapodConst::BODY_FRONT_LENGTH,	-HexapodConst::BODY_FRONT_WIDTH,	0.0f); }	// 脚0 右上
	else if (_leg_num == 1) { return SVector(0.0f,								-HexapodConst::BODY_CENTER_WIDTH,	0.0f); }	// 脚1 右横
	else if (_leg_num == 2)	{ return SVector(-HexapodConst::BODY_REAR_LENGTH,	-HexapodConst::BODY_REAR_WIDTH,		0.0f); }	// 脚2 右下
	else if (_leg_num == 3)	{ return SVector(-HexapodConst::BODY_REAR_LENGTH,	HexapodConst::BODY_REAR_WIDTH,		0.0f); }	// 脚3 左下
	else if (_leg_num == 4)	{ return SVector(0.0f,								HexapodConst::BODY_CENTER_WIDTH,	0.0f); }	// 脚4 左横
	else if (_leg_num == 5)	{ return SVector(HexapodConst::BODY_FRONT_LENGTH,	HexapodConst::BODY_FRONT_WIDTH,		0.0f); }	// 脚5 左上

	return my_vec::SVector(0, 0, 0);
}
