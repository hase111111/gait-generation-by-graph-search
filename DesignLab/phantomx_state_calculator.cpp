#include "phantomx_state_calculator.h"

#include <cmath>

#include "cassert_define.h"
#include "designlab_line_segment2.h"
#include "designlab_math_util.h"
#include "phantomx_const.h"

namespace dlm = designlab::math_util;


PhantomXStateCalclator::PhantomXStateCalclator() :
	free_leg_pos_({ {
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[0]),170 * sin(PhantomXConst::kCoxaDefaultAngle[0]),-25},
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[1]),170 * sin(PhantomXConst::kCoxaDefaultAngle[1]),-25},
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[2]),170 * sin(PhantomXConst::kCoxaDefaultAngle[2]),-25},
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[3]),170 * sin(PhantomXConst::kCoxaDefaultAngle[3]),-25},
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[4]),170 * sin(PhantomXConst::kCoxaDefaultAngle[4]),-25},
		{170 * cos(PhantomXConst::kCoxaDefaultAngle[5]),170 * sin(PhantomXConst::kCoxaDefaultAngle[5]),-25}
	} })
{
	//脚の付け根の位置を初期化する
	local_leg_base_pos_[0] = designlab::Vector3{ PhantomXConst::kCoxaBaseOffsetX, -PhantomXConst::kCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };	// 脚0 右上
	local_leg_base_pos_[1] = designlab::Vector3{ 0.0f, -PhantomXConst::kCenterCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };						// 脚1 右横
	local_leg_base_pos_[2] = designlab::Vector3{ -PhantomXConst::kCoxaBaseOffsetX, -PhantomXConst::kCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };// 脚2 右下
	local_leg_base_pos_[3] = designlab::Vector3{ -PhantomXConst::kCoxaBaseOffsetX, PhantomXConst::kCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };	// 脚3 左下
	local_leg_base_pos_[4] = designlab::Vector3{ 0.0f, PhantomXConst::kCenterCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };						// 脚4 左横
	local_leg_base_pos_[5] = designlab::Vector3{ PhantomXConst::kCoxaBaseOffsetX, PhantomXConst::kCoxaBaseOffsetY, PhantomXConst::kCoxaBaseOffsetZ };	// 脚5 左上


	// is_able_leg_pos_ を初期化する．悪夢の4重ループ
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		for (int x = 0; x < kLegPosDivNum; x++)
		{
			for (int y = 0; y < kLegPosDivNum; y++)
			{
				for (int z = 0; z < kLegPosDivNum; z++)
				{
					is_able_leg_pos_[i][x][y][z] = InitIsAbleLegPos(i, x, y, z);
				}
			}
		}
	}
}


void PhantomXStateCalclator::CalculateAllJointState(const RobotStateNode& node, std::array<HexapodJointState, HexapodConst::kLegNum>* joint_state) const
{
	assert(joint_state != nullptr);		//joint_stateはnullptrではない．

	//逆運動学の式はReference/Hexapodの画像を参照してください

	//計算を行う．
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		CalculateLocalJointState(i, node.leg_pos[i], &joint_state->at(i));

		assert(joint_state->at(i).local_joint_position.size() == 4);
		assert(joint_state->at(i).joint_angle.size() == 3);

		(*joint_state)[i].global_joint_position.clear();
		(*joint_state)[i].global_joint_position.resize(4);

		(*joint_state)[i].global_joint_position[0] = node.global_center_of_mass + designlab::RotateVector3(GetLocalLegBasePosition(i), node.rot);
		
		for (int j = 0; j < 4; j++)
		{
			(*joint_state)[i].global_joint_position[j] = 
				node.global_center_of_mass + designlab::RotateVector3(GetLocalLegBasePosition(i) + joint_state->at(i).local_joint_position[j], node.rot);
		}
	}
}

bool PhantomXStateCalclator::IsVaildJointState(const RobotStateNode& node, const std::array<HexapodJointState, HexapodConst::kLegNum>& joint_state) const
{
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		assert(joint_state[i].local_joint_position.size() == 4);
		assert(joint_state[i].joint_angle.size() == 3);

		// coxa関節の範囲内に存在しているかを確認する
		if (! PhantomXConst::IsVaildCoxaAngle(i, joint_state[i].joint_angle[0])) { return false; }

		// femur関節の範囲内に存在しているかを確認する
		if (! PhantomXConst::IsVaildFemurAngle(joint_state[i].joint_angle[1])) { return false; }

		// tibia関節の範囲内に存在しているかを確認する
		if (! PhantomXConst::IsVaildTibiaAngle(joint_state[i].joint_angle[2])) { return false; }

		// リンクの長さを確認する
		if (!dlm::IsEqual((joint_state[i].local_joint_position[0] - joint_state[i].local_joint_position[1]).GetLength(), PhantomXConst::kCoxaLength)) 
		{
			return false; 
		}

		if (!dlm::IsEqual((joint_state[i].local_joint_position[1] - joint_state[i].local_joint_position[2]).GetLength(), PhantomXConst::kFemurLength)) 
		{
			return false; 
		}

		if (!dlm::IsEqual((joint_state[i].local_joint_position[2] - joint_state[i].local_joint_position[3]).GetLength(), PhantomXConst::kTibiaLength)) 
		{
			return false; 
		}

		// 脚位置と脚先座標が一致しているかを確認する
		if (joint_state[i].local_joint_position[3] !=  node.leg_pos[i]) { return false; }
	}

	return true;
}


designlab::Vector3 PhantomXStateCalclator::ConvertGlobalToLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	if (consider_rot)
	{
		return designlab::RotateVector3(leg_pos - global_center_of_mass, robot_rot * -1) - GetLocalLegBasePosition(leg_index);
	}
	else
	{
		return leg_pos - global_center_of_mass - GetLocalLegBasePosition(leg_index);
	}
}

designlab::Vector3 PhantomXStateCalclator::GetFreeLegPosition(const int leg_index) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	return free_leg_pos_[leg_index];
}


designlab::Vector3 PhantomXStateCalclator::GetLocalLegBasePosition(const int leg_index) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	return local_leg_base_pos_[leg_index];
}

designlab::Vector3 PhantomXStateCalclator::GetLocalLegPosition(const int leg_index, const designlab::Vector3& leg_pos) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	return leg_pos + GetLocalLegBasePosition(leg_index);
}


designlab::Vector3 PhantomXStateCalclator::GetGlobalLegBasePosition(const int leg_index, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	if (consider_rot) { return designlab::RotateVector3(GetLocalLegBasePosition(leg_index), robot_rot) + global_center_of_mass; }
	else { return GetLocalLegBasePosition(leg_index) + global_center_of_mass; }
}


designlab::Vector3 PhantomXStateCalclator::GetGlobalLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	if (consider_rot) { return designlab::RotateVector3(GetLocalLegBasePosition(leg_index) + leg_pos, robot_rot) + global_center_of_mass; }
	else { return global_center_of_mass + GetLocalLegBasePosition(leg_index) + leg_pos; }
}


bool PhantomXStateCalclator::IsLegInRange(const int leg_index, const designlab::Vector3& leg_pos) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	//範囲外ならfalse
	if (GetLegPosIndex(leg_pos.x) < 0 || kLegPosDivNum <= GetLegPosIndex(leg_pos.x)) { return false; }
	if (GetLegPosIndex(leg_pos.y) < 0 || kLegPosDivNum <= GetLegPosIndex(leg_pos.y)) { return false; }
	if (GetLegPosIndex(leg_pos.z) < 0 || kLegPosDivNum <= GetLegPosIndex(leg_pos.z)) { return false; }

	if (is_able_leg_pos_[leg_index][GetLegPosIndex(leg_pos.x)][GetLegPosIndex(leg_pos.y)][GetLegPosIndex(leg_pos.z)])
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


bool PhantomXStateCalclator::IsLegInterfering(const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const
{
	//重心を原点とした，座標系において，脚の干渉を調べる．

	//脚の干渉を調べる．
	designlab::Vector2 leg_pos_xy[HexapodConst::kLegNum];
	designlab::Vector2 joint_pos_xy[HexapodConst::kLegNum];

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		joint_pos_xy[i] = GetLocalLegBasePosition(i).ProjectedXY();
		leg_pos_xy[i] = leg_pos[i].ProjectedXY() + joint_pos_xy[i];
	}

	//隣の脚との干渉を調べる．
	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		designlab::LineSegment2 line1(joint_pos_xy[i], leg_pos_xy[i]);
		designlab::LineSegment2 line2(joint_pos_xy[(i + 1) % HexapodConst::kLegNum], leg_pos_xy[(i + 1) % HexapodConst::kLegNum]);

		if (line1.HasIntersection(line2)) { return true; }
	}

	return false;
}


bool PhantomXStateCalclator::InitIsAbleLegPos(const int leg_index, const int x, const int y, const int z) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．

	float x_pos = (kLegPosMax - kLegPosMin) / kLegPosDivNum * x + kLegPosMin;
	float y_pos = (kLegPosMax - kLegPosMin) / kLegPosDivNum * y + kLegPosMin;
	float z_pos = (kLegPosMax - kLegPosMin) / kLegPosDivNum * z + kLegPosMin;

	designlab::Vector3 leg_pos{x_pos, y_pos, z_pos};		//脚先の位置


	//仮 x=0 y=0 の時はfalseにする
	if (abs(x_pos) <= (kLegPosMax - kLegPosMin) / (kLegPosDivNum - 1.0f) && abs(y_pos) <= (kLegPosMax - kLegPosMin) / (kLegPosDivNum - 1.0f)) { return false; }

	if (leg_pos.ProjectedXY().GetLength() < kMinLegR) { return false; }

	// 関節の角度を計算する
	HexapodJointState joint_state;

	CalculateLocalJointState(leg_index, leg_pos, &joint_state);
	
	// coxa関節の範囲内に存在しているかを確認する
	if (! PhantomXConst::IsVaildCoxaAngle(leg_index, joint_state.joint_angle[0])) { return false; }

	// femur関節の範囲内に存在しているかを確認する
	if (! PhantomXConst::IsVaildFemurAngle(joint_state.joint_angle[1])) { return false; }

	// tibia関節の範囲内に存在しているかを確認する
	if (! PhantomXConst::IsVaildTibiaAngle(joint_state.joint_angle[2])) { return false; }

	// リンクの長さを確認する
	if (!dlm::IsEqual((joint_state.local_joint_position[0] - joint_state.local_joint_position[1]).GetLength(), PhantomXConst::kCoxaLength)) 
	{
		return false; 
	}

	if (!dlm::IsEqual((joint_state.local_joint_position[1] - joint_state.local_joint_position[2]).GetLength(), PhantomXConst::kFemurLength)) 
	{
		return false; 
	}

	if (!dlm::IsEqual((joint_state.local_joint_position[2] - joint_state.local_joint_position[3]).GetLength(), PhantomXConst::kTibiaLength)) 
	{
		return false; 
	}

	return true;
}


void PhantomXStateCalclator::CalculateLocalJointState(const int leg_index, const designlab::Vector3& leg_pos, HexapodJointState* joint_state) const
{
	assert(0 <= leg_index && leg_index < HexapodConst::kLegNum);	//leg_indexは 0～5 である．


	//各パラメータを初期化する
	const int kJointNum = 4;
	const int kJointAngleNum = kJointNum - 1;

	(*joint_state).local_joint_position.clear();
	(*joint_state).joint_angle.clear();

	(*joint_state).local_joint_position.resize(kJointNum);
	(*joint_state).joint_angle.resize(kJointAngleNum);


	// coxa jointの計算
	(*joint_state).local_joint_position[0] = designlab::Vector3{ 0, 0, 0 };	//脚座標系ではcoxa jointは原点にある．

	// 脚先の追加
	(*joint_state).local_joint_position[3] = leg_pos;

	// coxa angleの計算
	{
		float coxa_joint_angle = std::atan2f(leg_pos.y, leg_pos.x);

		if (leg_pos.y == 0 && leg_pos.x == 0) { coxa_joint_angle = PhantomXConst::kCoxaDefaultAngle[leg_index]; }

		if (! PhantomXConst::IsVaildCoxaAngle(leg_index, coxa_joint_angle))
		{
			//範囲外ならば，180度回転させた時に範囲内にあるかを調べる．
			if (PhantomXConst::IsVaildCoxaAngle(leg_index, coxa_joint_angle + dlm::kFloatPi))
			{
				coxa_joint_angle += dlm::kFloatPi;
			}
			else if (PhantomXConst::IsVaildCoxaAngle(leg_index, coxa_joint_angle - dlm::kFloatPi))
			{
				coxa_joint_angle -= dlm::kFloatPi;
			}
		}

		(*joint_state).joint_angle[0] = coxa_joint_angle;
	}

	// femur jointの計算
	{
		const designlab::Vector3 femur_joint_pos = designlab::Vector3{
			PhantomXConst::kCoxaLength * std::cos((*joint_state).joint_angle[0]),
			PhantomXConst::kCoxaLength * std::sin((*joint_state).joint_angle[0]),
			0
		};

		(*joint_state).local_joint_position[1] = femur_joint_pos;

		// そもそも脚先が脚の付け根から届かない場合
		if (! dlm::CanMakeTriangle((leg_pos - femur_joint_pos).GetLength(), PhantomXConst::kFemurLength, PhantomXConst::kTibiaLength)) 
		{
			float angle_ft = std::atan2(leg_pos.z - femur_joint_pos.z, (leg_pos.ProjectedXY() - femur_joint_pos.ProjectedXY()).GetLength());
			float angle_ft_phase = angle_ft + dlm::kFloatPi;
			angle_ft_phase = angle_ft_phase > dlm::kFloatPi ? angle_ft_phase - dlm::kFloatPi : angle_ft_phase;

			designlab::Vector3 candidate_leg_pos = femur_joint_pos + designlab::Vector3{
				(PhantomXConst::kFemurLength + PhantomXConst::kTibiaLength) * std::cos((*joint_state).joint_angle[0]) * std::cos(angle_ft),
				(PhantomXConst::kFemurLength + PhantomXConst::kTibiaLength) * std::sin((*joint_state).joint_angle[0]) * std::cos(angle_ft) ,
				(PhantomXConst::kFemurLength + PhantomXConst::kTibiaLength) * std::sin(angle_ft)
			};

			designlab::Vector3 candidate_leg_pos_phase = femur_joint_pos + designlab::Vector3{
				(PhantomXConst::kFemurLength * std::cos(angle_ft_phase) + PhantomXConst::kTibiaLength * std::cos(angle_ft)) * std::cos((*joint_state).joint_angle[0]),
				(PhantomXConst::kFemurLength * std::cos(angle_ft_phase) + PhantomXConst::kTibiaLength * std::cos(angle_ft)) * std::sin((*joint_state).joint_angle[0]),
				PhantomXConst::kFemurLength * std::sin(angle_ft_phase) + PhantomXConst::kTibiaLength * std::sin(angle_ft)
			};

			float distance = (leg_pos - candidate_leg_pos).GetLength();
			float distance_phase = (leg_pos - candidate_leg_pos_phase).GetLength();

			float angle_f = 0,angle_t = 0;

			if (distance < distance_phase)
			{	
				angle_f = angle_ft;
				angle_t = 0;
			}
			else
			{
				angle_f = angle_ft_phase;
				angle_t = -dlm::kFloatPi;
			}

			(*joint_state).joint_angle[1] = angle_f;
			(*joint_state).joint_angle[2] = angle_t;

			(*joint_state).local_joint_position[2] = femur_joint_pos + designlab::Vector3{
				PhantomXConst::kFemurLength * std::cos(angle_f) * std::cos((*joint_state).joint_angle[0]),
				PhantomXConst::kFemurLength * std::cos(angle_f) * std::sin((*joint_state).joint_angle[0]),
				PhantomXConst::kFemurLength * std::sin(angle_f)
			};

			(*joint_state).local_joint_position[3] = (*joint_state).local_joint_position[2] + designlab::Vector3{
				PhantomXConst::kTibiaLength * std::cos(angle_f + angle_t) * std::cos((*joint_state).joint_angle[0]),
				PhantomXConst::kTibiaLength * std::cos(angle_f + angle_t) * std::sin((*joint_state).joint_angle[0]),
				PhantomXConst::kTibiaLength * std::sin(angle_f + angle_t)
			};

			(*joint_state).is_in_range = false;
			return; 
		}
	}


	// femur angle の計算
	const designlab::Vector3 femur_to_leg = leg_pos - (*joint_state).local_joint_position[1];		//脚先から第一関節までの長さ．
	const float femur_to_leg_x = femur_to_leg.ProjectedXY().GetLength() * 																//脚先へ向かう方向をxの正方向にする座標系に置き換える 			
	((leg_pos.ProjectedXY().GetSquaredLength() > (*joint_state).local_joint_position[1].ProjectedXY().GetSquaredLength()) ? 1.f : -1.f);	//脚先が第一関節よりも近い場合は正の方向にする．
	const float femur_to_leg_z = femur_to_leg.z;

	{
		const float arccos_upper = femur_to_leg.GetSquaredLength() + dlm::Squared(PhantomXConst::kFemurLength) - dlm::Squared(PhantomXConst::kTibiaLength);
		const float arccos_lower = 2 * PhantomXConst::kFemurLength * femur_to_leg.GetLength();
		const float arccos_arg = arccos_upper / arccos_lower;

		const float fumur_joint_angle = std::acos(arccos_arg) + std::atan2(femur_to_leg_z, femur_to_leg_x);
		//const float fumur_joint_angle = -std::acos(arccos_arg) + std::atan2(femur_to_leg_z, femur_to_leg_x);

		(*joint_state).joint_angle[1] = fumur_joint_angle;
	}

	// tibia jointの計算
	{
		const designlab::Vector3 femur_to_tibia = designlab::Vector3{
			PhantomXConst::kFemurLength * std::cos((*joint_state).joint_angle[0]) * std::cos((*joint_state).joint_angle[1]),
			PhantomXConst::kFemurLength * std::sin((*joint_state).joint_angle[0]) * std::cos((*joint_state).joint_angle[1]),
			PhantomXConst::kFemurLength * std::sin((*joint_state).joint_angle[1])
		};

		designlab::Vector3 tibia_joint_pos = (*joint_state).local_joint_position[1] + femur_to_tibia;

		(*joint_state).local_joint_position[2] = tibia_joint_pos;
	}


	// tibia angleの計算
	{
		float tibia_angle = std::atan2(
			(femur_to_leg_z - PhantomXConst::kFemurLength * std::sin((*joint_state).joint_angle[1])),
			(femur_to_leg_x - PhantomXConst::kFemurLength * std::cos((*joint_state).joint_angle[1]))
		) - (*joint_state).joint_angle[1];

		(*joint_state).joint_angle[2] = tibia_angle;
	}

	(*joint_state).is_in_range = true;
}