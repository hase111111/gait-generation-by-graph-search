
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "result_file_exporter.h"

#include <format>
#include <map>

#include <algorithm>
#include <filesystem>

#include <magic_enum.hpp>

#include "average_calculator.h"
#include "cmdio_util.h"
#include "math_util.h"
#include "map_file_exporter.h"
#include "stopwatch.h"


namespace designlab
{

namespace sf = ::std::filesystem;  // 長すぎるので，filesystemの名前空間を短縮する．


const std::string ResultFileConst::kDirectoryPath =
sf::current_path().string() + "\\" + "result";

const std::string ResultFileConst::kLegDirectoryName = "leg_pos";

const std::string ResultFileConst::kNodeListName = "node_list";

const std::string ResultFileConst::kMapStateName = "map_state";

const std::string ResultFileConst::kDetailFileName = "simulation_result_detail";

const std::string ResultFileConst::kSuccessfulCount = "simulation_successful_count";


ResultFileExporter::ResultFileExporter(
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr) :
    calculator_ptr_(calculator_ptr)
{
}


void ResultFileExporter::CreateRootDirectory()
{
    using enum OutputDetail;

    // 結果出力先フォルダがなければ作成する．
    if (!sf::exists(ResultFileConst::kDirectoryPath))
    {
        CmdIOUtil::FormatOutput(kInfo,
            "結果出力先フォルダ {} が存在しないので作成します．",
            ResultFileConst::kDirectoryPath);

        sf::create_directory(ResultFileConst::kDirectoryPath);
    }
}

void ResultFileExporter::PushSimulationResult(const SimulationResultRecord& result)
{
    // 結果をセットする．
    result_list_.push_back(result);
}

void ResultFileExporter::Export() const
{
    using enum OutputDetail;

    // 結果出力先フォルダがなければ終了する．
    if (!sf::exists(ResultFileConst::kDirectoryPath))
    {
        CmdIOUtil::Output("出力先フォルダがないので終了します．", kError);
        return;
    }

    // 出力先フォルダを作成する．
    std::string output_folder_path = MakeOutputDirectory();

    // NodeListを出力する．
    ExportEachNodeList(output_folder_path);

    // MapStateを出力する．
    ExportEachMapState(output_folder_path);

    // シミュレーション詳細を出力する．
    ExportEachSimulationDetail(output_folder_path);

    // シミュレーション全体の結果を出力する．
    ExportSuccessfulCount(output_folder_path);

    // 脚の位置を出力する．
    ExportEachLegPos(output_folder_path);

    // 脚の位置を出力する．
    ExportAllLegPos(output_folder_path);

    // 成功したシミュレーションのみを出力する．
    ExportEachLegPosAllSuccessfulSimulation(output_folder_path);

    // 成功したシミュレーションのみを出力する．
    ExportAllLegPosAllSuccessfulSimulation(output_folder_path);

    CmdIOUtil::Output("結果の出力が完了しました．", kInfo);
}


std::string ResultFileExporter::MakeOutputDirectory() const
{
    using enum OutputDetail;

    CmdIOUtil::Output("フォルダ名を入力してください．", kInfo);
    const auto input_result = CmdIOUtil::InputDirName();

    Stopwatch stopwatch;
    const std::string folder_name = std::format("{}\\{}_{}",
        ResultFileConst::kDirectoryPath,
        input_result, stopwatch.GetNowTimeString());

    // 指定されたフォルダを作成する．
    if (!sf::exists(folder_name))
    {
        sf::create_directory(folder_name);
        CmdIOUtil::Output(std::format("フォルダ {} を作成しました．", folder_name), kInfo);
    }

    return folder_name;
}

void ResultFileExporter::ExportEachNodeList(const std::string& path) const
{
    using enum OutputDetail;

    CmdIOUtil::Output("NodeListを出力します．", kInfo);

    for (size_t i = 0; i < result_list_.size(); ++i)
    {
        // 出力先ファイルを作成する．
        std::string output_file_name = std::format("{}\\{}{}.csv", path, ResultFileConst::kNodeListName, i + 1);

        std::ofstream ofs(output_file_name);

        // ファイルが作成できなかった場合は，なにも出力しない．
        if (!ofs)
        {
            CmdIOUtil::Output(std::format("ファイル {} を作成できませんでした．", output_file_name), kError);
            return;
        }

        for (const auto& j : result_list_[i].graph_search_result_recorder)
        {
            ofs << j.result_node << "\n";  // ノードを出力する．
        }

        ofs.close();  // ファイルを閉じる．

        CmdIOUtil::Output("出力ファイル : " + output_file_name, kInfo);
    }

    CmdIOUtil::Output("NodeListの出力が完了しました．", kInfo);
}

void ResultFileExporter::ExportEachMapState(const std::string& path) const
{
    using enum designlab::OutputDetail;

    CmdIOUtil::Output("MapStateを出力します．", kInfo);

    for (size_t i = 0; i < result_list_.size(); ++i)
    {
        // 出力先ファイルを作成する．
        std::string output_file_name = std::format("{}\\{}{}.csv", path, ResultFileConst::kMapStateName, i + 1);

        MapFileExporter map_file_exporter;

        if (map_file_exporter.ExportMap(output_file_name, result_list_[i].map_state))
        {
            CmdIOUtil::Output("出力ファイル : " + output_file_name, kInfo);
        }
        else
        {
            CmdIOUtil::Output("出力ファイル : " + output_file_name, kInfo);
        }
    }

    CmdIOUtil::Output("MapStateの出力が完了しました．", kInfo);
}

void ResultFileExporter::ExportEachSimulationDetail(const std::string& path) const
{
    using enum designlab::OutputDetail;

    CmdIOUtil::Output("シミュレーション詳細を出力します．", kInfo);

    for (size_t i = 0; i < result_list_.size(); ++i)
    {
        // 出力先ファイルを作成する．
        std::string output_file_name = std::format("{}\\{}{}.csv", path, ResultFileConst::kDetailFileName, i + 1);

        std::ofstream ofs(output_file_name);

        // ファイルが作成できなかった場合は，なにも出力しない．
        if (!ofs)
        {
            CmdIOUtil::Output(std::format("ファイル {} を作成できませんでした．", output_file_name), kError);
            return;
        }

        const SimulationResultRecord recorder = result_list_[i];

        // 結果を出力する．
        ofs << recorder.ToCsvString() << std::endl;


        // 時間の統計を出力する．

        double max_time = recorder.graph_search_result_recorder[1].computation_time;

        double min_time = max_time;

        AverageCalculator<double> average_calculator;

        // 最初のノードは除く(計算時間0で固定のため)
        if (recorder.graph_search_result_recorder.size() > 1)
        {
            for (size_t j = 1; j < recorder.graph_search_result_recorder.size(); ++j)
            {
                const double time = recorder.graph_search_result_recorder[j].computation_time;

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
        if (recorder.graph_search_result_recorder.size() > 1)
        {
            float x_move_sum = 0.0f;
            float y_move_sum = 0.0f;
            float z_move_sum = 0.0f;

            for (size_t j = 0; j != recorder.graph_search_result_recorder.size() - 1; ++j)
            {
                RobotStateNode current_node = recorder.graph_search_result_recorder[j].result_node;
                RobotStateNode next_node = recorder.graph_search_result_recorder[j + 1].result_node;
                Vector3 com_dif = next_node.center_of_mass_global_coord - current_node.center_of_mass_global_coord;

                x_move_sum += com_dif.x;
                y_move_sum += com_dif.y;
                z_move_sum += com_dif.z;
            }

            const double x_move_average = x_move_sum /
                static_cast<double>(recorder.graph_search_result_recorder.size() - 1);

            const double y_move_average = y_move_sum /
                static_cast<double>(recorder.graph_search_result_recorder.size() - 1);

            const double z_move_average = z_move_sum /
                static_cast<double>(recorder.graph_search_result_recorder.size() - 1);

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

        CmdIOUtil::Output("出力ファイル : " + output_file_name, kInfo);
    }

    CmdIOUtil::Output("シミュレーション詳細の出力が完了しました．", kInfo);
}

void ResultFileExporter::ExportSuccessfulCount(const std::string& path) const
{
    using enum designlab::OutputDetail;
    using enum designlab::enums::SimulationResult;

    CmdIOUtil::Output(std::format("シミュレーション全体の結果を出力します．シミュレーション数 : {}", result_list_.size()), kInfo);

    // 出力先ファイルを作成する．
    std::string output_file_name = std::format("{}\\{}.csv", path, ResultFileConst::kSuccessfulCount);

    std::ofstream ofs(output_file_name);

    // ファイルが作成できなかった場合は，なにも出力しない．
    if (!ofs)
    {
        CmdIOUtil::Output(std::format("ファイルを作成できませんでした．"), kError);
        return;
    }

    ofs << "シミュレーション数, " << result_list_.size() << "\n";

    // 成功したシミュレーション数を数える．
    std::map<enums::SimulationResult, int> result_count;

    for (const auto& i : result_list_)
    {
        result_count[i.simulation_result]++;
    }

    // シミュレーション結果を出力する．
    ofs << std::format("成功したシミュレーション数, {} \n", result_count[kSuccess]);

    ofs << std::format("失敗したシミュレーション数, {} \n", result_count[kFailureByGraphSearch] + result_count[kFailureByLoopMotion] + result_count[kFailureByNodeLimitExceeded]);

    ofs << std::format("グラフ探索に失敗, {} \n", result_count[kFailureByGraphSearch]);

    ofs << std::format("デッドロックに陥った, {} \n", result_count[kFailureByLoopMotion]);

    ofs << std::format("ノード数制限を超えた, {} \n", result_count[kFailureByNodeLimitExceeded]);
}

void ResultFileExporter::ExportEachLegPos(const std::string& path) const
{
    // ディレクトリを作成する．
    std::string leg_pos_dir_path = path + "\\" + ResultFileConst::kLegDirectoryName;

    if (!sf::exists(leg_pos_dir_path))
    {
        sf::create_directory(leg_pos_dir_path);
    }

    // ファイルを作成する．
    for (size_t i = 0; i < result_list_.size(); ++i)
    {
        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            std::string output_file_name = std::format("{}\\simulation{}_leg{}.csv", leg_pos_dir_path, i + 1, j + 1);

            std::ofstream ofs(output_file_name);

            // ファイルが作成できなかった場合は，なにも出力しない．
            if (!ofs)
            {
                CmdIOUtil::Output(std::format("ファイル {} を作成できませんでした．", output_file_name), OutputDetail::kError);
                return;
            }

            ofs << GetHeader() << "\n";

            std::optional<Vector3> past_pos;

            for (const auto& recorder : result_list_[i].graph_search_result_recorder)
            {
                const Vector3 current_pos = recorder.result_node.leg_pos[j];

                // 変化がない場合はスキップする．
                if (past_pos.has_value() && current_pos == past_pos) { continue; }

                // 高さが変化している and 平行に移動している場合は中継点も出力する．
                if (past_pos.has_value() &&
                    current_pos.z != past_pos.value().z &&
                    current_pos.ProjectedXY() != past_pos.value().ProjectedXY())
                {
                    if (current_pos.z < past_pos.value().z)
                    {
                        const Vector3 relay_point = Vector3(current_pos.x, current_pos.y, past_pos.value().z);
                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 接地時．
                        ofs << current_pos.GetLength() << "," << past_pos.value().z << ",true,g," << std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) << "\n";
                    }
                    else
                    {
                        const Vector3 relay_point = Vector3(past_pos.value().x, past_pos.value().y, current_pos.z);
                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 遊脚時．
                        ofs << past_pos.value().GetLength() << "," << current_pos.z << ",true,l," << std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) << "\n";
                    }
                }

                const auto joint_state = calculator_ptr_->CalculateJointState(j, current_pos);

                ofs << current_pos.ProjectedXY().GetLength() << "," <<
                    current_pos.z << ",false," << GetLegChangeStatus(past_pos, current_pos) << "," <<
                    std::boolalpha << calculator_ptr_->IsValidJointState(j, current_pos, joint_state) << "\n";

                past_pos = current_pos;
            }
        }
    }
}

void ResultFileExporter::ExportAllLegPos(const std::string& path) const
{
    // ディレクトリを作成する．
    std::string leg_pos_dir_path = path + "\\" + ResultFileConst::kLegDirectoryName;

    if (!sf::exists(leg_pos_dir_path))
    {
        sf::create_directory(leg_pos_dir_path);
    }

    for (size_t i = 0; i < result_list_.size(); ++i)
    {
        std::string output_file_name = std::format("{}\\simulation{}_all_leg.csv", leg_pos_dir_path, i + 1);

        std::ofstream ofs(output_file_name);

        // ファイルが作成できなかった場合は，なにも出力しない．
        if (!ofs)
        {
            CmdIOUtil::Output(std::format("ファイル {} を作成できませんでした．", output_file_name), OutputDetail::kError);
            return;
        }

        ofs << GetHeader() << "\n";

        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            std::optional<Vector3> past_pos;

            for (const auto& recorder : result_list_[i].graph_search_result_recorder)
            {
                const Vector3 current_pos = recorder.result_node.leg_pos[j];

                // 変化がない場合はスキップする．
                if (past_pos.has_value() && current_pos == past_pos) { continue; }

                // 高さが変化している and 平行に移動している場合は中継点も出力する．
                if (past_pos.has_value() &&
                    current_pos.z != past_pos.value().z &&
                   current_pos.ProjectedXY() != past_pos.value().ProjectedXY())
                {
                    if (current_pos.z < past_pos.value().z)
                    {
                        const Vector3 relay_point = Vector3(current_pos.x, current_pos.y, past_pos.value().z);
                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 接地時
                        ofs << current_pos.GetLength() << "," << past_pos.value().z << ",true,g," <<
                            std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) <<
                            "," << j << "\n";
                    }
                    else
                    {
                        const Vector3 relay_point = Vector3(past_pos.value().x, past_pos.value().y, current_pos.z);
                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 遊脚時
                        ofs << past_pos.value().GetLength() << "," << current_pos.z << ",true,l," <<
                            std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) <<
                            "," << j << "\n";
                    }
                }

                const auto joint_state = calculator_ptr_->CalculateJointState(j, current_pos);

                ofs << current_pos.ProjectedXY().GetLength() << "," << current_pos.z << ",false," <<
                    GetLegChangeStatus(past_pos, current_pos) << "," <<
                    std::boolalpha << calculator_ptr_->IsValidJointState(j, current_pos, joint_state) <<
                    "," << j << "\n";

                past_pos = current_pos;
            }
        }
    }
}

void ResultFileExporter::ExportEachLegPosAllSuccessfulSimulation(const std::string& path) const
{
    // ディレクトリを作成する．
    std::string leg_pos_dir_path = path + "\\" + ResultFileConst::kLegDirectoryName;

    if (!sf::exists(leg_pos_dir_path))
    {
        sf::create_directory(leg_pos_dir_path);
    }

    // ファイルを作成する．

    for (int j = 0; j < HexapodConst::kLegNum; ++j)
    {
        std::string output_file_name = std::format("{}\\all_simulation_leg{}.csv", leg_pos_dir_path, j + 1);

        std::ofstream ofs(output_file_name);

        // ファイルが作成できなかった場合は，なにも出力しない．
        if (!ofs)
        {
            CmdIOUtil::Output(std::format("ファイル {} を作成できませんでした．", output_file_name), OutputDetail::kError);
            return;
        }

        ofs << GetHeader() << "\n";

        for (int k = 0; k < result_list_.size(); ++k)
        {
            if (result_list_[k].simulation_result != enums::SimulationResult::kSuccess) { continue; }

            std::optional<Vector3> past_pos;

            for (const auto& recorder : result_list_[k].graph_search_result_recorder)
            {
                const Vector3 current_pos = recorder.result_node.leg_pos[j];

                // 変化がない場合はスキップする．
                if (past_pos.has_value() && current_pos == past_pos) { continue; }

                // 高さが変化している and 平行に移動している場合は中継点も出力する．
                if (past_pos.has_value() &&
                    current_pos.z != past_pos.value().z &&
                    current_pos.ProjectedXY() != past_pos.value().ProjectedXY())
                {
                    if (current_pos.z < past_pos.value().z)
                    {
                        const Vector3 relay_point = { current_pos.x, current_pos.y, past_pos.value().z };

                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 接地時
                        ofs << current_pos.GetLength() << "," << past_pos.value().z << ",true,g," << std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) << "\n";
                    }
                    else
                    {
                        const Vector3 relay_point = Vector3(past_pos.value().x, past_pos.value().y, current_pos.z);
                        const auto joint_state = calculator_ptr_->CalculateJointState(j, relay_point);

                        // 遊脚時
                        ofs << past_pos.value().GetLength() << "," << current_pos.z << ",true,l," << std::boolalpha << calculator_ptr_->IsValidJointState(j, relay_point, joint_state) << "\n";
                    }
                }

                const auto joint_state = calculator_ptr_->CalculateJointState(j, current_pos);

                ofs << current_pos.ProjectedXY().GetLength() << "," <<
                    current_pos.z << ",false," << GetLegChangeStatus(past_pos, current_pos) << "," <<
                    std::boolalpha << calculator_ptr_->IsValidJointState(j, current_pos, joint_state) << "\n";

                past_pos = current_pos;
            }
        }

        ofs.close();
    }
}

void ResultFileExporter::ExportAllLegPosAllSuccessfulSimulation(
    const std::string& path) const
{
    using enum designlab::OutputDetail;
    using enum designlab::enums::SimulationResult;

    // ディレクトリを作成する．
    std::string leg_pos_dir_path = path + "\\" + ResultFileConst::kLegDirectoryName;

    if (!sf::exists(leg_pos_dir_path))
    {
        sf::create_directory(leg_pos_dir_path);
    }

    std::string output_file_name =
        std::format("{}\\all_simulation_all_leg.csv", leg_pos_dir_path);

    std::ofstream ofs(output_file_name);

    // ファイルが作成できなかった場合は，なにも出力しない．
    if (!ofs)
    {
        CmdIOUtil::FormatOutput(kError,
            "ファイル {} を作成できませんでした．", output_file_name);
        return;
    }

    ofs << GetHeader() << "\n";

    for (int j = 0; j < HexapodConst::kLegNum; ++j)
    {
        for (int k = 0; k < result_list_.size(); ++k)
        {
            if (result_list_[k].simulation_result != kSuccess) { continue; }

            for (int l = 0; l < HexapodConst::kLegNum; ++l)
            {
                std::optional<Vector3> past_pos;

                for (const auto& recorder : result_list_[k].graph_search_result_recorder)
                {
                    const Vector3 current_pos = recorder.result_node.leg_pos[l];

                    // 変化がない場合はスキップする．
                    if (past_pos.has_value() && current_pos == past_pos) { continue; }

                    // 高さが変化している and 平行に移動している場合は中継点も出力する．
                    if (past_pos.has_value() &&
                        current_pos.z != past_pos.value().z &&
                        current_pos.ProjectedXY() != past_pos.value().ProjectedXY())
                    {
                        if (current_pos.z < past_pos.value().z)
                        {
                            const Vector3 relay_point = Vector3(current_pos.x, current_pos.y, past_pos.value().z);
                            const auto joint_state = calculator_ptr_->CalculateJointState(l, relay_point);

                            // 接地時
                            ofs << current_pos.GetLength() << "," << past_pos.value().z << ",true,g," << std::boolalpha << calculator_ptr_->IsValidJointState(l, relay_point, joint_state) << "," << l << "\n";
                        }
                        else
                        {
                            const Vector3 relay_point = Vector3(past_pos.value().x, past_pos.value().y, current_pos.z);
                            const auto joint_state = calculator_ptr_->CalculateJointState(l, relay_point);

                            // 遊脚時
                            ofs << past_pos.value().GetLength() << "," << current_pos.z << ",true,l," << std::boolalpha << calculator_ptr_->IsValidJointState(l, relay_point, joint_state) << "," << l << "\n";
                        }
                    }

                    const auto joint_state = calculator_ptr_->CalculateJointState(l, current_pos);

                    ofs << current_pos.ProjectedXY().GetLength() << "," << current_pos.z << ",false," << GetLegChangeStatus(past_pos, current_pos) << "," << std::boolalpha << calculator_ptr_->IsValidJointState(l, current_pos, joint_state) << "," << l << "\n";

                    past_pos = current_pos;
                }
            }
        }

        ofs.close();
    }
}

std::string ResultFileExporter::GetHeader() const
{
    return "x,z,relay,string,error,index";
}

std::string ResultFileExporter::GetLegChangeStatus(const std::optional<Vector3>& past, const Vector3& current) const
{
    if (!past.has_value()) { return "f"; }

    if (math_util::IsEqual(past.value().z, current.z)) { return "b"; }

    if (past.value().z > current.z) { return "g"; }

    if (past.value().z < current.z) { return "l"; }

    return "o";
}

}  // namespace designlab
