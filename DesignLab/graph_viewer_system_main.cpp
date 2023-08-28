#include "graph_viewer_system_main.h"

#include <iostream>

#include <boost/thread.hpp>

#include "viewer_graphic_main_builder.h"
#include "pass_finder_hato_thread.h"
#include "pass_finder_factory_hato.h"
#include "hexapod_state_calculator.h"
#include "Define.h"
#include "designlab_timer.h"
#include "StringToValue.h"


//二度と追記しないだろうと全てをべた書きしています．
//めちゃくちゃ読みづらいだろうと思うけど，許して．

using StrtoVal::StrToInt;

GraphViewerSystemMain::GraphViewerSystemMain(const SApplicationSettingRecorder* const setting) : mp_setting(setting)
{
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "     GraphViewerSystemMain : GraphViewer起動" << std::endl;
	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;

	//ロボットのデータを初期化する．
	HexapodStateCalclator::initLegR();

	//マップを生成する
	std::cout << "GraphViewerSystemMain : マップを生成します．" << std::endl;
	std::cout << "GraphViewerSystemMain : オプションを入力してください" << std::endl;
	MapCreator::printAllMapCreateMode();
	std::string _mode;
	std::cout << std::endl << "input : ";
	std::cin >> _mode;
	std::cout << std::endl;

	MapCreator::printAllMapCreateOption();
	std::string _option;
	std::cout << std::endl << "input : ";
	std::cin >> _option;
	std::cout << std::endl;
	m_map_state.init(static_cast<EMapCreateMode>(StrToInt(_mode)), StrToInt(_option), false);
	std::cout << "MapCreator : マップを生成しました．" << std::endl << std::endl;

	//仲介人を初期化する
	std::cout << "GraphicDataBroker : 仲介人を初期化します．" << std::endl << std::endl;
	m_graphic_data_broker.setMapState(m_map_state);

	//グラフィックシステムを初期化する
	std::cout << "GraphicSystem : グラフィックシステムを初期化します．" << std::endl << std::endl;
	m_graphic_system.init(std::make_unique<ViewerGraphicMainBuilder>(), &m_graphic_data_broker, setting);

	//グラフ木作成クラスを初期化する
	std::cout << "GraphCreator : グラフ木作成クラスを初期化します．" << std::endl << std::endl;

	mp_pass_finder = std::make_unique<PassFinderHatoThread>(std::make_unique<PassFinderFactoryHato>());

	//初期化終了
	std::cout << "GraphViewerSystemMain : GraphViewer初期化終了．起動します" << std::endl << std::endl;

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "                   GraphViewer" << std::endl;
	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;
}


void GraphViewerSystemMain::main()
{
	//グラフィックシステムを起動する
	std::cout << "GraphicSystem : 別スレッドでグラフィックシステムを起動します．" << std::endl << std::endl;
	boost::thread _thread_graphic(&GraphicSystem::main, &m_graphic_system);

	//ノードを初期化する
	std::cout << "GraphViewerSystemMain : ノードを初期化します．" << std::endl << std::endl;
	SNode _node;
	_node.init(false);
	std::cout << _node;
	std::cout << std::endl;

	std::vector<SNode> _graph;

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
				if (!mp_pass_finder)
				{
					std::cout << "GraphViewerSystemMain : グラフ木作成クラスが初期化されていません" << std::endl;
					std::cout << "GraphViewerSystemMain : プログラムを終了します" << std::endl;
					break;
				}

				std::cout << "IGraphTreeCreator : グラフを作成します" << std::endl;

				DesignlabTimer _timer;
				_timer.start();
				createGraph(_node, _graph);
				_timer.end();
				setGraphToBroker(_graph);
				std::cout << "IGraphTreeCreator : グラフを作成しました" << std::endl;
				std::cout << "IGraphTreeCreator : グラフ作成にかかった時間 : " << _timer.getMilliSecond() << " [ms]" << std::endl;
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
						std::cout << _graph[_node_num];
						std::cout << std::endl;
						std::cout << "IGraphTreeCreator : グラフを作成します" << std::endl;

						DesignlabTimer _timer;
						_timer.start();
						createGraph(_graph[_node_num], _graph);
						_timer.end();
						setGraphToBroker(_graph);
						std::cout << "IGraphTreeCreator : グラフを作成しました" << std::endl;
						std::cout << "IGraphTreeCreator : グラフ作成にかかった時間 : " << _timer.getMilliSecond() << " [ms]" << std::endl;
						std::cout << std::endl;
					}
					else
					{
						std::cout << "--------------------------------------------------" << std::endl;
						std::cout << "GraphViewerSystemMain : ノードを表示します" << std::endl;
						std::cout << std::endl;
						std::cout << _graph[_node_num];
						std::cout << std::endl;
					}
				}
			}
			else if (_menu == 3)
			{
				m_graphic_data_broker.deleteAllNode();
				_graph.clear();
				std::cout << "GraphViewerSystemMain : グラフを全削除しました" << std::endl;
				std::cout << std::endl;
			}
			else
			{
				//終了するか質問する
				if (askYesNo("GraphViewerSystemMain : 終了しますか？")) { break; }
			}
		}
	}
}

void GraphViewerSystemMain::createGraph(const SNode parent, std::vector<SNode>& graph)
{
	SNode parent_node = parent;
	parent_node.changeParentNode();

	STarget target;
	target.TargetMode = ETargetMode::StraightPosition;
	target.TargetPosition = { 100000,0,0 };
	target.RotationCenter = { 0,100000,0 };

	SNode fake_result_node;

	mp_pass_finder->getNextNodebyGraphSearch(parent_node, &m_map_state, target, fake_result_node);

	mp_pass_finder->getGraphTree(&graph);

	std::cout << fake_result_node;
}

void GraphViewerSystemMain::setGraphToBroker(const std::vector<SNode>& _graph)
{
	m_graphic_data_broker.deleteAllNode();

	for (auto& i : _graph)
	{
		m_graphic_data_broker.pushNode(i);
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

void GraphViewerSystemMain::showGraphStatus(const std::vector<SNode>& _graph) const
{
	std::cout << "GraphViewerSystemMain : グラフの状態を表示します．" << std::endl;
	std::cout << "GraphViewerSystemMain : グラフのノード数 : " << _graph.size() << std::endl;

	if (_graph.size() > 0)
	{
		std::vector<int> _depth_num((size_t)Define::GRAPH_SEARCH_DEPTH + 1);

		std::cout << "GraphViewerSystemMain : グラフ探索の最大深さ : " << (int)Define::GRAPH_SEARCH_DEPTH << std::endl;

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
