#include "system_main_graph_viewer.h"

#include <bitset>
#include <iostream>

#include <boost/thread.hpp>
#include <magic_enum.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "string_util.h"
#include "gait_pattern_generator_basic.h"
#include "graph_search_const.h"
#include "node_initializer.h"
#include "phantomx_mk2.h"


namespace designlab
{

SystemMainGraphViewer::SystemMainGraphViewer(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
	std::unique_ptr<IMapCreator>&& map_creator,
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator)),
	map_creator_ptr_(std::move(map_creator)),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	assert(graph_tree_creator_ptr_ != nullptr);
	assert(map_creator_ptr_ != nullptr);
	assert(broker_ptr_ != nullptr);
	assert(setting_ptr_ != nullptr);

	CmdIOUtil::OutputTitle("グラフ確認モード");	//タイトルを表示する

	map_state_ = map_creator_ptr_->InitMap();

	broker_ptr_->map_state.SetData(map_state_);
}


void SystemMainGraphViewer::Main()
{
	//早期リターン
	if (!graph_tree_creator_ptr_)
	{
		CmdIOUtil::Output("グラフ木作成クラスが初期化されていない．終了する", enums::OutputDetail::kError);
		return;
	}

	CmdIOUtil::Output("別スレッドでGUIを起動する．", enums::OutputDetail::kInfo);

	//ノードを初期化する
	CmdIOUtil::Output("ノードを初期化する．", enums::OutputDetail::kSystem);

	NodeInitializer node_initializer{ Vector3{0.f,0.f,30.f}, enums::HexapodMove::kNone };
	RobotStateNode first_node = node_initializer.InitNode();
	std::vector<RobotStateNode> graph;


	while (true)
	{
		OutputGraphStatus(graph);

		if (graph.size() == 0)
		{
			// グラフがない場合，

			CmdIOUtil::Output("まだグラフを生成していない．", enums::OutputDetail::kSystem);

			if (CmdIOUtil::InputYesNo("グラフを作成しますか？"))
			{
				CreateGraph(first_node, &graph);	// グラフを作成する．

				broker_ptr_->graph.SetData(graph);	// グラフ木の値を仲介人にセットする．これでGUIにグラフが表示される．
			}
			else
			{
				//終了するか質問する
				if (CmdIOUtil::InputYesNo("終了しますか？")) { break; }
			}
		}
		else
		{
			//グラフがある場合

			CmdIOUtil::Output("グラフを操作する", enums::OutputDetail::kSystem);
			CmdIOUtil::Output("操作メニューを表示します", enums::OutputDetail::kSystem);

			//操作メニューを表示する

			std::vector<std::function<void()>> func_list;	//操作をおこなう関数をラムダ式で受け取るvector

			func_list.push_back(
				[&]()
				{
					RobotStateNode selected = SelectNodeByInput(graph);
					CreateGraph(selected, &graph);
					broker_ptr_->graph.SetData(graph);
				}
			);

			func_list.push_back(
				[&]()
				{
					RobotStateNode selected = SelectNodeByInput(graph);

					CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
					CmdIOUtil::OutputHorizontalLine("*", enums::OutputDetail::kSystem);
					CmdIOUtil::Output(selected.ToString(), enums::OutputDetail::kSystem);
					CmdIOUtil::OutputHorizontalLine("*", enums::OutputDetail::kSystem);
					CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
				}
			);

			func_list.push_back(
				[&]()
				{
					graph.clear();
					broker_ptr_->graph.Clean();
					CmdIOUtil::Output("グラフを全て削除した", enums::OutputDetail::kSystem);
					CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
				}
			);


			CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
			CmdIOUtil::Output("操作を選択してください", enums::OutputDetail::kSystem);
			CmdIOUtil::Output("　0 : ノード選択し，そのノードを親にしてグラフを生成する", enums::OutputDetail::kSystem);
			CmdIOUtil::Output("　1 : ノード選択して表示する", enums::OutputDetail::kSystem);
			CmdIOUtil::Output("　2 : グラフを全削除する", enums::OutputDetail::kSystem);
			CmdIOUtil::Output("　3 : 終了する", enums::OutputDetail::kSystem);

			int selected_index =
				CmdIOUtil::InputInt(0, static_cast<int>(func_list.size()), static_cast<int>(func_list.size()), "整数で操作を選択してください．範囲外の値の場合終了します．");

			//選択された操作を実行する
			if (selected_index < func_list.size())
			{
				func_list[selected_index]();
			}
			else
			{
				if (CmdIOUtil::InputYesNo("終了しますか？")) { break; }
			}
		}

	}	//while (true)
}


void SystemMainGraphViewer::CreateGraph(const RobotStateNode parent, std::vector<RobotStateNode>* graph)
{
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::Output("グラフ木を作成する", enums::OutputDetail::kSystem);
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);

	// グラフ探索をする
	RobotStateNode parent_node = parent;
	parent_node.ChangeLootNode();

	RobotStateNode fake_result_node;

	DividedMapState devide_map;
	devide_map.Init(map_state_, parent_node.center_of_mass_global_coord);

	GaitPatternGraphTree graph_tree(1000000);
	graph_tree.Reset();
	graph_tree.AddNode(parent_node);


	graph_tree_creator_ptr_->Init(devide_map);

	stopwatch_.Start();

	GraphSearchResult result =
		graph_tree_creator_ptr_->CreateGraphTree(0, 5, &graph_tree);

	stopwatch_.End();

	// グラフ探索の結果を取得する
	for (int i = 0; i < graph_tree.GetGraphSize(); i++)
	{
		(*graph).push_back(graph_tree.GetNode(i));
	}


	// グラフ探索の結果を表示する
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::Output("グラフ探索終了", enums::OutputDetail::kSystem);
	CmdIOUtil::Output("グラフ探索にかかった時間 : " + stopwatch_.GetElapsedMilliSecondString(), enums::OutputDetail::kSystem);

	std::string res_str = magic_enum::enum_name<enums::Result>(result.result).data();
	res_str.erase(0, 1);	//先頭のkを削除する

	CmdIOUtil::Output("グラフ探索結果 : " + res_str, enums::OutputDetail::kSystem);
}

void SystemMainGraphViewer::OutputGraphStatus(const std::vector<RobotStateNode>& graph) const
{
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::OutputHorizontalLine("=", enums::OutputDetail::kSystem);
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::Output("グラフの状態を表示します．", enums::OutputDetail::kSystem);
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::Output("グラフのノードの数 : " + std::to_string(graph.size()), enums::OutputDetail::kSystem);


	if (graph.size() > 0)
	{
		//深さごとのノード数を記録する

		std::vector<int> depth_num(GraphSearchConst::kMaxDepth + 1);

		CmdIOUtil::Output("SystemMainGraphViewer : グラフ探索の最大深さ : " + std::to_string(GraphSearchConst::kMaxDepth), enums::OutputDetail::kSystem);

		for (const auto& i : graph)
		{
			if (i.depth < depth_num.size())
			{
				depth_num[i.depth]++;
			}
		}

		//深さごとのノード数を表示する

		int depth_cnt = 0;

		for (const auto& i : depth_num)
		{
			CmdIOUtil::Output("・深さ" + std::to_string(depth_cnt) + " : " + std::to_string(i), enums::OutputDetail::kSystem);
			depth_cnt++;
		}
	}
	else
	{
		CmdIOUtil::Output("グラフが空なので，深さごとのノード数を表示できません．", enums::OutputDetail::kSystem);
	}

	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::OutputHorizontalLine("=", enums::OutputDetail::kSystem);
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
}

RobotStateNode SystemMainGraphViewer::SelectNodeByInput(const std::vector<RobotStateNode>& graph) const
{
	CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
	CmdIOUtil::Output("ノードを選択する", enums::OutputDetail::kSystem);

	if (graph.size() == 0)
	{
		CmdIOUtil::Output("グラフが空なので，初期状態のノードを返す", enums::OutputDetail::kSystem);

		NodeInitializer node_initializer{ Vector3{0.f,0.f,30.f}, enums::HexapodMove::kNone };
		RobotStateNode first_node = node_initializer.InitNode();

		return first_node;
	}
	else
	{
		CmdIOUtil::Output("グラフの中から1つのノードを選択してください．", enums::OutputDetail::kSystem);

		//ノードを選択する
		int selected_node_index = CmdIOUtil::InputInt(0, static_cast<int>(graph.size()) - 1, 0, "整数でノードを選択してください．");

		CmdIOUtil::Output("選択されたノード，" + std::to_string(selected_node_index) + "番を親にする．", enums::OutputDetail::kSystem);

		return graph[selected_node_index];
	}
}

}	//namespace designlab