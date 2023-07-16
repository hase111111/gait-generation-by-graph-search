#include "GraphViewerSystemMain.h"
#include "HexapodStateCalculator.h"
#include <iostream>
#include <boost/thread.hpp>


//二度と追記しないだろうと全てをべた書きしています．
//めちゃくちゃ読みづらいだろうと思うけど，許して．


GraphViewerSystemMain::GraphViewerSystemMain()
{
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "GraphViewerSystemMain : GraphViewer起動" << std::endl;
	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << std::endl;

	//ロボットのデータを初期化する．
	HexapodStateCalclator::initLegR();

	//マップを生成する
	std::cout << "GraphViewerSystemMain : マップを生成します．" << std::endl;
	std::cout << "GraphViewerSystemMain : オプションを入力してください" << std::endl;
	MapCreator::printAllMapCreateMode();
	int _mode = 0;
	std::cout << std::endl << "input : ";
	std::cin >> _mode;
	std::cout << std::endl;

	MapCreator::printAllMapCreateOption();
	int _option = 0;
	std::cout << std::endl << "input : ";
	std::cin >> _option;
	std::cout << std::endl;
	m_MapState.init(static_cast<EMapCreateMode>(_mode), _option, false);
	std::cout << "MapCreator : マップを生成しました．" << std::endl << std::endl;

	//仲介人を初期化する
	std::cout << "GraphicDataBroker : 仲介人を初期化します．" << std::endl << std::endl;
	m_GraphicDataBroker.setMapState(m_MapState);

	//グラフィックシステムを初期化する
	std::cout << "GraphicSystem : グラフィックシステムを初期化します．" << std::endl << std::endl;
	m_GraphicSystem.init(&m_GraphicDataBroker);

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
	boost::thread _thread_graphic(&GraphicSystem::main, &m_GraphicSystem);

	//ノードを初期化する
	std::cout << "GraphViewerSystemMain : ノードを初期化します．" << std::endl << std::endl;
	SNode _node;
	_node.init(false);
	std::cout << _node;
	std::cout << std::endl;

	std::vector<SNode> _graph;

	while (true)
	{
		showGraphStatus(_graph);

		if (_graph.size() == 0)
		{
			std::cout << "GraphViewerSystemMain : まだグラフを生成していません" << std::endl;
			if (askYesNo("GraphViewerSystemMain : グラフを作成しますか？")) { break; }
		}
		else
		{
			std::cout << "GraphViewerSystemMain : グラフを操作します" << std::endl;
		}

		//終了するか質問する
		if (askYesNo("GraphViewerSystemMain : 終了しますか？")) { break; }
	}
}

bool GraphViewerSystemMain::askYesNo(const std::string& question) const
{
	std::cout << question << " ( y / n )" << std::endl;
	std::cout << "input : ";
	char _input;
	std::cin >> _input;
	std::cout << std::endl;

	if (_input == 'y') { return true; }
	return false;
}

void GraphViewerSystemMain::showGraphStatus(const std::vector<SNode>& _graph) const
{
	std::cout << "GraphViewerSystemMain : グラフの状態を表示します．" << std::endl;
	std::cout << "GraphViewerSystemMain : グラフのノード数 : " << _graph.size() << std::endl;
	std::cout << std::endl;
}
