#pragma once
#include "pch.h"
#include "../DesignLab/LegState.h"

namespace LegStateEditTest
{
	TEST(GroundCheckFunc, GetGroundedLegNum)
	{
		int _leg_state = 0;

		//全脚接地，結果は6
		ComType::EComPattern _com_pattern = ComType::EComPattern::front;
		bool _ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 6);

		//全脚離陸，結果は0
		bool _ground2[HexapodConst::LEG_NUM] = { false,false,false,false,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground2, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 0);

		//5脚接地，結果は5
		bool _ground3[HexapodConst::LEG_NUM] = { true,true,true,true,true,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground3, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 5);

		//1脚接地，結果は1
		bool _ground4[HexapodConst::LEG_NUM] = { false,false,false,false,false,true };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground4, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 1);

		//4脚接地，結果は4
		bool _ground5[HexapodConst::LEG_NUM] = { true,true,true,true,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground5, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 4);

		//2脚接地，結果は2
		bool _ground6[HexapodConst::LEG_NUM] = { false,false,true,true,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground6, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 2);

		//3脚接地，結果は3
		bool _ground7[HexapodConst::LEG_NUM] = { true,true,true,false,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground7, _leg_pos);

		EXPECT_EQ(LegStateEdit::getGroundedLegNum(_leg_state), 3);
	}

	TEST(GroundCheckFunc, GetLiftedLegNum)
	{
		int _leg_state = 0;

		//全脚接地，結果は0
		ComType::EComPattern _com_pattern = ComType::EComPattern::front;
		bool _ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 0);

		//全脚離陸，結果は6
		bool _ground2[HexapodConst::LEG_NUM] = { false,false,false,false,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground2, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 6);

		//5脚接地，結果は1
		bool _ground3[HexapodConst::LEG_NUM] = { true,true,true,true,true,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground3, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 1);

		//1脚接地，結果は5
		bool _ground4[HexapodConst::LEG_NUM] = { false,false,false,false,false,true };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground4, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 5);

		//4脚接地，結果は2
		bool _ground5[HexapodConst::LEG_NUM] = { true,true,true,true,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground5, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 2);

		//2脚接地，結果は4
		bool _ground6[HexapodConst::LEG_NUM] = { false,false,true,true,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground6, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 4);

		//3脚接地，結果は3
		bool _ground7[HexapodConst::LEG_NUM] = { true,true,true,false,false,false };
		_leg_state = LegStateEdit::makeLegState(_com_pattern, _ground7, _leg_pos);

		EXPECT_EQ(LegStateEdit::getLiftedLegNum(_leg_state), 3);
	}
}
