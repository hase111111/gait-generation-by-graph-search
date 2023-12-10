#include "system_main_simulation.h"

#include <boost/thread.hpp>

#include <magic_enum.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "dead_lock_checker.h"
#include "designlab_string_util.h"
#include "node_initializer.h"
#include "map_creator_for_simulation.h"

namespace dl = ::designlab;
namespace dlio = ::designlab::cmdio;
namespace dlsu = ::designlab::string_util;


SystemMainSimulation::SystemMainSimulation(
	std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
	std::unique_ptr<IMapCreator>&& map_creator_ptr,
	std::unique_ptr<ISimulationEndChecker>&& simu_end_checker_ptr,
	std::unique_ptr<ITargetUpdater>&& target_updater_ptr,
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr
) :
	pass_finder_ptr_(std::move(gait_pattern_generator_ptr)),
	map_creator_ptr_(std::move(map_creator_ptr)),
	simu_end_checker_ptr_(std::move(simu_end_checker_ptr)),
	target_updater_ptr_(std::move(target_updater_ptr)),
	broker_ptr_(broker_ptr),
	setting_ptr_(setting_ptr)
{
	assert(pass_finder_ptr_ != nullptr);
	assert(map_creator_ptr_ != nullptr);
	assert(simu_end_checker_ptr_ != nullptr);
	assert(target_updater_ptr_ != nullptr);
	assert(broker_ptr_ != nullptr);
	assert(setting_ptr_ != nullptr);

	//結果をファイルに出力するクラスを初期化する．
	result_exporter_.Init();

	//マップを生成する．
	map_state_ = map_creator_ptr_->InitMap();

	//仲介人にマップを渡す．
	broker_ptr_->map_state.SetData(map_state_);
}


void SystemMainSimulation::Main()
{
	dlio::OutputTitle("シミュレーションモード");	//コマンドラインにタイトルを表示する．
	OutputSetting();	//コマンドラインに設定を表示する．

	DeadLockChecker dead_lock_checker;	//ノードの妥当性をチェックするクラスを用意する．

	//シミュレーションを行う回数分ループする．
	for (int i = 0; i < kSimurateNum; i++)
	{
		NodeInitializer node_initializer;							//ノードを初期化するクラスを用意する．
		RobotStateNode current_node = node_initializer.InitNode();	//現在のノードの状態を格納する変数．

		TargetRobotState target = target_updater_ptr_->Init();		//目標地点を決定する．

		//シミュレーションの結果を格納する変数．
		SimulationResultRecorder record;

		record.graph_search_result_recoder.push_back(
			GraphSearchResultRecoder{ current_node , 0, GraphSearchResult::kSuccess }
		);


		dlio::Output("シミュレーション" + std::to_string(i + 1) + "回目を開始します", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("[初期ノードの状態]", OutputDetail::kInfo);
		dlio::Output(current_node.ToString(), OutputDetail::kInfo);
		dlio::OutputNewLine(1, OutputDetail::kInfo);

		if (setting_ptr_->do_step_execution_each_simulation)
		{
			dlio::OutputNewLine(1, OutputDetail::kSystem);

			if (!dlio::InputYesNo("シミュレーションを開始しますか"))
			{
				break;
			}
		}


		if (setting_ptr_->do_gui_display) { broker_ptr_->graph.PushBack(current_node); }	//グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．


		//最大歩容生成回数分までループする．
		for (int j = 0; j < kGaitPatternGenerationLimit; j++)
		{
			current_node.ChangeLootNode();

			target = target_updater_ptr_->Update(current_node);	//目標地点を更新する．

			timer_.Start();			//タイマースタート

			RobotStateNode result_node;		//グラフ探索の結果を格納する変数．

			const GraphSearchResult result_state = pass_finder_ptr_->GetNextNodebyGraphSearch(current_node, map_state_, target, &result_node);		//グラフ探索を行う．

			timer_.End();			//タイマーストップ

			// ノード，計算時間，結果を格納する．
			record.graph_search_result_recoder.push_back(
				GraphSearchResultRecoder{ result_node , timer_.GetElapsedMilliSecond(), result_state }
			);


			//グラフ探索に失敗
			if (result_state != GraphSearchResult::kSuccess && result_state != GraphSearchResult::kFailureByLegPathGenerationError)
			{
				record.simulation_result = SimulationResult::kFailureByGraphSearch;	//シミュレーションの結果を格納する変数を失敗に更新する．

				dlio::Output(
					"シミュレーションに失敗しました．SimulationResult = " +
					dlsu::EnumToStringRemoveTopK(record.simulation_result) +
					"/ GraphSearch = " +
					dlsu::EnumToStringRemoveTopK(result_state),
					OutputDetail::kSystem
				);

				break;	//次の歩容が生成できなかったら，このループを抜け，次のシミュレーションへ進む．
			}


			current_node = result_node;		//次の歩容が生成できているならば，ノードを更新する．

			if (setting_ptr_->do_gui_display) { broker_ptr_->graph.PushBack(current_node); }			//グラフィックが有効ならば仲介人に結果を通達する．

			dlio::OutputNewLine(1, OutputDetail::kInfo);
			dlio::Output("[ シミュレーション" + std::to_string(i + 1) + "回目 / 歩容生成" + std::to_string(j + 1) + "回目 ] ", OutputDetail::kInfo);	//現在のシミュレーションの回数をコマンドラインに出力する．
			dlio::Output(current_node.ToString(), OutputDetail::kInfo);	//現在のノードの状態をコマンドラインに出力する．
			dlio::OutputHorizontalLine("-", OutputDetail::kInfo);

			dead_lock_checker.AddNode(current_node);													//動作チェッカーにもノードを通達する．

			//動作がループして失敗
			if (dead_lock_checker.IsDeadLock())
			{
				record.simulation_result = SimulationResult::kFailureByLoopMotion;	//シミュレーションの結果を格納する変数を失敗に更新する．

				dlio::Output(
					"シミュレーションに失敗しました．SimulationResult = " +
					dlsu::EnumToStringRemoveTopK(record.simulation_result) +
					"/ GraphSearch = " +
					dlsu::EnumToStringRemoveTopK(result_state),
					OutputDetail::kSystem
				);

				break;	//動作がループしてしまっているならば，ループを一つ抜け，次のシミュレーションへ進む．
			}

			//成功時の処理
			if (simu_end_checker_ptr_->IsEnd(current_node))
			{
				record.simulation_result = SimulationResult::kSuccess;	//シミュレーションの結果を格納する変数を成功に更新する．

				dlio::Output(
					"シミュレーションに成功しました．SimulationResult = " +
					dlsu::EnumToStringRemoveTopK(record.simulation_result),
					OutputDetail::kSystem
				);

				break;	//成功したら，このループを抜け，次のシミュレーションへ進む．
			}

			//ステップ実行にしているならば，ここで一時停止する．
			if (setting_ptr_->do_step_execution_each_gait)
			{
				dlio::OutputNewLine(1, OutputDetail::kSystem);
				dlio::WaitAnyKey("キー入力で次の歩容を生成します");
			}

		}	//歩容生成のループ終了

		record.map_state = map_state_;					//シミュレーションの結果を格納する変数にマップの状態を格納する．
		result_exporter_.PushSimulationResult(record);	//シミュレーションの結果をファイルに出力する．
		result_exporter_.ExportLatestNodeList();		//最新のノードリストをファイルに出力する．
		result_exporter_.ExportLatestMapState();		//最新のマップ状態をファイルに出力する．

		broker_ptr_->simu_end_index.PushBack(broker_ptr_->graph.GetSize() - 1);	//仲介人にシミュレーション終了を通達する．

		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::OutputHorizontalLine("=", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);

	}	//シミュレーションのループ終了


	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("シミュレーション終了", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);

	result_exporter_.ExportResult();	//シミュレーションの結果を全てファイルに出力する．
	result_exporter_.ExportAllResultDetail();
	dlio::OutputNewLine(1, OutputDetail::kSystem);

	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::Output("シミュレーションを終了します", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
}


void SystemMainSimulation::OutputSetting() const
{
	dlio::Output("[設定]", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);


	if (setting_ptr_->do_cmd_output)
	{
		dlio::Output("・コマンドラインへの出力を行います", OutputDetail::kSystem);

		std::string output_str = magic_enum::enum_name(setting_ptr_->cmd_output_detail).data();
		dlio::Output("　　・priorityが" + output_str + "以上のもののみ出力されます", OutputDetail::kSystem);
	}
	else
	{
		std::string output_str = magic_enum::enum_name(OutputDetail::kSystem).data();
		dlio::Output("・コマンドラインへの出力を行いません．(priorityが" + output_str + "のものは例外的に出力されます)", OutputDetail::kSystem);
	}

	dlio::OutputNewLine(1, OutputDetail::kSystem);


	if (setting_ptr_->do_step_execution_each_simulation)
	{
		dlio::Output("・シミュレーションをステップ実行します", OutputDetail::kSystem);
	}
	else
	{
		dlio::Output("・シミュレーションをステップ実行しません", OutputDetail::kSystem);
	}

	dlio::OutputNewLine(1, OutputDetail::kSystem);


	if (setting_ptr_->do_step_execution_each_gait)
	{
		dlio::Output("・各歩容をステップ実行します", OutputDetail::kSystem);
	}
	else
	{
		dlio::Output("・各歩容をステップ実行しません", OutputDetail::kSystem);
	}

	dlio::OutputNewLine(1, OutputDetail::kSystem);

	if (setting_ptr_->do_gui_display)
	{
		dlio::Output("・GUIを表示します", OutputDetail::kSystem);
	}
	else
	{
		dlio::Output("・GUIを表示しません", OutputDetail::kSystem);
	}

	dlio::OutputNewLine(1, OutputDetail::kSystem);
	dlio::OutputHorizontalLine("-", OutputDetail::kSystem);
	dlio::OutputNewLine(1, OutputDetail::kSystem);
}