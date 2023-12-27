
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "result_file_exporter.h"

#include <algorithm>
#include <filesystem>
#include <map>

#include <magic_enum.hpp>

#include "average_calculator.h"
#include "cmdio_util.h"
#include "math_util.h"
#include "map_file_exporter.h"
#include "stopwatch.h"


namespace designlab
{

namespace sf = ::std::filesystem;  // 長すぎるので，filesystemの名前空間を短縮する．


const std::string ResultFileConst::kDirectoryPath = sf::current_path().string() + "\\result";

const std::string ResultFileConst::kFileName = "sim_result";

const std::string ResultFileConst::kDetailFileName = "sim_result_detail";

const std::string ResultFileConst::kNodeListName = "node_list";

const std::string ResultFileConst::kMapStateName = "map_state";


ResultFileExporter::ResultFileExporter() :
    init_success_(false),
    do_export_(true)
{
}

void ResultFileExporter::Init()
{
    // resultフォルダがなければ作成する．
    if (!sf::exists(ResultFileConst::kDirectoryPath))
    {
        CmdIOUtil::Output("結果出力先フォルダ " + ResultFileConst::kDirectoryPath +
                          "が存在しないので作成します．", enums::OutputDetail::kInfo);

        sf::create_directory(ResultFileConst::kDirectoryPath);
    }

    // フォルダ名を指定する．現在時刻を取得し，それをフォルダ名にする．
    Stopwatch timer;
    folder_name_ = timer.GetNowTimeString();


    // 出力先フォルダを作成する．
    std::string output_folder_name = ResultFileConst::kDirectoryPath + "/" + folder_name_;

    if (sf::exists(output_folder_name))
    {
        // すでに同名のフォルダが存在する場合は，初期化失敗フラグを立てる．
        init_success_ = false;

        CmdIOUtil::Output("結果出力先のフォルダ " + output_folder_name +
                          "はすでに存在します．", enums::OutputDetail::kError);

        return;
    }

    sf::create_directory(output_folder_name);  // フォルダを作成する．

    if (!sf::exists(output_folder_name))
    {
        // 今度は逆に，フォルダが作成できなかった場合は，初期化失敗フラグを立てる．
        init_success_ = false;

        CmdIOUtil::Output("結果出力先のフォルダ " + output_folder_name +
                          "を作成できませんでした．", enums::OutputDetail::kError);

        return;
    }

    init_success_ = true;
}

void ResultFileExporter::PushSimulationResult(const SimulationResultRecord& simu_result)
{
    // 結果をセットする．
    result_list_.push_back(simu_result);
}


void ResultFileExporter::ExportLatestNodeList() const
{
    // 初期化ができていない場合は，なにも出力しない．
    // また，出力フラグが falseの場合もなにも出力しない．
    if (!init_success_)
    {
        CmdIOUtil::Output("結果出力先のフォルダの初期化に失敗しているため，"
                          "NodeListを出力できません", enums::OutputDetail::kError);
        return;
    }

    if (!do_export_)
    {
        CmdIOUtil::Output("結果出力フラグが false のため，NodeListを出力しません",
                          enums::OutputDetail::kInfo);
        return;
    }

    CmdIOUtil::Output("NodeListを出力します．", enums::OutputDetail::kInfo);

    // 出力先ファイルを作成する．
    std::string output_file_name = ResultFileConst::kDirectoryPath + "\\" +
        folder_name_ + "\\" + ResultFileConst::kNodeListName +
        std::to_string(result_list_.size()) + ".csv";

    std::ofstream ofs(output_file_name);

    // ファイルが作成できなかった場合は，なにも出力しない．
    if (!ofs)
    {
        CmdIOUtil::Output("ファイル " + output_file_name + "を作成できませんでした．",
                          enums::OutputDetail::kError);
        return;
    }

    for (const auto& i : result_list_.back().graph_search_result_recoder)
    {
        ofs << i.result_node << "\n";  // ノードを出力する．
    }

    ofs.close();  // ファイルを閉じる．

    CmdIOUtil::Output("出力完了 : " + output_file_name, enums::OutputDetail::kInfo);
}

void ResultFileExporter::ExportLatestMapState() const
{
    // 初期化ができていない場合は，なにも出力しない．
    // また，出力フラグが falseの場合もなにも出力しない．
    if (!init_success_)
    {
        CmdIOUtil::Output("結果出力先のフォルダの初期化に失敗しているため，"
                          "MapStateを出力できません", enums::OutputDetail::kError);
        return;
    }

    if (!do_export_)
    {
        CmdIOUtil::Output("結果出力フラグが falseのため，"
                          "MapStateを出力しません", enums::OutputDetail::kInfo);
        return;
    }

    CmdIOUtil::Output("MapStateを出力します．", enums::OutputDetail::kInfo);

    // 出力先ファイルを作成する．
    std::string output_file_name = ResultFileConst::kDirectoryPath + "\\" +
        folder_name_ + "\\" + ResultFileConst::kMapStateName +
        std::to_string(result_list_.size()) + ".csv";

    MapFileExporter map_file_exporter;

    if (map_file_exporter.ExportMap(output_file_name, result_list_.back().map_state))
    {
        CmdIOUtil::Output("出力完了 : " + output_file_name, enums::OutputDetail::kInfo);
    }
    else
    {
        CmdIOUtil::Output("出力失敗 : " + output_file_name, enums::OutputDetail::kInfo);
    }
}

void ResultFileExporter::ExportAllResultDetail() const
{
    // 初期化ができていない場合は，なにも出力しない．
    // また，出力フラグが falseの場合もなにも出力しない．
    if (!init_success_)
    {
        CmdIOUtil::Output("結果出力先のフォルダの初期化に失敗しているため，"
                          "結果を出力できません", enums::OutputDetail::kError);
        return;
    }

    if (!do_export_)
    {
        CmdIOUtil::Output("結果出力フラグが false のため，結果を出力しません",
                          enums::OutputDetail::kInfo);
        return;
    }

    CmdIOUtil::Output("シミュレーション全体の結果を出力します．"
                      "シミュレーション数 : " + std::to_string(result_list_.size()),
                      enums::OutputDetail::kInfo);

    // 出力先ファイルを作成する．
    std::string output_file_name = ResultFileConst::kDirectoryPath + "\\" + folder_name_ +
        "\\" + ResultFileConst::kDetailFileName + ".csv";

    std::ofstream ofs(output_file_name);

    // ファイルが作成できなかった場合は，なにも出力しない．
    if (!ofs)
    {
        CmdIOUtil::Output("ファイル " + output_file_name + " を作成できませんでした．",
                          enums::OutputDetail::kError);
        return;
    }

    ofs << "シミュレーション数" << result_list_.size() << "\n";

    // 成功したシミュレーション数を数える．
    std::map<enums::SimulationResult, int> result_count;

    for (const auto& i : result_list_)
    {
        result_count[i.simulation_result]++;
    }

    ofs << "成功したシミュレーション数," <<
        result_count[enums::SimulationResult::kSuccess] << "\n";

    ofs << "失敗したシミュレーション数," <<
        result_count[enums::SimulationResult::kFailureByGraphSearch] +
        result_count[enums::SimulationResult::kFailureByLoopMotion] +
        result_count[enums::SimulationResult::kFailureByNodeLimitExceeded] << "\n";

    ofs << "グラフ探索に失敗," <<
        result_count[enums::SimulationResult::kFailureByGraphSearch] << "\n";

    ofs << "デッドロックに陥った," <<
        result_count[enums::SimulationResult::kFailureByLoopMotion] << "\n";

    ofs << "ノード数制限を超えた," <<
        result_count[enums::SimulationResult::kFailureByNodeLimitExceeded] << "\n";
}


void ResultFileExporter::ExportResult() const
{
    // 初期化ができていない場合は，なにも出力しない．
    // また，出力フラグが falseの場合もなにも出力しない．
    if (!init_success_)
    {
        CmdIOUtil::Output("結果出力先のフォルダの初期化に失敗しているため，"
                          "結果を出力できません", enums::OutputDetail::kError);
        return;
    }

    if (!do_export_)
    {
        CmdIOUtil::Output("結果出力フラグが false のため，結果を出力しません",
                          enums::OutputDetail::kInfo);
        return;
    }

    CmdIOUtil::Output("結果を出力します．シミュレーション数 : " +
                      std::to_string(result_list_.size()), enums::OutputDetail::kInfo);

    for (int i = 0; i < result_list_.size(); i++)
    {
        if (OutputResultDetail(result_list_[i], i))
        {
            CmdIOUtil::Output("出力完了 : シミュレーション番号 " +
                              std::to_string(i + 1), enums::OutputDetail::kInfo);
        }
        else
        {
            CmdIOUtil::Output("出力失敗 : シミュレーション番号 " +
                              std::to_string(i + 1), enums::OutputDetail::kInfo);
        }
    }
}

bool ResultFileExporter::OutputResultDetail(const SimulationResultRecord& recorder,
                                            const int index) const
{
    // 出力先ファイルを作成する．
    std::string output_file_name =
        ResultFileConst::kDirectoryPath + "\\" + folder_name_ + "\\" +
        ResultFileConst::kFileName + std::to_string(index + 1) + ".csv";

    std::ofstream ofs(output_file_name);

    // ファイルが作成できなかった場合は，なにも出力しない．
    if (!ofs) { return false; }

    // 結果を出力する．
    ofs << recorder.ToCsvString() << std::endl;


    // 時間の統計を出力する．

    double max_time = recorder.graph_search_result_recoder[1].computation_time;

    double min_time = max_time;

    AverageCalculator<double> average_calculator;

    // 最初のノードは除く(計算時間0で固定のため)
    if (recorder.graph_search_result_recoder.size() > 1)
    {
        for (size_t i = 1; i < recorder.graph_search_result_recoder.size(); ++i)
        {
            const double time = recorder.graph_search_result_recoder[i].computation_time;

            if (time > max_time) { max_time = time; }

            if (time < min_time) { min_time = time; }

            average_calculator.AddData(time);
        }
    }

    ofs << "最大探索時間," << math_util::FloatingPointNumToString(max_time) <<
        ",[milli_sec]" << std::endl;

    ofs << "最小探索時間," << math_util::FloatingPointNumToString(min_time) <<
        ",[milli_sec]" << std::endl;

    ofs << "総合探索時間," <<
        math_util::FloatingPointNumToString(average_calculator.GetSum().value_or(-1.f)) <<
        ",[milli_sec]" << std::endl;

    ofs << "平均探索時間," <<
        math_util::FloatingPointNumToString(average_calculator.GetAverage().value_or(-1.f)) <<
        ",[milli_sec]" << std::endl;

    ofs << "分散," <<
        math_util::FloatingPointNumToString(average_calculator.GetVariance().value_or(-1.f)) <<
        ",[milli_sec^2]" << std::endl;

    ofs << "標準偏差," << math_util::FloatingPointNumToString(
        average_calculator.GetStandardDeviation().value_or(-1.f)) <<
        ",[milli_sec]" << std::endl;


    const double time_1sigma_plus = average_calculator.GetAverage().value_or(-1.f) +
        average_calculator.GetStandardDeviation().value_or(-1.f);

    const double time_1sigma_minus = (std::max)(
        average_calculator.GetAverage().value_or(-1.f) -
        average_calculator.GetStandardDeviation().value_or(-1.f), 0.0);

    ofs << "全データの約68%は" <<
        math_util::FloatingPointNumToString(time_1sigma_plus) << " [milli_sec]以下で" <<
        math_util::FloatingPointNumToString(time_1sigma_minus) << " [milli_sec]以上です．" <<
        std::endl << std::endl;


    // 移動距離の統計を出力する．
    if (recorder.graph_search_result_recoder.size() > 1)
    {
        float x_move_sum = 0.0f;
        float y_move_sum = 0.0f;
        float z_move_sum = 0.0f;

        for (size_t i = 0; i != recorder.graph_search_result_recoder.size() - 1; ++i)
        {
            RobotStateNode current_node = recorder.graph_search_result_recoder[i].result_node;
            RobotStateNode next_node = recorder.graph_search_result_recoder[i + 1].result_node;
            designlab::Vector3 com_dif = next_node.center_of_mass_global_coord -
                current_node.center_of_mass_global_coord;

            x_move_sum += com_dif.x;
            y_move_sum += com_dif.y;
            z_move_sum += com_dif.z;
        }

        const double x_move_average = x_move_sum /
            static_cast<double>(recorder.graph_search_result_recoder.size() - 1);

        const double y_move_average = y_move_sum /
            static_cast<double>(recorder.graph_search_result_recoder.size() - 1);

        const double z_move_average = z_move_sum /
            static_cast<double>(recorder.graph_search_result_recoder.size() - 1);

        ofs << "X方向総移動距離," << math_util::FloatingPointNumToString(x_move_sum) <<
            ",[mm]" << std::endl;

        ofs << "Y方向総移動距離," << math_util::FloatingPointNumToString(y_move_sum) <<
            ",[mm]" << std::endl;

        ofs << "Z方向総移動距離," << math_util::FloatingPointNumToString(z_move_sum) <<
            ",[mm]" << std::endl;

        ofs << "X方向平均移動距離," << math_util::FloatingPointNumToString(x_move_average) <<
            ",[mm/動作]" << std::endl;

        ofs << "Y方向平均移動距離," << math_util::FloatingPointNumToString(y_move_average) <<
            ",[mm/動作]" << std::endl;

        ofs << "Z方向平均移動距離," << math_util::FloatingPointNumToString(z_move_average) <<
            ",[mm/動作]" << std::endl;
    }

    ofs.close();

    CmdIOUtil::Output("出力ファイル : " + output_file_name, enums::OutputDetail::kInfo);

    return true;
}

}  // namespace designlab
