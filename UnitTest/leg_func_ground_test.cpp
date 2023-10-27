#include "pch.h"

#include "../DesignLab/leg_state.h"


namespace dllf = ::designlab::leg_func;


namespace
{
	template<typename T, size_t S>
	std::array<T, S> MakeArraySameValue(const T& value)
	{
		std::array<T, S> res;
		res.fill(value);
		return res;
	}

	const DiscreteComPos kComPattern = DiscreteComPos::kFront;
	const auto kLegPos = MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter);
}


namespace designlab::test::node::leg_state
{
	TEST(LegFuncTest, GetLegGroundedBitTest)
	{
		const std::vector<std::array<bool, HexapodConst::kLegNum>> testcase_list{
			MakeArraySameValue<bool, HexapodConst::kLegNum>(true),
			MakeArraySameValue<bool, HexapodConst::kLegNum>(false),
			{ true, false, true, false, true, false },
			{ false, true, false, true, false, true },
			{ true, true, true, true, true, true },
			{ false, false, false, false, false, false },
		};

		for (const auto& i : testcase_list)
		{
			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, i, kLegPos);
			dllf::LegGroundedBit leg_ground = dllf::GetLegGroundedBit(leg_state);

			std::string error_message = "";

			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				EXPECT_EQ(leg_ground[j], i[j]) << error_message;
			}
		}
	}

}
