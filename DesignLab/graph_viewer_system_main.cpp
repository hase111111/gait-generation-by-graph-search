#include "graph_viewer_system_main.h"

#include <iostream>

#include <boost/thread.hpp>

#include "define.h"
#include "cmdio_util.h"
#include "stopwatch.h"
#include "graph_search_const.h"
#include "pass_finder_basic.h"
#include "phantomx_state_calculator.h"
#include "StringToValue.h"


// 二度と追記しないだろうと全てをべた書きしています．
// めちゃくちゃ読みづらいだろうと思うけど，許して．

namespace dlio = designlab::cmdio;
using StrtoVal::StrToInt;


GraphViewerSystemMain::GraphViewerSystemMain(
	std::unique_ptr<IPassFinder>&& pass_finder_ptr,
	std::unique_ptr<IGraphicMain>&& graphic_main_ptr,
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr) :
	graphic_system_(std::move(graphic_main_ptr), setting_ptr),
	pass_finder_ptr_(std::move(pass_finder_ptr)),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	dlio::OutputTitle("グラフ確認モード");	//タイトルを表示する

	//マップを生成する
	std::cout << "GraphViewerSystemMain : マップを生成します．" << std::endl;
	std::cout << "GraphViewerSystemMain : オプションを入力してください" << std::endl;
	MapCreator::PrintAllMapCreateMode();
	std::string _mode;
	std::cout << std::endl << "input : ";
	std::cin >> _mode;
	std::cout << std::endl;

	MapCreator::printAllMapCreateOption();
	std::string _option;
	std::cout << std::endl << "input : ";
	std::cin >> _option;
	std::cout << std::endl;
	MapCreator map_creator;
	map_state_ = map_creator.Create(static_cast<MapCreateMode>(StrToInt(_mode)), StrToInt(_option));
	std::cout << "MapCreator : マップを生成しました．" << std::endl << std::endl;


	//仲介人を初期化する
	std::cout << "GraphicDataBroker_Old : 仲介人を初期化します．" << std::endl << std::endl;
	broker_ptr_->map_state.SetData(map_state_);
}


void GraphViewerSystemMain::Main()
{
	//グラフィックシステムを起動する
	dlio::Output("別スレッドでGUIを起動します．", OutputDetail::kInfo);

	boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system_);

	//ノードを初期化する
	std::cout << "GraphViewerSystemMain : ノードを初期化します．" << std::endl << std::endl;
	RobotStateNode _node;
	_node.Init(false);
	std::cout << _node.ToString();
	std::cout << std::endl;

	std::vector<RobotStateNode> _graph;

	while (true)
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << std::endl;
		showGraphStatus(_graph);

		if (_graph.size() == 0)
		{
			std::cout << "GraphViewerSystemMain : まだグラフを生成していません" << std::endl;

			if (askYesNo("GraphViewerSystemMain : グラフを作成しますか？"))
			{
				if (!pass_finder_ptr_)
				{
					std::cout << "GraphViewerSystemMain : グラフ木作成クラスが初期化されていません" << std::endl;
					std::cout << "GraphViewerSystemMain : プログラムを終了します" << std::endl;
					break;
				}

				std::cout << "IGraphTreeCreator : グラフを作成します" << std::endl;

				Stopwatch _timer;
				_timer.Start();
				CreateGraph(_node, _graph);
				_timer.End();
				SetGraphToBroker(_graph);
				std::cout << "IGraphTreeCreator : グラフを作成しました" << std::endl;
				std::cout << "IGraphTreeCreator : グラフ作成にかかった時間 : " << _timer.GetElapsedMilliSecond() << " [ms]" << std::endl;
				std::cout << std::endl;
			}
			else
			{
				//終了するか質問する
				if (askYesNo("GraphViewerSystemMain : 終了しますか？")) { break; }
			}
		}
		else
		{
			std::cout << "GraphViewerSystemMain : グラフを操作します" << std::endl;

			//操作メニューを表示する
			std::cout << "GraphViewerSystemMain : 操作メニューを表示します" << std::endl;
			std::cout << "GraphViewerSystemMain : 1 : ノード選択し，そのノードを親にしてグラフを生成する" << std::endl;
			std::cout << "GraphViewerSystemMain : 2 : ノード選択して表示する" << std::endl;
			std::cout << "GraphViewerSystemMain : 3 : グラフを全削除する" << std::endl;
			std::cout << "GraphViewerSystemMain : other : 終了する" << std::endl;
			std::cout << std::endl;
			std::cout << "input : ";
			std::string _str;
			std::cin >> _str;
			int _menu = StrToInt(_str);
			std::cout << std::endl;

			if (_menu == 1 || _menu == 2)
			{
				std::cout << "GraphViewerSystemMain : ノードを選択してください" << std::endl;
				std::cout << "GraphViewerSystemMain : 0 ～ " << _graph.size() - 1 << " の数字を入力してください" << std::endl;
				std::cout << std::endl;
				std::cout << "input : ";

				std::string _str_node;
				std::cin >> _str_node;
				int _node_num = StrToInt(_str_node);
				std::cout << std::endl;

				if (_node_num < 0 || _node_num >= _graph.size())
				{
					std::cout << "GraphViewerSystemMain : 無効なノード番号です" << std::endl;
					std::cout << std::endl;
					continue;
				}
				else
				{
					if (_menu == 1)
					{
						std::cout << "--------------------------------------------------" << std::endl;
						std::cout << "GraphViewerSystemMain : ノードを選択し，そのノードを親にしてグラフを生成します" << std::endl;
						std::cout << std::endl;
						std::cout << _graph[_node_num].ToString();
						std::cout << std::endl;
						std::cout << "IGraphTreeCreator : グラフを作成します" << std::endl;

						Stopwatch _timer;
						_timer.Start();
						CreateGraph(_graph[_node_num], _graph);
						_timer.End();
						SetGraphToBroker(_graph);
						std::cout << "IGraphTreeCreator : グラフを作成しました" << std::endl;
						std::cout << "IGraphTreeCreator : グラフ作成にかかった時間 : " << _timer.GetElapsedMilliSecond() << " [ms]" << std::endl;
						std::cout << std::endl;
					}
					else
					{
						std::cout << "--------------------------------------------------" << std::endl;
						std::cout << "GraphViewerSystemMain : ノードを表示します" << std::endl;
						std::cout << std::endl;
						std::cout << _graph[_node_num].ToString();
						std::cout << std::endl;
					}
				}
			}
			else if (_menu == 3)
			{
				broker_ptr_->graph.Clean();
				_graph.clear();
				std::cout << "GraphViewerSystemMain : グラフを全削除しました" << std::endl;
				std::cout << std::endl;
			}
			else
			{
				//終了するか質問する
				dlio::Output("終了します．", OutputDetail::kSystem);

				if (dlio::InputYesNo()) { break; }
			}
		}
	}
}

void GraphViewerSystemMain::CreateGraph(const RobotStateNode parent, std::vector<RobotStateNode>& graph)
{
	RobotStateNode parent_node = parent;
	parent_node.ChangeParentNode();

	STarget target;
	target.TargetMode = ETargetMode::StraightPosition;
	target.TargetPosition = { 100000,0,0 };
	target.RotationCenter = { 0,100000,0 };

	RobotStateNode fake_result_node;

	GraphSearchResult result =
		pass_finder_ptr_->GetNextNodebyGraphSearch(parent_node, map_state_, target, &fake_result_node);

	graph.clear();

	pass_finder_ptr_->GetGraphTree(&graph);

	if(result != GraphSearchResult::kSuccess)std::cout << fake_result_node.ToString();
}

void GraphViewerSystemMain::SetGraphToBroker(const std::vector<RobotStateNode>& _graph)
{
	broker_ptr_->graph.Clean();

	for (auto& i : _graph)
	{
		broker_ptr_->graph.PushBack(i);
	}
}

bool GraphViewerSystemMain::askYesNo(const std::string& question) const
{
	std::cout << question << " ( y / n )" << std::endl;
	std::cout << std::endl;
	std::cout << "input : ";
	std::string _input;
	std::cin >> _input;
	std::cout << std::endl;

	if (_input == "y" || _input == "Y" || _input == "yes" || _input == "Yes") { return true; }
	return false;
}

void GraphViewerSystemMain::showGraphStatus(const std::vector<RobotStateNode>& _graph) const
{
	std::cout << "GraphViewerSystemMain : グラフの状態を表示します．" << std::endl;
	std::cout << "GraphViewerSystemMain : グラフのノード数 : " << _graph.size() << std::endl;

	if (_graph.size() > 0)
	{
		std::vector<int> _depth_num((size_t)GraphSearchConst::kMaxDepth + 1);

		std::cout << "GraphViewerSystemMain : グラフ探索の最大深さ : " << (int)GraphSearchConst::kMaxDepth << std::endl;

		for (const auto& i : _graph)
		{
			_depth_num.at(static_cast<size_t>(i.depth))++;
		}

		int _cnt = 0;

		for (const auto& i : _depth_num)
		{
			std::cout << "GraphViewerSystemMain : 深さ" << _cnt << " : " << i << std::endl;
			_cnt++;
		}
	}

	std::cout << std::endl;
}
