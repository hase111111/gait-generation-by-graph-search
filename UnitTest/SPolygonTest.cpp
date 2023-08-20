#include "pch.h"
#include "../DesignLab/MyPolygon.h"
#include "../DesignLab/MyPolygon.cpp"

using namespace dl_vec;

namespace SPolygon2Test
{
	//addVertex関数のテスト
	TEST(SPolygon2Func, AddVertex)
	{
		//四角形を作成し，その頂点の座標を確認する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertexNum(), 4);
		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), SVector2(0, 1));

		//6角形を作成し，その頂点の座標を確認する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_EQ(polygon2.getVertexNum(), 6);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), SVector2(-1, 1));
		EXPECT_EQ(polygon2.getVertex(5), SVector2(-1, 0));
	}

	//addVertexCheckForDuplicates関数のテスト
	TEST(SPolygon2Func, AddVertexCheckForDuplicates)
	{
		//重複する頂点を追加し，その頂点の座標を確認する
		SPolygon2 polygon;
		polygon.addVertexCheckForDuplicates({ 0, 0 });
		polygon.addVertexCheckForDuplicates({ 1, 0 });
		polygon.addVertexCheckForDuplicates({ 1, 1 });
		polygon.addVertexCheckForDuplicates({ 0, 1 });
		polygon.addVertexCheckForDuplicates({ 0, 0 });	//重複する頂点

		EXPECT_EQ(polygon.getVertexNum(), 4);
		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), SVector2(0, 1));

		//重複しない頂点を追加し，その頂点の座標を確認する
		SPolygon2 polygon2;
		polygon2.addVertexCheckForDuplicates({ 0, 0 });
		polygon2.addVertexCheckForDuplicates({ 1, 0 });
		polygon2.addVertexCheckForDuplicates({ 1, 1 });
		polygon2.addVertexCheckForDuplicates({ 0, 1 });
		polygon2.addVertexCheckForDuplicates({ -1, 1 });

		EXPECT_EQ(polygon2.getVertexNum(), 5);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), SVector2(-1, 1));
	}

	//removeVertex関数のテスト
	TEST(SPolygon2Func, RemoveVertex)
	{
		//四角形を作成し，頂点を削除する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		polygon.removeVertex(0);
		EXPECT_EQ(polygon.getVertexNum(), 3);
		EXPECT_EQ(polygon.getVertex(0), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(2), SVector2(0, 1));

		polygon.removeVertex(1);
		EXPECT_EQ(polygon.getVertexNum(), 2);
		EXPECT_EQ(polygon.getVertex(0), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(0, 1));

		polygon.removeVertex(1);
		EXPECT_EQ(polygon.getVertexNum(), 1);
		EXPECT_EQ(polygon.getVertex(0), SVector2(1, 0));

		polygon.removeVertex(0);
		EXPECT_EQ(polygon.getVertexNum(), 0);

		//存在しない頂点を指定した場合何もしない．四角形を作成し，存在しない頂点を指定する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		polygon2.removeVertex(4);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));

		polygon2.removeVertex(5);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));

		//負の値を指定した場合
		polygon2.removeVertex(-1);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));
	}

	//removeLastVertex関数のテスト
	TEST(SPolygon2Func, RemoveLastVertex)
	{
		//四角形を作成し，頂点を削除する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 3);
		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), SVector2(1, 1));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 2);
		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 0));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 1);
		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 0);

		//6角形を作成し，頂点を削除する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		polygon2.removeLastVertex();
		EXPECT_EQ(polygon2.getVertexNum(), 5);
		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), SVector2(-1, 1));
	}

	//getVertexNum関数のテスト
	TEST(SPolygon2Func, GetVertexNum)
	{
		//頂点がない多角形を作成し，頂点数を確認する
		SPolygon2 polygon2;
		EXPECT_EQ(polygon2.getVertexNum(), 0);

		//4角形を作成し，頂点数を確認する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertexNum(), 4);

		//頂点を全て削除し，頂点数を確認する
		polygon.reset();

		EXPECT_EQ(polygon.getVertexNum(), 0);

		//6角形を作成し，頂点数を確認する
		SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_EQ(polygon3.getVertexNum(), 6);

		//頂点を削除し，頂点数を確認する
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 5);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 4);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 3);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 2);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 1);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 0);
	}

	//getVertex関数のテスト
	TEST(SPolygon2Func, GetVertex)
	{
		//4角形を作成し，その頂点の座標を確認する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), SVector2(0, 1));

		//6角形を作成し，その頂点の座標を確認する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_EQ(polygon2.getVertex(0), SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), SVector2(-1, 1));
		EXPECT_EQ(polygon2.getVertex(5), SVector2(-1, 0));
	}

	//isConvex関数のテスト
	TEST(SPolygon2Func, IsConvex_TurnRight)
	{
		//4角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_TRUE(polygon.isConvex());

		//6角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_TRUE(polygon2.isConvex());

		//凸多角形でない多角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ 0.5f, 0.5f });

		EXPECT_FALSE(polygon3.isConvex());

		//頂点数が3未満の多角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon4;
		polygon4.addVertex({ 0, 0 });
		polygon4.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.isConvex());
	}

	//isConvex関数のテスト(左回り)
	TEST(SPolygon2Func, IsConvex_TurnLeft)
	{
		//4角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 0, 1 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon.isConvex());

		//6角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ -1, 0 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon2.isConvex());

		//凸多角形でない多角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 0.5f, 0.5f });


		EXPECT_FALSE(polygon3.isConvex());

		//頂点数が3未満の多角形を作成し，凸多角形かどうかを確認する
		SPolygon2 polygon4;
		polygon4.addVertex({ 0, 0 });
		polygon4.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.isConvex());
	}
}