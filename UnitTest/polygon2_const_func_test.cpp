#include "pch.h"

#include "../DesignLab/designlab_polygon2.h"


namespace designlab::test::common::math
{
	TEST(Polygon2Test, GetMaxXTest)
	{
		//三角形を生成して，X座標の最大値を求める
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.GetMaxX(), 1.0f);

		//四角形を生成して，X座標の最大値を求める
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.GetMaxX(), 1.0f);

		//先ほどの4角形に点を追加して，X座標の最大値を求める
		polygon2.AddVertex({ 2, 0.5 });
		polygon2.AddVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.GetMaxX(), 2.0f);

		//6角形を生成して，X座標の最大値を求める
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ -1, 1 });
		polygon3.AddVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.GetMaxX(), 1.0f);

		//先ほどの6角形から点を削除して，X座標の最大値を求める
		polygon3.RemoveLastVertex();
		polygon3.RemoveLastVertex();

		EXPECT_FLOAT_EQ(polygon3.GetMaxX(), 1.0f);
	}

	//GetMinXのテスト
	TEST(Polygon2Test, GetMinXTest)
	{
		//三角形を生成して，X座標の最小値を求める
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.GetMinX(), 0.0f);

		//四角形を生成して，X座標の最小値を求める
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.GetMinX(), 0.0f);

		//先ほどの4角形に点を追加して，X座標の最小値を求める
		polygon2.AddVertex({ 2, 0.5 });
		polygon2.AddVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.GetMinX(), -5.0f);

		//6角形を生成して，X座標の最小値を求める
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ -1, 1 });
		polygon3.AddVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.GetMinX(), -1.0f);

		//先ほどの6角形から点を削除して，X座標の最小値を求める
		polygon3.RemoveLastVertex();
		polygon3.RemoveLastVertex();

		EXPECT_FLOAT_EQ(polygon3.GetMinX(), 0.0f);
	}

	//GetMaxYのテスト
	TEST(Polygon2Test, GetMaxYTest)
	{
		//三角形を生成して，Y座標の最大値を求める
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.GetMaxY(), 1.0f);

		//四角形を生成して，Y座標の最大値を求める
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.GetMaxY(), 1.0f);

		//先ほどの4角形に点を追加して，Y座標の最大値を求める
		polygon2.AddVertex({ 2, 0.5 });
		polygon2.AddVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.GetMaxY(), 1.0f);

		//6角形を生成して，Y座標の最大値を求める
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ -1, 1 });
		polygon3.AddVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.GetMaxY(), 1.0f);

		//先ほどの6角形から点を削除して，Y座標の最大値を求める
		polygon3.RemoveLastVertex();
		polygon3.RemoveLastVertex();

		EXPECT_FLOAT_EQ(polygon3.GetMaxY(), 1.0f);
	}

	TEST(Polygon2Test, GetMinYTest)
	{
		//三角形を生成して，Y座標の最小値を求める
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.GetMinY(), 0.0f);

		//四角形を生成して，Y座標の最小値を求める
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.GetMinY(), 0.0f);

		//先ほどの4角形に点を追加して，Y座標の最小値を求める
		polygon2.AddVertex({ 2, 0.5 });
		polygon2.AddVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.GetMinY(), 0.0f);

		//6角形を生成して，Y座標の最小値を求める
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ -1, 1 });
		polygon3.AddVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.GetMinY(), 0.0f);

		//先ほどの6角形から点を削除して，Y座標の最小値を求める
		polygon3.RemoveLastVertex();
		polygon3.RemoveLastVertex();

		EXPECT_FLOAT_EQ(polygon3.GetMinY(), 0.0f);
	}

}	//namespace dl_vec_test