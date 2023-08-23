#include "system_main.h"

#include <boost/thread.hpp>

#include "Define.h"
#include "designlab_math.h"
#include "my_timer.h"
#include "CmdIO.h"
#include "hexapod.h"
#include "hexapod_state_calculator.h"
#include "node_validity_checker.h"
#include "graphic_main_basic.h"
#include "graphic_main_test.h"


SystemMain::SystemMain(std::unique_ptr<IPassFinder>&& graph_search)
{
	//ロボットのデータを初期化する．
	Hexapod::makeLegROM_r();
	HexapodStateCalclator::initLegR();

	//マップを生成する．
	m_map_state.init(EMapCreateMode::FLAT, MapCreator::OPTION_STEP, true);

	//仲介人にマップを渡す．
	m_broker.setMapState(m_map_state);

	//グラフ探索クラスをセットする
	mp_pass_finder = std::move(graph_search);

	//画像ウィンドウを表示するクラスに仲介人のアドレスを渡して，初期化処理をする．
	m_graphic_system.init(std::make_unique<GraphicMainBasic>(&m_broker));

	//この探索での目標を設定する．
	m_target.TargetMode = ETargetMode::StraightPosition;
	m_target.TargetPosition = { 3000,0,0 };
}

void SystemMain::main()
{
	if (!mp_pass_finder)
	{
		//グラフ探索クラスがセットされていない場合は，エラーを出力して終了する．
		std::cout << "GraphSearch is not set." << std::endl;
		return;
	}

	CmdIO _cmd;	//コマンドラインに文字を描画するクラスを用意する．

	NodeValidityChecker node_checker;	//ノードの妥当性をチェックするクラスを用意する．

	//画像表示ウィンドウを別スレッドで立ち上げる．初期化に失敗したり，そもそも画像表示をしない設定になっていると立ち上がらない．
	boost::thread graphic_thread(&GraphicSystem::main, &m_graphic_system);


	//シミュレーションを行う回数分ループする．
	for (int i = 0; i < Define::SIMURATE_NUM; i++)
	{
		SNode current_node;										//現在のノードの状態を格納する変数．
		const bool _do_random_init = (i == 0) ? false : true;	// i の値が 0 ならばランダムな場所に初期化はしない．(i == 0)を評価して，trueならば前者(false)，falseならば後者(true)を代入する．
		current_node.init(_do_random_init);

		if (Define::FLAG_GRAPHIC_AVAILABLE) { m_broker.pushNode(current_node); }	//グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．

		_cmd.outputGraphSearchStaretMessage(i + 1);	//コマンドラインに開始時のメッセージを出力する．
		_cmd.outputNode(current_node, 0);			//コマンドラインに最初のノードの状態を出力する．


		//最大歩容生成回数分までループする．
		for (int i = 0; i < Define::GATE_PATTERN_GENERATE_NUM; i++)
		{
			MyTimer timer;		//タイマーを用意する．


			timer.start();		//タイマースタート

			SNode result_node;	//グラフ探索の結果を格納する変数．

			EGraphSearchResult result_state = mp_pass_finder->getNextNodebyGraphSearch(current_node, &m_map_state, m_target, result_node);		//グラフ探索を行う．

			timer.end();		//タイマーストップ


			if (!graphSeachResultIsSuccessful(result_state))
			{
				_cmd.outputErrorMessageInGraphSearch("Failed to generate the next gait.");

				break;	//次の歩容が生成できなかったら，このループを抜け，次のシミュレーションへ進む．
			}


			current_node = result_node;		//次の歩容が生成できているならば，ノードを更新する．

			if (Define::FLAG_GRAPHIC_AVAILABLE) { m_broker.pushNode(current_node); }			//グラフィックが有効ならば仲介人に結果を通達する．

			_cmd.outputNode(current_node, i + 1);												//コマンドラインに現在のノードを出力する．

			node_checker.setNode(current_node);													//動作チェッカーにもノードを通達する．


			if (node_checker.isLoopMove())
			{
				_cmd.outputErrorMessageInGraphSearch("Motion stuck in a loop.");

				break;	//動作がループしてしまっているならば，ループを一つ抜け，次のシミュレーションへ進む．
			}

			if (current_node.global_center_of_mass.x > Define::GOAL_TAPE) { break; }
		}


	}

	//画像表示ウィンドウの終了を待つ．
	std::cout << "Waiting for dxlib to finish." << std::endl;
	graphic_thread.join();
}
