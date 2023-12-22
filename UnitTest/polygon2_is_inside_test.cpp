#include "pch.h"

#include "../DesignLab/math_polygon2.h"


namespace designlab::test::common::math
{
	//IsInside関数のテスト(右回り)
	TEST(Polygon2Test, IsInsideRightTurn)
	{
		//4角形を作成し，内部にある点と外部にある点を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_TRUE(polygon.IsInside({ 0.5f, 0.5f }));		//内部にある点
		EXPECT_TRUE(polygon.IsInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon.IsInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon.IsInside({ 0.8f, 0.95f }));

		EXPECT_TRUE(polygon.IsInside({ 0.5f, 0.0f }));		//辺の上にある点
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 0.5f }));
		EXPECT_TRUE(polygon.IsInside({ 0.5f, 1.0f }));
		EXPECT_TRUE(polygon.IsInside({ 0.0f, 0.5f }));

		EXPECT_TRUE(polygon.IsInside({ 0.0f, 0.0f }));		//頂点にある点
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 0.0f }));
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 1.0f }));
		EXPECT_TRUE(polygon.IsInside({ 0.0f, 1.0f }));

		EXPECT_FALSE(polygon.IsInside({ 1.5f, 1.5f }));		//外部にある点
		EXPECT_FALSE(polygon.IsInside({ -0.5f, 1.5f }));
		EXPECT_FALSE(polygon.IsInside({ 1.5f, -0.5f }));
		EXPECT_FALSE(polygon.IsInside({ -0.5f, -0.5f }));

		EXPECT_FALSE(polygon.IsInside({ 0.0f, -0.5f }));	//辺の延長線上にある点
		EXPECT_FALSE(polygon.IsInside({ 0.0f, 1.5f }));

		EXPECT_FALSE(polygon.IsInside({ 10000.0f, 10000.0f }));	//かなり外部にある点
		EXPECT_FALSE(polygon.IsInside({ -10000.0f, 10000.0f }));
		EXPECT_FALSE(polygon.IsInside({ 10000.0f, -10000.0f }));
		EXPECT_FALSE(polygon.IsInside({ -10000.0f, -10000.0f }));


		//6角形を作成し，内部にある点と外部にある点を確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, -0.5f });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1.5f });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ -1, 0 });

		EXPECT_TRUE(polygon2.IsInside({ 0.5f, 0.5f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.8f, 0.95f }));
		EXPECT_FALSE(polygon2.IsInside({ 1.5f, 0.5f }));

		//頂点が2以下の多角形では必ず失敗する．
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });

		EXPECT_FALSE(polygon3.IsInside({ 0.5f, 0.5f }));

	}

	//IsInside関数のテスト(左回り)
	TEST(Polygon2Test, IsInsideLeftTurn)
	{
		//4角形を作成し，内部にある点と外部にある点を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 0, 1 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 1, 0 });

		EXPECT_TRUE(polygon.IsInside({ 0.5f, 0.5f }));		//内部にある点
		EXPECT_TRUE(polygon.IsInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon.IsInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon.IsInside({ 0.8f, 0.95f }));

		EXPECT_TRUE(polygon.IsInside({ 0.5f, 0.0f }));		//辺の上にある点
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 0.5f }));
		EXPECT_TRUE(polygon.IsInside({ 0.5f, 1.0f }));
		EXPECT_TRUE(polygon.IsInside({ 0.0f, 0.5f }));

		EXPECT_TRUE(polygon.IsInside({ 0.0f, 0.0f }));		//頂点にある点
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 0.0f }));
		EXPECT_TRUE(polygon.IsInside({ 1.0f, 1.0f }));
		EXPECT_TRUE(polygon.IsInside({ 0.0f, 1.0f }));

		EXPECT_FALSE(polygon.IsInside({ 1.5f, 1.5f }));		//外部にある点
		EXPECT_FALSE(polygon.IsInside({ -0.5f, 1.5f }));
		EXPECT_FALSE(polygon.IsInside({ 1.5f, -0.5f }));
		EXPECT_FALSE(polygon.IsInside({ -0.5f, -0.5f }));

		EXPECT_FALSE(polygon.IsInside({ 0.0f, -0.5f }));	//辺の延長線上にある点
		EXPECT_FALSE(polygon.IsInside({ 0.0f, 1.5f }));

		EXPECT_FALSE(polygon.IsInside({ 10000.0f, 100000.0f }));
		EXPECT_FALSE(polygon.IsInside({ -10000.0f, 10000.0f }));
		EXPECT_FALSE(polygon.IsInside({ 10000.0f, -10000.0f }));
		EXPECT_FALSE(polygon.IsInside({ -10000.0f, -10000.0f }));


		//6角形を作成し，内部にある点と外部にある点を確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, -0.5f });
		polygon2.AddVertex({ -1, 0 });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ 0, 1.5f });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 1, 0 });

		EXPECT_TRUE(polygon2.IsInside({ 0.5f, 0.5f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon2.IsInside({ 0.8f, 0.95f }));
		EXPECT_FALSE(polygon2.IsInside({ 1.5f, 0.5f }));

		//頂点が2以下の多角形では必ず失敗する．
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });

		EXPECT_FALSE(polygon3.IsInside({ 0.5f, 0.5f }));

	}
}