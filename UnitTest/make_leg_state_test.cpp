#include "pch.h"

#include "../DesignLab/leg_state.h"
#include "../DesignLab/leg_state.cpp"
#include "../DesignLab/discrete_leg_pos.h"
#include "../DesignLab/discrete_leg_pos.cpp"


// SCOPED_TRACEを書いておくと，失敗したときに，どのテストが失敗したかを表示してくれる．
// あまりきれいな実装ではないけど，google testについてのドキュメントが少なくてよくわからんのでこのまま


namespace dl_leg_test
{
	class MakeLegState : public ::testing::Test
	{
	protected:
		virtual void SetUp() {}
		virtual void TearDown() {}

		//! @brief bitの値が各ステータスの値と等しいか確認する
		void testLegState(const std::bitset<dl_leg::LEG_STATE_BIT_NUM>& leg_state, const DiscreteComPos& com_pattern, const bool is_ground[HexapodConst::LEG_NUM], const DiscreteLegPos leg_pos[HexapodConst::LEG_NUM])
		{
			EXPECT_EQ(dl_leg::getComPatternState(leg_state), com_pattern) << "bit : " + leg_state.to_string() + "\nfirst : "
				+ std::to_string(static_cast<int>(dl_leg::getComPatternState(leg_state))) + " second : " + std::to_string(static_cast<int>(com_pattern));

			for (int i = 0; i < HexapodConst::LEG_NUM; i++)
			{
				EXPECT_EQ(dl_leg::IsGrounded(leg_state, i), is_ground[i]) << "leg index : " + std::to_string(i) + "\nbit : " + leg_state.to_string() + "\nfirst : "
					+ std::to_string(dl_leg::IsGrounded(leg_state, i)) + " second:" + std::to_string(is_ground[i]);
				EXPECT_EQ(dl_leg::getLegState(leg_state, i), leg_pos[i]) << "leg index : " + std::to_string(i) + "\nbit : " + leg_state.to_string() + "\nfirst : "
					+ std::to_string(dl_leg::getLegState(leg_state, i)) + " second : " + std::to_string(leg_pos[i]);
			}
		}
	};


	TEST_F(MakeLegState, ChangeComPattren)
	{
		DiscreteComPos com_pattern = DiscreteComPos::kFront;
		bool is_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		DiscreteLegPos discretized_leg_pos[HexapodConst::LEG_NUM] = { DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
																				DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter };
		std::bitset<dl_leg::LEG_STATE_BIT_NUM> res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("first test");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}


		//重心パターンを変更して，重心パターンが変更されたか，また重心の値が変わらないこと，脚位置の値が変化しないことを確認
		com_pattern = DiscreteComPos::kBack;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 1");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kBackLeft;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 2");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kBackRight;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 3");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kFrontLeft;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 4");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kFrontRight;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 5");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kCenterBack;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 6");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}

		com_pattern = DiscreteComPos::kCenterFront;
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("test 7");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}
	}

	TEST_F(MakeLegState, ChangeGround)
	{
		// 様々な脚接地パターンを作成し， 脚接地パターンが変更されたか，また重心の値が変わらないこと，脚位置の値が変化しないことを確認
		DiscreteComPos com_pattern = DiscreteComPos::kFront;
		bool is_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		DiscreteLegPos discretized_leg_pos[HexapodConst::LEG_NUM] = { DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
																				DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter };
		std::bitset<dl_leg::LEG_STATE_BIT_NUM> res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos);
		{
			SCOPED_TRACE("first test");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos);
		}



		// 1脚 遊脚

		bool is_ground1[HexapodConst::LEG_NUM] = { false, true, true, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground1, discretized_leg_pos);
		{
			SCOPED_TRACE("test 1");
			testLegState(res, com_pattern, is_ground1, discretized_leg_pos);
		}


		bool is_ground2[HexapodConst::LEG_NUM] = { true, false, true, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground2, discretized_leg_pos);
		{
			SCOPED_TRACE("test 2");
			testLegState(res, com_pattern, is_ground2, discretized_leg_pos);
		}

		bool is_ground3[HexapodConst::LEG_NUM] = { true, true, false, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground3, discretized_leg_pos);
		{
			SCOPED_TRACE("test 3");
			testLegState(res, com_pattern, is_ground3, discretized_leg_pos);
		}

		bool is_ground4[HexapodConst::LEG_NUM] = { true, true, true, false, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground4, discretized_leg_pos);
		{
			SCOPED_TRACE("test 4");
			testLegState(res, com_pattern, is_ground4, discretized_leg_pos);
		}

		bool is_ground5[HexapodConst::LEG_NUM] = { true, true, true, true, false, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground5, discretized_leg_pos);
		{
			SCOPED_TRACE("test 5");
			testLegState(res, com_pattern, is_ground5, discretized_leg_pos);
		}

		bool is_ground6[HexapodConst::LEG_NUM] = { true, true, true, true, true, false };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground6, discretized_leg_pos);
		{
			SCOPED_TRACE("test 6");
			testLegState(res, com_pattern, is_ground6, discretized_leg_pos);
		}


		// 2脚 遊脚

		bool is_ground7[HexapodConst::LEG_NUM] = { false, false, true, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground7, discretized_leg_pos);
		{
			SCOPED_TRACE("test 7");
			testLegState(res, com_pattern, is_ground7, discretized_leg_pos);
		}

		bool is_ground8[HexapodConst::LEG_NUM] = { true, false, false, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground8, discretized_leg_pos);
		{
			SCOPED_TRACE("test 8");
			testLegState(res, com_pattern, is_ground8, discretized_leg_pos);
		}

		bool is_ground9[HexapodConst::LEG_NUM] = { true, true, false, false, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground9, discretized_leg_pos);
		{
			SCOPED_TRACE("test 9");
			testLegState(res, com_pattern, is_ground9, discretized_leg_pos);
		}

		bool is_ground10[HexapodConst::LEG_NUM] = { true, true, true, false, false, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground10, discretized_leg_pos);
		{
			SCOPED_TRACE("test 10");
			testLegState(res, com_pattern, is_ground10, discretized_leg_pos);
		}

		bool is_ground11[HexapodConst::LEG_NUM] = { true, true, true, true, false, false };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground11, discretized_leg_pos);
		{
			SCOPED_TRACE("test 11");
			testLegState(res, com_pattern, is_ground11, discretized_leg_pos);
		}

		bool is_ground12[HexapodConst::LEG_NUM] = { false, true, false, true, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground12, discretized_leg_pos);
		{
			SCOPED_TRACE("test 12");
			testLegState(res, com_pattern, is_ground12, discretized_leg_pos);
		}

		bool is_ground13[HexapodConst::LEG_NUM] = { true, false, true, false, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground13, discretized_leg_pos);
		{
			SCOPED_TRACE("test 13");
			testLegState(res, com_pattern, is_ground13, discretized_leg_pos);
		}

		bool is_ground14[HexapodConst::LEG_NUM] = { true, true, false, true, false, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground14, discretized_leg_pos);
		{
			SCOPED_TRACE("test 14");
			testLegState(res, com_pattern, is_ground14, discretized_leg_pos);
		}

		bool is_ground15[HexapodConst::LEG_NUM] = { true, true, true, false, true, false };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground15, discretized_leg_pos);
		{
			SCOPED_TRACE("test 15");
			testLegState(res, com_pattern, is_ground15, discretized_leg_pos);
		}

		bool is_ground16[HexapodConst::LEG_NUM] = { false, true, true, false, true, true };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground16, discretized_leg_pos);
		{
			SCOPED_TRACE("test 16");
			testLegState(res, com_pattern, is_ground16, discretized_leg_pos);
		}
	}

	TEST_F(MakeLegState, ChangeDiscreteLegPos)
	{
		//様々な脚位置パターンを作成する．
		DiscreteComPos com_pattern = DiscreteComPos::kFront;
		bool is_ground[HexapodConst::LEG_NUM] = { true, true, true, true, true, true };
		DiscreteLegPos discretized_leg_pos1[HexapodConst::LEG_NUM] = { DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
																				DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter };
		std::bitset<dl_leg::LEG_STATE_BIT_NUM> res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos1);
		{
			SCOPED_TRACE("test 1");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos1);
		}


		// 脚位置パターンを変更して， 脚位置パターンが変更されたか，また重心の値が変わらないこと，脚位置の値が変化しないことを確認
		DiscreteLegPos discretized_leg_pos2[HexapodConst::LEG_NUM] = { DiscreteLegPos::kBack, DiscreteLegPos::kCenter, DiscreteLegPos::kLowerBack,
																				DiscreteLegPos::kCenter, DiscreteLegPos::kUpperFront, DiscreteLegPos::kUpperBack };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos2);
		{
			SCOPED_TRACE("test 2");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos2);
		}


		DiscreteLegPos discretized_leg_pos3[HexapodConst::LEG_NUM] = { DiscreteLegPos::kLowerBack, DiscreteLegPos::kFront, DiscreteLegPos::kUpperBack,
																				DiscreteLegPos::kUpperBack, DiscreteLegPos::kLowerBack, DiscreteLegPos::kLowerBack };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos3);
		{
			SCOPED_TRACE("test 3");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos3);
		}


		DiscreteLegPos discretized_leg_pos4[HexapodConst::LEG_NUM] = { DiscreteLegPos::kUpperBack, DiscreteLegPos::kUpperBack, DiscreteLegPos::kUpperBack,
																						DiscreteLegPos::kUpperBack, DiscreteLegPos::kUpperBack, DiscreteLegPos::kUpperBack };
		res = dl_leg::MakeLegStateBit(com_pattern, is_ground, discretized_leg_pos4);
		{
			SCOPED_TRACE("test 4");
			testLegState(res, com_pattern, is_ground, discretized_leg_pos4);
		}
	}

}	//namespace dl_leg_test