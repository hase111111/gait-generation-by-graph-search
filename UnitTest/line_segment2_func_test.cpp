#include "pch.h"

#include "../DesignLab/designlab_line_segment2.h"


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
	TEST(LineSegment2Test, GetLengthTest)
	{
		const std::vector<std::tuple<dl::LineSegment2, float>> testcase_list{
			{ MakeLineSegment2({ 0.f, 0.f }, { 1.f, 1.f }),			sqrt(2.0f) },
			{ MakeLineSegment2({ 0.f, -3.5f }, { 1.f, -3.5f}),		1.0f },
			{ MakeLineSegment2({ -4.f, 10.f }, { -4.f, 11.f}),		1.0f },
			{ MakeLineSegment2({ 0.f, 0.f }, { 0.f, 0.f}),			0.0f },
			{ MakeLineSegment2({ 100.f, -30.f }, { 100.f, -30.f}),	0.0f },
			{ MakeLineSegment2({ 0.f, 0.f }, { 1.f, 2.f}),			sqrt(5.0f) },
			{ MakeLineSegment2({ 1.f, 2.f }, { 0.f, 0.f}),			sqrt(5.0f) },
			{ MakeLineSegment2({ 2.f, 1.f }, { 0.f, 0.f}),			sqrt(5.0f) },
			{ MakeLineSegment2({ 0.f, 0.f }, { 2.f, 2.f}),			sqrt(8.0f) },
		};

		for (const auto& i : testcase_list)
		{
			const auto& line = std::get<0>(i);
			const auto& expected = std::get<1>(i);

			std::string error_message = " start = " + line.start.ToString() + "\n end   = " + line.end.ToString() + "\n";

			EXPECT_FLOAT_EQ(line.GetLength(), expected) << error_message;
		}
	}

	TEST(LineSegment2Test, IsParallelTest)
	{
		const std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2, bool>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, {1,1}),			MakeLineSegment2({ 0, 1 }, { 1, 2 }),					true},
			{ MakeLineSegment2({ 10, 0 }, { 0, 0 }),		MakeLineSegment2({ 0, 0 }, { -400, 0 }),				true},
			{ MakeLineSegment2({ 35, 70 }, { 35, -200 }),	MakeLineSegment2({ -14.7f, -300 }, { -14.7f, 2000 }),	true},
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 0, 1 }, { 1, 0 }),					false},
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 0 }, { 0, 1 }),					false},
			{ MakeLineSegment2({ 1,1 }, { 0,0 }),			MakeLineSegment2({ 0,1 }, { 1,0 }),						false},
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ -5,-6 }, { -3,-1 }),					false},
			{ MakeLineSegment2({ 10, 10 }, { 99, 99 }),		MakeLineSegment2({ 1, 1 }, { 2, 3 }),					false},
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);
			const auto& expected = std::get<2>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n";

			if (expected) 
			{
				EXPECT_TRUE(line1.IsParallel(line2)) << error_message << " 正しくは平行となる．";
			}
			else 
			{
				EXPECT_FALSE(line1.IsParallel(line2)) << error_message << " 正しくは平行でない．";
			}
		}
	}

	TEST(LineSegment2Test, GetIntersectionTestCaseOfHasIntersection)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2, dl::Vector2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 0, 1 }, { 1, 0 }),		dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 0 }, { 0, 1 }),		dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ 1,1 }, { 0,0 }),			MakeLineSegment2({ 0,1 }, { 1,0 }),			dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 1 }, { 2, 3 }),		dl::Vector2(1, 1) },		//端点一致
			{ MakeLineSegment2({ -8, -14 }, { 5, 25 }),		MakeLineSegment2({ -7, 4 }, { 1, -12 }),	dl::Vector2(-4, -2) },
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);
			const auto& expected = std::get<2>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n" + 
				" 交点は" + expected.ToString() + "\n";

			EXPECT_EQ(line1.GetIntersection(line2), expected) << error_message;
		}
	}

	TEST(LineSegment2Test, GetIntersectionTestCaseOfHasNotIntersection)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ -5,-6 }, { -3,-1 }), },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ 1.5f, 1.5f }, { 2, 3 }), },
			{ MakeLineSegment2({ -1, 7 }, { 5, 25 }),	MakeLineSegment2({ -7, 4 }, { 1, -12 }), },
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n" +
				" 交点はないので(0,0)を返す\n";

			EXPECT_EQ(line1.GetIntersection(line2), dl::Vector2(0, 0)) << error_message;
		}
	}

	TEST(LineSegment2Test, GetIntersectionTestCaseOfParallelLine)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ 0, 1 }, { 1, 2 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ -6,-6 }, { -3,-3 }) },
			{ MakeLineSegment2({ -7, -7 }, { 10, 10 }),	MakeLineSegment2({ 1.5f, 1.5f }, { -5, -5 }) },
			{ MakeLineSegment2({ -1, 7 }, { 5, 25 }),	MakeLineSegment2({ -7, 4 }, { 1, -12 }) },
			{ MakeLineSegment2({ 1, 1 }, { 6, 11 }),	MakeLineSegment2({ 3, 5 }, { 10, 19 }) }
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n" +
				" 平行なので(0,0)を返す．重なっていて交点が無数にある場合も交点はない扱いになる．\n";

			EXPECT_EQ(line1.GetIntersection(line2), dl::Vector2(0, 0)) << error_message;
		}
	}

	TEST(LineSegment2Test, HasIntersectionTestCaseOfHasIntersection)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 0, 1 }, { 1, 0 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 0 }, { 0, 1 }) },
			{ MakeLineSegment2({ 1,1 }, { 0,0 }),			MakeLineSegment2({ 0,1 }, { 1,0 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 1 }, { 2, 3 }) },		//端点一致
			{ MakeLineSegment2({ -8, -14 }, { 5, 25 }),		MakeLineSegment2({ -7, 4 }, { 1, -12 }) },
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n";

			EXPECT_TRUE(line1.HasIntersection(line2)) << error_message;
		}
	}

	TEST(LineSegment2Test, HasIntersectionTestCaseOfHasNotIntersection)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ -5,-6 }, { -3,-1 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ 1.5f, 1.5f }, { 2, 3 }) },
			{ MakeLineSegment2({ -1, 7 }, { 5, 25 }),	MakeLineSegment2({ -7, 4 }, { 1, -12 }) },
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n";

			EXPECT_FALSE(line1.HasIntersection(line2)) << error_message;
		}
	}

	TEST(LineSegment2Test, HasIntersectionTestCaseOfParallelLine)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ 0, 1 }, { 1, 2 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ -6,-6 }, { -3,-3 }) },
			{ MakeLineSegment2({ -7, -7 }, { 10, 10 }),	MakeLineSegment2({ 1.5f, 1.5f }, { -5, -5 }) },
			{ MakeLineSegment2({ -1, 7 }, { 5, 25 }),	MakeLineSegment2({ -7, 4 }, { 1, -12 }) },
			{ MakeLineSegment2({ 1, 1 }, { 6, 11 }),	MakeLineSegment2({ 3, 5 }, { 10, 19 }) }
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n";

			EXPECT_FALSE(line1.HasIntersection(line2)) << error_message;
		}
	}

	TEST(LineSegment2Test, CheckAndGetIntersectionTestCaseOfTrue)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2, dl::Vector2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 0, 1 }, { 1, 0 }),		dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 0 }, { 0, 1 }),		dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ 1,1 }, { 0,0 }),			MakeLineSegment2({ 0,1 }, { 1,0 }),			dl::Vector2(0.5, 0.5) },
			{ MakeLineSegment2({ -8, -14 }, { 5, 25 }),		MakeLineSegment2({ -7, 4 }, { 1, -12 }),	dl::Vector2(-4, -2) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),			MakeLineSegment2({ 1, 1 }, { 2, 3 }),		dl::Vector2(1, 1) },	//端点一致
			{ MakeLineSegment2({ -15, -15 }, { 1, 1 }),		MakeLineSegment2({ 1, 1 }, { 21, 21 }),		dl::Vector2(1, 1) },	//平行かつ端点一致
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);
			const auto& expected = std::get<2>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n" + 
				" 交点は" + expected.ToString() + "\n";

			dl::Vector2 intersection;
			EXPECT_TRUE(line1.CheckAndGetIntersection(line2, &intersection)) << error_message;
			EXPECT_EQ(intersection, expected) << error_message;
		}
	}

	TEST(LineSegment2Test, CheckAndGetIntersectionTestCaseOfFalse)
	{
		std::vector<std::tuple<dl::LineSegment2, dl::LineSegment2>> testcase_list{
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ -5,-6 }, { -3,-1 }) },
			{ MakeLineSegment2({ 0, 0 }, { 1, 1 }),		MakeLineSegment2({ 1.5f, 1.5f }, { 2, 3 }) },
			{ MakeLineSegment2({ -1, 7 }, { 5, 25 }),	MakeLineSegment2({ -7, 4 }, { 1, -12 }) },
			{ MakeLineSegment2({ -15, -15 }, { 1, 1 }),	MakeLineSegment2({ 2, 1 }, { 21, 20 }) },	//平行で交点がない
			{ MakeLineSegment2({ -15, -15 }, { 1, 1 }),	MakeLineSegment2({ -4, -4 }, { 20, 20 }) },	//平行で交点が無数にある
		};

		for (const auto& i : testcase_list)
		{
			const auto& line1 = std::get<0>(i);
			const auto& line2 = std::get<1>(i);

			std::string error_message = " line1 = (" + line1.start.ToString() + ", " + line1.end.ToString() + ")\n" +
				" line2 = (" + line2.start.ToString() + ", " + line2.end.ToString() + ")\n" + 
				" 交点はない，あるいは交点が1つに定まらない．";

			dl::Vector2 intersection;
			EXPECT_FALSE(line1.CheckAndGetIntersection(line2, &intersection)) << error_message;
		}
	}

}	//namespace dl_vec_test