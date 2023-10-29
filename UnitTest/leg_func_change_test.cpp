#include "pch.h"

#include "../DesignLab/leg_state.h"


namespace dllf = ::designlab::leg_func;


namespace
{
	//! @brief std::arrayを同じ値で埋める
	template<typename T, size_t S>
	std::array<T, S> MakeArraySameValue(const T& value)
	{
		std::array<T, S> res;
		res.fill(value);
		return res;
	}

	const DiscreteComPos first_com_pattern_ = DiscreteComPos::kFront;
	const std::array<bool, HexapodConst::kLegNum> first_is_ground_ = MakeArraySameValue<bool, HexapodConst::kLegNum>(true);
	const std::array<DiscreteLegPos, HexapodConst::kLegNum> first_leg_pos_ = MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter);

	const dllf::LegStateBit first_leg_state_ = dllf::MakeLegStateBit(first_com_pattern_, first_is_ground_, first_leg_pos_);
}


namespace designlab::test::node::leg_state
{
	TEST(LegFuncTest, ChangeLegStateTest)
	{
		// 変更するindex，変更後の脚の状態，変更後の接地状態，の順のタプル
		const std::vector<std::tuple<int, DiscreteLegPos, bool>> testcase_list{
			{ 0, DiscreteLegPos::kFront, false},
			{ 1, DiscreteLegPos::kBack , false},
			{ 2, DiscreteLegPos::kCenter, false},
			{ 3, DiscreteLegPos::kLowerBack, false},
			{ 4, DiscreteLegPos::kLowerFront, false},
			{ 5, DiscreteLegPos::kUpperBack, false},
			{ 0, DiscreteLegPos::kFront, true},
			{ 1, DiscreteLegPos::kBack , true},
			{ 2, DiscreteLegPos::kCenter, true},
			{ 3, DiscreteLegPos::kLowerBack, true},
			{ 4, DiscreteLegPos::kLowerFront, true},
			{ 5, DiscreteLegPos::kUpperBack, true},
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = first_leg_state_;

			//tupleから値を取り出す
			int index = std::get<0>(i);
			DiscreteLegPos leg_pos = std::get<1>(i);
			bool is_ground = std::get<2>(i);

			//脚の状態を変更する
			dllf::ChangeLegState(index, leg_pos, is_ground, &leg_state);

			//変更した脚の位置が正しいか確認する．冗長だが，ダブルチェックする．
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				if (j == index)
				{
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), leg_pos) << "離散化された脚位置は指定された値に変更される必要があります．";
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), is_ground) << "指定された値に変更される必要があります．";
				}
				else
				{
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), first_leg_pos_[j]) << "指定されていない値は変更されてはいけません．";
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), dllf::GetDiscreteLegPos(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), first_is_ground_[j]) << "指定されていない値は変更されてはいけません．";
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), dllf::IsGrounded(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
				}
			}

			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), first_com_pattern_) << "重心パターンは変更されてはいけません．";
			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), dllf::GetDiscreteComPos(first_leg_state_)) << "重心パターンは変更されてはいけません．";
		}
	}

	TEST(LegFuncTest, ChangeDiscreteLegPosTest)
	{
		// 変更するindex，変更後の脚の状態の順のタプル．
		const std::vector<std::tuple<int, DiscreteLegPos>> testcase_list{
			{ 0, DiscreteLegPos::kFront},
			{ 1, DiscreteLegPos::kBack },
			{ 2, DiscreteLegPos::kCenter},
			{ 3, DiscreteLegPos::kLowerBack},
			{ 4, DiscreteLegPos::kLowerFront},
			{ 5, DiscreteLegPos::kUpperBack},
			{ 2, DiscreteLegPos::kFront},
			{ 3, DiscreteLegPos::kBack },
			{ 4, DiscreteLegPos::kCenter},
			{ 5, DiscreteLegPos::kLowerBack},
			{ 0, DiscreteLegPos::kLowerFront},
			{ 1, DiscreteLegPos::kUpperBack},
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = first_leg_state_;

			//tupleから値を取り出す．
			int index = std::get<0>(i);
			DiscreteLegPos leg_pos = std::get<1>(i);

			//脚の状態を変更する．
			dllf::ChangeDiscreteLegPos(index, leg_pos, &leg_state);

			//変更した脚の位置が正しいか確認する．冗長だが，ダブルチェックする．
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				if (j == index)
				{
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), leg_pos) << "離散化された脚位置は指定された値に変更される必要があります．";
				}
				else
				{
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), first_leg_pos_[j]) << "指定されていない値は変更されてはいけません．";
					EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), dllf::GetDiscreteLegPos(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
				}

				EXPECT_EQ(dllf::IsGrounded(leg_state, j), first_is_ground_[j]) << "接地状態は変更されてはいけません．";
				EXPECT_EQ(dllf::IsGrounded(leg_state, j), dllf::IsGrounded(first_leg_state_, j)) << "接地状態は変更されてはいけません．";
			}

			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), first_com_pattern_) << "重心パターンは変更されてはいけません．";
			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), dllf::GetDiscreteComPos(first_leg_state_)) << "重心パターンは変更されてはいけません．";
		}
	}

	TEST(LegFuncTest, ChangeIsGroundTest)
	{
		// 変更するindex，変更後の接地状態の順のタプル
		const std::vector<std::tuple<int, bool>> testcase_list{
			{ 0, false},
			{ 1, false},
			{ 2, false},
			{ 3, false},
			{ 4, false},
			{ 5, false},
			{ 0, true},
			{ 1, true},
			{ 2, true},
			{ 3, true},
			{ 4, true},
			{ 5, true},
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = first_leg_state_;

			//tupleから値を取り出す
			int index = std::get<0>(i);
			bool is_ground = std::get<1>(i);

			//脚の状態を変更する
			dllf::ChangeGround(index, is_ground, &leg_state);

			//変更した脚の位置が正しいか確認する．冗長だが，ダブルチェックする．
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				if (j == index)
				{
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), is_ground) << "指定された値に変更される必要があります．";
				}
				else
				{
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), first_is_ground_[j]) << "指定されていない値は変更されてはいけません．";
					EXPECT_EQ(dllf::IsGrounded(leg_state, j), dllf::IsGrounded(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
				}

				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), first_leg_pos_[j]) << "指定されていない値は変更されてはいけません．";
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), dllf::GetDiscreteLegPos(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
			}

			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), first_com_pattern_) << "重心パターンは変更されてはいけません．";
			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), dllf::GetDiscreteComPos(first_leg_state_)) << "重心パターンは変更されてはいけません．";
		}
	}

	TEST(LegFuncTest, ChangeDiscreteComPosTest)
	{
		// 変更するindex，変更後の重心パターンの順のタプル
		const std::vector<DiscreteComPos> testcase_list{
			{ DiscreteComPos::kFront},
			{ DiscreteComPos::kBack },
			{ DiscreteComPos::kBackLeft},
			{ DiscreteComPos::kBackRight},
			{ DiscreteComPos::kCenterBack},
			{ DiscreteComPos::kCenterFront},
			{ DiscreteComPos::kFrontLeft},
			{ DiscreteComPos::kFrontRight},
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = first_leg_state_;

			//tupleから値を取り出す
			DiscreteComPos com_pos = i;

			//脚の状態を変更する
			dllf::ChangeDiscreteComPos(com_pos, &leg_state);

			//変更した脚の位置が正しいか確認する
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				// 冗長だが，ダブルチェック

				EXPECT_EQ(dllf::IsGrounded(leg_state, j), first_is_ground_[j]) << "接地状態は変更されてはいけません．";
				EXPECT_EQ(dllf::IsGrounded(leg_state, j), dllf::IsGrounded(first_leg_state_, j)) << "接地状態は変更されてはいけません．";
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), first_leg_pos_[j]) << "指定されていない値は変更されてはいけません．";
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), dllf::GetDiscreteLegPos(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
			}

			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), com_pos) << "指定された値に変更される必要があります．";
		}
	}

	TEST(LegFuncTest, ChangeAllLegGround)
	{
		std::vector<dllf::LegGroundedBit> testcase_list{
			0b111111,	0b011111,	0b101111,	0b110111,	0b111011,	0b111101,
			0b111110,	0b011110,	0b101110,	0b110110,	0b111010,	0b111100,
			0b111100,	0b011100,	0b101100,	0b110100,	0b111000,	0b111010,
		};

		for (const auto& i : testcase_list) 
		{
			dllf::LegStateBit leg_state = first_leg_state_;

			//脚の状態を変更する
			dllf::ChangeAllLegGround(i, &leg_state);

			//変更した脚の位置が正しいか確認する
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				// 冗長だが，ダブルチェック

				EXPECT_EQ(dllf::IsGrounded(leg_state, j), i[j]) << "指定された値に変更される必要があります．";
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), first_leg_pos_[j]) << "指定されていない値は変更されてはいけません．";
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), dllf::GetDiscreteLegPos(first_leg_state_, j)) << "指定されていない値は変更されてはいけません．";
			}

			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), first_com_pattern_) << "重心パターンは変更されてはいけません．";
			EXPECT_EQ(dllf::GetDiscreteComPos(leg_state), dllf::GetDiscreteComPos(first_leg_state_)) << "重心パターンは変更されてはいけません．";
		}
	}

}	// namespace designlab::test::node::leg_state