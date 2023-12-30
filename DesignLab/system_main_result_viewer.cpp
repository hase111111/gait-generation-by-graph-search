
/// @author    hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "system_main_result_viewer.h"

#include <filesystem>
#include <vector>

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
    using enum enums::OutputDetail;

    CmdIOUtil::OutputTitle("Result Viewer System");

    while (true)
    {
        // ファイルツリーを表示し，ファイルを選択する．
        FileTree file_tree;

        std::string res_path;

        if (!file_tree.SelectFile(ResultFileConst::kDirectoryPath, -1,
            "csv", ResultFileConst::kNodeListName, &res_path))
        {
            CmdIOUtil::Output("該当のデータがありませんでした．終了します．", kSystem);

            break;
        }

        // ファイルを読み込む．

        std::vector<RobotStateNode> graph;  // データを受け取るための変数．
        MapState map_state;

        if (result_importer_.ImportNodeListAndMapState(res_path, &graph, &map_state))
        {
            // データを仲介人に渡す．
            broker_ptr_->graph.SetData(graph);
            broker_ptr_->map_state.SetData(map_state);
            broker_ptr_->simu_end_index.SetData({ graph.size() - 1 });

            // データを表示する．
            CmdIOUtil::Output("データを表示します．", kSystem);
            CmdIOUtil::OutputNewLine(1, kSystem);
            CmdIOUtil::WaitAnyKey();
            CmdIOUtil::OutputNewLine(1, kSystem);
            CmdIOUtil::OutputHorizontalLine("=", kSystem);
        }
        else
        {
            CmdIOUtil::Output("ファイルの読み込みに失敗しました．終了します．", kSystem);
        }

        // 終了するかどうかを選択

        if (CmdIOUtil::InputYesNo("このモードを終了しますか？"))
        {
            CmdIOUtil::OutputNewLine(1, kSystem);

            break;
        }

        CmdIOUtil::OutputNewLine(1, kSystem);
    }
}

}  // namespace designlab
