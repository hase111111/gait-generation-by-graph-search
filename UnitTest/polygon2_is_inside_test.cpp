#include "pch.h"

#include "../DesignLab/designlab_polygon.h"


namespace dl_vec_test
{
	//isInside関数のテスト(右回り)
	TEST(SPolygon2, IsInsideRightTurn)
	{
		//4角形を作成し，内部にある点と外部にある点を確認する
		designlab::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_TRUE(polygon.isInside({ 0.5f, 0.5f }));		//内部にある点
		EXPECT_TRUE(polygon.isInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon.isInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon.isInside({ 0.8f, 0.95f }));

		EXPECT_TRUE(polygon.isInside({ 0.5f, 0.0f }));		//辺の上にある点
		EXPECT_TRUE(polygon.isInside({ 1.0f, 0.5f }));
		EXPECT_TRUE(polygon.isInside({ 0.5f, 1.0f }));
		EXPECT_TRUE(polygon.isInside({ 0.0f, 0.5f }));

		EXPECT_TRUE(polygon.isInside({ 0.0f, 0.0f }));		//頂点にある点
		EXPECT_TRUE(polygon.isInside({ 1.0f, 0.0f }));
		EXPECT_TRUE(polygon.isInside({ 1.0f, 1.0f }));
		EXPECT_TRUE(polygon.isInside({ 0.0f, 1.0f }));

		EXPECT_FALSE(polygon.isInside({ 1.5f, 1.5f }));		//外部にある点
		EXPECT_FALSE(polygon.isInside({ -0.5f, 1.5f }));
		EXPECT_FALSE(polygon.isInside({ 1.5f, -0.5f }));
		EXPECT_FALSE(polygon.isInside({ -0.5f, -0.5f }));

		EXPECT_FALSE(polygon.isInside({ 0.0f, -0.5f }));	//辺の延長線上にある点
		EXPECT_FALSE(polygon.isInside({ 0.0f, 1.5f }));

		EXPECT_FALSE(polygon.isInside({ 10000.0f, 10000.0f }));	//かなり外部にある点
		EXPECT_FALSE(polygon.isInside({ -10000.0f, 10000.0f }));
		EXPECT_FALSE(polygon.isInside({ 10000.0f, -10000.0f }));
		EXPECT_FALSE(polygon.isInside({ -10000.0f, -10000.0f }));


		//6角形を作成し，内部にある点と外部にある点を確認する
		designlab::SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_TRUE(polygon2.isInside({ 0.5f, 0.5f }));
		EXPECT_TRUE(polygon2.isInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon2.isInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon2.isInside({ 0.8f, 0.95f }));
		EXPECT_FALSE(polygon2.isInside({ 1.5f, 0.5f }));

		//頂点が2以下の多角形では必ず失敗する．
		designlab::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon3.isInside({ 0.5f, 0.5f }));

	}

	//isInside関数のテスト(左回り)
	TEST(SPolygon2, IsInsideLeftTurn)
	{
		//4角形を作成し，内部にある点と外部にある点を確認する
		designlab::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 0, 1 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon.isInside({ 0.5f, 0.5f }));		//内部にある点
		EXPECT_TRUE(polygon.isInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon.isInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon.isInside({ 0.8f, 0.95f }));

		EXPECT_TRUE(polygon.isInside({ 0.5f, 0.0f }));		//辺の上にある点
		EXPECT_TRUE(polygon.isInside({ 1.0f, 0.5f }));
		EXPECT_TRUE(polygon.isInside({ 0.5f, 1.0f }));
		EXPECT_TRUE(polygon.isInside({ 0.0f, 0.5f }));

		EXPECT_TRUE(polygon.isInside({ 0.0f, 0.0f }));		//頂点にある点
		EXPECT_TRUE(polygon.isInside({ 1.0f, 0.0f }));
		EXPECT_TRUE(polygon.isInside({ 1.0f, 1.0f }));
		EXPECT_TRUE(polygon.isInside({ 0.0f, 1.0f }));

		EXPECT_FALSE(polygon.isInside({ 1.5f, 1.5f }));		//外部にある点
		EXPECT_FALSE(polygon.isInside({ -0.5f, 1.5f }));
		EXPECT_FALSE(polygon.isInside({ 1.5f, -0.5f }));
		EXPECT_FALSE(polygon.isInside({ -0.5f, -0.5f }));

		EXPECT_FALSE(polygon.isInside({ 0.0f, -0.5f }));	//辺の延長線上にある点
		EXPECT_FALSE(polygon.isInside({ 0.0f, 1.5f }));

		EXPECT_FALSE(polygon.isInside({ 10000.0f, 100000.0f }));
		EXPECT_FALSE(polygon.isInside({ -10000.0f, 10000.0f }));
		EXPECT_FALSE(polygon.isInside({ 10000.0f, -10000.0f }));
		EXPECT_FALSE(polygon.isInside({ -10000.0f, -10000.0f }));


		//6角形を作成し，内部にある点と外部にある点を確認する
		designlab::SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ -1, 0 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon2.isInside({ 0.5f, 0.5f }));
		EXPECT_TRUE(polygon2.isInside({ 0.3f, 0.7f }));
		EXPECT_TRUE(polygon2.isInside({ 0.25f, 0.45f }));
		EXPECT_TRUE(polygon2.isInside({ 0.8f, 0.95f }));
		EXPECT_FALSE(polygon2.isInside({ 1.5f, 0.5f }));

		//頂点が2以下の多角形では必ず失敗する．
		designlab::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon3.isInside({ 0.5f, 0.5f }));

	}
}