#include "pch.h"
#include "../DesignLab/MyLine.h"
#include "../DesignLab/MyLine.cpp"

using namespace my_vec;

namespace SLine2Test
{
	TEST(SLine2FuncTest, GetIntersection)
	{
		//辺が平行の場合交点はない，(0,0)が返る
		SLine2 line1 = SLine2({ 0, 0 }, { 1, 1 });
		SLine2 line2 = SLine2({ 0, 1 }, { 1, 2 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0, 0));

		//辺が平行でない場合交点がある
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 0, 1 }, { 1, 0 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0.5, 0.5));

		//辺の向きが逆でも交点は同じ
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 0 }, { 0, 1 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0.5, 0.5));

		line1 = SLine2({ 1,1 }, { 0,0 });
		line2 = SLine2({ 0,1 }, { 1,0 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0.5, 0.5));

		//辺の長さが足りず，接していない場合交点はない，(0,0)が返る
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ -5,-6 }, { -3,-1 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0, 0));

		//端点で接する場合，その点が返る．
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 1 }, { 2, 3 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(1, 1));
	}

	//hasIntersection関数のテスト
	TEST(SLine2FuncTest, HasIntersection)
	{
		//辺が平行の場合交点はない
		SLine2 line1 = SLine2({ 0, 0 }, { 1, 1 });
		SLine2 line2 = SLine2({ 0, 1 }, { 1, 2 });
		EXPECT_FALSE(line1.hasIntersection(line2));

		//辺が平行でない場合交点がある
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 0, 1 }, { 1, 0 });
		EXPECT_TRUE(line1.hasIntersection(line2));

		//辺の向きが逆でも交点は同じ
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 0 }, { 0, 1 });
		EXPECT_TRUE(line1.hasIntersection(line2));

		line1 = SLine2({ 1,1 }, { 0,0 });
		line2 = SLine2({ 0,1 }, { 1,0 });
		EXPECT_TRUE(line1.hasIntersection(line2));

		//辺の長さが足りず，接していない場合交点はない
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ -5,-6 }, { -3,-1 });
		EXPECT_FALSE(line1.hasIntersection(line2));

		//端点で接する場合，その点が返る．
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 1 }, { 2, 3 });
		EXPECT_TRUE(line1.hasIntersection(line2));
	}

	//getLength関数のテスト
	TEST(SLine2FuncTest, GetLength)
	{
		//辺の長さが正しく返る
		SLine2 line1 = SLine2({ 0, 0 }, { 1, 1 });
		EXPECT_FLOAT_EQ(line1.getLength(), sqrt(2));

		line1 = SLine2({ 0, 0 }, { 1, 0 });
		EXPECT_FLOAT_EQ(line1.getLength(), 1);

		line1 = SLine2({ 0, 0 }, { 0, 1 });
		EXPECT_FLOAT_EQ(line1.getLength(), 1);

		line1 = SLine2({ 0, 0 }, { 0, 0 });
		EXPECT_FLOAT_EQ(line1.getLength(), 0);

		line1 = SLine2({ 0, 0 }, { 1, 2 });
		EXPECT_FLOAT_EQ(line1.getLength(), sqrt(5));

		line1 = SLine2({ 0, 0 }, { 2, 1 });
		EXPECT_FLOAT_EQ(line1.getLength(), sqrt(5));

		line1 = SLine2({ 0, 0 }, { 2, 2 });
		EXPECT_FLOAT_EQ(line1.getLength(), sqrt(8));
	}

	//isParallel関数のテスト
	TEST(SLine2FuncTest, IsParallel)
	{
		//辺が平行の場合trueが返る
		SLine2 line1 = SLine2({ 0, 0 }, { 1, 1 });
		SLine2 line2 = SLine2({ 0, 1 }, { 1, 2 });
		EXPECT_TRUE(line1.isParallel(line2));

		line1 = SLine2({ 10, 0 }, { 0, 0 });
		line2 = SLine2({ 0, 0 }, { -400, 0 });
		EXPECT_TRUE(line1.isParallel(line2));

		line1 = SLine2({ 35, 70 }, { 35, -200 });
		line2 = SLine2({ -14.7f, -300 }, { -14.7f, 2000 });
		EXPECT_TRUE(line1.isParallel(line2));


		//辺が平行でない場合falseが返る
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 0, 1 }, { 1, 0 });
		EXPECT_FALSE(line1.isParallel(line2));

		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 0 }, { 0, 1 });
		EXPECT_FALSE(line1.isParallel(line2));

		line1 = SLine2({ 1,1 }, { 0,0 });
		line2 = SLine2({ 0,1 }, { 1,0 });
		EXPECT_FALSE(line1.isParallel(line2));

		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ -5,-6 }, { -3,-1 });
		EXPECT_FALSE(line1.isParallel(line2));

		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 1 }, { 2, 3 });
		EXPECT_FALSE(line1.isParallel(line2));
	}
}