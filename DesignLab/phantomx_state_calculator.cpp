#include "phantomx_state_calculator.h"

#include <cmath>

#include "cassert_define.h"
#include "designlab_line_segment2.h"
#include "designlab_math_util.h"

namespace dlm = designlab::math_util;


PhantomXStateCalclator::PhantomXStateCalclator() : 
	free_leg_pos_({ {
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[0]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[0]),-25},
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[1]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[1]),-25},
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[2]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[2]),-25},
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[3]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[3]),-25},
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[4]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[4]),-25},
		{160 * cos(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[5]),160 * sin(HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[5]),-25}
	} })
{
	//脚の付け根の位置を初期化する
	m_local_leg_base_pos[0] = designlab::Vector3(HexapodConst::BODY_FRONT_LENGTH, -HexapodConst::BODY_FRONT_WIDTH, 0.0f);	// 脚0 右上
	m_local_leg_base_pos[1] = designlab::Vector3(0.0f, -HexapodConst::BODY_CENTER_WIDTH, 0.0f);	// 脚1 右横
	m_local_leg_base_pos[2] = designlab::Vector3(-HexapodConst::BODY_REAR_LENGTH, -HexapodConst::BODY_REAR_WIDTH, 0.0f);	// 脚2 右下
	m_local_leg_base_pos[3] = designlab::Vector3(-HexapodConst::BODY_REAR_LENGTH, HexapodConst::BODY_REAR_WIDTH, 0.0f);	// 脚3 左下
	m_local_leg_base_pos[4] = designlab::Vector3(0.0f, HexapodConst::BODY_CENTER_WIDTH, 0.0f);	// 脚4 左横
	m_local_leg_base_pos[5] = designlab::Vector3(HexapodConst::BODY_FRONT_LENGTH, HexapodConst::BODY_FRONT_WIDTH, 0.0f);	// 脚5 左上


	// m_is_able_leg_pos を初期化する．悪夢の4重ループ
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		for (int x = 0; x < LEG_POS_DIV_NUM; x++)
		{
			for (int y = 0; y < LEG_POS_DIV_NUM; y++)
			{
				for (int z = 0; z < LEG_POS_DIV_NUM; z++)
				{
					m_is_able_leg_pos[i][x][y][z] = initIsAbleLegPos(i, x, y, z);
				}
			}
		}
	}
}


bool PhantomXStateCalclator::CalculateAllJointState(const RobotStateNode& node, std::array<HexapodJointState, HexapodConst::LEG_NUM>* joint_state) const
{
	assert(joint_state != nullptr);		//joint_stateはnullptrではない．

	//逆運動学の式はReference/Hexapodの画像を参照してください

	//計算を行う．
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		calculateLocalJointState(i, node.leg_pos[i], &joint_state->at(i));

		joint_state->at(i).global_joint_position.clear();
		joint_state->at(i).global_joint_position.resize(4);

		joint_state->at(i).global_joint_position[0] = node.global_center_of_mass + designlab::rotVector(GetLocalLegBasePosition(i), node.rot);
		joint_state->at(i).global_joint_position[1] = node.global_center_of_mass + designlab::rotVector(GetLocalLegBasePosition(i) + joint_state->at(i).local_joint_position[1], node.rot);
		joint_state->at(i).global_joint_position[2] = node.global_center_of_mass + designlab::rotVector(GetLocalLegBasePosition(i) + joint_state->at(i).local_joint_position[2], node.rot);
		joint_state->at(i).global_joint_position[3] = node.global_center_of_mass + designlab::rotVector(GetLocalLegBasePosition(i) + node.leg_pos[i], node.rot);
	}

	return false;
}


designlab::Vector3 PhantomXStateCalclator::ConvertGlobalToLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	if (consider_rot)
	{
		return designlab::rotVector(leg_pos - global_center_of_mass, robot_rot * -1) - GetLocalLegBasePosition(leg_index);
	}
	else
	{
		return leg_pos - global_center_of_mass - GetLocalLegBasePosition(leg_index);
	}
}

designlab::Vector3 PhantomXStateCalclator::GetFreeLegPosition(const int leg_index) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	return free_leg_pos_[leg_index];
}


designlab::Vector3 PhantomXStateCalclator::GetLocalLegBasePosition(const int leg_index) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	return m_local_leg_base_pos[leg_index];
}

designlab::Vector3 PhantomXStateCalclator::GetLocalLegPosition(const int leg_index, const designlab::Vector3& leg_pos) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	return leg_pos + GetLocalLegBasePosition(leg_index);
}


designlab::Vector3 PhantomXStateCalclator::GetGlobalLegBasePosition(const int leg_index, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	if (consider_rot) { return designlab::rotVector(GetLocalLegBasePosition(leg_index), robot_rot) + global_center_of_mass; }
	else { return GetLocalLegBasePosition(leg_index) + global_center_of_mass; }
}


designlab::Vector3 PhantomXStateCalclator::GetGlobalLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	if (consider_rot) { return designlab::rotVector(GetLocalLegBasePosition(leg_index) + leg_pos, robot_rot) + global_center_of_mass; }
	else { return global_center_of_mass + GetLocalLegBasePosition(leg_index) + leg_pos; }
}


bool PhantomXStateCalclator::IsLegInRange(const int leg_index, const designlab::Vector3& leg_pos) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	//範囲外ならfalse
	if (getLegPosIndex(leg_pos.x) < 0 || LEG_POS_DIV_NUM <= getLegPosIndex(leg_pos.x)) { return false; }
	if (getLegPosIndex(leg_pos.y) < 0 || LEG_POS_DIV_NUM <= getLegPosIndex(leg_pos.y)) { return false; }
	if (getLegPosIndex(leg_pos.z) < 0 || LEG_POS_DIV_NUM <= getLegPosIndex(leg_pos.z)) { return false; }

	if (m_is_able_leg_pos[leg_index][getLegPosIndex(leg_pos.x)][getLegPosIndex(leg_pos.y)][getLegPosIndex(leg_pos.z)])
	{
		const designlab::Vector2 leg_pos_xy = leg_pos.ProjectedXY();
		const designlab::Vector2 min_leg_pos_xy{HexapodConst::MOVABLE_LEG_RANGE_COS_MIN[leg_index], HexapodConst::MOVABLE_LEG_RANGE_SIN_MAX[leg_index]};
		const designlab::Vector2 max_leg_pos_xy{HexapodConst::MOVABLE_LEG_RANGE_COS_MAX[leg_index], HexapodConst::MOVABLE_LEG_RANGE_SIN_MIN[leg_index]};

		//脚の角度が範囲内にあるか調べる．外積計算で間にあるか調べる
		if (min_leg_pos_xy.Cross(leg_pos_xy) > 0.0f) { return false; }
		if (max_leg_pos_xy.Cross(leg_pos_xy) < 0.0f) { return false; }

		return true;
	}

	return false;
}


bool PhantomXStateCalclator::IsLegInterfering(const std::array<designlab::Vector3, HexapodConst::LEG_NUM>& leg_pos) const
{
	//重心を原点とした，座標系において，脚の干渉を調べる．

	//脚の干渉を調べる．
	designlab::Vector2 leg_pos_xy[HexapodConst::LEG_NUM];
	designlab::Vector2 joint_pos_xy[HexapodConst::LEG_NUM];

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		joint_pos_xy[i] = GetLocalLegBasePosition(i).ProjectedXY();
		leg_pos_xy[i] = leg_pos[i].ProjectedXY() + joint_pos_xy[i];
	}

	//隣の脚との干渉を調べる．
	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		designlab::LineSegment2 line1(joint_pos_xy[i], leg_pos_xy[i]);
		designlab::LineSegment2 line2(joint_pos_xy[(i + 1) % HexapodConst::LEG_NUM], leg_pos_xy[(i + 1) % HexapodConst::LEG_NUM]);

		if (line1.HasIntersection(line2)) { return true; }
	}

	return false;
}


bool PhantomXStateCalclator::initIsAbleLegPos(const int leg_index, const int x, const int y, const int z) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	float x_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * x + LEG_POS_MIN;
	float y_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * y + LEG_POS_MIN;
	float z_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * z + LEG_POS_MIN;

	designlab::Vector3 leg_pos{x_pos, y_pos, z_pos};		//脚先の位置


	//仮 x=0 y=0 の時はfalseにする
	if (abs(x_pos) <= (LEG_POS_MAX - LEG_POS_MIN) / (LEG_POS_DIV_NUM - 1.0f) && abs(y_pos) <= (LEG_POS_MAX - LEG_POS_MIN) / (LEG_POS_DIV_NUM - 1.0f)) { return false; }

	if (leg_pos.ProjectedXY().Length() < MIN_LEG_R) { return false; }

	// 関節の角度を計算する
	HexapodJointState joint_state;

	calculateLocalJointState(leg_index, leg_pos, &joint_state);


	// coxa関節の範囲内に存在しているかを確認する
	const float kCoxaMargim = dlm::ConvertDegToRad(0.0f);

	if (HexapodConst::PHANTOMX_COXA_ANGLE_MIN + HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index] + kCoxaMargim > joint_state.joint_angle[0]) { return false; }

	if (HexapodConst::PHANTOMX_COXA_ANGLE_MAX + HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index] - kCoxaMargim < joint_state.joint_angle[0]) { return false; }

	// femur関節の範囲内に存在しているかを確認する
	const float kFemurMargim = dlm::ConvertDegToRad(0.0f);

	if (joint_state.joint_angle[1] < HexapodConst::PHANTOMX_FEMUR_ANGLE_MIN + kFemurMargim || HexapodConst::PHANTOMX_FEMUR_ANGLE_MAX - kFemurMargim < joint_state.joint_angle[1]) { return false; }

	// tibia関節の範囲内に存在しているかを確認する
	const float kTibiaMargim = dlm::ConvertDegToRad(0.0f);

	if (joint_state.joint_angle[2] < HexapodConst::PHANTOMX_TIBIA_ANGLE_MIN + kTibiaMargim || HexapodConst::PHANTOMX_TIBIA_ANGLE_MAX - kTibiaMargim < joint_state.joint_angle[2]) { return false; }

	// リンクの長さを確認する
	if (!dlm::IsEqual((joint_state.local_joint_position[0] - joint_state.local_joint_position[1]).Length(), HexapodConst::PHANTOMX_COXA_LENGTH)) { return false; }

	if (!dlm::IsEqual((joint_state.local_joint_position[1] - joint_state.local_joint_position[2]).Length(), HexapodConst::PHANTOMX_FEMUR_LENGTH)) { return false; }

	if (!dlm::IsEqual((joint_state.local_joint_position[2] - joint_state.local_joint_position[3]).Length(), HexapodConst::PHANTOMX_TIBIA_LENGTH)) { return false; }

	return true;
}


void PhantomXStateCalclator::calculateLocalJointState(const int leg_index, const designlab::Vector3& leg_pos, HexapodJointState* joint_state) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::LEG_NUM);	//leg_indexは 0～5 である．

	const int kLinkNum = 4;
	const int kJointNum = kLinkNum - 1;

	(*joint_state).local_joint_position.clear();
	(*joint_state).joint_angle.clear();

	(*joint_state).local_joint_position.resize(kLinkNum);
	(*joint_state).joint_angle.resize(kJointNum);


	// coxa jointの計算
	(*joint_state).local_joint_position[0] = designlab::Vector3{ 0, 0, 0 };


	// coxa angleの計算
	float coxa_joint_angle = std::atan2f(leg_pos.y, leg_pos.x);

	if (leg_pos.y == 0 && leg_pos.x == 0) { coxa_joint_angle = HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index]; }

	(*joint_state).joint_angle[0] = coxa_joint_angle;


	// femur jointの計算
	const designlab::Vector3 femur_joint_pos = designlab::Vector3{ HexapodConst::PHANTOMX_COXA_LENGTH * std::cos(coxa_joint_angle), HexapodConst::PHANTOMX_COXA_LENGTH * std::sin(coxa_joint_angle), 0 };

	(*joint_state).local_joint_position[1] = femur_joint_pos;


	if ((leg_pos - femur_joint_pos).Length() > HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH) { return; }


	// tibia joint / femur angle の計算
	const float leg_to_f_x = (leg_pos - femur_joint_pos).ProjectedXY().Length();				//脚先から第一関節までの長さ．
	const float leg_to_f_z = leg_pos.z - femur_joint_pos.z;

	const float arccos_arg = (dlm::Squared(leg_to_f_x) + dlm::Squared(leg_to_f_z) + dlm::Squared(HexapodConst::PHANTOMX_FEMUR_LENGTH) - dlm::Squared(HexapodConst::PHANTOMX_TIBIA_LENGTH))
		/ (2 * HexapodConst::PHANTOMX_FEMUR_LENGTH * std::sqrt(dlm::Squared(leg_to_f_x) + dlm::Squared(leg_to_f_z)));

	float fumur_joint_angle1 = std::acos(arccos_arg) + std::atan2(leg_to_f_z, leg_to_f_x);
	float fumur_joint_angle2 = -std::acos(arccos_arg) + std::atan2(leg_to_f_z, leg_to_f_x);

	designlab::Vector3 tibia_joint_pos1 = femur_joint_pos +
		designlab::Vector3 { HexapodConst::PHANTOMX_FEMUR_LENGTH* std::cos(coxa_joint_angle)* std::cos(fumur_joint_angle1),
		HexapodConst::PHANTOMX_FEMUR_LENGTH* std::sin(coxa_joint_angle)* std::cos(fumur_joint_angle1),
		HexapodConst::PHANTOMX_FEMUR_LENGTH* std::sin(fumur_joint_angle1)
	};

	designlab::Vector3 tibia_joint_pos2 = femur_joint_pos +
		designlab::Vector3 { HexapodConst::PHANTOMX_FEMUR_LENGTH* std::cos(coxa_joint_angle)* std::cos(fumur_joint_angle2),
		HexapodConst::PHANTOMX_FEMUR_LENGTH* std::sin(coxa_joint_angle)* std::cos(fumur_joint_angle2),
		HexapodConst::PHANTOMX_FEMUR_LENGTH* std::sin(fumur_joint_angle2)
	};

	//if (tibia_joint_pos1.ProjectedXY().LengthSquare() < tibia_joint_pos2.ProjectedXY().LengthSquare())
	//{  ↑これは間違ってた
	if (false)
	{
		(*joint_state).local_joint_position[2] = tibia_joint_pos2;

		(*joint_state).joint_angle[1] = fumur_joint_angle2;
	}
	else
	{
		(*joint_state).local_joint_position[2] = tibia_joint_pos1;

		(*joint_state).joint_angle[1] = fumur_joint_angle1;
	}

	(*joint_state).joint_angle[2] = std::atan2((leg_to_f_z - HexapodConst::PHANTOMX_FEMUR_LENGTH * std::sin((*joint_state).joint_angle[1])),
		(leg_to_f_x - HexapodConst::PHANTOMX_FEMUR_LENGTH * std::cos((*joint_state).joint_angle[1]))) - (*joint_state).joint_angle[1];


	// 脚先の追加
	(*joint_state).local_joint_position[3] = leg_pos;
}
