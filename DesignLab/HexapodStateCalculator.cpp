#include "HexapodStateCalculator.h"

using namespace myvector;

myvector::SVector HexapodStateCalclator::getGlobalLegPos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num) + _node.Leg[_leg_num],_node.rot) + _node.global_center_of_mass;
}

myvector::SVector HexapodStateCalclator::getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const
{
	return rotVector(getLocalCoxaJointPos(_leg_num), _node.rot) + _node.global_center_of_mass;
}

myvector::SVector HexapodStateCalclator::getLocalCoxaJointPos(const int _leg_num) const
{
	//重心を原点とした座標．ロボットの正面をxの正，ロボットの上をzの正，右手座標系でy座標をとっている．
	//グローバル座標系のxyz軸とは別の軸なので，回転は考慮されていない．

	if (_leg_num == 0)		{ return myvector::VGet(HexapodConst::BODY_FRONT_LENGTH,	-HexapodConst::BODY_FRONT_WIDTH,	0.0f); }	// 脚0 右上
	else if (_leg_num == 1) { return myvector::VGet(0.0f,								-HexapodConst::BODY_CENTER_WIDTH,	0.0f); }	// 脚1 右横
	else if (_leg_num == 2)	{ return myvector::VGet(-HexapodConst::BODY_REAR_LENGTH,	-HexapodConst::BODY_REAR_WIDTH,		0.0f); }	// 脚2 右下
	else if (_leg_num == 3)	{ return myvector::VGet(-HexapodConst::BODY_REAR_LENGTH,	HexapodConst::BODY_REAR_WIDTH,		0.0f); }	// 脚3 左下
	else if (_leg_num == 4)	{ return myvector::VGet(0.0f,								HexapodConst::BODY_CENTER_WIDTH,	0.0f); }	// 脚4 左横
	else if (_leg_num == 5)	{ return myvector::VGet(HexapodConst::BODY_FRONT_LENGTH,	HexapodConst::BODY_FRONT_WIDTH,		0.0f); }	// 脚5 左上

	return myvector::SVector();
}
