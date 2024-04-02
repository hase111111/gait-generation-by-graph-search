
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "system_main_robot_control.h"

#include <format>

#include <boost/thread.hpp>

#include "cmdio_util.h"
#include "file_tree.h"
#include "serial_communication_thread.h"


namespace designlab
{

SystemMainRobotControl::SystemMainRobotControl(const std::shared_ptr<GraphicDataBroker>& broker_ptr) :
    broker_ptr_(broker_ptr)
{
}

void SystemMainRobotControl::Main()
{
    using enum OutputDetail;

    CmdIOUtil::OutputTitle("Result Viewer System");

    while (true)
    {
        // ファイルツリーを表示し，ファイルを選択する．
        FileTree file_tree;

        std::string res_path;

        if (!file_tree.SelectFile(kResultFileDirectoryPath, -1, "csv", ResultFileConst::kNodeListName, &res_path))
        {
            CmdIOUtil::Output("該当のデータがありませんでした．終了します．", kSystem);

            break;
        }

        // ファイルを読み込む．

        std::vector<RobotStateNode> graph;  // データを受け取るための変数．
        MapState map_state;

        if (result_importer_.ImportNodeListAndMapState(res_path, &graph, &map_state))
        {
            RemoveDoNotMoveNode(&graph);
            MergeContinuousMove(&graph);
            DivideSwingAndStance(&graph);

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
};

void SystemMainRobotControl::RemoveDoNotMoveNode(std::vector<RobotStateNode>* graph_ptr)
{
    // 脚先の座標のみを確認し，変更がない場合は削除する．
    std::optional<RobotStateNode> prev_pos = std::nullopt;

    for (auto itr = graph_ptr->begin(); itr != graph_ptr->end();)
    {
        if (prev_pos.has_value() && itr->leg_pos == prev_pos->leg_pos)
        {
            itr = graph_ptr->erase(itr);
        }
        else
        {
            prev_pos = *itr;
            ++itr;
        }
    }
}

void SystemMainRobotControl::MergeContinuousMove(std::vector<RobotStateNode>* graph_ptr)
{
    // 胴体の平行移動を連続して行うノードをまとめる．
    std::optional<RobotStateNode> prev_pos = std::nullopt;

    bool prv_is_body_move = false;

    for (auto itr = graph_ptr->begin(); itr != graph_ptr->end();)
    {
        if (prev_pos.has_value() &&
            (*itr).center_of_mass_global_coord != prev_pos.value().center_of_mass_global_coord &&
            (*itr).center_of_mass_global_coord.z == prev_pos.value().center_of_mass_global_coord.z)
        {
            prev_pos = *itr;

            if (prv_is_body_move)
            {
                (*(itr - 1)) = *itr;
                itr = graph_ptr->erase(itr);
                prv_is_body_move = false;
            }
            else
            {
                prv_is_body_move = true;
                ++itr;
            }
        }
        else
        {
            prv_is_body_move = false;
            prev_pos = *itr;
            ++itr;
        }
    }
}

void SystemMainRobotControl::DivideSwingAndStance(std::vector<RobotStateNode>* graph_ptr)
{
    // 脚先の座標のみを確認し，変更がない場合は削除する．
    std::optional<RobotStateNode> prev_pos = std::nullopt;

    for (auto itr = graph_ptr->begin(); itr != graph_ptr->end(); ++itr)
    {
        // 前回のノードがないならば，更新して次へ．
        if (!prev_pos.has_value())
        {
            prev_pos = *itr;
            continue;
        }

        // 脚先の座標のZ座標，つまり高さが変化しているかどうかを確認する．
        int swing_leg_num = 0;
        int stance_leg_num = 0;

        for (int i = 0; i < HexapodConst::kLegNum; i++)
        {
            if (!leg_func::IsGrounded(prev_pos.value().leg_state, i) && leg_func::IsGrounded((*itr).leg_state, i))
            {
                ++stance_leg_num;
            }

            if (leg_func::IsGrounded(prev_pos.value().leg_state, i) && !leg_func::IsGrounded((*itr).leg_state, i))
            {
                ++swing_leg_num;
            }
        }

        // 遊脚・接地を分ける．
        if (swing_leg_num > 0 && stance_leg_num > 0)
        {
            RobotStateNode stance_node = prev_pos.value();

            for (int i = 0; i < HexapodConst::kLegNum; i++)
            {
                // 前に遊脚，現在支持脚の場合のみ処理をする．
                if (!leg_func::IsGrounded(prev_pos.value().leg_state, i) && leg_func::IsGrounded((*itr).leg_state, i))
                {
                    // 脚位置の変更
                    stance_node.leg_pos[i] = (*itr).leg_pos[i];

                    // 脚の状態の変更
                    leg_func::ChangeGround(i, true, &stance_node.leg_state);
                }
            }

            // ノードを挿入する．
            itr = graph_ptr->insert(itr, stance_node);
        }

        prev_pos = *itr;
    }
}


} // namespace designlab
