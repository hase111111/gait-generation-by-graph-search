#include "system_main_graph_viewer.h"

#include <bitset>
#include <iostream>

#include <boost/thread.hpp>
#include <magic_enum.hpp>

#include "cmdio_util.h"
#include "designlab_string_util.h"
#include "gait_pattern_generator_basic.h"
#include "graph_search_const.h"
#include "node_initializer.h"
#include "phantomx_mk2.h"


namespace designlab
{

SystemMainGraphViewer::SystemMainGraphViewer(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator)),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	CmdIOUtil::OutputTitle("グラフ確認モード");	//タイトルを表示する

	//マップを生成する
	CmdIOUtil::Output("まずは，マップを生成する．オプションを整数で入力すること．", enums::OutputDetail::kSystem);

	SimulationMapParameter messanger = InputMapCreateMode();

	MapCreatorForSimulation map_creator(messanger);

	map_state_ = map_creator.InitMap();

	broker_ptr_->map_state.SetData(map_state_);	//仲介人を初期化する
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

SimulationMapParameter SystemMainGraphViewer::InputMapCreateMode() const
{
	SimulationMapParameter messanger;

	{
		const auto kMapCreateModeList = magic_enum::enum_values<enums::SimulationMapMode>();	//MapCreateModeのリストを取得する

		CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
		CmdIOUtil::Output("MapCreateModeを選択", enums::OutputDetail::kSystem);

		//MapCreateModeの一覧を出力する．
		for (int i = 0; i < kMapCreateModeList.size(); i++)
		{
			const std::string name = string_util::EnumToStringRemoveTopK(kMapCreateModeList[i]);	//MapCreateModeの名前を取得する

			CmdIOUtil::Output(std::to_string(i) + " : " + name, enums::OutputDetail::kSystem);
		}

		const int selected_mode_index = CmdIOUtil::InputInt(0, static_cast<int>(kMapCreateModeList.size()) - 1, 0);	//MapCreateModeのindexを入力させる

		messanger.mode = kMapCreateModeList[selected_mode_index];
	}

	{
		const auto kMapCreateOptionList = magic_enum::enum_values<enums::SimulationMapOption>();	//MapCreateOptionのリストを取得する

		//MapCreateOptionの合計値を計算する
		unsigned int option_sum = 0;

		for (const auto i : kMapCreateOptionList)
		{
			option_sum += static_cast<unsigned int>(i);
		}

		CmdIOUtil::OutputNewLine(1, enums::OutputDetail::kSystem);
		CmdIOUtil::Output("MapCreateOptionを選択 (複数指定したい場合は値を足し算すること)", enums::OutputDetail::kSystem);

		//MapCreateOptionの一覧を出力する．
		for (int i = 0; i < kMapCreateOptionList.size(); i++)
		{
			const std::string name = string_util::EnumToStringRemoveTopK(kMapCreateOptionList[i]);	//MapCreateOptionのリストを取得する

			unsigned int option_value = static_cast<unsigned int>(kMapCreateOptionList[i]);

			std::bitset<magic_enum::enum_count<enums::SimulationMapOption>()> bit(option_value);

			CmdIOUtil::Output(std::to_string(option_value) + " : " + name + " (" + bit.to_string() + ")", enums::OutputDetail::kSystem);
		}

		messanger.option = static_cast<unsigned int>(CmdIOUtil::InputInt(0, option_sum, 0));	//MapCreateOptionの合計値を入力させる
	}

	return messanger;
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