
//! @file      gait_pattern_graph_tree_test.h
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_TEST_H_
#define DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_TEST_H_

#include "doctest.h"

#include "gait_pattern_graph_tree.h"


TEST_SUITE("GaitPatternGraphTree::Constructor")
{
    using designlab::GaitPatternGraphTree;

    TEST_CASE("初期化した時，グラフの要素は０になるべき")
    {
        const GaitPatternGraphTree tree_10(10);
        const GaitPatternGraphTree tree_1000(1000);

        SUBCASE("グラフのサイズは0になるべき")
        {
            const float expect_size = 0;

            CHECK(tree_10.GetGraphSize() == expect_size);

            CHECK(tree_1000.GetGraphSize() == expect_size);
        }

        SUBCASE("グラフが空になるべき")
        {
            CHECK(tree_10.IsEmpty());

            CHECK(tree_1000.IsEmpty());
        }
    }

    TEST_CASE("初期化した時，ルートは存在するべきでない")
    {
        GaitPatternGraphTree tree_10(10);
        CHECK_FALSE(tree_10.HasRoot());

        GaitPatternGraphTree tree_1000(1000);
        CHECK_FALSE(tree_1000.HasRoot());
    }
}

TEST_SUITE("GaitPatternGraphTree::HasRoot")
{
    using designlab::GaitPatternGraphTree;
    using designlab::RobotStateNode;

    TEST_CASE("根ノードがある時，trueを返すべき")
    {
        RobotStateNode root_node;
        root_node.ChangeLootNode();

        // Arrange1.
        GaitPatternGraphTree tree_10(10);
        CHECK_FALSE(tree_10.HasRoot());
        tree_10.AddNode(root_node);

        // Assert1.
        CHECK(tree_10.HasRoot());

        // Arrange2.
        GaitPatternGraphTree tree_1000(1000);
        CHECK_FALSE(tree_1000.HasRoot());
        tree_1000.AddNode(root_node);

        // Assert2.
        CHECK(tree_1000.HasRoot());
    }
}

#endif  // DESIGNLAB_GAIT_PATTERN_GRAPH_TREE_TEST_H_
