#include "system_main_result_viewer.h"

#include <filesystem>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "file_tree.h"
#include "map_state.h"
#include "result_file_exporter.h"


namespace dlio = designlab::cmdio;


SystemMainResultViewer::SystemMainResultViewer(
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecord> setting_ptr
) :
	broker_ptr_(broker_ptr)
{
}

void SystemMainResultViewer::Main()
{
	dlio::OutputTitle("Result Viewer System");

	while (true)
	{
		// ファイルツリーを表示し，ファイルを選択する
		FileTree file_tree;

		std::string res_path;
		
		if ( ! file_tree.SelectFile(ResultFileConst::kDirectoryPath, -1, "csv", ResultFileConst::kNodeListName, &res_path)) 
		{
			dlio::Output("該当のデータがありませんでした．終了します．", OutputDetail::kSystem);

			break;
		}


		// ファイルを読み込む

		std::vector<RobotStateNode> graph;		// データを受け取るための変数
		MapState map_state;

		if (result_importer_.ImportNodeListAndMapState(res_path, &graph, &map_state)) 
		{
			// データを仲介人に渡す
			broker_ptr_->graph.SetData(graph);
			broker_ptr_->map_state.SetData(map_state);
			broker_ptr_->simu_end_index.SetData({graph.size() - 1});

			// データを表示する
			dlio::Output("データを表示します．", OutputDetail::kSystem);
			dlio::OutputNewLine(1, OutputDetail::kSystem);
			dlio::WaitAnyKey();
			dlio::OutputNewLine(1, OutputDetail::kSystem);
			dlio::OutputHorizontalLine("=", OutputDetail::kSystem);
		}
		else 
		{
			dlio::Output("ファイルの読み込みに失敗しました．終了します．", OutputDetail::kSystem);
		}

		// 終了するかどうかを選択

		if (dlio::InputYesNo("このモードを終了しますか？"))
		{
			dlio::OutputNewLine(1, OutputDetail::kSystem);

			break;
		}

		dlio::OutputNewLine(1, OutputDetail::kSystem);
	}

}