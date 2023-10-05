#include "pch.h"

#include "../DesignLab/designlab_polygon2.h"
#include "../DesignLab/designlab_polygon.cpp"


namespace dl_vec_test
{
	//addVertex関数のテスト
	TEST(Polygon2, AddVertex)
	{
		//四角形を作成し，その頂点の座標を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_EQ(polygon.GetVertexNum(), 4);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon.GetVertex(3), designlab::Vector2(0, 1));

		//6角形を作成し，その頂点の座標を確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ -1, 0 });

		EXPECT_EQ(polygon2.GetVertexNum(), 6);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));
		EXPECT_EQ(polygon2.GetVertex(4), designlab::Vector2(-1, 1));
		EXPECT_EQ(polygon2.GetVertex(5), designlab::Vector2(-1, 0));
	}

	//addVertexCheckForDuplicates関数のテスト
	TEST(Polygon2, AddVertexCheckForDuplicates)
	{
		//重複する頂点を追加し，その頂点の座標を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertexCheckForDuplicates({ 0, 0 });
		polygon.AddVertexCheckForDuplicates({ 1, 0 });
		polygon.AddVertexCheckForDuplicates({ 1, 1 });
		polygon.AddVertexCheckForDuplicates({ 0, 1 });
		polygon.AddVertexCheckForDuplicates({ 0, 0 });	//重複する頂点

		EXPECT_EQ(polygon.GetVertexNum(), 4);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon.GetVertex(3), designlab::Vector2(0, 1));

		//重複しない頂点を追加し，その頂点の座標を確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertexCheckForDuplicates({ 0, 0 });
		polygon2.AddVertexCheckForDuplicates({ 1, 0 });
		polygon2.AddVertexCheckForDuplicates({ 1, 1 });
		polygon2.AddVertexCheckForDuplicates({ 0, 1 });
		polygon2.AddVertexCheckForDuplicates({ -1, 1 });

		EXPECT_EQ(polygon2.GetVertexNum(), 5);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));
		EXPECT_EQ(polygon2.GetVertex(4), designlab::Vector2(-1, 1));
	}

	//removeVertex関数のテスト
	TEST(Polygon2, RemoveVertex)
	{
		//四角形を作成し，頂点を削除する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		polygon.RemoveVertex(0);
		EXPECT_EQ(polygon.GetVertexNum(), 3);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon.GetVertex(2), designlab::Vector2(0, 1));

		polygon.RemoveVertex(1);
		EXPECT_EQ(polygon.GetVertexNum(), 2);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(0, 1));

		polygon.RemoveVertex(1);
		EXPECT_EQ(polygon.GetVertexNum(), 1);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(1, 0));

		polygon.RemoveVertex(0);
		EXPECT_EQ(polygon.GetVertexNum(), 0);

		//存在しない頂点を指定した場合何もしない．四角形を作成し，存在しない頂点を指定する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });

		polygon2.RemoveVertex(4);
		EXPECT_EQ(polygon2.GetVertexNum(), 4);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));

		polygon2.RemoveVertex(5);
		EXPECT_EQ(polygon2.GetVertexNum(), 4);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));

		//負の値を指定した場合
		polygon2.RemoveVertex(-1);
		EXPECT_EQ(polygon2.GetVertexNum(), 4);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));
	}

	//removeLastVertex関数のテスト
	TEST(Polygon2, RemoveLastVertex)
	{
		//四角形を作成し，頂点を削除する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		polygon.RemoveLastVertex();
		EXPECT_EQ(polygon.GetVertexNum(), 3);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(2), designlab::Vector2(1, 1));

		polygon.RemoveLastVertex();
		EXPECT_EQ(polygon.GetVertexNum(), 2);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 0));

		polygon.RemoveLastVertex();
		EXPECT_EQ(polygon.GetVertexNum(), 1);
		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));

		polygon.RemoveLastVertex();
		EXPECT_EQ(polygon.GetVertexNum(), 0);

		//6角形を作成し，頂点を削除する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ -1, 0 });

		polygon2.RemoveLastVertex();
		EXPECT_EQ(polygon2.GetVertexNum(), 5);
		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));
		EXPECT_EQ(polygon2.GetVertex(4), designlab::Vector2(-1, 1));
	}

	//getVertexNum関数のテスト
	TEST(Polygon2, GetVertexNum)
	{
		//頂点がない多角形を作成し，頂点数を確認する
		designlab::Polygon2 polygon2;
		EXPECT_EQ(polygon2.GetVertexNum(), 0);

		//4角形を作成し，頂点数を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_EQ(polygon.GetVertexNum(), 4);

		//頂点を全て削除し，頂点数を確認する
		polygon.Reset();

		EXPECT_EQ(polygon.GetVertexNum(), 0);

		//6角形を作成し，頂点数を確認する
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ -1, 1 });
		polygon3.AddVertex({ -1, 0 });

		EXPECT_EQ(polygon3.GetVertexNum(), 6);

		//頂点を削除し，頂点数を確認する
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 5);
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 4);
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 3);
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 2);
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 1);
		polygon3.RemoveVertex(0);
		EXPECT_EQ(polygon3.GetVertexNum(), 0);
	}

	//getVertex関数のテスト
	TEST(Polygon2, GetVertex)
	{
		//4角形を作成し，その頂点の座標を確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_EQ(polygon.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon.GetVertex(3), designlab::Vector2(0, 1));

		//6角形を作成し，その頂点の座標を確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, 0 });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1 });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ -1, 0 });

		EXPECT_EQ(polygon2.GetVertex(0), designlab::Vector2(0, 0));
		EXPECT_EQ(polygon2.GetVertex(1), designlab::Vector2(1, 0));
		EXPECT_EQ(polygon2.GetVertex(2), designlab::Vector2(1, 1));
		EXPECT_EQ(polygon2.GetVertex(3), designlab::Vector2(0, 1));
		EXPECT_EQ(polygon2.GetVertex(4), designlab::Vector2(-1, 1));
		EXPECT_EQ(polygon2.GetVertex(5), designlab::Vector2(-1, 0));
	}

	//IsConvex関数のテスト
	TEST(Polygon2, IsConvexRightTurn)
	{
		//4角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 1, 0 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 0, 1 });

		EXPECT_TRUE(polygon.IsConvex());

		//6角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, -0.5f });
		polygon2.AddVertex({ 1, 0 });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 0, 1.5f });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ -1, 0 });

		EXPECT_TRUE(polygon2.IsConvex());

		//凸多角形でない多角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ 0.5f, 0.5f });

		EXPECT_FALSE(polygon3.IsConvex());

		//頂点数が3未満の多角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon4;
		polygon4.AddVertex({ 0, 0 });
		polygon4.AddVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.IsConvex());
	}

	//IsConvex関数のテスト(左回り)
	TEST(Polygon2, IsConvexLeftTurn)
	{
		//4角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon;
		polygon.AddVertex({ 0, 0 });
		polygon.AddVertex({ 0, 1 });
		polygon.AddVertex({ 1, 1 });
		polygon.AddVertex({ 1, 0 });

		EXPECT_TRUE(polygon.IsConvex());

		//6角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon2;
		polygon2.AddVertex({ 0, -0.5f });
		polygon2.AddVertex({ -1, 0 });
		polygon2.AddVertex({ -1, 1 });
		polygon2.AddVertex({ 0, 1.5f });
		polygon2.AddVertex({ 1, 1 });
		polygon2.AddVertex({ 1, 0 });

		EXPECT_TRUE(polygon2.IsConvex());

		//凸多角形でない多角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon3;
		polygon3.AddVertex({ 0, 0 });
		polygon3.AddVertex({ 0, 1 });
		polygon3.AddVertex({ 1, 1 });
		polygon3.AddVertex({ 1, 0 });
		polygon3.AddVertex({ 0.5f, 0.5f });


		EXPECT_FALSE(polygon3.IsConvex());

		//頂点数が3未満の多角形を作成し，凸多角形かどうかを確認する
		designlab::Polygon2 polygon4;
		polygon4.AddVertex({ 0, 0 });
		polygon4.AddVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.IsConvex());
	}

}	//namespace dl_vec_test
