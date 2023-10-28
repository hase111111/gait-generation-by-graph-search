#include "pch.h"

#include "../DesignLab/leg_state.h"
#include "../DesignLab/leg_state.cpp"


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
}


namespace designlab::test::node::leg_state
{
	TEST(LegFuncTest, MakeLegStateBitTestComPos)
	{
		// 重心タイプの状態のみを変更して，これらがうまく反映されているか確認する

		const auto leg_pos = MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter);
		const auto is_ground = MakeArraySameValue<bool, HexapodConst::kLegNum>(true);

		std::vector<DiscreteComPos> testcase_list{
			DiscreteComPos::kFront,
			DiscreteComPos::kFrontRight,
			DiscreteComPos::kBackRight,
			DiscreteComPos::kBack,
			DiscreteComPos::kBackLeft,
			DiscreteComPos::kFrontLeft,
			DiscreteComPos::kCenterFront,
			DiscreteComPos::kCenterBack
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(i, is_ground, leg_pos);
			auto com_pos = dllf::GetDiscreteComPos(leg_state);

			std::string error_message = " leg_state = " + leg_state.to_string() + ",\n com_pattern : 正解" + std::to_string(static_cast<int>(i)) +
				", 結果" + std::to_string(static_cast<int>(com_pos));

			EXPECT_EQ(com_pos, i) << error_message;
		}
	}

	TEST(LegFuncTest, MakeLegStateBitTestGroundState)
	{
		// 接地・遊脚の状態のみを変更して，これらがうまく反映されているか確認する

		const DiscreteComPos com_pattern = DiscreteComPos::kFront;
		const auto leg_pos = MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter);

		const std::vector<std::array<bool, HexapodConst::kLegNum>> testcase_list{
			MakeArraySameValue<bool, HexapodConst::kLegNum>(true),
			MakeArraySameValue<bool, HexapodConst::kLegNum>(false),
			{ true, false, true, false, true, false },
			{ false, true, false, true, false, true },
			{ true, true, true, true, true, true },
			{ false, false, false, false, false, false },
		};

		for (const auto &i : testcase_list)
		{
			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(com_pattern, i, leg_pos);

			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				std::string error_message = " leg_state = " + leg_state.to_string() + ",\n leg_index = " + std::to_string(j);

				if (i[j])
				{
					EXPECT_TRUE(dllf::IsGrounded(leg_state, j)) << error_message;
				}
				else
				{
					EXPECT_FALSE(dllf::IsGrounded(leg_state, j)) << error_message;
				}
			}
		}
	}

	TEST(LegFuncTest, MakeLegStateBitTestLegPos)
	{
		// 接地・遊脚の状態のみを変更して，これらがうまく反映されているか確認する

		const DiscreteComPos com_pattern = DiscreteComPos::kFront;
		const auto is_ground = MakeArraySameValue<bool, HexapodConst::kLegNum>(true);

		const std::vector<std::array<DiscreteLegPos, HexapodConst::kLegNum>> testcase_list = {
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kFront),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kBack),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kUpperFront),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kUpperBack),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kLowerFront),
			MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kLowerBack),
			{DiscreteLegPos::kLowerFront, DiscreteLegPos::kBack, DiscreteLegPos::kUpperFront, 
				DiscreteLegPos::kLowerFront, DiscreteLegPos::kFront, DiscreteLegPos::kCenter}
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(com_pattern, is_ground, i);

			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				std::string error_message = " leg_state = " + leg_state.to_string() + ",\n leg_index = " + std::to_string(j) + 
					", 正解" + std::to_string(static_cast<int>(i[j])) + ", 結果" + std::to_string(static_cast<int>(dllf::GetDiscreteLegPos(leg_state, j)));
			
				EXPECT_EQ(dllf::GetDiscreteLegPos(leg_state, j), i[j]) << error_message;
			}
		}
	}
}