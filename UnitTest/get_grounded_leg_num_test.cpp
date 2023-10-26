#include "pch.h"
//
//#include "../DesignLab/leg_state.h"
//
//
//namespace dl_leg_test
//{
//	TEST(GroundCheck, GetGroundedLegNum)
//	{
//		// テストケース1 全脚接地
//		DiscreteComPos com = DiscreteComPos::kFront;
//		bool is_ground[HexapodConst::kLegNum] = { true, true, true, true, true, true };
//		DiscreteLegPos discretized_leg_pos[HexapodConst::kLegNum] = { DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
//																				DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter };
//		std::bitset<dl_leg::kLegStateBitNum> res = dl_leg::MakeLegStateBit(com, is_ground, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 6);
//
//
//		// テストケース2 一部脚接地 ( 3脚 )
//		bool is_ground2_1[HexapodConst::kLegNum] = { true, false, true, false, true, false };
//		res = dl_leg::MakeLegStateBit(com, is_ground2_1, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 3);
//
//		bool is_ground2_2[HexapodConst::kLegNum] = { false, true, false, true, false, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground2_2, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 3);
//
//		bool is_ground2_3[HexapodConst::kLegNum] = { false, false, true, true, false, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground2_3, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 3);
//
//
//		// テストケース3 全脚非接地
//		bool is_ground3[HexapodConst::kLegNum] = { false, false, false, false, false, false };
//		res = dl_leg::MakeLegStateBit(com, is_ground3, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 0);
//
//
//		// テストケース4 一部脚接地 ( 4脚 )
//		bool is_ground4_1[HexapodConst::kLegNum] = { false, true, true, true, false, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground4_1, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 4);
//
//		bool is_ground4_2[HexapodConst::kLegNum] = { true, false, true, true, true, false };
//		res = dl_leg::MakeLegStateBit(com, is_ground4_2, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 4);
//
//		bool is_ground4_3[HexapodConst::kLegNum] = { true, true, false, true, false, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground4_3, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 4);
//
//		bool is_ground4_4[HexapodConst::kLegNum] = { true, true, true, false, true, false };
//		res = dl_leg::MakeLegStateBit(com, is_ground4_4, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 4);
//
//
//		// テストケース5 一部脚接地 ( 5脚 )
//		bool is_ground5_1[HexapodConst::kLegNum] = { false, true, true, true, true, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground5_1, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 5);
//
//		bool is_ground5_2[HexapodConst::kLegNum] = { true, false, true, true, true, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground5_2, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 5);
//
//		bool is_ground5_3[HexapodConst::kLegNum] = { true, true, false, true, true, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground5_3, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 5);
//
//		bool is_ground5_4[HexapodConst::kLegNum] = { true, true, true, false, true, true };
//		res = dl_leg::MakeLegStateBit(com, is_ground5_4, discretized_leg_pos);
//		EXPECT_EQ(dl_leg::GetGroundedLegNum(res), 5);
//	}
//}