#include "pch.h"
#include "../DesignLab/MyPolygon.h"


namespace SPolygon2Test
{
	//GetMaxX関数のテスト
	TEST(SPolygon2Func, GetMaxX)
	{
		//三角形を生成して，X座標の最大値を求める
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.getMaxX(), 1.0f);

		//四角形を生成して，X座標の最大値を求める
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.getMaxX(), 1.0f);

		//先ほどの4角形に点を追加して，X座標の最大値を求める
		polygon2.addVertex({ 2, 0.5 });
		polygon2.addVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.getMaxX(), 2.0f);

		//6角形を生成して，X座標の最大値を求める
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.getMaxX(), 1.0f);

		//先ほどの6角形から点を削除して，X座標の最大値を求める
		polygon3.removeLastVertex();
		polygon3.removeLastVertex();

		EXPECT_FLOAT_EQ(polygon3.getMaxX(), 1.0f);
	}

	//GetMinXのテスト
	TEST(SPolygon2Func, GetMinX)
	{
		//三角形を生成して，X座標の最小値を求める
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.getMinX(), 0.0f);

		//四角形を生成して，X座標の最小値を求める
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.getMinX(), 0.0f);

		//先ほどの4角形に点を追加して，X座標の最小値を求める
		polygon2.addVertex({ 2, 0.5 });
		polygon2.addVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.getMinX(), -5.0f);

		//6角形を生成して，X座標の最小値を求める
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.getMinX(), -1.0f);

		//先ほどの6角形から点を削除して，X座標の最小値を求める
		polygon3.removeLastVertex();
		polygon3.removeLastVertex();

		EXPECT_FLOAT_EQ(polygon3.getMinX(), 0.0f);
	}

	//GetMaxYのテスト
	TEST(SPolygon2Func, GetMaxY)
	{
		//三角形を生成して，Y座標の最大値を求める
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.getMaxY(), 1.0f);

		//四角形を生成して，Y座標の最大値を求める
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.getMaxY(), 1.0f);

		//先ほどの4角形に点を追加して，Y座標の最大値を求める
		polygon2.addVertex({ 2, 0.5 });
		polygon2.addVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.getMaxY(), 1.0f);

		//6角形を生成して，Y座標の最大値を求める
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.getMaxY(), 1.0f);

		//先ほどの6角形から点を削除して，Y座標の最大値を求める
		polygon3.removeLastVertex();
		polygon3.removeLastVertex();

		EXPECT_FLOAT_EQ(polygon3.getMaxY(), 1.0f);
	}

	TEST(SPolygon2Func, GetMinY)
	{
		//三角形を生成して，Y座標の最小値を求める
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon.getMinY(), 0.0f);

		//四角形を生成して，Y座標の最小値を求める
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		EXPECT_FLOAT_EQ(polygon2.getMinY(), 0.0f);

		//先ほどの4角形に点を追加して，Y座標の最小値を求める
		polygon2.addVertex({ 2, 0.5 });
		polygon2.addVertex({ -5, 0.5 });

		EXPECT_FLOAT_EQ(polygon2.getMinY(), 0.0f);

		//6角形を生成して，Y座標の最小値を求める
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_FLOAT_EQ(polygon3.getMinY(), 0.0f);

		//先ほどの6角形から点を削除して，Y座標の最小値を求める
		polygon3.removeLastVertex();
		polygon3.removeLastVertex();

		EXPECT_FLOAT_EQ(polygon3.getMinY(), 0.0f);
	}

}	//namespace SPolygon2Test