#pragma onne
#include "pch.h"
#include "../DesignLab/LegState.h"
#include "../DesignLab/LegState.cpp"

namespace LegStateEditTest
{
	TEST(GroundCheckFunc, GetGroundedLegNumWithVector)
	{
		// テストケース1 全脚接地
		ComType::EComPattern _com = ComType::EComPattern::front;
		bool _leg_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };

		int leg_state = LegStateEdit::makeLegState(_com, _leg_ground, _leg_pos);

		std::vector<int> _res;
		LegStateEdit::getGroundedLegNumWithVector(leg_state, _res);

		EXPECT_EQ(_res.size(), 6);

		// テストケース2 一部脚接地 ( 3脚 )
		bool _leg_ground2[HexapodConst::LEG_NUM] = { true, false, true, false, true, false };

		int leg_state2 = LegStateEdit::makeLegState(_com, _leg_ground2, _leg_pos);

		std::vector<int> _res2;
		LegStateEdit::getGroundedLegNumWithVector(leg_state2, _res2);

		EXPECT_EQ(_res2.size(), 3);

		std::map<int, bool> _res2_map;
		for (auto& i : _res2) { _res2_map[i] = true; }
		EXPECT_EQ(_res2_map.count(0), 1);
		EXPECT_EQ(_res2_map.count(2), 1);
		EXPECT_EQ(_res2_map.count(4), 1);

		// テストケース3 全脚非接地
		bool _leg_ground3[HexapodConst::LEG_NUM] = { false, false, false, false, false, false };
		int _leg_pos3[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };

		int leg_state3 = LegStateEdit::makeLegState(_com, _leg_ground3, _leg_pos);

		std::vector<int> _res3;
		LegStateEdit::getGroundedLegNumWithVector(leg_state3, _res3);

		EXPECT_EQ(_res3.size(), 0);

		// テストケース4 一部脚接地 ( 4脚 )
		bool _leg_ground4[HexapodConst::LEG_NUM] = { false, true, true, true, false, true };

		int leg_state4 = LegStateEdit::makeLegState(_com, _leg_ground4, _leg_pos);

		std::vector<int> _res4;
		LegStateEdit::getGroundedLegNumWithVector(leg_state4, _res4);

		EXPECT_EQ(_res4.size(), 4);

		std::map<int, bool> _res4_map;
		for (auto& i : _res4) { _res4_map[i] = true; }
		EXPECT_EQ(_res4_map.count(1), 1);
		EXPECT_EQ(_res4_map.count(2), 1);
		EXPECT_EQ(_res4_map.count(3), 1);
		EXPECT_EQ(_res4_map.count(5), 1);


		// テストケース5 一部脚接地 ( 5脚 )
		bool _leg_ground5[HexapodConst::LEG_NUM] = { false, true, true, true, true, true };

		int leg_state5 = LegStateEdit::makeLegState(_com, _leg_ground5, _leg_pos);

		std::vector<int> _res5;
		LegStateEdit::getGroundedLegNumWithVector(leg_state5, _res5);

		EXPECT_EQ(_res5.size(), 5);

		std::map<int, bool> _res5_map;
		for (auto& i : _res5) { _res5_map[i] = true; }
		EXPECT_EQ(_res5_map.count(1), 1);
		EXPECT_EQ(_res5_map.count(2), 1);
		EXPECT_EQ(_res5_map.count(3), 1);
		EXPECT_EQ(_res5_map.count(4), 1);
		EXPECT_EQ(_res5_map.count(5), 1);
	}

	TEST(GroundCheckFunc, GetLiftedLegNumWithVector)
	{
		// テストケース1 全脚接地
		ComType::EComPattern _com = ComType::EComPattern::front;
		bool _leg_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		int _leg_pos[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };

		int leg_state = LegStateEdit::makeLegState(_com, _leg_ground, _leg_pos);

		std::vector<int> _res;
		LegStateEdit::getLiftedLegNumWithVector(leg_state, _res);

		EXPECT_EQ(_res.size(), 0);

		// テストケース2 一部脚接地 ( 3脚 )
		bool _leg_ground2[HexapodConst::LEG_NUM] = { true, false, true, false, true, false };

		int leg_state2 = LegStateEdit::makeLegState(_com, _leg_ground2, _leg_pos);

		std::vector<int> _res2;
		LegStateEdit::getLiftedLegNumWithVector(leg_state2, _res2);

		EXPECT_EQ(_res2.size(), 3);

		std::map<int, bool> _res2_map;
		for (auto& i : _res2) { _res2_map[i] = true; }
		EXPECT_EQ(_res2_map.count(1), 1);
		EXPECT_EQ(_res2_map.count(3), 1);
		EXPECT_EQ(_res2_map.count(5), 1);


		// テストケース3 全脚非接地
		bool _leg_ground3[HexapodConst::LEG_NUM] = { false, false, false, false, false, false };
		int _leg_pos3[HexapodConst::LEG_NUM] = { 4,4,4,4,4,4 };

		int leg_state3 = LegStateEdit::makeLegState(_com, _leg_ground3, _leg_pos);

		std::vector<int> _res3;
		LegStateEdit::getLiftedLegNumWithVector(leg_state3, _res3);

		EXPECT_EQ(_res3.size(), 6);


		// テストケース4 一部脚接地 ( 4脚 )
		bool _leg_ground4[HexapodConst::LEG_NUM] = { false, true, true, true, false, true };

		int leg_state4 = LegStateEdit::makeLegState(_com, _leg_ground4, _leg_pos);

		std::vector<int> _res4;
		LegStateEdit::getLiftedLegNumWithVector(leg_state4, _res4);

		EXPECT_EQ(_res4.size(), 2);

		std::map<int, bool> _res4_map;
		for (auto& i : _res4) { _res4_map[i] = true; }
		EXPECT_EQ(_res4_map.count(0), 1);
		EXPECT_EQ(_res4_map.count(4), 1);

		// テストケース5 一部脚接地 ( 5脚 )
		bool _leg_ground5[HexapodConst::LEG_NUM] = { false, true, true, true, true, true };

		int leg_state5 = LegStateEdit::makeLegState(_com, _leg_ground5, _leg_pos);

		std::vector<int> _res5;
		LegStateEdit::getLiftedLegNumWithVector(leg_state5, _res5);

		EXPECT_EQ(_res5.size(), 1);

		std::map<int, bool> _res5_map;
		for (auto& i : _res5) { _res5_map[i] = true; }
		EXPECT_EQ(_res5_map.count(0), 1);
	}
}