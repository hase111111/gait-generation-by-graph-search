#include "pch.h"
#include "../DesignLab/MyVector2.h"

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

		//辺の長さが足りず，接していない場合交点はない，(0,0)が返る
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ -5,-6 }, { -3,-1 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(0, 0));

		//端点で接する場合，その点が返る．
		line1 = SLine2({ 0, 0 }, { 1, 1 });
		line2 = SLine2({ 1, 1 }, { 2, 3 });
		EXPECT_EQ(line1.getIntersection(line2), SVector2(1, 1));
	}
}