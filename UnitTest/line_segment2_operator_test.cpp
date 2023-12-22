#include "pch.h"

#include "../DesignLab/math_line_segment2.h"


namespace dl = ::designlab;


namespace
{
	//! @brief 線分を作成する
	dl::LineSegment2 MakeLineSegment2(const dl::Vector2& start, const dl::Vector2& end)
	{
		dl::LineSegment2 res;
		res.start = start;
		res.end = end;
		return res;
	}
}


namespace designlab::test::common::math
{
	TEST(LineSegment2Test, AssignmentOperatorTest)
	{
		dl::LineSegment2 line = MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4));
		dl::LineSegment2 line2;
		line2 = line;
		EXPECT_EQ(line2.start, dl::Vector2(1, 2));
		EXPECT_EQ(line2.end, dl::Vector2(3, 4));
	}

	TEST(LineSegment2Test, EqualityOperatorTest)
	{
		const std::vector<std::pair<dl::LineSegment2, dl::LineSegment2>> kTestcaseListTrueCase{
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)),		MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)) },
			{ MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)),		MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)) },
			{ MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)),	MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)) },
		};

		for (const auto& i : kTestcaseListTrueCase)
		{
			EXPECT_TRUE(i.first == i.second) << "始点と終点がそれぞれ一致しているならば等しい．順不同でないので注意．";
		}

		const std::vector<std::pair<dl::LineSegment2, dl::LineSegment2>> kTestcaseListFalseCase{
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)),		MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 5)) },
			{ MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)),		MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(1, 0)) },
			{ MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)),	MakeLineSegment2(dl::Vector2(7, 4), dl::Vector2(102, 11)) },
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)),		MakeLineSegment2(dl::Vector2(3, 4), dl::Vector2(1, 2)) },
		};

		for (const auto& i : kTestcaseListFalseCase)
		{
			EXPECT_FALSE(i.first == i.second) << "始点と終点がそれぞれ一致していないならば等しくない．順不同でないので注意．";
		}
	}

	TEST(LineSegment2Test, InequalityOperatorTest)
	{
		const std::vector<std::pair<dl::LineSegment2, dl::LineSegment2>> kTestcaseListTrueCase{
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)), MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 5)) },
			{ MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)),		MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(1, 0)) },
			{ MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)),	MakeLineSegment2(dl::Vector2(7, 4), dl::Vector2(102, 11)) },
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)),		MakeLineSegment2(dl::Vector2(3, 4), dl::Vector2(1, 2)) },
		};

		for (const auto& i : kTestcaseListTrueCase)
		{
			EXPECT_TRUE(i.first != i.second) << "始点と終点がそれぞれ一致していないならば等しくない．順不同でないので注意．";
		}

		const std::vector<std::pair<dl::LineSegment2, dl::LineSegment2>> kTestcaseListFalseCase{
			{ MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)), MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4)) },
			{ MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)),		MakeLineSegment2(dl::Vector2(0, 0), dl::Vector2(0, 0)) },
			{ MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)),	MakeLineSegment2(dl::Vector2(10, 2), dl::Vector2(3, -45)) },
		};

		for (const auto& i : kTestcaseListFalseCase)
		{
			EXPECT_FALSE(i.first != i.second) << "始点と終点がそれぞれ一致しているならば等しい．順不同でないので注意．";
		}
	}
}