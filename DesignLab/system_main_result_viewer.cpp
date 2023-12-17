#include "system_main_result_viewer.h"

#include <filesystem>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "file_tree.h"
#include "map_state.h"
#include "result_file_exporter.h"


namespace designlab
{

SystemMainResultViewer::SystemMainResultViewer(
	const std::shared_ptr<GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const ApplicationSettingRecord> setting_ptr
) :
	broker_ptr_(broker_ptr)
{
}

void SystemMainResultViewer::Main()
{
	cmdio::OutputTitle("Result Viewer System");

	while (true)
	{
		// ファイルツリーを表示し，ファイルを選択する
		FileTree file_tree;

		std::string res_path;

		if (!file_tree.SelectFile(ResultFileConst::kDirectoryPath, -1, "csv", ResultFileConst::kNodeListName, &res_path))
		{
			cmdio::Output("該当のデータがありませんでした．終了します．", enums::OutputDetail::kSystem);

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
			broker_ptr_->simu_end_index.SetData({ graph.size() - 1 });

			// データを表示する
			cmdio::Output("データを表示します．", enums::OutputDetail::kSystem);
			cmdio::OutputNewLine(1, enums::OutputDetail::kSystem);
			cmdio::WaitAnyKey();
			cmdio::OutputNewLine(1, enums::OutputDetail::kSystem);
			cmdio::OutputHorizontalLine("=", enums::OutputDetail::kSystem);
		}
		else
		{
			cmdio::Output("ファイルの読み込みに失敗しました．終了します．", enums::OutputDetail::kSystem);
		}

		// 終了するかどうかを選択

		if (cmdio::InputYesNo("このモードを終了しますか？"))
		{
			cmdio::OutputNewLine(1, enums::OutputDetail::kSystem);

			break;
		}

		cmdio::OutputNewLine(1, enums::OutputDetail::kSystem);
	}
}

}	// namespace designlab