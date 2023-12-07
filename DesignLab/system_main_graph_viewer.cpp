#include "system_main_graph_viewer.h"

#include <bitset>
#include <iostream>

#include <boost/thread.hpp>
#include <magic_enum.hpp>

#include "cmdio_util.h"
#include "define.h"
#include "designlab_string_util.h"
#include "gait_pattern_generator_basic.h"
#include "graph_search_const.h"
#include "node_initializer.h"
#include "phantomx_mk2.h"


namespace dlio = designlab::cmdio;
namespace dlsu = designlab::string_util;


SystemMainGraphViewer::SystemMainGraphViewer(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator)),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	dlio::OutputTitle("グラフ確認モード");	//タイトルを表示する

	//マップを生成する
	dlio::Output("まずは，マップを生成する．オプションを整数で入力すること．", OutputDetail::kSystem);

	MapCreateModeMessenger messanger = InputMapCreateMode();

	MapCreatorForSimulation map_creator(messanger);

	map_state_ = map_creator.InitMap();

	broker_ptr_->map_state.SetData(map_state_);	//仲介人を初期化する
}


void SystemMainGraphViewer::Main()
{
	//早期リターン
	if (!graph_tree_creator_ptr_)
	{
		dlio::Output("グラフ木作成クラスが初期化されていない．終了する", OutputDetail::kError);
		return;
	}

	dlio::Output("別スレッドでGUIを起動する．", OutputDetail::kInfo);

	//ノードを初期化する
	dlio::Output("ノードを初期化する．", OutputDetail::kSystem);

	NodeInitializer node_initializer;
	RobotStateNode first_node = node_initializer.InitNode();
	std::vector<RobotStateNode> graph;


	while (true)
	{
		OutputGraphStatus(graph);

		if (graph.size() == 0)
		{
			// グラフがない場合，

			dlio::Output("まだグラフを生成していない．", OutputDetail::kSystem);

			if (dlio::InputYesNo("グラフを作成しますか？"))
			{
				CreateGraph(first_node, &graph);	// グラフを作成する．

				broker_ptr_->graph.SetData(graph);	// グラフ木の値を仲介人にセットする．これでGUIにグラフが表示される．
			}
			else
			{
				//終了するか質問する
				if (dlio::InputYesNo("終了しますか？")) { break; }
			}
		}
		else
		{
			//グラフがある場合

			dlio::Output("グラフを操作する", OutputDetail::kSystem);
			dlio::Output("操作メニューを表示します", OutputDetail::kSystem);

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

					dlio::OutputNewLine(1, OutputDetail::kSystem);
					dlio::OutputHorizontalLine("*", OutputDetail::kSystem);
					dlio::Output(selected.ToString(), OutputDetail::kSystem);
					dlio::OutputHorizontalLine("*", OutputDetail::kSystem);
					dlio::OutputNewLine(1, OutputDetail::kSystem);
				}
			);

			func_list.push_back(
				[&]()
				{
					graph.clear();
					broker_ptr_->graph.Clean();
					dlio::Output("グラフを全て削除した", OutputDetail::kSystem);
					dlio::OutputNewLine(1, OutputDetail::kSystem);
				}
			);


			dlio::OutputNewLine(1, OutputDetail::kSystem);
			dlio::Output("操作を選択してください", OutputDetail::kSystem);
			dlio::Output("　0 : ノード選択し，そのノードを親にしてグラフを生成する", OutputDetail::kSystem);
			dlio::Output("　1 : ノード選択して表示する", OutputDetail::kSystem);
			dlio::Output("　2 : グラフを全削除する", OutputDetail::kSystem);
			dlio::Output("　3 : 終了する", OutputDetail::kSystem);

			int selected_index =
				dlio::InputInt(0, static_cast<int>(func_list.size()), static_cast<int>(func_list.size()), "整数で操作を選択してください．範囲外の値の場合終了します．");

			//選択された操作を実行する
			if (selected_index < func_list.size())
			{
				func_list[selected_index]();
			}
			else
			{
				if (dlio::InputYesNo("終了しますか？")) { break; }
			}
		}

	}	//while (true)
}


void SystemMainGraphViewer::CreateGraph(const RobotStateNode parent, std::vector<RobotStateNode>* graph)
{
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("グラフ木を作成する", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);

	// グラフ探索をする
	RobotStateNode parent_node = parent;
	parent_node.ChangeLootNode();

	TargetRobotState target;
	RobotStateNode fake_result_node;

	DevideMapState devide_map;
	devide_map.Init(map_state_, parent_node.global_center_of_mass);

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
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("グラフ探索終了", OutputDetail::kSystem);
	dlio::Output("グラフ探索にかかった時間 : " + stopwatch_.GetElapsedMilliSecondString(), OutputDetail::kSystem);

	std::string res_str = magic_enum::enum_name<GraphSearchResult>(result).data();
	res_str.erase(0, 1);	//先頭のkを削除する

	dlio::Output("グラフ探索結果 : " + res_str, OutputDetail::kSystem);
}

void SystemMainGraphViewer::OutputGraphStatus(const std::vector<RobotStateNode>& graph) const
{
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::OutputHorizontalLine("=", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("グラフの状態を表示します．", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("グラフのノードの数 : " + std::to_string(graph.size()), OutputDetail::kSystem);


	if (graph.size() > 0)
	{
		//深さごとのノード数を記録する

		std::vector<int> depth_num(GraphSearchConst::kMaxDepth + 1);

		dlio::Output("SystemMainGraphViewer : グラフ探索の最大深さ : " + std::to_string(GraphSearchConst::kMaxDepth), OutputDetail::kSystem);

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
			dlio::Output("・深さ" + std::to_string(depth_cnt) + " : " + std::to_string(i), OutputDetail::kSystem);
			depth_cnt++;
		}
	}
	else
	{
		dlio::Output("グラフが空なので，深さごとのノード数を表示できません．", OutputDetail::kSystem);
	}

	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::OutputHorizontalLine("=", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
}

MapCreateModeMessenger SystemMainGraphViewer::InputMapCreateMode() const
{
	MapCreateModeMessenger messanger;

	{
		const auto kMapCreateModeList = magic_enum::enum_values<MapCreateMode>();	//MapCreateModeのリストを取得する

		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("MapCreateModeを選択", OutputDetail::kSystem);

		//MapCreateModeの一覧を出力する．
		for (int i = 0; i < kMapCreateModeList.size(); i++)
		{
			const std::string name = dlsu::EnumToStringRemoveTopK(kMapCreateModeList[i]);	//MapCreateModeの名前を取得する

			dlio::Output(std::to_string(i) + " : " + name, OutputDetail::kSystem);
		}

		const int selected_mode_index = dlio::InputInt(0, static_cast<int>(kMapCreateModeList.size()) - 1, 0);	//MapCreateModeのindexを入力させる

		messanger.mode = kMapCreateModeList[selected_mode_index];
	}

	{
		const auto kMapCreateOptionList = magic_enum::enum_values<MapCreateOption>();	//MapCreateOptionのリストを取得する

		//MapCreateOptionの合計値を計算する
		unsigned int option_sum = 0;

		for (const auto i : kMapCreateOptionList)
		{
			option_sum += static_cast<unsigned int>(i);
		}

		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("MapCreateOptionを選択 (複数指定したい場合は値を足し算すること)", OutputDetail::kSystem);

		//MapCreateOptionの一覧を出力する．
		for (int i = 0; i < kMapCreateOptionList.size(); i++)
		{
			const std::string name = dlsu::EnumToStringRemoveTopK(kMapCreateOptionList[i]);	//MapCreateOptionのリストを取得する

			unsigned int option_value = static_cast<unsigned int>(kMapCreateOptionList[i]);

			std::bitset<magic_enum::enum_count<MapCreateOption>()> bit(option_value);

			dlio::Output(std::to_string(option_value) + " : " + name + " (" + bit.to_string() + ")", OutputDetail::kSystem);
		}

		messanger.option = static_cast<unsigned int>(dlio::InputInt(0, option_sum, 0));	//MapCreateOptionの合計値を入力させる
	}

	return messanger;
}

RobotStateNode SystemMainGraphViewer::SelectNodeByInput(const std::vector<RobotStateNode>& graph) const
{
	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("ノードを選択する", OutputDetail::kSystem);

	if (graph.size() == 0)
	{
		dlio::Output("グラフが空なので，初期状態のノードを返す", OutputDetail::kSystem);

		NodeInitializer node_initializer;
		RobotStateNode first_node = node_initializer.InitNode();

		return first_node;
	}
	else
	{
		dlio::Output("グラフの中から1つのノードを選択してください．", OutputDetail::kSystem);

		//ノードを選択する
		int selected_node_index = dlio::InputInt(0, static_cast<int>(graph.size()) - 1, 0, "整数でノードを選択してください．");

		dlio::Output("選択されたノード，" + std::to_string(selected_node_index) + "番を親にする．", OutputDetail::kSystem);

		return graph[selected_node_index];
	}
}