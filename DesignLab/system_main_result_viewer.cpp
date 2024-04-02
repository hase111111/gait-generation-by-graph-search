
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "system_main_result_viewer.h"

#include <filesystem>
#include <optional>
#include <vector>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "file_tree.h"
#include "interpolated_node_creator.h"
#include "map_state.h"
#include "result_file_exporter.h"


namespace designlab
{

SystemMainResultViewer::SystemMainResultViewer(
    const std::shared_ptr<GraphicDataBroker>& broker_ptr,
    const std::shared_ptr<const ApplicationSettingRecord> setting_ptr,
    const std::shared_ptr<const IHexapodJointCalculator> joint_calculator,
    const std::shared_ptr<const IHexapodCoordinateConverter> converter) :
    broker_ptr_(broker_ptr),
    joint_calculator_(joint_calculator),
    converter_(converter)
{
}

void SystemMainResultViewer::Main()
{
    using enum OutputDetail;

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
            // 異常値を出力する．
            // OutputErrorLegPos(res_path, graph);

            // データを仲介人に渡す．
            broker_ptr_->graph.SetData(graph);
            broker_ptr_->map_state.SetData(map_state);
            broker_ptr_->simulation_end_index.SetData({ graph.size() - 1 });

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

void SystemMainResultViewer::OutputErrorLegPos(const std::string& file, const std::vector<RobotStateNode>& nodes)
{
    CmdIOUtil::Output("異常値のデータを出力します", OutputDetail::kSystem);

    // 後ろの .csv を削除する
    std::string file_name = file.substr(0, file.size() - 4);

    const std::string output_path = file_name + "_error_leg_pos.txt";

    CmdIOUtil::Output("出力先: " + output_path, OutputDetail::kSystem);

    // すでにファイルが存在する場合は削除する．
    if (std::filesystem::exists(output_path))
    {
        std::filesystem::remove(output_path);
    }

    // まず，脚接地点が可動範囲外のノードを探す．
    std::vector<Vector2> error_leg_pos;

    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            HexapodJointState joint_state = joint_calculator_->CalculateJointState(j, nodes[i].leg_pos[j]);

            if (!joint_calculator_->IsValidJointState(j, nodes[i].leg_pos[j], joint_state))
            {
                const auto length = nodes[i].leg_pos[j].ProjectedXY().GetLength();

                error_leg_pos.push_back({ length, nodes[i].leg_pos[j].z });
            }
        }
    }

    // 次に脚軌道が可動範囲外を通過するノードを探す．
    std::vector<Vector2> error_first;
    std::vector<Vector2> error_second;
    std::vector<Vector2> error_end;

    for (int i = 0; i < nodes.size() - 1; i++)
    {
        for (int j = 0; j < HexapodConst::kLegNum; j++)
        {
            // 現在と次のノードがともに可動範囲内にない場合，スキップする．
            HexapodJointState current_joint_state = joint_calculator_->CalculateJointState(j, nodes[i].leg_pos[j]);
            HexapodJointState next_joint_state = joint_calculator_->CalculateJointState(j, nodes[i + 1].leg_pos[j]);

            if (!joint_calculator_->IsValidJointState(j, nodes[i].leg_pos[j], current_joint_state) &&
                !joint_calculator_->IsValidJointState(j, nodes[i + 1].leg_pos[j], next_joint_state))
            {
                continue;
            }

            // 補完するノードを生成する．
            InterpolatedNodeCreator creator(converter_);

            std::vector<RobotStateNode> interpolated_nodes = creator.CreateInterpolatedNode(nodes[i], nodes[i + 1]);

            // 補完ノードの中で可動範囲外になるノードを探す．
            bool error = false;

            for (int k = 0; k < interpolated_nodes.size(); k++)
            {
                HexapodJointState joint_state = joint_calculator_->CalculateJointState(j, interpolated_nodes[k].leg_pos[j]);

                if (!joint_calculator_->IsValidJointState(j, interpolated_nodes[k].leg_pos[j], joint_state))
                {
                    error = true;

                    break;
                }
            }

            // 可動範囲外になるノードがあった場合，エラーとして記録する．
            if (error)
            {
                const auto length = nodes[i].leg_pos[j].ProjectedXY().GetLength();
                error_first.push_back({ length, nodes[i].leg_pos[j].z });

                const auto length3 = nodes[i + 1].leg_pos[j].ProjectedXY().GetLength();
                error_end.push_back({ length3, nodes[i + 1].leg_pos[j].z });

                Vector2 second;
                if (length < length3)
                {
                    second = Vector2{ error_first.back().x, error_end.back().y };
                }
                else
                {
                    second = Vector2{ error_end.back().x, error_first.back().y };
                }

                error_second.push_back(second);
            }
        }
    }

    // 結果をファイルに書き込む．

    // ファイルを開く．
    std::ofstream ofs(output_path);

    {
        ofs << "    x_position += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_leg_pos[i].x << ", ";
        }

        ofs << "]" << std::endl;
        ofs << "    z_position += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_leg_pos[i].y << ", ";
        }

        ofs << "]" << std::endl;
    }

    // 中継点を出力する．
    {
        ofs << "    x_position_first += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_first[i].x << ", ";
        }

        ofs << "]" << std::endl;
        ofs << "    z_position_first += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_first[i].y << ", ";
        }

        ofs << "]" << std::endl;
    }

    {
        ofs << "    x_position_second += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_second[i].x << ", ";
        }

        ofs << "]" << std::endl;
        ofs << "    z_position_second += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_second[i].y << ", ";
        }

        ofs << "]" << std::endl;
    }

    {
        ofs << "    x_position_end += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_end[i].x << ", ";
        }

        ofs << "]" << std::endl;
        ofs << "    z_position_end += [";

        for (int i = 0; i < error_leg_pos.size(); i++)
        {
            ofs << error_end[i].y << ", ";
        }

        ofs << "]" << std::endl;
    }
}

}  // namespace designlab
