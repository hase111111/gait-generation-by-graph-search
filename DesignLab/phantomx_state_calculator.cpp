#include "phantomx_state_calculator.h"

#include <cmath>


void PhantomXStateCalclator::init()
{
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


bool PhantomXStateCalclator::calculateAllJointState(const SNode& node, SHexapodJointState* const joint_state) const
{
	return false;
}


dl_vec::SVector PhantomXStateCalclator::getLocalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos) const
{
	return dl_vec::SVector();
}


dl_vec::SVector PhantomXStateCalclator::getGlobalLegBasePosition(const int leg_index, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const
{
	return dl_vec::SVector();
}


dl_vec::SVector PhantomXStateCalclator::getGlobalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const
{
	return dl_vec::SVector();
}


bool PhantomXStateCalclator::isLegInRange(const int leg_index, const dl_vec::SVector& leg_pos) const
{
	return m_is_able_leg_pos[leg_index][getLegPosIndex(leg_pos.x)][getLegPosIndex(leg_pos.y)][getLegPosIndex(leg_pos.z)];
}


bool PhantomXStateCalclator::isLegInterfering(const dl_vec::SVector leg_pos[HexapodConst::LEG_NUM]) const
{
	return false;
}


bool PhantomXStateCalclator::initIsAbleLegPos(const int leg_index, const int x, const int y, const int z) const
{
	float x_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * x + LEG_POS_MIN;
	float y_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * y + LEG_POS_MIN;
	float z_pos = (LEG_POS_MAX - LEG_POS_MIN) / LEG_POS_DIV_NUM * z + LEG_POS_MIN;

	dl_vec::SVector leg_pos{x_pos, y_pos, z_pos};		//脚先の位置


	// coxa関節の範囲内に存在しているかを確認する
	float coxa_angle = 0;

	if (x_pos == 0 && y_pos == 0)
	{
		coxa_angle = HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index];
	}
	else
	{
		coxa_angle = std::atan2f(y_pos, x_pos);
	}

	if (HexapodConst::PHANTOMX_COXA_ANGLE_MIN + HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index] > coxa_angle)
	{
		return false;
	}
	if (HexapodConst::PHANTOMX_COXA_ANGLE_MAX + HexapodConst::PHANTOMX_COXA_DEFAULT_ANGLE[leg_index] < coxa_angle)
	{
		return false;
	}


	//三角形の辺の長さの関係から，有効な距離かを確認する
	dl_vec::SVector coxa_joint_pos = dl_vec::SVector{
		std::cosf(coxa_angle) * HexapodConst::PHANTOMX_COXA_LENGTH,	std::sinf(coxa_angle) * HexapodConst::PHANTOMX_COXA_LENGTH, 0
	};

	const float a = HexapodConst::PHANTOMX_FEMUR_LENGTH;
	const float b = HexapodConst::PHANTOMX_TIBIA_LENGTH;
	const float c = (leg_pos - coxa_joint_pos).length();

	if (a + b < c) { return false; }
	if (b + c < a) { return false; }
	if (c + a < b) { return false; }


	return true;
}
