﻿
//! @file system_main_graph_viewer.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "system_main_graph_viewer.h"

#include <format>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <boost/thread.hpp>
#include <magic_enum.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "gait_pattern_generator_basic.h"
#include "graph_search_const.h"
#include "node_initializer.h"
#include "phantomx_mk2.h"
#include "string_util.h"


namespace designlab
{

SystemMainGraphViewer::SystemMainGraphViewer(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
    std::unique_ptr<IMapCreator>&& map_creator,
    const std::shared_ptr<GraphicDataBroker>& broker_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr) :
    graph_tree_creator_ptr_(std::move(graph_tree_creator)),
    map_creator_ptr_(std::move(map_creator)),
    broker_ptr_(broker_ptr),
    setting_ptr_(setting_ptr)
{
    assert(graph_tree_creator_ptr_ != nullptr);
    assert(map_creator_ptr_ != nullptr);
    assert(broker_ptr_ != nullptr);
    assert(setting_ptr_ != nullptr);

    CmdIOUtil::OutputTitle("Graph Viewer Mode");  // タイトルを表示する．

    map_state_ = map_creator_ptr_->InitMap();

    broker_ptr_->map_state.SetData(map_state_);
}


void SystemMainGraphViewer::Main()
{
    using enum OutputDetail;

    // ノードを初期化する．
    CmdIOUtil::InfoOutput("Initializes a node.");

    RobotStateNode first_node = NodeInitializer{ Vector3{0.f, 0.f, 30.f}, EulerXYZ {}, HexapodMove::kComUpDown }.InitNode();
    std::vector<RobotStateNode> graph;

    while (true)
    {
        OutputGraphStatus(graph);

        if (graph.size() == 0)
        {
            // グラフがない場合，

            CmdIOUtil::Output("No graphs have been generated yet.", kSystem);

            if (CmdIOUtil::InputYesNo("Do you want to create a graph?"))
            {
                CreateGraph(first_node, &graph);  // グラフを作成する．

                // グラフ木の値を仲介人にセットする．これでGUIにグラフが表示される．
                broker_ptr_->graph.SetData(graph);
            }
            else
            {
                // 終了するか質問する．
                if (CmdIOUtil::InputYesNo("Do you wish to terminate?"))
                {
                    break;
                }
            }
        }
        else
        {
            // グラフがある場合．

            CmdIOUtil::Output("Displays the operation menu.", kSystem);

            // 操作メニューを表示する．

            // 操作をおこなう関数をラムダ式宣言し，関数ポインタで受け取る vector．
            std::vector<std::function<void()>> func_list;

            func_list.push_back(
              [&]()
              {
                  RobotStateNode selected = SelectNodeByInput(graph);
                  CreateGraph(selected, &graph);
                  broker_ptr_->graph.SetData(graph);
              });

            func_list.push_back(
              [&]()
              {
                  RobotStateNode selected = SelectNodeByInput(graph);

                  CmdIOUtil::OutputNewLine(1, kSystem);
                  CmdIOUtil::OutputHorizontalLine("*", kSystem);
                  CmdIOUtil::Output(selected.ToString(), kSystem);
                  CmdIOUtil::OutputHorizontalLine("*", kSystem);
                  CmdIOUtil::OutputNewLine(1, kSystem);
              });

            func_list.push_back(
              [&]()
              {
                  graph.clear();
                  broker_ptr_->graph.Clean();
                  CmdIOUtil::Output("グラフを全て削除しました", kSystem);
                  CmdIOUtil::OutputNewLine(1, kSystem);
              });


            CmdIOUtil::OutputNewLine(1, kSystem);
            CmdIOUtil::Output("操作を選択してください", kSystem);
            CmdIOUtil::Output("　0 : ノード選択し，そのノードを親にしてグラフを生成する", kSystem);
            CmdIOUtil::Output("　1 : ノード選択して表示する", kSystem);
            CmdIOUtil::Output("　2 : グラフを全削除する", kSystem);
            CmdIOUtil::Output("　3 : 終了する", kSystem);

            int selected_index =
                CmdIOUtil::InputInt(0, static_cast<int>(func_list.size()),
                                    static_cast<int>(func_list.size()),
                                    "整数で操作を選択してください．範囲外の値の場合終了します．");

            // 選択された操作を実行する．
            if (selected_index < func_list.size())
            {
                func_list[selected_index]();
            }
            else
            {
                if (CmdIOUtil::InputYesNo("終了しますか？"))
                {
                    break;
                }
            }
        }
    }  // while (true)
}


void SystemMainGraphViewer::CreateGraph(const RobotStateNode parent, std::vector<RobotStateNode>* graph)
{
    assert(graph != nullptr);

    using enum OutputDetail;

    CmdIOUtil::OutputNewLine(1, kSystem);
    CmdIOUtil::Output("グラフ木を作成する", kSystem);
    CmdIOUtil::OutputNewLine(1, kSystem);

    // グラフ探索をする．
    RobotStateNode parent_node = parent;
    parent_node.ChangeLootNode();

    RobotStateNode fake_result_node;

    DividedMapState divided_map;
    divided_map.Init(map_state_, parent_node.center_of_mass_global_coord);

    GaitPatternGraphTree graph_tree(1000000);
    graph_tree.Reset();
    graph_tree.AddNode(parent_node);


    graph_tree_creator_ptr_->Init(divided_map);

    stopwatch_.Start();

    const GraphSearchResult result = graph_tree_creator_ptr_->CreateGraphTree(0, 5, &graph_tree);

    stopwatch_.End();

    // グラフ探索の結果を取得する．
    (*graph).clear();

    for (int i = 0; i < graph_tree.GetGraphSize(); i++)
    {
        (*graph).push_back(graph_tree.GetNode(i));
    }


    // グラフ探索の結果を表示する．
    CmdIOUtil::OutputNewLine(1, kSystem);
    CmdIOUtil::Output("グラフ探索終了", kSystem);
    CmdIOUtil::Output(std::format("グラフ探索にかかった時間 : {}", stopwatch_.GetElapsedMilliSecondString()), kSystem);

    CmdIOUtil::Output(std::format("グラフ探索結果 : {}", string_util::EnumToStringRemoveTopK(result.result)), kSystem);
}

void SystemMainGraphViewer::OutputGraphStatus(const std::vector<RobotStateNode>& graph) const
{
    const OutputDetail detail = OutputDetail::kInfo;

    CmdIOUtil::OutputNewLine(1, detail);
    CmdIOUtil::OutputHorizontalLine("=", detail);
    CmdIOUtil::OutputNewLine(1, detail);
    CmdIOUtil::Output("Displays the status of the graph.", detail);
    CmdIOUtil::OutputNewLine(1, detail);
    CmdIOUtil::FormatOutput(detail, "Number of nodes in the graph : {}", graph.size());


    if (graph.size() > 0)
    {
        // 深さごとのノード数を記録する．

        std::vector<int> depth_num(GraphSearchConst::kMaxDepth + 1);

        CmdIOUtil::Output(std::format("Maximum depth of graph search : {}", GraphSearchConst::kMaxDepth), detail);

        for (const auto& i : graph)
        {
            if (i.depth < depth_num.size())
            {
                depth_num[i.depth]++;
            }
        }

        // 深さごとのノード数を表示する．

        int depth_cnt = 0;

        for (const auto& i : depth_num)
        {
            CmdIOUtil::FormatOutput(detail, "- depth {} : {}", depth_cnt, i);
            depth_cnt++;
        }
    }
    else
    {
        CmdIOUtil::Output("グラフが空なので，深さごとのノード数を表示できません．", detail);
    }

    CmdIOUtil::OutputNewLine(1, detail);
    CmdIOUtil::OutputHorizontalLine("=", detail);
    CmdIOUtil::OutputNewLine(1, detail);
}

RobotStateNode SystemMainGraphViewer::SelectNodeByInput(
    const std::vector<RobotStateNode>& graph) const
{
    using enum OutputDetail;

    CmdIOUtil::OutputNewLine(1, kSystem);
    CmdIOUtil::Output("ノードを選択する", kSystem);

    if (graph.size() == 0)
    {
        CmdIOUtil::Output("グラフが空なので，初期状態のノードを返す", kSystem);

        NodeInitializer node_initializer{ Vector3{0.f, 0.f, 30.f}, EulerXYZ(), HexapodMove::kNone };
        RobotStateNode first_node = node_initializer.InitNode();

        return first_node;
    }
    else
    {
        CmdIOUtil::Output("グラフの中から1つのノードを選択してください．", kSystem);

        // ノードを選択する．
        int selected_node_index = CmdIOUtil::InputInt(0,
                                                      static_cast<int>(graph.size()) - 1,
                                                      0,
                                                      "整数でノードを選択してください．");

        CmdIOUtil::Output(std::format("選択されたノード，{}番を親にします．",
                          selected_node_index), kSystem);

        return graph[selected_node_index];
    }
}

}  // namespace designlab
