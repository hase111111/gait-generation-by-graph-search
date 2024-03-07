
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#include "dxlib_gui_display_node_switcher.h"

#include <algorithm>

#include <Dxlib.h>

#include "font_loader.h"


namespace designlab
{

DxlibGuiDisplayNodeSwitcher::DxlibGuiDisplayNodeSwitcher(
    const int window_x, const int window_y) :
    AbstractDxlibGui(kWidth, kHeight),
    window_x_(window_x),
    window_y_(window_y),
    display_node_num_(0),
    all_node_num_(0),
    simulation_num_(0),
    do_auto_animation_(false),
    animation_speed_(kAnimeSpeedMin)
{
    const int button_interval = 10;
    const int button_width = 40;
    const int kButtonLeftX = button_width / 2 + gui_left_pos_x_ + 10;
    const int kButtonTopY =
        button_width / 2 + gui_top_pos_y_ + kTitleBarHeight + 100;

    // 各種ボタンを作成する．
    button_.push_back(std::make_unique<SimpleButton>(
        "<<", kButtonLeftX, kButtonTopY, button_width, button_width));
    button_.back()->SetActivateFunction([this]() { MoveMostPrevNode(); });

    button_.push_back(std::make_unique<SimpleButton>(
        "<", kButtonLeftX + (button_interval + button_width) * 1, kButtonTopY,
        button_width, button_width));
    button_.back()->SetActivateFunction([this]() { MovePrevNode(); });

    button_.push_back(std::make_unique<SimpleButton>(
        "再生\n停止",
        kButtonLeftX + (button_interval + button_width) * 2,
        kButtonTopY,
        button_width, button_width));
    button_.back()->SetActivateFunction(
        [this]() { do_auto_animation_ = !do_auto_animation_; });

    button_.push_back(std::make_unique<SimpleButton>(
        ">", kButtonLeftX + (button_interval + button_width) * 3, kButtonTopY,
        button_width, button_width));
    button_.back()->SetActivateFunction([this]() { MoveNextNode(); });

    button_.push_back(std::make_unique<SimpleButton>(
        ">>", kButtonLeftX + (button_interval + button_width) * 4, kButtonTopY,
        button_width, button_width));
    button_.back()->SetActivateFunction([this]() { MoveMostNextNode(); });

    button_.push_back(std::make_unique<SimpleButton>(
        "↓", kButtonLeftX + (button_interval + button_width) * 3,
        kButtonTopY + (button_interval + button_width),
        button_width, button_width));
    button_.back()->SetActivateFunction([this]()
                      {
                          if (animation_speed_ > kAnimeSpeedMin)
                          {
                              --animation_speed_;
                          }
                      });

    button_.push_back(std::make_unique<SimpleButton>(
        "↑", kButtonLeftX + (button_interval + button_width) * 4,
        kButtonTopY + (button_interval + button_width),
        button_width, button_width));
    button_.back()->SetActivateFunction([this]()
                      {
                          if (animation_speed_ < kAnimeSpeedMax)
                          {
                              ++animation_speed_;
                          }
                      });

    button_.push_back(std::make_unique<SimpleButton>(
        "Prev Simu",
        kButtonLeftX + (button_interval + button_width) * 1,
        kButtonTopY + (button_interval + button_width) * 2,
        button_width * 2, button_width));
    button_.back()->SetActivateFunction([this]() { MovePrevSimulation(); });

    button_.push_back(std::make_unique<SimpleButton>(
        "Next Simu",
        kButtonLeftX + (button_interval + button_width) * 3,
        kButtonTopY + (button_interval + button_width) * 2,
        button_width * 2, button_width));
    button_.back()->SetActivateFunction([this]() { MoveNextSimulation(); });

    const int close_button_size = 28;
    const int close_button_x = gui_left_pos_x_ + kWidth - close_button_size / 2 - 2;
    const int close_button_y = gui_top_pos_y_ + close_button_size / 2 + 2;

    button_.push_back(std::make_unique<SimpleButton>(
        "×", close_button_x, close_button_y,
        close_button_size, close_button_size));
    button_.back()->SetActivateFunction([this]() { SetVisible(false); });
}

void DxlibGuiDisplayNodeSwitcher::SetGraphData(
    const size_t node_num, const std::vector<size_t>& simulation_end_index)
{
    all_node_num_ = node_num;

    simulation_end_index_.clear();

    simulation_end_index_ = simulation_end_index;
}


size_t DxlibGuiDisplayNodeSwitcher::GetDisplayNodeNum() const
{
    // 範囲外の値を返さないようにする．
    if (display_node_num_ > all_node_num_ && all_node_num_ != 0)
    {
        return all_node_num_ - 1;
    }


    // 範囲内の値ならば，そのまま返す．
    return display_node_num_;
}


void DxlibGuiDisplayNodeSwitcher::Update()
{
    ++counter_;

    // 自動再生を行う．
    if (do_auto_animation_ && !in_animation_)
    {
        MoveNextNode();
    }

    // ボタンを更新する．
    for (auto& i : button_)
    {
        i->Update();
    }

    if (!IsInWindow())
    {
        SetVisible(false);
    }
}

void DxlibGuiDisplayNodeSwitcher::Draw() const
{
    DrawBackground("NodeSwitcher");

    // ボタンを描画する．
    for (const auto& i : button_)
    {
        i->Draw();
    }

    // 文字を描画する．
    const int text_left_x = gui_left_pos_x_ + 10;

    const unsigned int text_color = GetColor(0, 0, 0);

    DrawFormatStringToHandle(
      text_left_x,
      gui_top_pos_y_ + kTitleBarHeight + 10,
      text_color,
      font_handle_,
      "[シミュレーション%d回目(全%d回)]",
      simulation_num_ + 1,
      GetAllSimulationNum(),
      display_node_num_,
      all_node_num_);

    int start_node_num = 0;
    int end_node_num = 0;

    if (simulation_num_ == 0)
    {
        start_node_num = 0;

        if (!simulation_end_index_.empty())
        {
            end_node_num = static_cast<int>(simulation_end_index_[0]);
        }
        else
        {
            end_node_num = static_cast<int>(all_node_num_ - 1);
        }
    }
    else if (simulation_num_ == static_cast<int>(simulation_end_index_.size()))
    {
        start_node_num = static_cast<int>(
            simulation_end_index_[simulation_num_ - 1] + 1);
        end_node_num = static_cast<int>(all_node_num_ - 1);
    }
    else
    {
        start_node_num = static_cast<int>(
            simulation_end_index_[simulation_num_ - 1] + 1);
        end_node_num = static_cast<int>(simulation_end_index_[simulation_num_]);
    }

    DrawFormatStringToHandle(
        text_left_x,
        gui_top_pos_y_ + kTitleBarHeight + 30,
        text_color,
        font_handle_,
        "表示ノード : %d (%d～%d)",
        display_node_num_, start_node_num, end_node_num, all_node_num_ - 1);

    DrawFormatStringToHandle(
        text_left_x,
        gui_top_pos_y_ + kTitleBarHeight + 50,
        text_color,
        font_handle_,
        "全ノード : %d ", all_node_num_ - 1);

    DrawFormatStringToHandle(
        text_left_x,
        gui_top_pos_y_ + kTitleBarHeight + 70,
        text_color,
        font_handle_,
        do_auto_animation_ == true ?
            "自動再生 : 再生/速度%d" : "自動再生 : 停止/速度%d",
        animation_speed_);

    DrawFormatStringToHandle(
        text_left_x,
        gui_top_pos_y_ + kTitleBarHeight + 150,
        text_color,
        font_handle_,
        "アニメーションの\n     速度変更");
}


void DxlibGuiDisplayNodeSwitcher::MoveMostPrevNode()
{
    // 候補．
    size_t candidate = 0;

    for (size_t i = 0; i < simulation_end_index_.size(); i++)
    {
        if (simulation_end_index_[i] < display_node_num_)
        {
            candidate = simulation_end_index_[i] + 1;
        }
        else
        {
            break;
        }
    }

    display_node_num_ = candidate;
}

void DxlibGuiDisplayNodeSwitcher::MovePrevNode()
{
    for (size_t i = 0; i < simulation_end_index_.size(); i++)
    {
        if (simulation_end_index_[i] + 1 == display_node_num_)
        {
            return;
        }
    }

    --display_node_num_;

    display_node_num_ =
        static_cast<size_t>((std::max)(static_cast<int>(display_node_num_), 0));
}

void DxlibGuiDisplayNodeSwitcher::MoveMostNextNode()
{
    // 候補．
    size_t candidate = all_node_num_ - 1;

    for (size_t i = 0; i < simulation_end_index_.size(); i++)
    {
        if (simulation_end_index_[i] >= display_node_num_)
        {
            candidate = simulation_end_index_[i];
            break;
        }
    }

    display_node_num_ = candidate;
}

void DxlibGuiDisplayNodeSwitcher::MoveNextNode()
{
    for (size_t i = 0; i < simulation_end_index_.size(); i++)
    {
        if (simulation_end_index_[i] == display_node_num_)
        {
            return;
        }
    }

    ++display_node_num_;

    display_node_num_ = (std::min)(display_node_num_, all_node_num_ - 1);
}

void DxlibGuiDisplayNodeSwitcher::MovePrevSimulation()
{
    // 前のシミュレーションへ移動する．
    --simulation_num_;

    simulation_num_ = static_cast<size_t>((std::max)(simulation_num_, 0));

    // ノードをそのシミュレーションの最初のノードに移動する．
    if (simulation_num_ == 0)
    {
        display_node_num_ = 0;
    }
    else
    {
        display_node_num_ = simulation_end_index_[simulation_num_ - 1] + 1;
    }
}

void DxlibGuiDisplayNodeSwitcher::MoveNextSimulation()
{
    // 次のシミュレーションへ移動する．
    ++simulation_num_;


    simulation_num_ = static_cast<size_t>(
        (std::min)(simulation_num_, GetAllSimulationNum() - 1));


    // ノードをそのシミュレーションの最初のノードに移動する．
    if (simulation_num_ == 0)
    {
        display_node_num_ = 0;
    }
    else
    {
        display_node_num_ = simulation_end_index_[simulation_num_ - 1] + 1;
    }
}

int DxlibGuiDisplayNodeSwitcher::GetAllSimulationNum() const
{
    int all_simulation_num = 1;

    if (!simulation_end_index_.empty())
    {
        if (simulation_end_index_.back() == all_node_num_ - 1)
        {
            all_simulation_num = static_cast<int>(simulation_end_index_.size());
        }
        else
        {
            all_simulation_num = static_cast<int>(simulation_end_index_.size()) + 1;
        }
    }

    return all_simulation_num;
}

bool DxlibGuiDisplayNodeSwitcher::IsInWindow() const
{
    return gui_left_pos_x_ < window_x_ && gui_top_pos_y_ < window_y_ &&
        0 < gui_left_pos_x_ + kWidth && 0 < gui_top_pos_y_ + kHeight;
}

}  // namespace designlab
