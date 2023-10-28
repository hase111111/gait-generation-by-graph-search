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

	//! @brief 接地・遊脚の状態を文字列に変換する
	std::string ToString(const std::array<bool, HexapodConst::kLegNum>& is_ground)
	{
		std::string res = "{ ";
		for (const auto& i : is_ground)
		{
			res += (i ? "接地" : "遊脚");
			res += " ";
		}
		res += "}";
		return res;
	}

	//! @brief 接地している脚の番号を文字列に変換する
	std::string ToString(const std::vector<int>& grounded_leg_index)
	{
		std::string res = "{ ";
		for (const auto& i : grounded_leg_index)
		{
			res += std::to_string(i);
			res += " ";
		}
		res += "}";
		return res;
	}


	// テスト用の定数．テストフィクスチャを使うべきだったのかも...?

	const DiscreteComPos kComPattern = DiscreteComPos::kFront;
	const auto kLegPos = MakeArraySameValue<DiscreteLegPos, HexapodConst::kLegNum>(DiscreteLegPos::kCenter);

	// 接地をtrue, 遊脚をfalseとしたときの脚の状態の配列と，そのときの接地している脚の番号の配列のタプル...の配列（笑）
	const std::vector< std::tuple<std::array<bool, HexapodConst::kLegNum>, std::vector<int>> > kTestcaseList{
		{ MakeArraySameValue<bool, HexapodConst::kLegNum>(true),	{ 0, 1, 2, 3, 4, 5 } },
		{ MakeArraySameValue<bool, HexapodConst::kLegNum>(false),	{} },
		{ { true, false, true, false, true, false },	{ 0, 2, 4 } },
		{ { false, true, false, true, false, true },	{ 1, 3, 5 } },
		{ { false, true, true, true, true, true},		{ 1, 2, 3, 4, 5 } },
		{ { true, false, true, true, true, true},		{ 0, 2, 3, 4, 5 } },
		{ { true, true, false, true, true, true},		{ 0, 1, 3, 4, 5 } },
		{ { true, true, true, false, true, true},		{ 0, 1, 2, 4, 5 } },
		{ { true, true, true, true, false, true},		{ 0, 1, 2, 3, 5 } },
		{ { true, true, true, true, true, false},		{ 0, 1, 2, 3, 4 } },
		{ { false, false, true, true, true, true},		{ 2, 3, 4, 5 } },
		{ { false, true, false, true, true, true},		{ 1, 3, 4, 5 } },
		{ { false, true, true, false, true, true},		{ 1, 2, 4, 5 } },
		{ { false, true, true, true, false, true},		{ 1, 2, 3, 5 } },
		{ { false, true, true, true, true, false},		{ 1, 2, 3, 4 } },
		{ { false, false, false, true, true, true},		{ 3, 4, 5 } },
		{ { false, false, true, false, true, true},		{ 2, 4, 5 } },
		{ { false, false, true, true, false, true},		{ 2, 3, 5 } },
		{ { false, false, true, true, true, false},		{ 2, 3, 4 } },
		{ { false, false, false, false, true, true},	{ 4, 5 } },
		{ { false, false, false, true, false, true},	{ 3, 5 } },
		{ { false, false, false, true, true, false},	{ 3, 4 } },
		{ { false, false, false, false, false, true},	{ 5 } },
		{ { false, false, false, false, true, false},	{ 4 } },
	};
}


namespace designlab::test::node::leg_state
{
	// MakeLegStateBitは正常に動作することはleg_func_test.cppで確認済みなので，ここでは正常に動くことを前提としてテストする

	TEST(LegFuncTest, GetLegGroundedBitTest)
	{
		for (const auto& i : kTestcaseList)
		{
			const auto& is_ground = std::get<0>(i);
			std::ignore = std::get<1>(i);

			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, is_ground, kLegPos);
			dllf::LegGroundedBit leg_ground_bit = dllf::GetLegGroundedBit(leg_state);

			std::string error_message = "元の値は " + ToString(is_ground) + " です\n" + 
				"出力された値は " + leg_ground_bit.to_string() + " です\n";

			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				EXPECT_EQ(leg_ground_bit[j], is_ground[j]) << error_message << "index : " << std::to_string(j) << "\n";
			}
		}
	}

	TEST(LegFuncTest, GetGroundedLegNumTest)
	{
		for (const auto& i : kTestcaseList)
		{
			const auto& is_ground = std::get<0>(i);
			const auto& grounded_leg_index = std::get<1>(i);

			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, is_ground, kLegPos);
			int grounded_leg_num = dllf::GetGroundedLegNum(leg_state);

			std::string error_message = "脚の状態は" + ToString(is_ground) + " です\n" +
				"接地脚の本数は正しくは" + std::to_string(grounded_leg_index.size()) + "です\n" +
				"実際には" + std::to_string(grounded_leg_num) + "です";

			EXPECT_EQ(grounded_leg_num, grounded_leg_index.size()) << error_message;
		}
	}

	TEST(LegFuncTest, GetLiftedLegNumTest)
	{
		for (const auto& i : kTestcaseList)
		{
			const auto& is_ground = std::get<0>(i);
			const auto& grounded_leg_index = std::get<1>(i);

			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, is_ground, kLegPos);
			int lifted_leg_num = dllf::GetLiftedLegNum(leg_state);

			std::string error_message = "脚の状態は" + ToString(is_ground) + " です\n" +
				"遊脚の本数は正しくは" + std::to_string(HexapodConst::kLegNum - grounded_leg_index.size()) + "です\n" +
				"実際には" + std::to_string(lifted_leg_num) + "です";

			EXPECT_EQ(lifted_leg_num, HexapodConst::kLegNum - grounded_leg_index.size()) << error_message;
		}
	}

	TEST(LegFuncTest, GetGroundedLegIndexByVectorTest)
	{
		for (const auto& i : kTestcaseList)
		{
			const auto& is_ground = std::get<0>(i);
			const auto& grounded_leg_index = std::get<1>(i);

			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, is_ground, kLegPos);
			std::vector<int> res_index;
			dllf::GetGroundedLegIndexByVector(leg_state, &res_index);

			std::string error_message = "脚の状態は" + ToString(is_ground) + " です\n" +
				"接地している脚の番号は正しくは" + ToString(grounded_leg_index) + "です\n" +
				"実際には" + ToString(res_index) + "です";

			EXPECT_EQ(res_index, grounded_leg_index) << error_message;
		}
	}

	TEST(LegFuncTest, GetLiftedLegIndexByVectorTest)
	{
		for (const auto& i : kTestcaseList)
		{
			const auto& is_ground = std::get<0>(i);
			const auto& grounded_leg_index = std::get<1>(i);

			dllf::LegStateBit leg_state = dllf::MakeLegStateBit(kComPattern, is_ground, kLegPos);
			std::vector<int> res_index;
			dllf::GetLiftedLegIndexByVector(leg_state, &res_index);

			std::vector<int> lifted_leg_index;
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				if (std::find(grounded_leg_index.begin(), grounded_leg_index.end(), j) == grounded_leg_index.end())
				{
					lifted_leg_index.push_back(j);
				}
			}

			std::string error_message = "脚の状態は" + ToString(is_ground) + " です\n" +
				"遊脚している脚の番号は正しくは" + ToString(lifted_leg_index) + "です\n" +
				"実際には" + ToString(res_index) + "です";

			EXPECT_EQ(res_index, lifted_leg_index) << error_message;
		}
	}
}
