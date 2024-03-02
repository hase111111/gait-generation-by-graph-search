
//! @author    hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "abstract_dxlib_gui.h"

#include <string>

#include <DxLib.h>

#include "font_loader.h"


namespace designlab
{

AbstractDxlibGui::AbstractDxlibGui(int width, int height) :
    width_(width),
    height_(height),
    font_handle_(FontLoader::GetIns()->GetFontHandle("font/Yu_Gothic_UI.dft"))
{
}

void AbstractDxlibGui::SetPos(
    const int pos_x, const int pos_y,
    const unsigned int option, const bool this_is_first_time)
{
    const int past_x = gui_left_pos_x_;
    const int past_y = gui_top_pos_y_;

    if (option & kDxlibGuiAnchorLeft)
    {
        gui_left_pos_x_ = pos_x;
    }
    else if (option & kDxlibGuiAnchorMiddleX)
    {
        gui_left_pos_x_ = pos_x - width_ / 2;
    }
    else if (option & kDxlibGuiAnchorRight)
    {
        gui_left_pos_x_ = pos_x - width_;
    }

    if (option & kDxlibGuiAnchorTop)
    {
        gui_top_pos_y_ = pos_y;
    }
    else if (option & kDxlibGuiAnchorMiddleY)
    {
        gui_top_pos_y_ = pos_y - height_ / 2;
    }
    else if (option & kDxlibGuiAnchorBottom)
    {
        gui_top_pos_y_ = pos_y - height_;
    }

    const int diff_x = gui_left_pos_x_ - past_x;
    const int diff_y = gui_top_pos_y_ - past_y;

    for (auto& button : button_)
    {
        button->SetPos(button->GetPosMiddleX() + diff_x,
                       button->GetPosMiddleY() + diff_y,
                       kDxlibGuiAnchorMiddleXMiddleY);
    }

    if (this_is_first_time)
    {
        init_pos_x_ = gui_left_pos_x_;
        init_pos_y_ = gui_top_pos_y_;
    }
}

void AbstractDxlibGui::SetVisible(bool visible)
{
    visible_ = visible;

    for (auto& button : button_)
    {
        button->SetVisible(visible);
    }

    if (visible)
    {
        SetPos(init_pos_x_, init_pos_y_, kDxlibGuiAnchorLeftTop);
    }
}

void AbstractDxlibGui::ClickedAction(const DxlibMouseState& state)
{
    // 各ボタンの処理．
    for (auto& button : button_)
    {
        if (button->CursorOnGui(state.cursor_x, state.cursor_y))
        {
            button->ClickedAction(state);
        }
    }
}

bool AbstractDxlibGui::CursorOnGui(
    const int cursor_x, const int cursor_y) const noexcept
{
    if (!IsVisible())
    {
        return false;
    }

    return gui_left_pos_x_ < cursor_x && cursor_x < gui_left_pos_x_ + width_ &&
        gui_top_pos_y_ < cursor_y && cursor_y < gui_top_pos_y_ + height_;
}

bool AbstractDxlibGui::IsDraggable(int cursor_x, int cursor_y) const
{
    if (!IsVisible())
    {
        return false;
    }

    return CursorOnGui(cursor_x, cursor_y);
}

void AbstractDxlibGui::DraggedAction(
    int cursor_dif_x, int cursor_dif_y,
    unsigned int mouse_key_bit)
{
    if (!(mouse_key_bit & MOUSE_INPUT_LEFT))
    {
        return;
    }

    SetPos(gui_left_pos_x_ + cursor_dif_x,
           gui_top_pos_y_ + cursor_dif_y,
           kDxlibGuiAnchorLeftTop);
}

void AbstractDxlibGui::DrawBackground(const std::string& str) const
{
    const unsigned int base_color = GetColor(255, 255, 255);
    const unsigned int frame_color = GetColor(30, 30, 30);
    const unsigned int alpha = 200;

    const int frame_width = 1;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(gui_left_pos_x_ - frame_width,
            gui_top_pos_y_ - frame_width,
            gui_left_pos_x_ + width_ + frame_width,
            gui_top_pos_y_ + height_ + frame_width,
            frame_color, TRUE);
    DrawBox(gui_left_pos_x_,
            gui_top_pos_y_,
            gui_left_pos_x_ + width_,
            gui_top_pos_y_ + height_,
            base_color, TRUE);

    DrawBox(gui_left_pos_x_,
            gui_top_pos_y_,
            gui_left_pos_x_ + width_,
            gui_top_pos_y_ + kTitleBarHeight,
            base_color, TRUE);
    DrawBox(gui_left_pos_x_ - frame_width,
            gui_top_pos_y_ - frame_width,
            gui_left_pos_x_ + width_ + frame_width,
            gui_top_pos_y_ + kTitleBarHeight + frame_width,
            frame_color, FALSE);


    const int text_pos_x = gui_left_pos_x_ + 10;
    const int text_pos_y = gui_top_pos_y_ + 10;
    const unsigned int text_color = GetColor(10, 10, 10);
    DrawFormatStringToHandle(
        text_pos_x, text_pos_y, text_color, font_handle_, str.c_str());

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

}  // namespace designlab
