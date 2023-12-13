#pragma once

#include "doctest.h"

#include "gait_pattern_graph_tree.h"

TEST_SUITE("GaitPatternGraphTree")
{
	TEST_CASE("DefaultConstructor_初期化時_グラフの要素は０になるべき")
	{
		const GaitPatternGraphTree tree_10(10);
		const GaitPatternGraphTree tree_1000(1000);

		SUBCASE("初期化時_グラフのサイズが0になるべき")
		{
			CHECK(tree_10.GetGraphSize() == 0);

			CHECK(tree_1000.GetGraphSize() == 0);
		}

		SUBCASE("初期化時_グラフが空になるべき")
		{
			CHECK(tree_10.IsEmpty());

			CHECK(tree_1000.IsEmpty());
		}
	}

	TEST_CASE("DefaultConstructor_初期化時_ルートは存在するべきでない")
	{
		GaitPatternGraphTree tree_10(10);
		CHECK_FALSE(tree_10.HasRoot());

		GaitPatternGraphTree tree_1000(1000);
		CHECK_FALSE(tree_1000.HasRoot());
	}

	TEST_CASE("HasRoot_ルートを追加した時_ルートは存在するべき")
	{
		RobotStateNode root_node;
		root_node.ChangeLootNode();

		//Arrange1
		GaitPatternGraphTree tree_10(10);
		CHECK_FALSE(tree_10.HasRoot());
		tree_10.AddNode(root_node);

		//Assert1
		CHECK(tree_10.HasRoot());

		//Arrange2
		GaitPatternGraphTree tree_1000(1000);
		CHECK_FALSE(tree_1000.HasRoot());
		tree_1000.AddNode(root_node);

		//Assert2
		CHECK(tree_1000.HasRoot());
	}

	TEST_CASE("AddNodeTest")
	{
	}
}
