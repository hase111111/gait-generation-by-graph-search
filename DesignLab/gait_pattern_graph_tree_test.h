#pragma once

#include "doctest.h"

#include "gait_pattern_graph_tree.h"

TEST_SUITE("GaitPatternGraphTree")
{
	TEST_CASE("ConstructorTest")
	{
		SUBCASE("初期化時はグラフのサイズが0になる")
		{
			GaitPatternGraphTree tree_10(10);
			CHECK(tree_10.GetGraphSize() == 0);

			GaitPatternGraphTree tree_1000(1000);
			CHECK(tree_1000.GetGraphSize() == 0);
		}

		SUBCASE("初期化時はグラフが空になる")
		{
			GaitPatternGraphTree tree_10(10);
			CHECK(tree_10.IsEmpty());

			GaitPatternGraphTree tree_1000(1000);
			CHECK(tree_1000.IsEmpty());
		}
	}

	TEST_CASE("HasRootTest")
	{
		SUBCASE("初期化時はルートが存在しない")
		{
			GaitPatternGraphTree tree_10(10);
			CHECK_FALSE(tree_10.HasRoot());

			GaitPatternGraphTree tree_1000(1000);
			CHECK_FALSE(tree_1000.HasRoot());
		}

		SUBCASE("ルートを追加するとルートが存在する")
		{
			RobotStateNode root_node;
			root_node.ChangeLootNode();

			GaitPatternGraphTree tree_10(10);
			tree_10.AddNode(root_node);
			CHECK(tree_10.HasRoot());

			GaitPatternGraphTree tree_1000(1000);
			tree_1000.AddNode(root_node);
			CHECK(tree_1000.HasRoot());
		}
	}

	TEST_CASE("AddNodeTest")
	{
	}
}
