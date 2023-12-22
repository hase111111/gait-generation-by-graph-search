#include "pch.h"

#include "../DesignLab/math_vector2.h"


namespace dl = ::designlab;


namespace
{
	// テスト用のヘルパー関数

	dl::Vector2 MakeVec2(const float x, const float y)
	{
		dl::Vector2 vec;
		vec.x = x;
		vec.y = y;

		return vec;
	}

	std::string ToString(const dl::Vector2& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ")";
	}
}


namespace designlab::test::common::math
{
	TEST(Vector2Test, PlusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), MakeVec2(3, 4), MakeVec2(4, 6)},
			{ MakeVec2(0.4f, 0.9f), MakeVec2(11.7f, 8.3f), MakeVec2(12.1f, 9.2f)},
			{ MakeVec2(3, 7), MakeVec2(10, 42), MakeVec2(13, 49)},
			{ MakeVec2(10, 42), MakeVec2(3, 7), MakeVec2(13, 49)},
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector2& vec1 = std::get<0>(i);
			const dl::Vector2& vec2 = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" + 
				" " + ToString(vec2) + "の和は\n 正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 + vec2, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, CompoundAssignmentPlusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), MakeVec2(3, 4), MakeVec2(4, 6)},
			{ MakeVec2(0.4f, 0.9f), MakeVec2(11.7f, 8.3f), MakeVec2(12.1f, 9.2f) },
			{ MakeVec2(3, 7), MakeVec2(10, 42), MakeVec2(13, 49) },
			{ MakeVec2(10, 42), MakeVec2(3, 7), MakeVec2(13, 49) },
		};

		for (const auto& i : testcase_list)
		{
			dl::Vector2 vec1 = std::get<0>(i);
			const dl::Vector2& vec2 = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + ToString(vec2) + "の和は\n 正しくは" + ToString(expected) + "である．";

			vec1 += vec2;

			EXPECT_EQ(vec1, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, MinusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), MakeVec2(3, 4), MakeVec2(-2, -2)},
			{ MakeVec2(0.4f, 0.9f), MakeVec2(11.7f, 8.3f), MakeVec2(-11.3f, -7.4f) },
			{ MakeVec2(3, 7), MakeVec2(10, 42), MakeVec2(-7, -35) },
			{ MakeVec2(10, 42), MakeVec2(3, 7), MakeVec2(7, 35) },
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector2& vec1 = std::get<0>(i);
			const dl::Vector2& vec2 = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + ToString(vec2) + "の差は\n 正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 - vec2, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, CompoundAssignmentMinusOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, dl::Vector2, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), MakeVec2(3, 4), MakeVec2(-2, -2)},
			{ MakeVec2(0.4f, 0.9f), MakeVec2(11.7f, 8.3f), MakeVec2(-11.3f, -7.4f) },
			{ MakeVec2(3, 7), MakeVec2(10, 42), MakeVec2(-7, -35) },
			{ MakeVec2(10, 42), MakeVec2(3, 7), MakeVec2(7, 35) },
		};

		for (const auto& i : testcase_list)
		{
			dl::Vector2 vec1 = std::get<0>(i);
			const dl::Vector2& vec2 = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + ToString(vec2) + "の差は\n 正しくは" + ToString(expected) + "である．";

			vec1 -= vec2;

			EXPECT_EQ(vec1, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, MultipleOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, float, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), 3.f, MakeVec2(3, 6)},
			{ MakeVec2(0.4f, 0.9f), 10.f, MakeVec2(4.f, 9.f) },
			{ MakeVec2(3, 7), 0.5f, MakeVec2(1.5f, 3.5f) },
			{ MakeVec2(10, 42), -2.f, MakeVec2(-20.f, -84.f) },
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector2& vec1 = std::get<0>(i);
			const float s = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + std::to_string(s) + "の積は\n 正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 * s, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, MultiplyOperatorTestScalarFirst)
	{
		// vec, scalar, expected の順番のタプルを渡す
		std::vector<std::tuple<dl::Vector2, float, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), 3.f, MakeVec2(3, 6)},
			{ MakeVec2(0.4f, 0.9f), 10.f, MakeVec2(4.f, 9.f) },
			{ MakeVec2(3, 7), 0.5f, MakeVec2(1.5f, 3.5f) },
			{ MakeVec2(10, 42), -2.f, MakeVec2(-20.f, -84.f) },
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector2& vec1 = std::get<0>(i);
			const float s = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + std::to_string(s) + "と\n" +
				" " + ToString(vec1) + "の積は\n 正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(s * vec1, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, CompoundAssignmentMultipleOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, float, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), 3.f, MakeVec2(3, 6)},
			{ MakeVec2(0.4f, 0.9f), 10.f, MakeVec2(4.f, 9.f) },
			{ MakeVec2(3, 7), 0.5f, MakeVec2(1.5f, 3.5f) },
			{ MakeVec2(10, 42), -2.f, MakeVec2(-20.f, -84.f) },
		};

		for (const auto& i : testcase_list)
		{
			dl::Vector2 vec1 = std::get<0>(i);
			const float s = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + std::to_string(s) + "の積は\n 正しくは" + ToString(expected) + "である．";

			vec1 *= s;

			EXPECT_EQ(vec1, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, DivisionOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, float, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), 2.f, MakeVec2(0.5f, 1.f) },
			{ MakeVec2(0.4f, 0.9f), 10.f, MakeVec2(0.04f, 0.09f) },
			{ MakeVec2(3, 7), 0.5f, MakeVec2(6.f, 14.f) },
			{ MakeVec2(10, 42), -2.f, MakeVec2(-5.f, -21.f) },
		};

		for (const auto& i : testcase_list)
		{
			const dl::Vector2& vec1 = std::get<0>(i);
			const float s = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + std::to_string(s) + "の商は\n 正しくは" + ToString(expected) + "である．";
			EXPECT_EQ(vec1 / s, expected) << error_case_message;
		}
	}

	TEST(Vector2Test, CompoundAssignmentDivisionOperatorTest)
	{
		std::vector<std::tuple<dl::Vector2, float, dl::Vector2>> testcase_list{
			{ MakeVec2(1, 2), 2.f, MakeVec2(0.5f, 1.f) },
			{ MakeVec2(0.4f, 0.9f), 10.f, MakeVec2(0.04f, 0.09f) },
			{ MakeVec2(3, 7), 0.5f, MakeVec2(6.f, 14.f) },
			{ MakeVec2(10, 42), -2.f, MakeVec2(-5.f, -21.f) },
		};

		for (const auto& i : testcase_list)
		{
			dl::Vector2 vec1 = std::get<0>(i);
			const float s = std::get<1>(i);
			const dl::Vector2& expected = std::get<2>(i);

			std::string error_case_message = " " + ToString(vec1) + "と\n" +
				" " + std::to_string(s) + "の商は\n 正しくは" + ToString(expected) + "である．";

			vec1 /= s;

			EXPECT_EQ(vec1, expected) << error_case_message;
		}
	}

}	// namespace designlab::test::common::math