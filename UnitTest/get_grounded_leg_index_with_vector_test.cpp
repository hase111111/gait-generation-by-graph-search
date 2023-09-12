#include "pch.h"

#include "../DesignLab/leg_state.h"


namespace dl_leg_test
{
	class GroundCheckWithVector : public ::testing::Test
	{
	protected:
		void SetUp() override {}
		void TearDown() override {}

		//! @brief bool型の配列とvectorのindexが一致するか確認する
		void testBoolArrayAndVectorIndex(const bool bool_array[HexapodConst::LEG_NUM], const std::vector<int>& vec)
		{
			int cnt = 0;

			for (int i = 0; i < HexapodConst::LEG_NUM; i++)
			{
				if (bool_array[i]) { cnt++; }
			}

			ASSERT_EQ(cnt, vec.size());

			for (int i = 0; i < vec.size(); i++)
			{
				ASSERT_EQ(bool_array[vec[i]], true);
			}
		}
	};


	TEST_F(GroundCheckWithVector, GetGroundedLegIndexWithVector)
	{
		// テストケース1 全脚接地
		dl_com::EComPattern com = dl_com::EComPattern::FRONT;
		bool is_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		dl_leg::EDiscreteLegPos discretized_leg_pos[HexapodConst::LEG_NUM] = { dl_leg::EDiscreteLegPos::CENTER, dl_leg::EDiscreteLegPos::CENTER, dl_leg::EDiscreteLegPos::CENTER,
																				dl_leg::EDiscreteLegPos::CENTER, dl_leg::EDiscreteLegPos::CENTER, dl_leg::EDiscreteLegPos::CENTER };
		std::bitset<dl_leg::LEG_STATE_BIT_NUM> res = dl_leg::makeLegState(com, is_ground, discretized_leg_pos);
		std::vector<int> grounded_leg_index;
		dl_leg::getGroundedLegIndexWithVector(res, &grounded_leg_index);
		testBoolArrayAndVectorIndex(is_ground, grounded_leg_index);


		// テストケース2 5脚接地
		bool is_ground1_1[HexapodConst::LEG_NUM] = { true, true, true, true, true, false };
		res = dl_leg::makeLegState(com, is_ground1_1, discretized_leg_pos);
		dl_leg::getGroundedLegIndexWithVector(res, &grounded_leg_index);
		testBoolArrayAndVectorIndex(is_ground1_1, grounded_leg_index);

		bool is_ground1_2[HexapodConst::LEG_NUM] = { true, true, true, true, false, true };
		res = dl_leg::makeLegState(com, is_ground1_2, discretized_leg_pos);
		dl_leg::getGroundedLegIndexWithVector(res, &grounded_leg_index);
		testBoolArrayAndVectorIndex(is_ground1_2, grounded_leg_index);
	}
}