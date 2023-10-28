#include "pch.h"

#include "../DesignLab/designlab_line_segment2.h"
#include "../DesignLab/designlab_line_segment2.cpp"


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
	TEST(LineSegment2Test, DefaultConstructorTest)
	{
		dl::LineSegment2 line;
		EXPECT_EQ(line.start, dl::Vector2(0, 0)) << "デフォルトコンストラクタでは(0,0) (0,0)に初期化される．";
		EXPECT_EQ(line.end, dl::Vector2(0, 0)) << "デフォルトコンストラクタでは(0,0) (0,0)に初期化される．";
	}

	TEST(LineSegment2Test, ConstructorTest)
	{
		dl::LineSegment2 line = MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4));
		EXPECT_EQ(line.start, dl::Vector2(1, 2));
		EXPECT_EQ(line.end, dl::Vector2(3, 4));
	}

	TEST(LineSegment2Test, CopyConstructorTest)
	{
		dl::LineSegment2 line = MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4));
		dl::LineSegment2 line2 = line;
		EXPECT_EQ(line2.start, dl::Vector2(1, 2));
		EXPECT_EQ(line2.end, dl::Vector2(3, 4));
	}

	TEST(LineSegment2Test, MoveConstructorTest)
	{
		dl::LineSegment2 line = MakeLineSegment2(dl::Vector2(1, 2), dl::Vector2(3, 4));
		dl::LineSegment2 line2 = std::move(line);
		EXPECT_EQ(line2.start, dl::Vector2(1, 2));
		EXPECT_EQ(line2.end, dl::Vector2(3, 4));
	}

}	//namespace dl_vec_test

