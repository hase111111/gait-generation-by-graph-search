#include "pch.h"

#include "../DesignLab/designlab_line_segment2.h"
#include "../DesignLab/designlab_line.cpp"



namespace dl_vec_test
{
	TEST(LineSegment2, GetIntersection)
	{
		//辺が平行の場合交点はない，(0,0)が返る
		designlab::LineSegment2 line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		designlab::LineSegment2 line2 = designlab::LineSegment2({ 0, 1 }, { 1, 2 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(0, 0));

		//辺が平行でない場合交点がある
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 0, 1 }, { 1, 0 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(0.5, 0.5));

		//辺の向きが逆でも交点は同じ
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 0 }, { 0, 1 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(0.5, 0.5));

		line1 = designlab::LineSegment2({ 1,1 }, { 0,0 });
		line2 = designlab::LineSegment2({ 0,1 }, { 1,0 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(0.5, 0.5));

		//辺の長さが足りず，接していない場合交点はない，(0,0)が返る
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ -5,-6 }, { -3,-1 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(0, 0));

		//端点で接する場合，その点が返る．
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 1 }, { 2, 3 });
		EXPECT_EQ(line1.GetIntersection(line2), designlab::SVector2(1, 1));
	}

	//hasIntersection関数のテスト
	TEST(LineSegment2, HasIntersection)
	{
		//辺が平行の場合交点はない
		designlab::LineSegment2 line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		designlab::LineSegment2 line2 = designlab::LineSegment2({ 0, 1 }, { 1, 2 });
		EXPECT_FALSE(line1.HasIntersection(line2));

		//辺が平行でない場合交点がある
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 0, 1 }, { 1, 0 });
		EXPECT_TRUE(line1.HasIntersection(line2));

		//辺の向きが逆でも交点は同じ
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 0 }, { 0, 1 });
		EXPECT_TRUE(line1.HasIntersection(line2));

		line1 = designlab::LineSegment2({ 1,1 }, { 0,0 });
		line2 = designlab::LineSegment2({ 0,1 }, { 1,0 });
		EXPECT_TRUE(line1.HasIntersection(line2));

		//辺の長さが足りず，接していない場合交点はない
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ -5,-6 }, { -3,-1 });
		EXPECT_FALSE(line1.HasIntersection(line2));

		//端点で接する場合，その点が返る．
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 1 }, { 2, 3 });
		EXPECT_TRUE(line1.HasIntersection(line2));
	}

	//getLength関数のテスト
	TEST(LineSegment2, GetLength)
	{
		//辺の長さが正しく返る
		designlab::LineSegment2 line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		EXPECT_FLOAT_EQ(line1.GetLength(), sqrt(2.0f));

		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 0 });
		EXPECT_FLOAT_EQ(line1.GetLength(), 1.0f);

		line1 = designlab::LineSegment2({ 0, 0 }, { 0, 1 });
		EXPECT_FLOAT_EQ(line1.GetLength(), 1.0f);

		line1 = designlab::LineSegment2({ 0, 0 }, { 0, 0 });
		EXPECT_FLOAT_EQ(line1.GetLength(), 0.0f);

		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 2 });
		EXPECT_FLOAT_EQ(line1.GetLength(), sqrt(5.0f));

		line1 = designlab::LineSegment2({ 0, 0 }, { 2, 1 });
		EXPECT_FLOAT_EQ(line1.GetLength(), sqrt(5.0f));

		line1 = designlab::LineSegment2({ 0, 0 }, { 2, 2 });
		EXPECT_FLOAT_EQ(line1.GetLength(), sqrt(8.0f));
	}

	//isParallel関数のテスト
	TEST(LineSegment2, IsParallel)
	{
		//辺が平行の場合trueが返る
		designlab::LineSegment2 line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		designlab::LineSegment2 line2 = designlab::LineSegment2({ 0, 1 }, { 1, 2 });
		EXPECT_TRUE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 10, 0 }, { 0, 0 });
		line2 = designlab::LineSegment2({ 0, 0 }, { -400, 0 });
		EXPECT_TRUE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 35, 70 }, { 35, -200 });
		line2 = designlab::LineSegment2({ -14.7f, -300 }, { -14.7f, 2000 });
		EXPECT_TRUE(line1.IsParallel(line2));


		//辺が平行でない場合falseが返る
		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 0, 1 }, { 1, 0 });
		EXPECT_FALSE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 0 }, { 0, 1 });
		EXPECT_FALSE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 1,1 }, { 0,0 });
		line2 = designlab::LineSegment2({ 0,1 }, { 1,0 });
		EXPECT_FALSE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ -5,-6 }, { -3,-1 });
		EXPECT_FALSE(line1.IsParallel(line2));

		line1 = designlab::LineSegment2({ 0, 0 }, { 1, 1 });
		line2 = designlab::LineSegment2({ 1, 1 }, { 2, 3 });
		EXPECT_FALSE(line1.IsParallel(line2));
	}

}	//namespace dl_vec_test

