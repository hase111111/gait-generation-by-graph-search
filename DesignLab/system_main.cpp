#include "system_main.h"

#include <boost/thread.hpp>

#include "Define.h"
#include "designlab_math.h"
#include "designlab_cmdio.h"
#include "hexapod.h"
#include "hexapod_state_calculator.h"
#include "node_validity_checker.h"
#include "graphic_main_basic.h"
#include "graphic_main_test.h"


SystemMain::SystemMain(std::unique_ptr<AbstractPassFinder>&& graph_search, std::unique_ptr<IPassFinderFactory>&& graph_search_factory,
	std::unique_ptr<IGraphicMainBuilder>&& builder, std::shared_ptr<AbstractHexapodStateCalculator> calc, SApplicationSettingRecorder* recorder) : mp_setting(recorder)
{
	//ロボットのデータを初期化する．
	Hexapod::makeLegROM_r();
	HexapodStateCalclator_Old::initLegR();

	//結果をファイルに出力するクラスを初期化する．
	m_result_exporter.init();

	//マップを生成する．
	m_map_state.init(EMapCreateMode::FLAT, MapCreator::OPTION_SLOPE, true);

	//仲介人にマップを渡す．
	m_broker.setMapState(m_map_state);

	//グラフ探索クラスをセットする
	mp_pass_finder = std::move(graph_search);

	mp_pass_finder->init(std::move(graph_search_factory), calc, mp_setting);


	//画像ウィンドウを表示するクラスに仲介人のアドレスを渡して，初期化処理をする．
	m_graphic_system.init(std::move(builder), calc, &m_broker, mp_setting);

	//この探索での目標を設定する．
	m_target.TargetMode = ETargetMode::StraightPosition;
	m_target.TargetPosition = { 3000,0,0 };
}


void SystemMain::main()
{
	outputTitle();	//コマンドラインにタイトルを表示する．


	if (!mp_pass_finder)
	{
		//早期リターン，グラフ探索クラスがセットされていない場合は，エラーを出力して終了する．
		dl_cio::output(mp_setting, "パスファインダークラスがありません\nシミュレーションを終了します．", EOutputPriority::ERROR_MES, false, true);
		return;
	}

	if (!mp_setting)
	{
		//早期リターン，設定クラスがセットされていない場合は，エラーを出力して終了する．
		dl_cio::output(mp_setting, "設定クラスがありません\nシミュレーションを終了します．", EOutputPriority::ERROR_MES, false, true);
		return;
	}


	outputSetting();	//コマンドラインに設定を表示する．


	NodeValidityChecker node_checker;	//ノードの妥当性をチェックするクラスを用意する．


	//画像表示ウィンドウを別スレッドで立ち上げる．
	boost::thread graphic_thread(&GraphicSystem::main, &m_graphic_system);


	//シミュレーションを行う回数分ループする．
	for (int i = 0; i < Define::SIMURATE_NUM; i++)
	{
		SNode current_node;										//現在のノードの状態を格納する変数．
		const bool do_random_init = false;//(i == 0) ? false : true;	// i の値が 0 ならばランダムな場所に初期化はしない．(i == 0)を評価して，trueならば前者(false)，falseならば後者(true)を代入する．
		current_node.init(do_random_init);

		SSimulationResultRecorder record;	//シミュレーションの結果を格納する変数．
		record.result_nodes.push_back(current_node);	//シミュレーションの結果を格納する変数に現在のノードの状態を追加する．
		record.simulation_result = ESimulationResult::FAILURE_BY_NODE_LIMIT_EXCEEDED;	//シミュレーションの結果を格納する変数を成功に初期化する．


		dl_cio::output(mp_setting, "シミュレーション" + std::to_string(i + 1) + "回目を開始します", EOutputPriority::SYSTEM);
		dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::SYSTEM);
		dl_cio::output(mp_setting, "[初期ノードの状態]", EOutputPriority::INFO);
		dl_cio::output(mp_setting, std::to_string(current_node), EOutputPriority::INFO);
		dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::INFO);

		if ((*mp_setting).do_step_execution)
		{
			dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::SYSTEM);
			dl_cio::waitAnyKey(mp_setting, "キー入力でシミュレーションを開始します", EOutputPriority::SYSTEM);
		}


		if ((*mp_setting).gui_display) { m_broker.pushNode(current_node); }	//グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．


		//最大歩容生成回数分までループする．
		for (int j = 0; j < Define::GATE_PATTERN_GENERATE_NUM; j++)
		{
			m_timer.start();		//タイマースタート

			SNode result_node;		//グラフ探索の結果を格納する変数．

			EGraphSearchResult result_state = mp_pass_finder->getNextNodebyGraphSearch(current_node, &m_map_state, m_target, result_node);		//グラフ探索を行う．

			m_timer.end();			//タイマーストップ


			record.computation_time.push_back(m_timer.getMicroSecond() / 1000);	//計算時間を格納する．
			record.graph_search_results.push_back(result_state);			//グラフ探索の結果を格納する．
			record.result_nodes.push_back(result_node);	//シミュレーションの結果を格納する変数に現在のノードの状態を追加する．


			//グラフ探索に失敗
			if (!graphSeachResultIsSuccessful(result_state))
			{
				dl_cio::output(mp_setting, "シミュレーションに失敗しました．SimulationResult = " + std::to_string(ESimulationResult::FAILURE_BY_GRAPH_SEARCH) + "/ GraphSearch = " + std::to_string(result_state));

				record.simulation_result = ESimulationResult::FAILURE_BY_GRAPH_SEARCH;	//シミュレーションの結果を格納する変数を失敗に更新する．

				break;	//次の歩容が生成できなかったら，このループを抜け，次のシミュレーションへ進む．
			}


			current_node = result_node;		//次の歩容が生成できているならば，ノードを更新する．

			if ((*mp_setting).gui_display) { m_broker.pushNode(current_node); }			//グラフィックが有効ならば仲介人に結果を通達する．

			dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::INFO);
			dl_cio::outputHorizontalLine(mp_setting, false, EOutputPriority::INFO);
			dl_cio::output(mp_setting, "[ シミュレーション" + std::to_string(i + 1) + "回目 / 歩容生成" + std::to_string(j + 1) + "回目 ] ", EOutputPriority::INFO);	//現在のシミュレーションの回数をコマンドラインに出力する．
			dl_cio::output(mp_setting, std::to_string(current_node), EOutputPriority::INFO);	//現在のノードの状態をコマンドラインに出力する．

			if ((*mp_setting).do_step_execution_each_gait)
			{
				dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::SYSTEM);
				dl_cio::waitAnyKey(mp_setting, "キー入力で次の歩容を生成します", EOutputPriority::SYSTEM);
			}


			node_checker.setNode(current_node);													//動作チェッカーにもノードを通達する．


			//動作がループして失敗
			if (node_checker.isLoopMove())
			{
				dl_cio::output(mp_setting, "シミュレーションに失敗しました．SimulationResult = " + std::to_string(ESimulationResult::FAILURE_BY_LOOP_MOTION) + "/ GraphSearch = " + std::to_string(result_state));

				record.simulation_result = ESimulationResult::FAILURE_BY_LOOP_MOTION;	//シミュレーションの結果を格納する変数を失敗に更新する．

				break;	//動作がループしてしまっているならば，ループを一つ抜け，次のシミュレーションへ進む．
			}


			//成功時の処理
			if (current_node.global_center_of_mass.x > Define::GOAL_TAPE)
			{
				dl_cio::output(mp_setting, "シミュレーションに成功しました．SimulationResult = " + std::to_string(ESimulationResult::SUCCESS));

				record.simulation_result = ESimulationResult::SUCCESS;	//シミュレーションの結果を格納する変数を成功に更新する．

				break;
			}

		}	//歩容生成のループ終了

		m_result_exporter.exportResult(record);	//シミュレーションの結果をファイルに出力する．

		m_broker.setSimuEnd();	//仲介人にシミュレーション終了を通達する．

		dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::SYSTEM);
		dl_cio::outputHorizontalLine(mp_setting, true, EOutputPriority::SYSTEM);
		dl_cio::outputNewLine(mp_setting, 1, EOutputPriority::SYSTEM);

	}	//シミュレーションのループ終了


	dl_cio::outputNewLine(mp_setting);
	dl_cio::output(mp_setting, "シミュレーション終了");
	dl_cio::outputNewLine(mp_setting);


	//画像表示ウィンドウの終了を待つ．
	if ((*mp_setting).gui_display)
	{
		dl_cio::output(mp_setting, "DXlib(gui)の終了を待っています．GUIのXボタンを押してください");

		graphic_thread.join();
	}

	dl_cio::outputNewLine(mp_setting);
	dl_cio::output(mp_setting, "シミュレーションを終了します");
	dl_cio::outputNewLine(mp_setting);
	dl_cio::outputHorizontalLine(mp_setting, true);
	dl_cio::outputNewLine(mp_setting);
}


void SystemMain::outputTitle() const
{
	dl_cio::outputNewLine(mp_setting);
	dl_cio::outputHorizontalLine(mp_setting, true);
	dl_cio::outputNewLine(mp_setting);
	dl_cio::outputCenter(mp_setting, "シミュレーションモード");
	dl_cio::outputNewLine(mp_setting);
	dl_cio::outputHorizontalLine(mp_setting, true);
	dl_cio::outputNewLine(mp_setting);
}


void SystemMain::outputSetting() const
{
	dl_cio::output(mp_setting, "[設定]");
	dl_cio::outputNewLine(mp_setting);


	if ((*mp_setting).cmd_output)
	{
		dl_cio::output(mp_setting, "・コマンドラインへの出力を行います");


		dl_cio::output(mp_setting, "　　・priorityが" + std::to_string(mp_setting->cmd_permission) + "以上のもののみ出力されます");
	}
	else
	{
		dl_cio::output(mp_setting, "・コマンドラインへの出力を行いません．(priorityが" + std::to_string(EOutputPriority::SYSTEM) + "のものは例外的に出力されます)");
	}

	dl_cio::outputNewLine(mp_setting);


	if ((*mp_setting).do_step_execution)
	{
		dl_cio::output(mp_setting, "・ステップ実行を行います");
	}
	else
	{
		dl_cio::output(mp_setting, "・ステップ実行を行いません");
	}

	dl_cio::outputNewLine(mp_setting);


	if ((*mp_setting).do_step_execution_each_gait)
	{
		dl_cio::output(mp_setting, "・各歩容をステップ実行時にします");
	}
	else
	{
		dl_cio::output(mp_setting, "・各歩容をステップ実行時にしません");
	}

	dl_cio::outputNewLine(mp_setting);

	if ((*mp_setting).gui_display)
	{
		dl_cio::output(mp_setting, "・GUIを表示します");
	}
	else
	{
		dl_cio::output(mp_setting, "・GUIを表示しません");
	}

	dl_cio::outputNewLine(mp_setting);
	dl_cio::outputHorizontalLine(mp_setting);
	dl_cio::outputNewLine(mp_setting);
}
