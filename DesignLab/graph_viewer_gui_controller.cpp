
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "graph_viewer_gui_controller.h"

#include <string>

#include <magic_enum.hpp>

#include <Dxlib.h>

#include "graphic_const.h"
#include "graph_search_const.h"
#include "keyboard.h"
#include "leg_state.h"


namespace designlab
{

GraphViewerGUIController::GraphViewerGUIController(
    const std::vector<RobotStateNode>* const p_graph,
    size_t* const p_display_node_index,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr) :
    graph_ptr_(p_graph),
    display_node_index_ptr_(p_display_node_index),
    setting_ptr_(setting_ptr)
{
}


void GraphViewerGUIController::Update()
{
    InputNumber();
    UpdateChildrenList();
    ChangeDisplayNodeIndex();

    keyboard_.Update();
}


void GraphViewerGUIController::Draw() const
{
    DrawGraphData();
    DrawNodeControlPanel();

    if (graph_ptr_->size() != 0 && *display_node_index_ptr_ < graph_ptr_->size())
    {
        // DrawNodeData(graph_ptr_->at(*display_node_index_ptr_));
    }
}


void GraphViewerGUIController::DrawGraphData() const
{
    const int kBoxSizeX = 250;
    const int kBoxSizeY = 200;
    const int kBoxMinX = setting_ptr_->window_size_x - kBoxSizeX - 10;
    const int kBoxMinY = setting_ptr_->window_size_y - kBoxSizeY - 10;
    const unsigned int kBaseColor = GetColor(255, 255, 255);

    // 枠
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(kBoxMinX, kBoxMinY, kBoxMinX + kBoxSizeX, kBoxMinY + kBoxSizeY,
            kBaseColor, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const unsigned int kTextColor = GetColor(10, 10, 10);

    // テキスト
    if (graph_ptr_->size() == 0)
    {
        DrawString(kBoxMinX + 10, kBoxMinY + 10,
                   "ノード数 : 0", kTextColor);
        DrawString(kBoxMinX + 10, kBoxMinY + 30,
                   "グラフを生成してください", kTextColor);
    }
    else
    {
        DrawFormatString(kBoxMinX + 10, kBoxMinY + 10, kTextColor,
                         "総ノード数:%d", graph_ptr_->size());
        DrawFormatString(kBoxMinX + 10, kBoxMinY + 30, kTextColor,
                         "表示ノード:%d番", *display_node_index_ptr_);

        // 深さごとのノードの数．
        for (size_t i = 0; i < graph_node_depth_data_.size(); i++)
        {
            DrawFormatString(
                kBoxMinX + 10,
                kBoxMinY + 50 + 20 * static_cast<int>(i),
                kTextColor,
                "　(深さ%dのノード:%d)",
                static_cast<int>(i), static_cast<int>(graph_node_depth_data_.at(i)));
        }
    }
}


void GraphViewerGUIController::DrawNodeControlPanel() const
{
    const int kBoxSizeX = 350;
    const int kBoxSizeY = 250;
    const int kBoxMinX = 10;
    const int kBoxMinY = 10;
    const unsigned int kBaseColor = GetColor(255, 255, 255);

    // 枠
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(kBoxMinX, kBoxMinY,
            kBoxMinX + kBoxSizeX, kBoxMinY + kBoxSizeY,
            kBaseColor, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const unsigned int kTextColor = GetColor(10, 10, 10);

    // テキスト
    DrawFormatString(kBoxMinX + 10, kBoxMinY + 10,
                     kTextColor, "input ( C でクリア)");

    if (input_number_ < 0)
    {
        DrawFormatString(
            kBoxMinX + 10, kBoxMinY + 30,
            kTextColor, "　数字を入力してください");
    }
    else
    {
        DrawFormatString(
            kBoxMinX + 10, kBoxMinY + 30,
            kTextColor, "　%d", input_number_);
    }

    if (graph_ptr_->size() > *display_node_index_ptr_)
    {
        DrawFormatString(
            kBoxMinX + 10, kBoxMinY + 60, kTextColor,
            "%d番ノードの親ノード:%d番",
            *display_node_index_ptr_,
            graph_ptr_->at(*display_node_index_ptr_).parent_index);

        DrawFormatString(
            kBoxMinX + 10, kBoxMinY + 90, kTextColor,
            "%d番ノードの子ノード数:%d個",
            children_list_.first,
            children_list_.second.size());

        std::string str = children_list_.second.size() == 0 ? "None" : "　";

        for (size_t i = 0; i < children_list_.second.size(); i++)
        {
            if (i > (size_t)6 * 5 - 1)
            {
                str += "...";
                break;
            }

            str += std::to_string(children_list_.second.at(i)) + ",";

            if (i % 6 == 0 && i != 0)
            {
                str += "\n　";
            }
        }

        DrawFormatString(
            kBoxMinX + 10, kBoxMinY + 110,
            kTextColor,
            "%d番ノードの子ノードリスト", children_list_.first);
        DrawFormatString(kBoxMinX + 10, kBoxMinY + 130, kTextColor, str.c_str());
    }
}


void GraphViewerGUIController::DrawNodeData(const RobotStateNode& node) const
{
    using leg_func::GetDiscreteComPos;
    using leg_func::GetDiscreteLegPos;

    const int kBoxSizeX = 400;
    const int KBoxSizeY = 300;
    const int kBoxMinX = setting_ptr_->window_size_x - 25 - kBoxSizeX;
    const int kBoxMinY = 25;
    const unsigned int kBoxColor = GetColor(255, 255, 255);
    const unsigned int kBoxAlpha = 128;

    // 枠．
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);

    DrawBox(
        kBoxMinX, kBoxMinY,
        kBoxMinX + kBoxSizeX, kBoxMinY + KBoxSizeY, kBoxColor, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // テキスト．
    const unsigned int kTextColor = GetColor(10, 10, 10);
    const int kTextXPos = kBoxMinX + 10;
    const int kTextYMinPos = kBoxMinY + 10;
    const int kTextYInterval = 30;

    int text_line = 0;

    DrawFormatString(
        kTextXPos,
        kTextYMinPos + kTextYInterval * (text_line++),
        kTextColor,
        "重心：%d，脚位置：%d,%d,%d,%d,%d,%d",
        GetDiscreteComPos(node.leg_state),
        GetDiscreteLegPos(node.leg_state, 0),
        GetDiscreteLegPos(node.leg_state, 1),
        GetDiscreteLegPos(node.leg_state, 2),
        GetDiscreteLegPos(node.leg_state, 3),
        GetDiscreteLegPos(node.leg_state, 4),
        GetDiscreteLegPos(node.leg_state, 5));

    // 重心を表示する．
    DrawFormatString(
        kTextXPos,
        kTextYMinPos + kTextYInterval * (text_line++),
        kTextColor,
        "重心位置(x:%5.3f,y:%5.3f,z:%5.3f)",
        node.center_of_mass_global_coord.x,
        node.center_of_mass_global_coord.y,
        node.center_of_mass_global_coord.z);

    // 遊脚か接地脚か．
    std::string str = "";
    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        if (leg_func::IsGrounded(node.leg_state, i))
        {
            str += "接地,";
        }
        else
        {
            str += "遊脚,";
        }
    }

    DrawFormatString(
        kTextXPos,
        kTextYMinPos + kTextYInterval * (text_line++),
        kTextColor,
        "脚の状態：%s",
        str.c_str());

    // 脚の位置を表示する．
    for (int i = 0; i < HexapodConst::kLegNum; i++)
    {
        DrawFormatString(
            kTextXPos,
            kTextYMinPos + kTextYInterval * (text_line++),
            kTextColor,
            "%d番脚の位置(x:%5.3f,y:%5.3f,z:%5.3f)",
            i, node.leg_pos[i].x,
            node.leg_pos[i].y,
            node.leg_pos[i].z);
    }

    // 深さと次の動作を表示する．
    DrawFormatString(
        kTextXPos,
        kTextYMinPos + kTextYInterval * (text_line++),
        kTextColor,
        "深さ：%d, 次の動作 : %s",
        node.depth,
        static_cast<std::string>(magic_enum::enum_name(node.next_move)).c_str());
}


void GraphViewerGUIController::InputNumber()
{
    // Cキーでリセット．
    if (keyboard_.GetPressingCount(KEY_INPUT_C) == 1)
    {
        input_number_ = -1;
        return;
    }

    // 数字入力．
    int input_number = -1;

    if (keyboard_.GetPressingCount(KEY_INPUT_0) == 1)
    {
        input_number = 0;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_1) == 1)
    {
        input_number = 1;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_2) == 1)
    {
        input_number = 2;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_3) == 1)
    {
        input_number = 3;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_4) == 1)
    {
        input_number = 4;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_5) == 1)
    {
        input_number = 5;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_6) == 1)
    {
        input_number = 6;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_7) == 1)
    {
        input_number = 7;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_8) == 1)
    {
        input_number = 8;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_9) == 1)
    {
        input_number = 9;
    }

    if (input_number >= 0)
    {
        if (input_number_ < 0)
        {
            input_number_ = input_number;
        }
        else
        {
            input_number_ *= 10;
            input_number_ += input_number;
        }
    }
}


void GraphViewerGUIController::ChangeDisplayNodeIndex()
{
    if (graph_ptr_->size() == 0)
    {
        return;
    }

    if (keyboard_.GetPressingCount(KEY_INPUT_DOWN) == 1)
    {
        (*display_node_index_ptr_)--;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_UP) == 1)
    {
        (*display_node_index_ptr_)++;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_RETURN) == 1)
    {
        (*display_node_index_ptr_) = input_number_;
        input_number_ = -1;
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_P) == 1)
    {
        if (graph_ptr_->size() > *display_node_index_ptr_)
        {
            (*display_node_index_ptr_) =
                graph_ptr_->at(*display_node_index_ptr_).parent_index;
        }
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_LEFT) == 1 &&
             !children_list_.second.empty())
    {
        display_children_list_index_--;

        if (display_children_list_index_ < 0)
        {
            display_children_list_index_ =
                static_cast<int>(children_list_.second.size()) - 1;
        }

        display_children_list_index_ =
            (display_children_list_index_ < 0) ? 0 : display_children_list_index_;

        (*display_node_index_ptr_) =
            children_list_.second.at(display_children_list_index_);
    }
    else if (keyboard_.GetPressingCount(KEY_INPUT_RIGHT) == 1 &&
             !children_list_.second.empty())
    {
        display_children_list_index_++;

        if (display_children_list_index_ >= children_list_.second.size())
        {
            display_children_list_index_ = 0;
        }

        (*display_node_index_ptr_) =
            children_list_.second.at(display_children_list_index_);
    }


    if (*display_node_index_ptr_ < 0)
    {
        *display_node_index_ptr_ = graph_ptr_->size() - 1;
    }
    else if (*display_node_index_ptr_ >= graph_ptr_->size())
    {
        *display_node_index_ptr_ = 0;
    }
}


void GraphViewerGUIController::UpdateChildrenList()
{
    if (graph_ptr_->size() == 0)
    {
        return;
    }

    if (keyboard_.GetPressingCount(KEY_INPUT_U) == 1)
    {
        children_list_.first = static_cast<int>(*display_node_index_ptr_);
        children_list_.second.clear();

        const size_t kGraphSize = graph_ptr_->size();

        for (size_t i = 0; i < kGraphSize; i++)
        {
            if (graph_ptr_->at(i).parent_index == children_list_.first)
            {
                children_list_.second.push_back(static_cast<int>(i));
            }
        }
    }
}


void GraphViewerGUIController::UpdateGraphNodeDepthData()
{
    graph_node_depth_data_.clear();

    if (graph_ptr_->size() > 0)
    {
        graph_node_depth_data_.resize((size_t)GraphSearchConst::kMaxDepth + 1);

        for (size_t i = 0; i < graph_ptr_->size(); ++i)
        {
            const auto index = static_cast<size_t>(graph_ptr_->at(i).depth);
            graph_node_depth_data_.at(index)++;
        }
    }
}

}  // namespace designlab
