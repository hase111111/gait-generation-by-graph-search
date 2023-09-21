#include "simulation_system_main.h"

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "define.h"
#include "hexapod_state_calculator.h"
#include "node_validity_checker.h"
#include "graphic_main_basic.h"
#include "graphic_main_test.h"

namespace dlio = designlab::cmdio;


SimulationSystemMain::SimulationSystemMain(
		std::unique_ptr<IPassFinder>&& pass_finder_ptr,
		std::unique_ptr<IGraphicMain>&& graphic_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr
	) :
	pass_finder_ptr_(std::move(pass_finder_ptr)),
	graphic_system_(std::move(graphic_ptr), setting_ptr),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	//ロボットのデータを初期化する．
	HexapodStateCalclator_Old::initLegR();

	//結果をファイルに出力するクラスを初期化する．
	result_exporter_.init();

	//マップを生成する．
	MapCreator map_creator;
	map_state_ = map_creator.Create(EMapCreateMode::FLAT, MapCreator::OPTION_NONE, true);

	//仲介人にマップを渡す．
	broker_ptr_->map_state.set_data(map_state_);

	//この探索での目標を設定する．
	target_.TargetMode = ETargetMode::StraightPosition;
	target_.TargetPosition = { 3000,0,0 };
}


void SimulationSystemMain::Main()
{
	OutputTitle();	//コマンドラインにタイトルを表示する．


	if (!pass_finder_ptr_)
	{
		//早期リターン，グラフ探索クラスがセットされていない場合は，エラーを出力して終了する．
		dlio::Output("パスファインダークラスがありません\nシミュレーションを終了します．", OutputDetail::kError);
		return;
	}

	if (!setting_ptr_)
	{
		//早期リターン，設定クラスがセットされていない場合は，エラーを出力して終了する．
		dlio::Output("設定クラスがありません\nシミュレーションを終了します．", OutputDetail::kError);
		return;
	}


	OutputSetting();	//コマンドラインに設定を表示する．


	NodeValidityChecker node_checker;	//ノードの妥当性をチェックするクラスを用意する．


	//画像表示ウィンドウを別スレッドで立ち上げる．
	boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system_);


	//シミュレーションを行う回数分ループする．
	for (int i = 0; i < Define::kSimurateNum; i++)
	{
		SNode current_node;										//現在のノードの状態を格納する変数．
		const bool do_random_init = (i == 0) ? false : true;	// i の値が 0 ならばランダムな場所に初期化はしない．(i == 0)を評価して，trueならば前者(false)，falseならば後者(true)を代入する．
		current_node.Init(do_random_init);

		SSimulationResultRecorder record;	//シミュレーションの結果を格納する変数．
		record.result_nodes.push_back(current_node);	//シミュレーションの結果を格納する変数に現在のノードの状態を追加する．
		record.simulation_result = ESimulationResult::FAILURE_BY_NODE_LIMIT_EXCEEDED;	//シミュレーションの結果を格納する変数を成功に初期化する．


		dlio::Output("シミュレーション" + std::to_string(i + 1) + "回目を開始します", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("[初期ノードの状態]", OutputDetail::kInfo);
		dlio::Output(current_node.ToString(), OutputDetail::kInfo);
		dlio::OutputNewLine(1, OutputDetail::kInfo);

		if (setting_ptr_->do_step_execution)
		{
			dlio::OutputNewLine(1, OutputDetail::kSystem);
			dlio::WaitAnyKey("キー入力でシミュレーションを開始します");
		}


		if (setting_ptr_->gui_display) { broker_ptr_->graph.push_back(current_node); }	//グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．


		//最大歩容生成回数分までループする．
		for (int j = 0; j < Define::kGaitPatternGenerationLimit; j++)
		{
			timer_.Start();			//タイマースタート

			SNode result_node;		//グラフ探索の結果を格納する変数．

			EGraphSearchResult result_state = pass_finder_ptr_->GetNextNodebyGraphSearch(current_node, map_state_, target_, &result_node);		//グラフ探索を行う．

			timer_.End();			//タイマーストップ


			record.computation_time.push_back(timer_.GetElapsedMicroSecond() / 1000);	//計算時間を格納する．
			record.graph_search_results.push_back(result_state);			//グラフ探索の結果を格納する．
			record.result_nodes.push_back(result_node);		//シミュレーションの結果を格納する変数に現在のノードの状態を追加する．


			//グラフ探索に失敗
			if (!graphSeachResultIsSuccessful(result_state))
			{
				dlio::Output("シミュレーションに失敗しました．SimulationResult = " + std::to_string(ESimulationResult::FAILURE_BY_GRAPH_SEARCH) + "/ GraphSearch = " + std::to_string(result_state));

				record.simulation_result = ESimulationResult::FAILURE_BY_GRAPH_SEARCH;	//シミュレーションの結果を格納する変数を失敗に更新する．

				break;	//次の歩容が生成できなかったら，このループを抜け，次のシミュレーションへ進む．
			}


			current_node = result_node;		//次の歩容が生成できているならば，ノードを更新する．

			if (setting_ptr_->gui_display) { broker_ptr_->graph.push_back(current_node); }			//グラフィックが有効ならば仲介人に結果を通達する．

			dlio::OutputNewLine(1, OutputDetail::kInfo);
			dlio::OutputHorizontalLine(false, OutputDetail::kInfo);
			dlio::Output("[ シミュレーション" + std::to_string(i + 1) + "回目 / 歩容生成" + std::to_string(j + 1) + "回目 ] ", OutputDetail::kInfo);	//現在のシミュレーションの回数をコマンドラインに出力する．
			dlio::Output(current_node.ToString(), OutputDetail::kInfo);	//現在のノードの状態をコマンドラインに出力する．

			if (setting_ptr_->do_step_execution_each_gait)
			{
				dlio::OutputNewLine(1, OutputDetail::kSystem);
				dlio::WaitAnyKey("キー入力で次の歩容を生成します");
			}


			node_checker.setNode(current_node);													//動作チェッカーにもノードを通達する．


			//動作がループして失敗
			if (node_checker.isLoopMove())
			{
				dlio::Output("シミュレーションに失敗しました．SimulationResult = " + std::to_string(ESimulationResult::FAILURE_BY_LOOP_MOTION) + "/ GraphSearch = " + std::to_string(result_state));

				record.simulation_result = ESimulationResult::FAILURE_BY_LOOP_MOTION;	//シミュレーションの結果を格納する変数を失敗に更新する．

				break;	//動作がループしてしまっているならば，ループを一つ抜け，次のシミュレーションへ進む．
			}


			//成功時の処理
			if (current_node.global_center_of_mass.x > Define::kGoalTape)
			{
				dlio::Output("シミュレーションに成功しました．SimulationResult = " + std::to_string(ESimulationResult::SUCCESS));

				record.simulation_result = ESimulationResult::SUCCESS;	//シミュレーションの結果を格納する変数を成功に更新する．

				break;
			}

		}	//歩容生成のループ終了

		result_exporter_.exportResult(record);	//シミュレーションの結果をファイルに出力する．

		broker_ptr_->simu_end_index.push_back(broker_ptr_->graph.size() - 1);	//仲介人にシミュレーション終了を通達する．

		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::OutputHorizontalLine(true, OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);

	}	//シミュレーションのループ終了


	dlio::OutputNewLine();
	dlio::Output("シミュレーション終了");
	dlio::OutputNewLine();


	//画像表示ウィンドウの終了を待つ．
	if (setting_ptr_->gui_display)
	{
		dlio::Output("DXlib(gui)の終了を待っています．GUIのXボタンを押してください");

		graphic_thread.join();
	}

	dlio::OutputNewLine();
	dlio::Output("シミュレーションを終了します");
	dlio::OutputNewLine();
	dlio::OutputHorizontalLine(true);
	dlio::OutputNewLine();
}


void SimulationSystemMain::OutputTitle() const
{
	dlio::OutputNewLine();
	dlio::OutputHorizontalLine(true);
	dlio::OutputNewLine();
	dlio::OutputCenter("シミュレーションモード");
	dlio::OutputNewLine();
	dlio::OutputHorizontalLine(true);
	dlio::OutputNewLine();
}


void SimulationSystemMain::OutputSetting() const
{
	dlio::Output("[設定]");
	dlio::OutputNewLine();


	if (setting_ptr_->cmd_output)
	{
		dlio::Output("・コマンドラインへの出力を行います");


		dlio::Output("　　・priorityが" + std::to_string(setting_ptr_->cmd_permission) + "以上のもののみ出力されます");
	}
	else
	{
		dlio::Output("・コマンドラインへの出力を行いません．(priorityが" + std::to_string(OutputDetail::kSystem) + "のものは例外的に出力されます)");
	}

	dlio::OutputNewLine();


	if (setting_ptr_->do_step_execution)
	{
		dlio::Output("・シミュレーションをステップ実行します");
	}
	else
	{
		dlio::Output("・シミュレーションをステップ実行しません");
	}

	dlio::OutputNewLine();


	if (setting_ptr_->do_step_execution_each_gait)
	{
		dlio::Output("・各歩容をステップ実行します");
	}
	else
	{
		dlio::Output("・各歩容をステップ実行しません");
	}

	dlio::OutputNewLine();

	if (setting_ptr_->gui_display)
	{
		dlio::Output("・GUIを表示します");
	}
	else
	{
		dlio::Output("・GUIを表示しません");
	}

	dlio::OutputNewLine();
	dlio::OutputHorizontalLine();
	dlio::OutputNewLine();
}
