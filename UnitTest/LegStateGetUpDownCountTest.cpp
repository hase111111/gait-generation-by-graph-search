#pragma once
#include "pch.h"
#include "../DesignLab/LegState.h"

namespace LegStateEditTest
{
	TEST(GroundCheckFunc, GetLegUpDownCount)
	{
		//ãrà íuÇSÅCëSÇƒê⁄ínÇÃãrèÛë‘ÇçÏê¨
		ComType::EComPattern com_pattern = ComType::EComPattern::front;
		bool is_ground[HexapodConst::LEG_NUM] = { true,true,true,true,true,true };
		int leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };
		int leg_state = LegStateEdit::makeLegState(com_pattern, is_ground, leg_pos);

		bool is_ground2[HexapodConst::LEG_NUM] = { true,true,false,true,true,false };
		int leg_state2 = LegStateEdit::makeLegState(com_pattern, is_ground2, leg_pos);
		EXPECT_EQ(LegStateEdit::getLegUpDownCount(leg_state, leg_state2), 2);

		bool is_ground3[HexapodConst::LEG_NUM] = { true,true,false,true,false,false };
		int leg_state3 = LegStateEdit::makeLegState(com_pattern, is_ground3, leg_pos);
		EXPECT_EQ(LegStateEdit::getLegUpDownCount(leg_state, leg_state3), 3);

		bool is_ground4[HexapodConst::LEG_NUM] = { true,false,false,true,false,false };
		int leg_state4 = LegStateEdit::makeLegState(com_pattern, is_ground4, leg_pos);
		EXPECT_EQ(LegStateEdit::getLegUpDownCount(leg_state, leg_state4), 4);

		bool is_ground5[HexapodConst::LEG_NUM] = { false,false,false,true,true,true };
		bool is_ground6[HexapodConst::LEG_NUM] = { true,true,true,false,false,false };
		int leg_state5 = LegStateEdit::makeLegState(com_pattern, is_ground5, leg_pos);
		int leg_state6 = LegStateEdit::makeLegState(com_pattern, is_ground6, leg_pos);
		EXPECT_EQ(LegStateEdit::getLegUpDownCount(leg_state5, leg_state6), 6);
	}
}