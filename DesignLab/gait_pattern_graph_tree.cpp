﻿
//! @file gait_pattern_graph_tree.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_graph_tree.h"


namespace designlab
{

const RobotStateNode& GaitPatternGraphTree::GetParentNode(const int index, const int depth) const
{
    assert(0 <= index);
    assert(index < graph_size_);
    assert(0 <= depth);
    assert(depth <= nodes_[index].depth);

    int res_index = index;

    while (true)
    {
        if (depth == nodes_[res_index].depth)
        {
            break;
        }

        res_index = nodes_[res_index].parent_index;
    }

    return nodes_[res_index];
}

const int GaitPatternGraphTree::GetParentNodeIndex(const int index, const int depth) const
{
    assert(0 <= depth);
    assert(depth <= nodes_[index].depth);
    assert(0 <= index);
    assert(index < graph_size_);

    int res_index = index;

    while (true)
    {
        if (depth == nodes_[res_index].depth)
        {
            break;
        }

        res_index = nodes_[res_index].parent_index;
    }

    return res_index;
}

}  // namespace designlab
